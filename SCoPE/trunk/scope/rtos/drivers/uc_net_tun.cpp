//==========================================================================
//	uc_net_tun.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié feb 13 2008
//	Description:
//==========================================================================
//  The following code is derived, directly or indirectly, from SCoPE,
//  released June 30, 2008. Copyright (C) 2006 Design of Systems on Silicon (DS2)
//  The Initial Developer of the Original Code is the University of Cantabria
//  for Design of Systems on Silicon (DS2)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) for more details.
//
//  You should have received a copy of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL)
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  For more information about SCOPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================
#include <rtos/api/posix/uc_signal.h>
#include <sys/socket.h>
#include <errno.h>
#include "uc_fops.h"
#include "uc_poll.h"

#include "hal/uc_noc_interface.h"
#include "rtos/drivers/uc_net_tun.h"
#include "rtos/drivers/uc_linux_if.h"
#include "rtos/low_level/uc_ll_ctrl_io.h"
#include "rtos/drivers/uc_linux_miscdevice.h"
#include "rtos/drivers/uc_linux_module.h"
#include "rtos/drivers/uc_driver_functions.h"
#include "rtos/drivers/uc_io.h"
#include "hal/uc_memory_map.h"
#include "rtos/kernel/uc_process_class.h"
#include "rtos/drivers/linux/uc_interrupt.h"
#include "rtos/kernel/uc_rtos_class.h"

extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;

#define TUN_MINOR            200
#define NET_START   0x60000000
#define DRV_NAME        "tun"
#define DRV_VERSION     "1.6"
#define DRV_DESCRIPTION "Universal TUN/TAP device driver"
#define DRV_COPYRIGHT   ""
#define MODULE_LICENSE "Dual BSD/GPL"




static int tun_chr_open(struct inode *inode, struct uc_file * file);
static int tun_chr_close(struct inode *inode, struct uc_file *file);

static int tun_chr_fasync(int fd, struct uc_file *file, int on);
static int tun_chr_ioctl(struct inode *inode, struct uc_file *file, unsigned int cmd, unsigned long arg);
static loff_t tun_chr_lseek(struct uc_file * file, loff_t offset, int origin);

static ssize_t tun_chr_read(struct uc_file * file, char * buf, size_t count, loff_t *pos);
static ssize_t tun_chr_write(struct uc_file * file, const char * buf, size_t count, loff_t *pos);

static unsigned int tun_chr_poll(struct uc_file *file, poll_table * wait);



static LIST_HEAD(tun_dev_list);

static struct ethtool_ops *tun_ethtool_ops;

static struct file_operations tun_fops = {
	NULL,
	NULL,
	tun_chr_read,
	tun_chr_write,
	tun_chr_poll,
	tun_chr_ioctl,
	tun_chr_open,
	NULL,
	tun_chr_close
};

static struct uc_miscdevice tun_miscdev = {
	TUN_MINOR,
	"/dev/net/tun",
	&tun_fops
};

static int tun_net_open(struct net_device *dev){
	netif_start_queue(dev);
	return 0;
}

static int tun_net_close(struct net_device *dev){
	netif_stop_queue(dev);
	return 0;
}

static int tun_net_xmit(struct sk_buff *skb, struct net_device *dev){
	struct tun_struct *tun = (struct tun_struct *)netdev_priv(dev);

	/* Queue frame */
	skb_queue_tail(&tun->readq, skb) ;

	if (tun->flags & TUN_FASYNC)
		kill_fasync(&tun->fasync, SIGIO, POLL_IN);

	/* Wake up process */
//	wake_up(&tun->read_wait);
	wake_up(&dev->read_wait);

	return 0;
}

static void tun_setup(struct net_device *dev);

int tun_net_init(struct net_device *dev)
{
	struct tun_struct *tun = (struct tun_struct *)netdev_priv(dev);

	tun_setup(dev);

	dev->type               = ARPHRD_ETHER;
	dev->hard_header_len    = ETH_HLEN;
	dev->mtu                = ETH_DATA_LEN;
	dev->addr_len           = ETH_ALEN;
	dev->tx_queue_len       = 1000; /* Ethernet wants good queues */
	dev->flags              = IFF_BROADCAST|IFF_MULTICAST;

/*	dev->hard_header_len = 4;
	dev->addr_len = 32;
	dev->mtu = 1500;
	dev->type =ARPHRD_ETHER;
	dev->tx_queue_len = 10;
	dev->set_multicast_list = tun_net_mclist;
*/

	return 0;
}

static void tun_setup(struct net_device *dev)
{
	struct tun_struct *tun = (struct tun_struct *)netdev_priv(dev);

	skb_queue_head_init(&tun->readq);

	init_waitqueue_head(&dev->read_wait);

	tun->owner = (uid_t)-1;

//        SET_MODULE_OWNER(dev);
	dev->open = tun_net_open;
	dev->hard_start_xmit = tun_net_xmit;
	dev->stop = tun_net_close;

	unsigned char *addr = dev->dev_addr;
	uc_ioreadBurst((void**)&addr, 6, (void *)(NET_START + 0x3F));
	dev->base_addr = NET_START;
}

/* Get packet from user space buffer(already verified) */
static __inline__ ssize_t tun_get_user(struct tun_struct *tun, const char *buf, size_t len)
{
	struct sk_buff *skb;
	struct uc_packet_info packet;
	char *i_buffer;

//	if (len > TUN_MAX_FRAME)
//		return -EINVAL;

	if (!(skb = alloc_skb(sizeof(packet) + 2, 0))) {
//		tun->stats.rx_dropped++;
		return -ENOMEM;
	}

	i_buffer=(char*)malloc(len);

	packet.source=(unsigned int)((unsigned char *)buf)[11];
	packet.dest=(unsigned int)((unsigned char *)buf)[5];
	if(((unsigned char*)buf)[5]==0xff)
		if(((unsigned char*)buf)[4]==0xff)
			if(((unsigned char*)buf)[3]==0xff)
				if(((unsigned char*)buf)[2]==0xff)
					if(((unsigned char*)buf)[1]==0xff)
						if(((unsigned char*)buf)[0]==0xff)
							packet.dest=0xffffffff;

	packet.packet = (void*)i_buffer;
	packet.size = len;
	memcpy(i_buffer, buf, len);

	skb_reserve(skb, 2);
	memcpy(skb_put(skb, sizeof(packet)), &packet, sizeof(packet));

	skb->dev = tun->dev;
//	skb->mac.raw = skb->data;
//	skb->protocol = pi.proto;

	netif_rx(skb);

//	tun->stats.rx_packets++;
//	tun->stats.rx_bytes += len;

	return len;
}

#include "rtos/utils/uc_randn.h"
extern long long uc_segment_time;

irqreturn_t tun_interrupt_handler(int irq, void *dev,struct pt_regs *regs){
	struct sk_buff *skb;
	int len,size=16;
	struct uc_packet_info *packet = new uc_packet_info;
	wait_queue_t *wq;
	void *buffer;
	struct list_head *entry;
	struct tun_struct *tun;


	tun = (struct tun_struct *)netdev_priv(__get_dev_by_name("/dev/net/tun"));

//	(struct tun_struct *)uc_find_file("/dev/net/tun")->private_data;
//	tun= (struct tun_struct *)netdev_priv((net_device*)dev);

//	FIXME: Add the time of the current irq handling (consider TCP/IP cost?)
//	uc_segment_time += randn (4000000, 5000, 0) ;

	wake_up(&tun->dev->read_wait);

	char *foo_data;
	while((len = uc_ioreadBurst((void**)&packet,size,(void*)NET_START)) > 0)
	{
		if (len != 16) {
			printf("Error: packet with wrong size\n");
		}
		// Simulation of the real data to transfer
		foo_data = new char[packet->size];
		uc_ioreadBurst((void**)(&foo_data), packet->size - 16/*NoC packet size*/, (void*)(NET_START + 0x2F));
		delete foo_data;

		len = packet->size;
		buffer = packet->packet;

		if (!(skb = alloc_skb(len + 2, 0))) {
			delete packet;
			return (irqreturn_t)-ENOMEM;
		}

		skb_reserve(skb, 2);
		memcpy(skb_put(skb, len), buffer, len);
		skb->dev = tun->dev;
		tun_net_xmit(skb,skb->dev);
	}
	delete packet;
	return 0;
}

int tun_init(void)
{
	if (uc_misc_register(&tun_miscdev)) {
		return -EIO;
	}

	return 0;
}

void tun_cleanup(void)
{
	uc_misc_deregister(&tun_miscdev);
}


static int tun_chr_open(struct inode *inode, struct uc_file * file)
{
	struct tun_struct *tun = NULL;
// 	struct uc_periph_control control;
	struct net_device *dev;

	if(!(dev=__get_dev_by_name("/dev/net/tun"))){
		tun = (struct tun_struct *)malloc(sizeof(struct tun_struct));
		if (tun == NULL)
			return -ENOMEM;
	
		memset(tun, 0, sizeof(struct tun_struct));
		file->private_data = tun;

		dev = (struct net_device *)malloc(sizeof(struct net_device));

		dev->priv = (void*)tun;
		strcpy(dev->name, "/dev/net/tun");

		tun_net_init(dev);
		register_netdevice(dev);

		request_irq(NET_IRQ, tun_interrupt_handler, 0, "/dev/net/tun", dev);

		// NOTE This is deprecated code, ignored
// 		control.send_irq_on_rec = 1;
// 		uc_iowriteCtrl(&control,(void*)NET_START);

		tun->dev=dev;
		skb_queue_head_init(&tun->readq);
		//init_waitqueue_head(&tun->dev->read_wait);
	}
	else{
		tun = (struct tun_struct *)dev->priv;
		file->private_data = tun;
	}
	tun->opened++;
	return 0;
}

static int tun_chr_close(struct inode *inode, struct uc_file *file) {
	struct tun_struct *tun = (struct tun_struct *)file->private_data;

	if(tun->opened>1){
		tun->opened--;
		file->private_data = NULL;
		return 0;
	}

	if (tun->flags & TUN_IFF_SET) {
//		rtnl_lock();
//		dev_close(&tun->dev);
//		rtnl_unlock();

		/* Drop TX queue */
//		skb_queue_purge(&tun->txq);
//		unregister_netdev(&tun->dev);
	}

	free_irq(NET_IRQ, tun->dev);
	free(tun);
	file->private_data = NULL;

	return 0;
}

static int tun_chr_fasync(int fd, struct uc_file *file, int on) {
/*	struct tun_struct *tun = (struct tun_struct *)file->private_data;
	int ret;

	if ((ret = fasync_helper(fd, file, on, &tun->fasync)) < 0)
	return ret;

	if (on)
	tun->flags |= TUN_FASYNC;
	else
	tun->flags &= ~TUN_FASYNC;
*/
	return 0;
}

static int tun_chr_ioctl(struct inode *inode, struct uc_file *file, unsigned int cmd, unsigned long arg) {
	struct tun_struct *tun = (struct tun_struct *)file->private_data;
	struct ifreq ifr;

	// Macros TUNSETIFF and TUNSETNOCSUM are not constant values and can not be used in a "case"
	if (cmd == TUNSETIFF ){
		if(arg==0) return -EFAULT;
		memcpy(&ifr, (void*)arg, sizeof ifr);

		int err;
		ifr.ifr_name[IFNAMSIZ-1] = '\0';
		strcpy(ifr.ifr_name, tun->dev->name);
		if (ifr.ifr_flags & IFF_TUN) {
			tun->flags |= TUN_TUN_DEV;
		} else if (ifr.ifr_flags & IFF_TAP) {
			tun->flags |= TUN_TAP_DEV;
		}
		if (ifr.ifr_flags & IFF_NO_PI)
			tun->flags |= TUN_NO_PI;


		if (arg==0)
			return -EFAULT;
		memcpy((void*)arg, &ifr, sizeof(ifr));
		return 0;
	}

	if ( _IOC_TYPE(cmd) == 0x89){
		if(arg==0) return -EFAULT;
		memcpy(&ifr, (void*)arg, sizeof ifr);
	}

	if (cmd == TUNSETNOCSUM){
			if (arg)
				tun->flags |= TUN_NOCHECKSUM;
			else
				tun->flags &= ~TUN_NOCHECKSUM;
			return 0;
	} 
	switch (cmd) {

		case SIOCGIFHWADDR:
			/** Get the character device's hardware address. */
			memcpy(ifr.ifr_hwaddr.sa_data, tun->dev->dev_addr,
				   min(sizeof ifr.ifr_hwaddr.sa_data, sizeof tun->dev->dev_addr));
			if(arg==0) return -EFAULT;
			memcpy( (void*)arg, &ifr, sizeof ifr);
			return 0;

		case SIOCSIFHWADDR:
			/** Set the character device's hardware address. This is used when
		 	 * filtering packets being sent from the network device to the character
			 * device.
			 */
			memcpy(tun->dev->dev_addr, ifr.ifr_hwaddr.sa_data,
				   min(sizeof ifr.ifr_hwaddr.sa_data, sizeof tun->dev->dev_addr));
			return 0;
	
		default:
			return -EINVAL;
	}

	return 0;
}


static loff_t tun_chr_lseek(struct uc_file *file, loff_t offset, int origin) {
	return -ESPIPE;
}

static ssize_t tun_chr_read(struct uc_file * file, char * buf, size_t count, loff_t *pos) {
	int size_to_copy;

	struct tun_struct *tun = (struct tun_struct *)file->private_data;

	struct sk_buff *skb;
	ssize_t ret = 0;
	const char ones[ ETH_ALEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	char addr[ ETH_ALEN];
	DECLARE_WAITQUEUE(wait, qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread);
	while (count) {
		/* Read frames from device queue */
		if (!(skb=skb_dequeue(&tun->readq))) {
			if (file->f_flags & O_NONBLOCK) {
				ret = -EAGAIN;
				break;
			}
/*			if (signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}
*/
			/* Nothing to read, let's sleep */
			set_current_state(TASK_INTERRUPTIBLE);
			add_wait_queue(&tun->dev->read_wait, &wait);
			schedule();
			remove_wait_queue(&tun->dev->read_wait, &wait);
			set_current_state(TASK_RUNNING);
			continue;
		}
		netif_start_queue(tun->dev);
		/**
		 * Decide whether to accept this packet. This code is designed to
		 * behave identically to an Ethernet interface. Accept the packet if
		 * - the packet is addressed to us.
		 * - the packet is broadcast.
		 * - we are promiscuous. 			-- Not implemented
		 * - the packet is multicast and		-- Not implemented
		 *   - we are multicast promiscous.		-- Not implemented
		 *   - we belong to the multicast group.	-- Not implemented
		 */

		if ((sizeof(addr))>skb->len) {
			size_to_copy = skb->len;
		} else {
			size_to_copy = sizeof(addr);
		}

		memcpy(addr, skb->data, size_to_copy);
//                bit_nr = ether_crc(sizeof addr, addr) >> 26;
		if ( (memcmp(addr, tun->dev->dev_addr, sizeof(addr)) == 0)
					|| (memcmp(addr, ones, sizeof(addr)) == 0) )
		{
			if (skb->len> count) {
				ret = count;
			}
			else {
				ret = skb->len;
			}

			memcpy(buf, skb->data, ret);

			free(skb);
			break;
		}

		free(skb);
	}

	return ret;
}

static ssize_t tun_chr_write(struct uc_file * file, const char * buf, size_t count, loff_t *pos) {
	struct tun_struct *tun = (struct tun_struct *)file->private_data;
	return tun_get_user(tun, buf, count);
}

static unsigned int tun_chr_poll(struct uc_file *file, poll_table * wait) {
	struct tun_struct *tun = (struct tun_struct *)file->private_data;

	poll_wait(file, &tun->dev->read_wait, wait);

	if (skb_queue_len(&tun->readq))
		return POLLIN | POLLRDNORM | POLLOUT | POLLWRNORM;

	return POLLOUT | POLLWRNORM;
}

module_init(tun_init);
module_exit(tun_cleanup);

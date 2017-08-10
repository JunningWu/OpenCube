//==========================================================================
//	uc_linux_miscdevice.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar feb 12 2008
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

#ifndef _UC_LINUX_MISC_H_
#define _UC_LINUX_MISC_H_

// #include "uc_linux_if.h"
#include "if.h"
#include "uc_io.h"
// #include "list.h"

// netdevice.h
#define MAX_ADDR_LEN    32              /* Largest hardware address length */
#define LL_MAX_HEADER   48
#define MAX_HEADER LL_MAX_HEADER

#ifdef __cplusplus
extern "C" {
#endif

struct net_device
{
	char				name[IFNAMSIZ];
	unsigned long			base_addr;      /* device I/O address   */
	unsigned int			irq;            /* device IRQ number    */
	unsigned char			if_port;

// 	int						node_id;

	unsigned long			state;
	struct net_device		*next;
	void				*priv;
	unsigned int			type;

	unsigned int			flags;
	unsigned int			size;

	unsigned char			dma;
	unsigned long			mem_end;        /* shared mem end       */
	unsigned long			mem_start;      /* shared mem start     */

	int						(*init)(struct net_device *dev);

	unsigned char			dev_addr[MAX_ADDR_LEN];
	unsigned char			broadcast[MAX_ADDR_LEN];

	struct wait_queue_head_t		read_wait;
	struct wait_queue_head_t		write_wait;
	struct wait_queue_head_t		exc_wait;

	struct list_head		poll_list;

	int						(*poll) (struct net_device *dev, int *quota);
	int						(*hard_start_xmit) (struct sk_buff *skb, struct net_device *dev);
	int						(*open)(struct net_device *dev);
	int						(*stop)(struct net_device *dev);

	int						hard_header_len;
	int						mtu;
	int						addr_len;
	int						tx_queue_len;
	//...//
};

//miscdevice.h

struct device;
struct class_device;

struct uc_miscdevice  {
	int minor;
	const char *name;
	struct file_operations *fops;
	struct list_head list;
	struct device *parent;
	struct device *this_device;
};

int uc_misc_register(struct uc_miscdevice * misc);
int uc_misc_deregister(struct uc_miscdevice * misc);

net_device *__get_dev_by_name(const char *name);
void register_netdevice(struct net_device *dev);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

//==========================================================================
//	uc_net_tun.h
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


#ifndef _UC_NET_TUN_H_
#define _UC_NET_TUN_H_

/*#include "rtos/drivers/if.h"*/
#include "../../../include/posix/sys/ioctl.h"
//#include <sys/ioctl.h>
#include "if.h"
#include <linux/if_tun.h>
/*#include "rtos/drivers/uc_linux_miscdevice.h"
#include "rtos/drivers/uc_fops.h"
#include "rtos/drivers/uc_poll.h"*/
#include "uc_fops.h"



/* Number of devices */
//#define TUN_MAX_DEV	255

/* TX queue size */
//#define TUN_TXQ_SIZE	10

/* Max frame size */
#define TUN_MAX_FRAME	4096

/* TUN device flags */
//#define TUN_TUN_DEV 	0x0001
//#define TUN_TAP_DEV	0x0002
//#define TUN_TYPE_MASK   0x000f

//#define TUN_FASYNC	0x0010
//#define TUN_NOCHECKSUM	0x0020
//#define TUN_NO_PI	0x0040

#define TUN_IFF_SET	0x1000

/* Ioctl defines */
//#define TUNSETNOCSUM (('T'<< 8) | 200)
//#define TUNSETDEBUG  (('T'<< 8) | 201)
//#define TUNSETIFF    (('T'<< 8) | 202)

/* TUNSETIFF ifr flags */
//#define IFF_TUN		0x0001
//#define IFF_TAP		0x0002
//#define IFF_NO_PI	0x1000

struct uc_tun_pi {
	unsigned short flags;
	unsigned short proto;
};
#define TUN_PKT_STRIP	0x0001
#define tun_pi uc_tun_pi


struct uc_tun_struct {
	struct list_head        list;
	unsigned long 		flags;
	int                     attached;
	int 			opened;
	uid_t                   owner;

//	wait_queue_head_t	read_wait;

	struct sk_buff_head	readq;

	struct net_device	*dev;

	struct fasync_struct    *fasync;
};

#define tun_struct uc_tun_struct


#define __init
//#define kmalloc malloc
//#define kfree free

int  tun_init(void);
void tun_cleanup(void);

#endif

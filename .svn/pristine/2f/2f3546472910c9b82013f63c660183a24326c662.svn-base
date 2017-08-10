//==========================================================================
//	uc_driver_functions.h
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

#ifndef _UC_DRIVER_FUNCTIONS_H_
#define _UC_DRIVER_FUNCTIONS_H_

#include "rtos/drivers/list.h"
#include "rtos/drivers/uc_linux_miscdevice.h"
#include "rtos/drivers/uc_fops.h"
#include "rtos/low_level/uc_ll_bus_io.h"
#include "rtos/api/posix/uc_signal.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ethtool_ops{};

// bitops/atomic.h
#define BITS_PER_LONG 32
#define BITOP_MASK(nr)          (1UL << ((nr) % BITS_PER_LONG))
#define BITOP_WORD(nr)          ((nr) / BITS_PER_LONG)

inline void clear_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BITOP_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BITOP_WORD(nr);
//	unsigned long flags;

//	_atomic_spin_lock_irqsave(p, flags);
	*p &= ~mask;
//	_atomic_spin_unlock_irqrestore(p, flags);
}

 inline void set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BITOP_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BITOP_WORD(nr);
//	unsigned long flags;
//	_atomic_spin_lock_irqsave(p, flags);
	*p |= mask;
//	_atomic_spin_unlock_irqrestore(p, flags);
}

 inline int test_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BITOP_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BITOP_WORD(nr);

	return (*p & mask) != 0;
}

// netdevice.h !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define __LINK_STATE_XOFF 0


 inline void netif_start_queue(struct net_device *dev)
{
	clear_bit(__LINK_STATE_XOFF, &dev->state);
}

 inline void netif_stop_queue(struct net_device *dev)
{
	set_bit(__LINK_STATE_XOFF, &dev->state);
}

 inline void *netdev_priv(struct net_device *dev)
{
	return dev->priv;
}


// skbuff !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 inline void skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk)
{
	struct sk_buff *next;
	struct sk_buff *prev;

	prev=(struct sk_buff *)list;

	list->qlen++;

	next = prev->next;
	newsk->next = next;
	newsk->prev = (struct sk_buff *)list;
	next->prev  = prev->next = newsk;
}

static inline int skb_queue_len(const struct sk_buff_head *list_)
{
	return list_->qlen;
}

 inline void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
	struct sk_buff *prev, *next;

	list->qlen++;
	next = (struct sk_buff *)list;
	prev = next->prev;
	newsk->next = next;
	newsk->prev = prev;
	next->prev  = prev->next = newsk;
}

inline struct sk_buff *skb_dequeue(struct sk_buff_head *list)
{
	struct sk_buff *next, *prev, *result;

	prev = (struct sk_buff *) list;
	next = prev->next;
	result = NULL;
	if (next != prev) {
		result       = next;
		next         = next->next;
		list->qlen--;
		next->prev   = prev;
		prev->next   = next;
		result->next = result->prev = NULL;
	}
	return result;
}

static inline void skb_reserve(struct sk_buff *skb, int len)
{
        skb->data += len;
        skb->tail += len;
}

static inline struct sk_buff *alloc_skb(unsigned int size,
                                         int priority){
	unsigned char * data;
	struct sk_buff *skb= (struct sk_buff *)calloc(1,sizeof(struct sk_buff)+size);
	data = (unsigned char*)((int)skb+sizeof(struct sk_buff));

	skb->truesize = size + sizeof(struct sk_buff);
	skb->head = data;
	skb->data = data;
	skb->tail = data;
	skb->end  = data + size;
	return skb;
}

static inline unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	unsigned char *tmp = skb->tail;
	skb->tail += len;
	skb->len  += len;
	if (skb->tail>skb->end)
		printf("error: buffer overflow in skb_put\n");
        return tmp;
}

int netif_rx(struct sk_buff *skb);

void kill_fasync(struct fasync_struct **fc, int sig, int band);

static inline void skb_queue_head_init(struct sk_buff_head *list)
{
	list->prev = list->next = (struct sk_buff *)list;
	list->qlen = 0;
}

static inline int skb_queue_empty(const struct sk_buff_head *list)
{
	return list->next == (struct sk_buff *)list;
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

//==========================================================================
//	uc_pool.h
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

#ifndef _UC_POLL_H_
#define _UC_POLL_H_

#include <poll.h>
#include "../api/posix/sys_uc_types.h"
#include "uc_io.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ~832 bytes of stack space used max in sys_select/sys_poll before allocating
  additional memory. */
#define MAX_STACK_ALLOC 832
#define FRONTEND_STACK_ALLOC     256
#define SELECT_STACK_ALLOC     FRONTEND_STACK_ALLOC
#define POLL_STACK_ALLOC     FRONTEND_STACK_ALLOC
#define WQUEUES_STACK_ALLOC     (MAX_STACK_ALLOC - FRONTEND_STACK_ALLOC)
#define N_INLINE_POLL_ENTRIES     (WQUEUES_STACK_ALLOC / sizeof(struct poll_table_entry))

struct poll_table_struct;

/*
* structures and helpers for f_op->poll implementations
*/
typedef void (*poll_queue_proc)(struct uc_file *, wait_queue_head_t *, struct poll_table_struct *);

typedef struct poll_table_struct {
	poll_queue_proc qproc;
} poll_table;

static inline void poll_wait(struct uc_file * filp, wait_queue_head_t * wait_address, poll_table *p)
{
	if (p && wait_address)
		p->qproc(filp, wait_address, p);
}

static inline void init_poll_funcptr(poll_table *pt, poll_queue_proc qproc)
{
	pt->qproc = qproc;
}

struct poll_table_entry {
	struct uc_file * filp;
	wait_queue_t wait;
	wait_queue_head_t * wait_address;
};

/*
* Structures and helpers for sys_poll/sys_poll
*/
struct poll_wqueues {
	poll_table pt;
	struct poll_table_page * table;
	int error;
	int inline_index;
	struct poll_table_entry inline_entries[N_INLINE_POLL_ENTRIES];
};

void poll_initwait(struct poll_wqueues *pwq);
void poll_freewait(struct poll_wqueues *pwq);

/*
* Scaleable version of the fd_set.
*/

/*
* Scaleable version of the fd_set.
*/

typedef struct {
	unsigned long *in, *out, *ex;
	unsigned long *res_in, *res_out, *res_ex;
} fd_set_bits;
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

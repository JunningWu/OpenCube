//==========================================================================
//	uc_driver_functions.cpp
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

#include "rtos/drivers/uc_driver_functions.h"
#include "hal/uc_noc_interface.h"
#include "rtos/kernel/uc_rtos_class.h"

extern __thread UC_rtos_class *qt_parent_rtos;

int netif_rx(struct sk_buff *skb){
	// Send data simulating the real data to tranfer, really only the pointer is sent to the NoC
	int size = ((struct uc_packet_info*)skb->data)->size - 16/*NoC packet size*/;
	char foo_data[size];
	uc_iowriteBurst(foo_data,size,(void*)(skb->dev->base_addr + 0x2F));

	// Send the NoC packet to the interface
	return uc_iowriteBurst(skb->data,skb->len,(void*)skb->dev->base_addr);
}

void kill_fasync(struct fasync_struct **fc, int sig, int band){
	struct fasync_struct *fa=*fc;
	while (fa) {
		int pid;
		/* IMPORTANT: pid = fa->pid.
			In linux  pid = fa->fa_file->f_owner.pid */
		pid = fa->pid;
		uc_siginfo_t si;

		/* IMPORTANT: signo = sig. In linux signo = fown-> signum */

		si.si_signo = sig;
		si.si_errno = 0;
		si.si_code  = band;
//		si.si_fd    = fa->fd;

		/* IMPORTANT:
		   Send SIGURG. Linux does not send it to processes which
		   have not set a queued signum: SIGURG has its own default
		   signalling mechanism. */

		qt_parent_rtos->get_process(pid)->signals_control.signal_process(&si);
		fa = fa->fa_next;
	}
}
//==========================================================================
//	uc_mqueue_class.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun mar 31 2008
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

#ifndef _UC_MQUEUE_CLASS_H
#define _UC_MQUEUE_CLASS_H

class uc_mqd_class : public UC_sleep_channel {
	public:
		uc_mqd_class() {
			type_of_channel = 3;
			channel_priority = 0;
			process_notify = NULL;
			to_destroy = 0;
		}

		uc_mq_attr attr;

		char *name;
		int mqd_id;

		UC_process_class *process_notify;
		struct uc_sigevent notify_event;

		vector<struct uc_mqd_params *> mqd_opened;
		UC_thread_class *parent;
		vector<struct mq_mess_element*> mq_mess_list;
		vector<UC_thread_class *> wr_blocked_threads;

		int unblock(UC_thread_class *thread);
		int to_destroy;
};

#endif

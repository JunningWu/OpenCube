//==========================================================================
//	uc_thread_scheduler_class.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar oct 30 2007
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

#ifndef UC_THREAD_SCHEDULER_CLASS_H
#define UC_THREAD_SCHEDULER_CLASS_H

#include <vector>
using namespace std;

#include "rtos/kernel/uc_task_class.h"

class UC_rtos_class;
class UC_task_class;
class UC_thread_class;

class UC_thread_scheduler_class {
public:
	UC_thread_scheduler_class(UC_task_class *task);

	~UC_thread_scheduler_class();

	UC_thread_class *reschedule();

	int goodness(UC_thread_class *thread);

	bool change_state(UC_thread_class *thread, UC_process_states initial_state, UC_process_states final_state);

	bool change_list_thread(UC_thread_class *thread, vector<UC_thread_class *> *initial_list, vector<UC_thread_class *> *final_list);
	bool change_list_thread_begin(UC_thread_class *thread, vector<UC_thread_class *> *initial_list, vector<UC_thread_class *> *final_list);

	void awake_processor(UC_thread_class *thread);

	vector<UC_thread_class *> m_ready_thread_list; ///< The list of ready threads
	vector<UC_thread_class *> m_noready_thread_list; ///< The list of no ready threads
	vector<UC_thread_class *> m_zombie_thread_list; ///< The list of zombie threads

private:

	UC_task_class *m_parent_task; ///< The parent task
};

#endif

//==========================================================================
//	uc_task_scheduler_class.h
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

#ifndef UC_TASK_SCHEDULER_CLASS_H
#define UC_TASK_SCHEDULER_CLASS_H

#include <vector>
using namespace std;

#include "rtos/kernel/uc_task_class.h"
#include "rtos/kernel/uc_cpu_class.h"
#include "rtos/kernel/uc_enums.h"
#include "rtos/kernel/uc_thread_scheduler_class.h"

class UC_task_class;
class UC_cpu_class;
class UC_rtos_class;

class UC_task_scheduler_class {
public:
	UC_task_scheduler_class(UC_rtos_class *rtos);

	~UC_task_scheduler_class();

	void new_task(UC_task_class *task);

	UC_thread_class * reschedule(UC_cpu_class *cpu);

	bool change_state(UC_task_class *task, UC_process_states initial_state, UC_process_states final_state);

	bool change_list_task(UC_task_class *task, vector<UC_task_class *> *initial_list, vector<UC_task_class *> *final_list);
	bool change_list_task_begin(UC_task_class *task, vector<UC_task_class *> *initial_list, vector<UC_task_class *> *final_list);

	vector<UC_task_class *> * get_list_from_state(UC_process_states state);

	bool delete_from_list(UC_task_class *task, vector<UC_task_class *> *list);

	void debug_lists_size();

	void check_system_status();

private:
	int goodness(struct UC_task_class *task, int cpu_id, struct UC_process_class *process);

	vector<UC_task_class *> m_created_list;		///< Queue of tasks in state: CREATED
	vector<UC_task_class *> m_ready_list;		///< Queue of tasks in state: READY
	vector<UC_task_class *> m_user_list;		///< Queue of tasks in state: USER
	vector<UC_task_class *> m_super_user_list;	///< Queue of tasks in state: SUPER_USER
	vector<UC_task_class *> m_waiting_list;		///< Queue of tasks in state: WAITING
	vector<UC_task_class *> m_blocked_list;		///< Queue of tasks in state: BLOCKED
	vector<UC_task_class *> m_zombie_list;		///< Queue of tasks in state: ZOMBIE

	UC_rtos_class *m_parent_rtos;
	UC_task_class *m_current_task; ///< Current executing task
};

#endif

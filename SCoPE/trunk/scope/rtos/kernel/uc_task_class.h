//==========================================================================
//	uc_task_class.h
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

#ifndef UC_TASK_CLASS_H
#define UC_TASK_CLASS_H

#include <iostream>
#include <vector>
using namespace std;

#include "systemc.h"

#include "rtos/kernel/uc_thread_class.h"
#include "rtos/kernel/uc_process_class.h"
#include "rtos/kernel/uc_thread_scheduler_class.h"
#include "rtos/kernel/uc_enums.h"
#include "rtos/api/posix/uc_sched.h"

class UC_process_class;
class UC_thread_class;
class UC_thread_scheduler_class;

class UC_task_class {
public:
	UC_task_class(UC_process_class *process);

	~UC_task_class();

	bool update_state(UC_process_states state);

	UC_thread_class *create_new_thread();

	void delete_thread(UC_thread_class *thread);
	void set_nice(int nice);
	int get_nice();

	UC_process_class *m_parent_process; ///< The parent process

	vector<UC_thread_class *> m_thread_list; ///< The list of children threads
	UC_thread_class * m_current_thread; ///< The thread actually running

	sc_time m_user_time; ///< Total time of execution of this task
	sc_time m_last_block_time; ///< Time of execution of the last block

	struct uc_sched_param m_schedparam; ///< Schedule parameters
	unsigned long m_policy; ///< Schedule policy
	int m_RR_counter; ///< Round Robin counter

	UC_process_states m_state; ///< The task's state
	bool m_kernel_mode; ///< Specify whether the task is in kernel mode
	int m_priority; ///< Task priority

	UC_thread_scheduler_class *m_thread_scheduler; ///< The thread scheduler

	/**
	 * \brief 32 bits cpu mask
	 *
	 * Each bit is a cpu allowed to execute this process
	 * 0x00000001 = cpu 0 allowed
	 * 0x80000000 = cpu 31 allowed
	 */
	unsigned int m_cpu_mask;

	int m_cpu; ///< The number of cpu in which it is executing


private:
	int m_nice; ///< Current priority if the task is running
};

#endif

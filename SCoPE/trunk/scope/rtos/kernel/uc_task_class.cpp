//==========================================================================
//	uc_task_class.cpp
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

#include "rtos/kernel/uc_task_class.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/kernel/uc_os_defines.h"

/**
 * \brief Constructor
 *
 * \param process The parent process of this task
 */
UC_task_class::UC_task_class(UC_process_class *process)
{
	m_parent_process = process;
	m_current_thread = NULL;
	m_user_time = SC_ZERO_TIME;
	m_last_block_time = SC_ZERO_TIME;
	m_policy = SCHED_OTHER;
	m_state = CREATED;
	m_kernel_mode = true;
	m_priority = 0;
	m_nice = 0;
	m_RR_counter = NICE_TO_TICKS(m_nice);
	m_thread_scheduler = new UC_thread_scheduler_class(this);
	m_schedparam.sched_priority = 0;
	m_cpu = -1;
	m_cpu_mask = -1;
}


UC_task_class::~UC_task_class()
{
	delete m_thread_scheduler;
	for(int i=0; i<m_thread_list.size();i++){
		delete m_thread_list[i];
	}
}

/**
 * \brief Updates the task state
 *
 * Gets the higher level of execution of its list of threads, updates its state
 * and changes its scheduler list
 * \param state The new thread's state to update the task
 * \return Returns true if success and false if failure
 */
bool UC_task_class::update_state(UC_process_states state) {
	UC_process_states old_state = m_state;
	if (state == m_state) {
		return true;
	}
	else if (state > m_state) {
		m_state = state;
	}
	else {
		m_state = state;
		vector<UC_thread_class *>::iterator it_thread;
		for (it_thread = m_thread_list.begin(); it_thread != m_thread_list.end(); it_thread++) {
			if ((*it_thread)->m_state > m_state) {
				m_state = (*it_thread)->m_state;
			}
		}
	}

	return m_parent_process->m_parent_rtos->m_task_scheduler->change_state(this, old_state, m_state);
}

/**
 * \brief Creates and initializes a new thread
 *
 * \return Returns a pointer to the created thread
 */
UC_thread_class *UC_task_class::create_new_thread() {
	UC_thread_class *thread = new UC_thread_class(this);
	m_thread_list.push_back(thread);
	m_thread_scheduler->m_noready_thread_list.push_back(thread);

	return thread;
}

/**
 * \brief Deletes a thread
 *
 * \param thread The thread to be deleted
 */
void UC_task_class::delete_thread(UC_thread_class *thread) {
	vector<UC_thread_class *>::iterator it_thread;
	for (it_thread = m_thread_list.begin(); it_thread != m_thread_list.end(); it_thread++) {
		if (*it_thread == thread) {
			m_thread_list.erase(it_thread);
			if (m_current_thread == thread) {
				m_current_thread = NULL;
			}
			delete thread;
			return;
		}
	}
}
/**
 * \brief set the task nice
 *
 * \param nice New nice value
 */
void UC_task_class::set_nice(int nice){
	int i;

	m_nice = nice;
	m_RR_counter = NICE_TO_TICKS(m_nice);	

	for(i = 0; i < m_thread_list.size(); i++ ){
		m_thread_list[i]->m_RR_counter = NICE_TO_TICKS(m_nice);
	}	
}
/**
 * \brief get the task nice value
 *
 * \return nice value
 */
int UC_task_class::get_nice(){
	return m_nice;
}


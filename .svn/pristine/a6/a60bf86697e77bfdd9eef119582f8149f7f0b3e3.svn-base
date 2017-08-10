//==========================================================================
//	uc_process_class.cpp
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

#include "rtos/kernel/uc_process_class.h"
#include "rtos/kernel/uc_rtos_class.h"


UC_process_class::UC_process_class(UC_rtos_class *rtos_parent, const char *name, float optimization)
{
	m_optimization_factor = optimization;
	m_parent_rtos = rtos_parent;
	m_stp = NULL;
	m_stp_destroy = false;
	m_rwlock_max = 0;
	m_pid = 0;

	m_process_name = new char[strlen(name)+1];
	strcpy(m_process_name, name);

	m_parent_rtos->m_tic_timer->create_clock(this);
	signals_control.set_parent_process(this);

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&m_process_mutex, &attr);
	pthread_mutex_lock(&m_process_mutex);

}


UC_process_class::~UC_process_class()
{
	if (m_process_name != NULL) {
		delete [] m_process_name;
	}
	m_parent_rtos->m_tic_timer->destroy_clock(this);
	for(int i=0; i<m_task_list.size();i++){
		delete m_task_list[i];
	}
}

/**
 * \brief Creates a new task
 *
 * \return The created task
 */
UC_task_class * UC_process_class::create_new_task() {
	// Creates a new task
	UC_task_class *task = new UC_task_class(this);
	m_task_list.push_back(task);

	// Inserts it in the scheduler
	m_parent_rtos->m_task_scheduler->new_task(task);

	return task;
}

/**
 * \brief Destroy this process
 */
void UC_process_class::destroy_process() {
	// Destroying the clocks
	m_parent_rtos->m_tic_timer->destroy_clock(this);
	m_parent_rtos->m_tic_timer->process_destroyed(this);
	m_parent_rtos->m_statistics->process_destroy(m_pid);


	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;
	for (it_task = m_task_list.begin(); it_task != m_task_list.end(); it_task++) {
		for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
			(*it_thread)->destroy_thread();
		}
	}

	// Erasing the UC_process_class
	vector<UC_process_class *>::iterator it_process;
	for (it_process = m_parent_rtos->m_process_list.begin(); it_process != m_parent_rtos->m_process_list.end(); it_process++) {
		if (*it_process == this) {
			m_parent_rtos->m_process_list.erase(it_process);
			pthread_mutex_unlock(&m_parent_rtos->m_rtos_mutex);
			delete this;
			break;
		}
	}
}

/**
 * \brief Return the running task of this process in the indicated cpu
 *
 * \param cpu Cpu id to search the current task
 * \return The current task in the cpu. If there is no current task in this thread return NULL
 */
UC_task_class * UC_process_class::get_current_task(int cpu){
	int i;

	for(i = 0; i < m_task_list.size(); i++){
		if (m_task_list[i]->m_cpu == cpu) {
			return m_task_list[i];
		}
	}
	return NULL;
}

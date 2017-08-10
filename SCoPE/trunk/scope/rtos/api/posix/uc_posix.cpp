//==========================================================================
//	uc_posix.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié oct 31 2007
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

#include "rtos/api/posix/uc_posix.h"
#include "rtos/kernel/uc_task_class.h"
#include "rtos/kernel/uc_thread_class.h"
#include "rtos/kernel/uc_os_defines.h"
#include "rtos/utils/uc_debug.h"
#include "systemc.h"

extern void *UC_qt_wrapper(void *void_thread);
extern void UC_user_code_wrapper(void *void_thread);

extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;

extern unsigned long long uc_segment_time;

UC_posix_class::UC_posix_class(UC_rtos_class *rtos)
{
	m_parent_rtos = rtos;
}


UC_posix_class::~UC_posix_class()
{
}

/**
 * \brief Creates a new POSIX thread
 *
 * Creates and initializes a new task and thread for process and execute a host pthread_create
 * \param process The parent UC_process_class
 * \param function The function to execute
 * \param args The arguments to execute the function
 * \return Returns the pointer to the created task
 */
UC_task_class * UC_posix_class::uc_pthread_create(UC_process_class *process, void* (*function)(void *), void *args, const char *name) {
	pthread_t th1;

	// Creates a new task
	UC_task_class *task = process->create_new_task();

	// Creates and initializes a new thread
	UC_thread_class *thread = task->create_new_thread();

	uc_pthread_attr_t attribs;
	uc_pthread_attr_init(&attribs);
	thread->m_attribs = attribs;

	thread->m_initial_function = function;
	thread->m_initial_function_args = args;
	thread->m_tid = m_parent_rtos->m_last_pid++;

	thread->m_func_name = new char[strlen(name)+1];
	strcpy(thread->m_func_name, name);

	pthread_create(&th1, NULL, UC_qt_wrapper, (void *)thread);

	return task;
}


/**
 * \brief Creates a thread for the UC_task_class task with the pthread_create parameters
 *
 * \param task The task that creates the thread
 * \param __attr Pthread attributes
 * \param __start_routine The function that must to execute the thread
 * \param __arg The arguments of the thread's function
 * \param func_name The function's name
 * \return The created thread
 */
UC_thread_class * UC_posix_class::uc_create_thread(UC_task_class *task, const uc_pthread_attr_t *__attr,
									 void *(*__start_routine) (void *), void *__arg, const char *func_name)
{
	// Creates attributes
	uc_pthread_attr_t attribs;
	if (__attr != NULL) {
		attribs = *__attr;
	}
	else {
		uc_pthread_attr_init(&attribs);
	}

	// Init attributes
	UC_thread_class *current_thread = task->m_current_thread;
	if (attribs.inheritsched == PTHREAD_INHERIT_SCHED) {
		attribs.scope		= current_thread->m_attribs.scope;
		attribs.schedpolicy	= current_thread->m_attribs.schedpolicy;
		attribs.schedparam	= current_thread->m_attribs.schedparam;
	}

	unsigned int tid = m_parent_rtos->m_last_pid++;

	UC_thread_class *new_thread;
	UC_task_class *new_task;
	if (attribs.scope == PTHREAD_SCOPE_SYSTEM || task == NULL){ // link to a new task
		new_task = task->m_parent_process->create_new_task();

		new_thread = new_task->create_new_thread();

		new_task->m_policy	= attribs.schedpolicy;
		new_task->m_schedparam	= attribs.schedparam;
	}
	else {
		new_task = task;
		new_thread = new_task->create_new_thread();
	}

	new_thread->signals.mask = current_thread->signals.mask;

	new_thread->m_attribs = attribs;
	new_thread->m_tid = tid;

	new_thread->m_func_name = new char[strlen(func_name)+1];
	strcpy(new_thread->m_func_name, func_name);
	new_thread->m_initial_function = __start_routine;
	new_thread->m_initial_function_args = __arg;

	stp_create(UC_user_code_wrapper, (void*)new_thread);

	return new_thread;
}

/**
 * \brief Simulates kernel execution time
 */
void UC_posix_class::eat_time(unsigned long long time) {
	// wait time in ns
	uc_segment_time += time;

/*	UC_thread_class *current_thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	current_thread->annotate_rtos_time( time, 0 );*/
}

/**
 * \brief Initializes the thread constraints check
 */
void UC_posix_class::thread_constraints_check_start() {
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if (thread->m_time_constraint != 0) {
// 		UC_DEBUG(EXIT, "Time constraint check is already running");
		printf("Time constraint check is already running\n");
	}

	thread->m_time_constraint = sc_time_stamp().value()/1000;
}

/**
 * \brief Checks thread constraints from specification model
 *
 * Actually this function checks that the thread estimated execution time
 * matchs with the specified by the parameters
 *
 * \param min minimum specified time
 * \param max maximum specified time
 */
void UC_posix_class::thread_constraints_check_end(long long min, long long max) {
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if (thread->m_time_constraint == 0) {
// 		UC_DEBUG(EXIT, "Time constraint check is not running");
		printf("Time constraint check is not running\n");
	}

	long long ns_diff = sc_time_stamp().value()/1000 - thread->m_time_constraint;

	if (ns_diff < min) {
// 		UC_DEBUG(EXIT, "Time constraint check failed, estimated time < minimum");
		printf("Time constraint check failed at thread \"%s\", estimated time (%lld) < minimum (%lld)\n", thread->m_func_name, ns_diff, min);
	}

	if (ns_diff > max) {
// 		UC_DEBUG(EXIT, "Time constraint check failed, estimated time > maximum");
		printf("Time constraint check failed at thread \"%s\", estimated time (%lld) > maximum (%lld)\n", thread->m_func_name, ns_diff, max);
	}

	thread->m_time_constraint = 0;
}

/**
 * \brief Gets the thread class from the tid
 *
 * Searchs in the current process a thread with the specified tid and
 * returns its thread class
 * \param tid The thread identifier to search
 * \return The tid's thread class
 */
UC_thread_class *UC_posix_class::uc_get_thread(uc_pthread_t tid) {
	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;
	for (it_task = qt_parent_process->m_task_list.begin(); it_task != qt_parent_process->m_task_list.end(); it_task++) {
		for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
			if ((*it_thread)->m_tid == tid) {
				return (*it_thread);
			}
		}
	}
	return NULL;
}

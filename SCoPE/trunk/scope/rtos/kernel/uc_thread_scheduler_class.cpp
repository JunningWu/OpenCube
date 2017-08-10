//==========================================================================
//	uc_thread_scheduler_class.cpp
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

#include "rtos/kernel/uc_thread_scheduler_class.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/kernel/uc_os_defines.h"

extern unsigned long long uc_segment_time;
extern unsigned long long uc_segment_instr;

UC_thread_scheduler_class::UC_thread_scheduler_class(UC_task_class *task)
{
	m_parent_task = task;
}


UC_thread_scheduler_class::~UC_thread_scheduler_class()
{
}

/**
 * \brief Schedules a thread for this task
 *
 * \return Returns the thread if success and NULL if failure
 */
UC_thread_class *UC_thread_scheduler_class::reschedule() {
	UC_thread_class *next_thread, *prev_thread;
	vector<UC_thread_class *>::iterator it_thread;
	int prio;
	int weight;

	prev_thread = m_parent_task->m_current_thread;
	// Reachs for the running process with higher priority
	prio = -1000;
	next_thread = NULL;

	while (true) {
		// Default process to select
		if (prev_thread != NULL) {
			if(prev_thread->m_state == SUPER_USER || prev_thread->m_state == USER){
				prio = goodness(prev_thread);
				next_thread = prev_thread;
			}
		}
	
		for (it_thread = m_ready_thread_list.begin(); it_thread != m_ready_thread_list.end(); it_thread++) {
			weight = goodness(*it_thread);
			if (weight > prio){
				prio = weight;
				next_thread = *it_thread;
			}
// 			else if(weight == prio) {
// 				if((*it_thread)->last_block_time < next_thread->last_block_time)
// 					next_thread = *it_thread;
// 			}
		}

		if (next_thread != NULL && next_thread == m_parent_task->m_current_thread){
			m_parent_task->m_parent_process->m_stp = next_thread->m_stp;
			return m_parent_task->m_current_thread; // running processes has higher priority than all ready tasks
		}

// 		prev_thread = m_parent_task->m_current_thread;

		// Do we need to re-calculate counters?
		if (prio == -1) {
			for (it_thread = m_ready_thread_list.begin(); it_thread != m_ready_thread_list.end(); it_thread++) {
				(*it_thread)->m_RR_counter = ((*it_thread)->m_RR_counter >> 1) +
						NICE_TO_TICKS(m_parent_task->get_nice());
			}
			continue;
		}
		break;
	}

	// from this point on nothing can prevent us from
	// switching to the next task, save this fact in
	// sched_data.
	next_thread->m_priority = prio;

	m_parent_task->m_parent_process->m_stp = next_thread->m_stp;
	return next_thread;
}

/**
 * \brief Calculates the real priority of a thread
 *
 * \param thread The thread to calculate the priority
 * \return The real priority of the given thread
 */
int UC_thread_scheduler_class::goodness(UC_thread_class *thread) {
	int weight;

	// select the current process after every other
	// runnable process, but before the idle thread.
	// Also, dont trigger a counter recalculation.
	weight = -1;
	if (thread->m_RR_counter < 0) {
// 		if(thread->m_blocking_channel != NULL && thread->m_channel_priority > weight) {
// 			weight = thread->m_channel_priority;
// 		}
		return weight;
	}

	// RT process
	if (thread->m_attribs.schedpolicy != SCHED_OTHER) {
		// Realtime process, select the first one on the
		// runqueue (taking priorities within processes
		// into account)
		weight = 1000 + thread->m_attribs.schedparam.sched_priority;
#if KERNEL_PRIORITY == 1
		if(thread->m_kernel_mode) {
			weight += 2000;
		}
#endif
// 		if (thread->m_signal_received || thread->m_in_signal) {
// 			weight += 4000;
// 		}

// 		if(thread->m_blocking_channel != NULL && thread->m_channel_priority > weight) {
// 			weight = thread->m_channel_priority;
// 		}
		return weight;
	}

	// NOT-RT process
	// Give the process a first-approximation goodness value
	// according to the number of clock-ticks it has left.
	// Don't do any other calculations if the time slice is
	// over

	weight = thread->m_RR_counter * TIME_SLICE_PENALTY;
	if (weight < 0) {
// 		if(thread->m_blocking_channel != NULL && thread->m_channel_priority > weight) {
// 			weight = thread->m_channel_priority;
// 		}
		return weight;
	}

// 	weight += (20 - thread->m_parent_task->m_nice) * NICE_PENALTY;
	if (weight > 999) {
		 weight = 999;
	}
#if KERNEL_PRIORITY == 1
	if(thread->m_kernel_mode) {
		 weight += 2000;
	}
#endif
// 	if(thread->m_signal_received || thread->m_in_signal) {
// 		 weight += 4000;
// 	}

// 	if(thread->m_blocking_channel != NULL && thread->m_channel_priority > weight) {
// 		weight = thread->m_channel_priority;
// 	}

	return weight;
}


/**
 * \brief Changes the thread state and update the task state
 *
 * \param thread The thread to change
 * \param initial_state The initial state of the task for checking
 * \param final_state The final state of the task
 * \return Returns true if success and false if failure
 */
bool UC_thread_scheduler_class::change_state(UC_thread_class *thread, UC_process_states initial_state, UC_process_states final_state) {

	int ret_val;

	switch (initial_state) {
		case ZOMBIE:
			break;
		case BLOCKED:
			if (final_state == READY) {
				change_list_thread(thread, &m_noready_thread_list, &m_ready_thread_list);
				ret_val = m_parent_task->update_state(final_state);
				awake_processor(thread);
				return ret_val;
			}
			break;
		case WAITING:
			if (final_state == USER) {
				// FIXME: Why this line was commented??? Moving the tread is mandaroty!!
				change_list_thread(thread, &m_ready_thread_list, &m_noready_thread_list);
				return m_parent_task->update_state(final_state);
			}
			break;
		case CREATED:
			if (final_state == READY) {
				change_list_thread(thread, &m_noready_thread_list, &m_ready_thread_list);
				ret_val = m_parent_task->update_state(final_state);
				awake_processor(thread);
				return ret_val;
			}
			break;
		case READY:
			if (final_state == SUPER_USER) {
				// FIXME: Why this line was commented??? Moving the tread is mandaroty!!
				change_list_thread(thread, &m_ready_thread_list, &m_noready_thread_list);
				return m_parent_task->update_state(final_state);
			}
			break;
		case USER:
			if (final_state == SUPER_USER) {
				m_parent_task->m_kernel_mode = true;
				thread->m_kernel_mode = true;
				return m_parent_task->update_state(final_state);
			}
			break;
		case SUPER_USER:
			switch (final_state) {
				case WAITING:
					thread->m_kernel_mode = false;
					m_parent_task->m_cpu = -1;
				// FIXME: Why this line was commented??? Moving the tread is mandaroty!!
 					change_list_thread_begin(thread, &m_noready_thread_list, &m_ready_thread_list);
					ret_val = m_parent_task->update_state(final_state);
					awake_processor(thread);
					return ret_val;

				case READY:
					m_parent_task->m_cpu = -1;
				// FIXME: Why this line was commented??? Moving the tread is mandaroty!!
 					change_list_thread_begin(thread, &m_noready_thread_list, &m_ready_thread_list);
					ret_val = m_parent_task->update_state(final_state);
					awake_processor(thread);
					return ret_val;

				case BLOCKED:
					m_parent_task->m_cpu = -1;
					change_list_thread(thread, &m_ready_thread_list, &m_noready_thread_list);
					return m_parent_task->update_state(final_state);

				case ZOMBIE:
					vector<UC_thread_class *>::iterator it_thread;	
					// FIXME: As thread is moved always it is no_ready_thread_list, no ready_list
					for (it_thread = m_noready_thread_list.begin(); it_thread != m_noready_thread_list.end(); it_thread++) {
						if (*it_thread == thread) {
							m_noready_thread_list.erase(it_thread);
// 							m_zombie_thread_list.push_back(*it_thread);

							m_parent_task->delete_thread(thread);

// 							thread->annotate_switch_time(ZOMBIE_SWITCH);
							return m_parent_task->update_state(final_state);
						}
					}
			}
			break;
	}

	return false;
}

/**
 * \brief Changes a thread from a list to another
 *
 * \param thread The thread to change
 * \param initial_list The list to search the thread into
 * \param final_list The list in which insert the thread
 * \return Returns true if success and false if failure
 */
bool UC_thread_scheduler_class::change_list_thread(UC_thread_class *thread, vector<UC_thread_class *> *initial_list,
											   vector<UC_thread_class *> *final_list)
{
	// Modified: when moving a thread in the same list, it is moved to the the end of the list. Thus "if" is deleted
/*	if (initial_list == final_list) {
		return true;
	}*/
	for (vector<UC_thread_class *>::iterator it_thread = initial_list->begin(); it_thread != initial_list->end(); it_thread++) {
		if (*it_thread == thread) {
			initial_list->erase(it_thread);
			final_list->push_back(thread);
			return true;
		}
	}

	return false;
}

/**
 * \brief Changes a thread from a list to another, placing it at the beginning of the target list
 *
 * \param thread The thread to change
 * \param initial_list The list to search the thread into
 * \param final_list The list in which insert the thread
 * \return Returns true if success and false if failure
 */
bool UC_thread_scheduler_class::change_list_thread_begin(UC_thread_class *thread, vector<UC_thread_class *> *initial_list,
											   vector<UC_thread_class *> *final_list)
{
	if (initial_list == final_list) {
		return true;
	}
	for (vector<UC_thread_class *>::iterator it_thread = initial_list->begin(); it_thread != initial_list->end(); it_thread++) {
		if (*it_thread == thread) {
			initial_list->erase(it_thread);
			final_list->insert(final_list->begin(),thread);
			return true;
		}
	}

	return false;
}


/**
 * \brief When a thread awakes, it checks if there is a processor empty. Thus send and IPI to the processor
 *
 * \param thread Thread that is awakening
 */
void UC_thread_scheduler_class::awake_processor(UC_thread_class *thread){
	unsigned int cpu_mask;
	UC_rtos_class *rtos;
	int i, mask = 1;

	cpu_mask = thread->m_parent_task->m_cpu_mask;
	rtos = thread->m_parent_task->m_parent_process->m_parent_rtos;

	// For all processors, if the task can be executed on it, if the processor is in idle, 
	// and the thread is not the interrupt thread, then a signal is sent
	for(i = 0; i<rtos->get_num_cpus(); i++){
		if(cpu_mask & mask){
			if((*rtos)[i]->m_current_task == (*rtos)[i]->m_idle_task ){
				if(thread->m_parent_task != (*rtos)[i]->m_interruptions_task){
					(*rtos)[i]->send_irq(1);
					break;
				}
			}
		}
		mask<<=1;
	}
}

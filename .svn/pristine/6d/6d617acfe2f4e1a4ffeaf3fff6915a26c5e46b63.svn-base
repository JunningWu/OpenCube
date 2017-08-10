//==========================================================================
//	uc_task_scheduler_class.cpp
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

#include "rtos/kernel/uc_task_scheduler_class.h"
#include "rtos/kernel/uc_thread_scheduler_class.h"
#include "rtos/kernel/uc_os_defines.h"
#include "rtos/utils/uc_simulator_configuration.h"
#include <assert.h>
#include "rtos/utils/uc_backtrace.h"

UC_task_scheduler_class::UC_task_scheduler_class(UC_rtos_class *rtos)
{
	m_parent_rtos = rtos;
}


UC_task_scheduler_class::~UC_task_scheduler_class()
{
}

/**
 * \brief Inserts a new task into the created list
 *
 * \param task The task to be inserted
 */
void UC_task_scheduler_class::new_task(UC_task_class *task) {
	m_created_list.push_back(task);
}

/**
 * \brief Reschedules a indicated processor
 *
 * Chooses a task to be executed and awakes the qt thread with the highest priority
 * \param cpu The processor to reschedule
 * \return A pointer to the next thread scheduled. NULL when idle task is scheduled
 */
UC_thread_class * UC_task_scheduler_class::reschedule(UC_cpu_class *cpu) {
	UC_task_class *next_task;
	UC_task_class *prev_task = m_current_task;
	UC_thread_class *next_thread = NULL;
	UC_thread_class *prev_thread = NULL;
	vector<UC_task_class *>::iterator it_task;
	int weight;
	int cpu_id = cpu->m_id;
	int prio;

	if (prev_task != NULL) {
		prev_thread = prev_task->m_current_thread;
	}

	// For RR policy and SCHED_OTHER
	// Check if the task RR counter has finished, update it and move the task to the end of the ready list
	// MODIF: It has to be done before checking for interruptions. Otherwise, if an interruption arrives when the thread is preempted, the RR_counter is not updated, before next time the prev_task is the interruption and not the RR task
	if (prev_task != NULL && prev_task != cpu->m_idle_task) {
		// move an exhausted RR process to be last
		if (prev_task->m_policy == SCHED_RR || prev_task->m_policy == SCHED_OTHER ) {
			if (prev_task->m_RR_counter == 0) {
				if(prev_task->m_state != USER || prev_task->m_state != SUPER_USER) {
					prev_task->m_RR_counter = NICE_TO_TICKS(prev_task->get_nice());
				}
				if(prev_task->m_state == WAITING || prev_task->m_state == READY) {
					delete_from_list(prev_task, &m_ready_list);
					m_ready_list.push_back(prev_task);
				}
			}
		}

		// move an exhausted RR thread to be last
		if (prev_thread != NULL && (prev_thread->m_attribs.schedpolicy == SCHED_RR || prev_thread->m_attribs.schedpolicy == SCHED_RR)) {
			if (prev_thread->m_RR_counter <= 0) {
				if(prev_thread->m_state != USER || prev_thread->m_state != SUPER_USER) {
					prev_thread->m_RR_counter = NICE_TO_TICKS(prev_task->get_nice());
				}
				if(prev_thread->m_state == WAITING || prev_thread->m_state == READY) {
					prev_thread->m_RR_counter = NICE_TO_TICKS(prev_task->get_nice());
					// Put it the last one in the queue
					UC_thread_scheduler_class *th_sched = prev_task->m_thread_scheduler;
					th_sched->change_list_thread(prev_thread, &th_sched->m_ready_thread_list, &th_sched->m_ready_thread_list);
				}
			}
		}
	}

	if (cpu->m_active_interruptions != 0 && cpu->m_interruptions_task->m_thread_scheduler->m_ready_thread_list.size()>0) {
		next_task = cpu->m_interruptions_task;
		prio = 999;
	}
	else {

		// Searchs for the running process with higher priority

		while (true) {
			// Default process to select
			prio = -1000;
			next_task = cpu->m_idle_task;

			// Starts considering the current task as the next task (if it is in executable state). 
			// Then, a task with higher priority is searched in the ready list.
			// Once the next task is decided, if it is not the current task, it is added to the ready list.
			if (prev_thread != NULL) {
				if(prev_thread->m_state == SUPER_USER || prev_thread->m_state == USER){
					prio = goodness(prev_task, cpu_id, prev_task->m_parent_process);
					next_task = prev_task;
				}
			}

			for (it_task = m_ready_list.begin(); it_task != m_ready_list.end(); it_task++) {
				if ( (*it_task)->m_cpu_mask & (1 << cpu_id) ) { // Mask checking
					if((*it_task)->m_cpu == cpu_id || (*it_task)->m_cpu == -1) {
						if (prev_task == NULL) {
							weight = goodness((*it_task), cpu_id, NULL);
						}
						else {
							weight = goodness((*it_task), cpu_id, prev_task->m_parent_process);
						}
						if (weight > prio) {
							prio = weight;
							next_task = (*it_task);
						}
					}
				}
			}

			// If idle
			if (prio == -1000) {
				m_current_task = next_task;
				cpu->m_current_task = next_task;
				return next_thread;
			}

			// For OTHER policy
			// Counter == 0, update all exhausted counters and continue the reschedule
			if (prio == -1) {
				for (it_task = m_ready_list.begin(); it_task != m_ready_list.end(); it_task++) {
					if(  (*it_task)->m_state!= WAITING && (*it_task)->m_state!= READY ){
						cout<<"Error when scheduling: task not ready in ready queue"<<endl;
						assert(0);
					}
					if( ( (*it_task)->m_cpu_mask & (1 << cpu_id) ) &&
						( (*it_task)->m_cpu == cpu_id || (*it_task)->m_cpu == -1 ) )
					{
						(*it_task)->m_RR_counter = ((*it_task)->m_RR_counter >> 1) +
								NICE_TO_TICKS((*it_task)->get_nice());
					}
				}
				continue;
			}

			break;
		}
		check_system_status();
	}

	next_task->m_cpu = cpu_id;
	next_task->m_priority = prio;
	m_current_task = next_task;
	cpu->m_current_task = next_task;

	next_thread = next_task->m_thread_scheduler->reschedule();

	next_thread->m_cpu = cpu_id;

	if (next_thread != prev_thread) {
		if (next_thread != cpu->m_interruptions_thread) {
			cpu->m_statistics->m_thread_switches++;
			next_thread->annotate_rtos_time(THREAD_SWITCH_TIME, THREAD_SWITCH_INSTR);
		}
		// If next task is not the current task and current task is in executable state, it is added to the ready list.
		if (prev_thread != NULL) {

			if(prev_thread->m_state == SUPER_USER){
				prev_thread->set_next_state( prev_thread->m_state, BLOCKED );
				prev_thread->update_state();
				prev_thread->set_next_state( prev_thread->m_state, READY);
				prev_thread->update_state();
			} else {
				prev_thread->set_next_state( prev_thread->m_state, SUPER_USER);
				prev_thread->update_state();
				prev_thread->set_next_state( prev_thread->m_state, WAITING);
				prev_thread->update_state();
			}
		}
	}

	if (prev_task != next_task) {
		if ((next_task != cpu->m_interruptions_task) && (prev_task != cpu->m_interruptions_task)) {
			cpu->m_statistics->m_context_switches++;
			next_thread->annotate_rtos_time(CONTEXT_SWITCH_TIME, CONTEXT_SWITCH_INSTR);
			// When the current task changes it is required a cache flush
			if(cpu->m_icache != NULL){
				cpu->m_icache->flush();
			}
			if(cpu->m_dcache != NULL){
				cpu->m_dcache->flush();
			}
		}
		if (prev_task != NULL) {
			prev_task->m_cpu = -1;
		}
	}


	// Once selected the next thread, it must be resumed: go to User or Super_user state
	if (next_thread->m_kernel_mode == true) {
		if (next_thread->m_state != SUPER_USER) {
			next_thread->set_next_state(READY, SUPER_USER);
			next_thread->update_state();
		}
	}
	else {
		next_thread->set_next_state(WAITING, USER);
		next_thread->update_state();
	}

	next_task->m_current_thread = next_thread;
	next_task->m_cpu = cpu->m_id;

	return next_thread;
}

/**
 * \brief Deletes a task from a given list
 *
 * \param task The task to delete
 * \param list The list to delete from
 * \return Returns true if success and false if failure
 */
bool UC_task_scheduler_class::delete_from_list(UC_task_class *task, vector<UC_task_class *> *list) {
	vector<UC_task_class *>::iterator it_task;
	for (it_task = list->begin(); it_task != list->end(); it_task++) {
		if (*it_task == task) {
			list->erase(it_task);
			return true;
		}
	}

	return false;
}

/**
 * \brief Changes the state of execution of a task
 *
 * Changes the task from the initial list to the final one. \n
 * If all tasks of the current process are ZOMBIE, it destroys the host, POSIX thread
 * \param task The task to change the state
 * \param initial_state The initial state of the task
 * \param final_state The final state of the task
 * \return Returns true if success and false if failure
 */
bool UC_task_scheduler_class::change_state(UC_task_class *task, UC_process_states initial_state, UC_process_states final_state) {

	int ret;
	int cpu_id;
	UC_cpu_class *cpu;
	UC_process_class *process = task->m_parent_process;

	// Modified: When moving from SU to ready list, it must be placed at the beginning of the list (Preemptions)
	if (initial_state == SUPER_USER) {
		if (final_state == WAITING || final_state == READY ) {
			ret = change_list_task_begin(task, get_list_from_state(initial_state), get_list_from_state(final_state));
		}
		else{
			change_list_task(task, get_list_from_state(initial_state), get_list_from_state(final_state));
		}
	}
	else{
		change_list_task(task, get_list_from_state(initial_state), get_list_from_state(final_state));
	}

	if (final_state == ZOMBIE) {
		m_zombie_list.erase(m_zombie_list.begin());

		delete_from_list(task, &process->m_task_list);
		cpu_id = task->m_cpu;
		cpu = task->m_parent_process->m_parent_rtos->get_processor(cpu_id);
		m_current_task = NULL;
		cpu->m_current_task = NULL;

		delete task;

		if (process->m_task_list.size() == 0) {
			// All tasks are ZOMBIE, destroying the host, POSIX thread
			process->m_stp_destroy = true;
			process->m_stp = NULL;
			pthread_mutex_unlock(&process->m_process_mutex);
			pthread_mutex_lock(&process->m_parent_rtos->m_rtos_mutex);
		}
	}
	return ret;
}

/**
 * \brief Changes a task from a list to another
 *
 * \param task The task to change
 * \param initial_list The list to search the task into
 * \param final_list The list in which insert the task
 * \return Returns true if success and false if failure
 */
bool UC_task_scheduler_class::change_list_task(UC_task_class *task, vector<UC_task_class *> *initial_list,
		 										vector<UC_task_class *> *final_list)
{
	for (vector<UC_task_class *>::iterator it_task = initial_list->begin(); it_task != initial_list->end(); it_task++) {
		if (*it_task == task) {
			initial_list->erase(it_task);
			final_list->push_back(task);
			return true;
		}
	}

	return false;
}
/**
 * \brief Changes a task from a list to the beginning of another list
 *
 * \param task The task to change
 * \param initial_list The list to search the task into
 * \param final_list The list in which insert the task
 * \return Returns true if success and false if failure
 */
bool UC_task_scheduler_class::change_list_task_begin(UC_task_class *task, vector<UC_task_class *> *initial_list,
		 										vector<UC_task_class *> *final_list)
{
	for (vector<UC_task_class *>::iterator it_task = initial_list->begin(); it_task != initial_list->end(); it_task++) {
		if (*it_task == task) {
			initial_list->erase(it_task);
			final_list->insert(final_list->begin(),task);
			return true;
		}
	}

	return false;
}
			

/**
 * \brief Gets the correspondent list from a given process state
 *
 * \param state The state to get the list from
 * \return Returns a pointer to the correspondent list
 */
vector<UC_task_class *> * UC_task_scheduler_class::get_list_from_state(UC_process_states state) {
	switch (state) {
		case ZOMBIE:
			return &m_zombie_list;
		case BLOCKED:
			return &m_blocked_list;
		case WAITING:
			return &m_ready_list;
		case CREATED:
			return &m_created_list;
		case READY:
			return &m_ready_list;
		case USER:
			return &m_user_list;
		case SUPER_USER:
			return &m_super_user_list;
		default:
			return NULL;
	}
}

/**
 * \brief Calculates the real priority of a task
 *
 * \param task The task to calculate the priority
 * \param cpu_id The cpu identifier which is in reschedule state
 * \param process The process of the current running task
 * \return The real priority of the given task
 */
int UC_task_scheduler_class::goodness(struct UC_task_class *task, int cpu_id, struct UC_process_class *process)
{
	int weight = -1;

	if (task->m_RR_counter < 0) {
		return -1;
	}

	// RT process
	if (task->m_policy != SCHED_OTHER) {
		weight = 1000 + task->m_schedparam.sched_priority;

#if KERNEL_PRIORITY == 1
		if (task->m_kernel_mode == true) {
			weight += 2000;
		}
#endif

// 		if(task->m_parent_process->signals_control.sigpending(NULL,NULL) ||
// 				 task->m_parent_process->signals_control.in_signal)
// 		{
// 			weight+=4000;
// 		}
		return weight;
	}

	// NOT-RT process
	weight = task->m_RR_counter * TIME_SLICE_PENALTY ;

	if (weight < 0) {
		return weight;
	}
/*
	// Give an advantage to the same process
	if (task->m_parent_process == process) {
		// NOTE previous version: weight += CHANGE_MM_PENALTY; ==> the current process never leaves the cpu
		//      when the other processes have the same priority (review)
		weight += CHANGE_MM_PENALTY;
	}
*/
	weight += (20 - task->get_nice()) * NICE_PENALTY;

	if(weight > 999) {
		weight = 999;
	}

#if KERNEL_PRIORITY == 1
	if (task->m_kernel_mode == true) {
		weight += 2000;
	}
#endif

// 	if(task->m_parent_process->signals_control.sigpending(NULL,NULL) ||
// 		  task->pr_table->signals_control.in_signal)
// 	{
// 		weight+=4000;
// 	}
	return weight;
}

/**
 * \brief Debug function that prints the size of all task lists
 */
void UC_task_scheduler_class::debug_lists_size() {
	cout << "Created   list size: " << m_created_list.size() << endl;
	cout << "Ready     list size: " << m_ready_list.size() << endl;
	cout << "Waiting   list size: " << m_waiting_list.size() << endl;
	cout << "User      list size: " << m_user_list.size() << endl;
	cout << "Superuser list size: " << m_super_user_list.size() << endl;
	cout << "Blocked   list size: " << m_blocked_list.size() << endl;
	cout << "Zombie    list size: " << m_zombie_list.size() << endl;
}

void UC_task_scheduler_class::check_system_status(){
	bool ok=true, flag;
	UC_process_states state;
	UC_thread_class *thread;
	UC_task_class *task;
	UC_process_class *process;

	if(!uc_get_check_system_status_flag()){
		return;
	}

	for(int i=0;i<m_created_list.size();i++){
		if(m_created_list[i]->m_state != CREATED){
			ok=false;
			cout<<"Wrong task in create list "<<endl;
		}
	}
	for(int i=0;i<m_ready_list.size();i++){
		if(m_ready_list[i]->m_state != READY && m_ready_list[i]->m_state != WAITING){
			ok=false;
			cout<<"Wrong task in ready list "<<endl;
		}
	}
/*	for(int i=0;i<m_waiting_list.size();i++){
		if(m_waiting_list[i]->m_state != WAITING){
			ok=false;
			cout<<"Wrong task in waiting list "<<endl;
		}
	}*/
	for(int i=0;i<m_user_list.size();i++){
		if(m_user_list[i]->m_state != USER){
			ok=false;
			cout<<"Wrong task in user list "<<endl;
		}
	}
	for(int i=0;i<m_super_user_list.size();i++){
		if(m_super_user_list[i]->m_state != SUPER_USER){
			ok=false;
			cout<<"Wrong task in super user list "<<endl;
		}
	}
	for(int i=0;i<m_blocked_list.size();i++){
		if(m_blocked_list[i]->m_state != BLOCKED){
			ok=false;
			cout<<"Wrong task in bloked list "<<endl;
		}
	}
	for(int i=0;i<m_zombie_list.size();i++){
		if(m_zombie_list[i]->m_state != ZOMBIE){
			ok=false;
			cout<<"Wrong task in zombie list "<<endl;
		}
	}


	
	for(int i=0; i<m_parent_rtos->m_process_list.size();i++){
		process = m_parent_rtos->m_process_list[i];
		for(int j=0; j<process->m_task_list.size();j++){
			task = process->m_task_list[j];
			flag = false;
			switch (task->m_state){
				case SUPER_USER:
					for(int k=0;k<m_super_user_list.size();k++){
						if(m_super_user_list[k] == task){
							flag = true;
							break;
						}
					}
					if(!flag){
						ok=false;
						cout<<"Task in Super User not in the list "<<" i "<<i<<" j "<<j <<endl;
					}
					break;
				case USER:
					for(int k=0;k<m_user_list.size();k++){
						if(m_user_list[k] == task){
							flag = true;
							break;
						}
					}
					if(!flag){
						ok=false;
						cout<<"Task in User not in the list "<<" i "<<i<<" j "<<j <<endl;
					}
					break;
				case WAITING:
/*					for(int k=0;k<m_waiting_list.size();k++){
						if(m_waiting_list[k] == task){
							flag = true;
							break;
						}
					}
					if(!flag){
						ok=false;
						cout<<"Task in Waiting not in the list "<<" i "<<i<<" j "<<j <<endl;
					}
					break;*/
				case READY:
					for(int k=0;k<m_ready_list.size();k++){
						if(m_ready_list[k] == task){
							flag = true;
							break;
						}
					}
					if(!flag){
						ok=false;
						cout<<"Task in ready not in the list "<<" i "<<i<<" j "<<j <<endl;
					}
					break;
				case BLOCKED:
					for(int k=0;k<m_blocked_list.size();k++){
						if(m_blocked_list[k] == task){
							flag = true;
							break;
						}
					}
					if(!flag){
						ok=false;
						cout<<"Task in blocked not in the list "<<" i "<<i<<" j "<<j <<endl;
					}
					break;
				case CREATED:
					for(int k=0;k<m_created_list.size();k++){
						if(m_created_list[k] == task){
							flag = true;
							break;
						}
					}
					if(!flag){
						ok=false;
						cout<<"Task in created not in the list "<<" i "<<i<<" j "<<j <<endl;
					}
					break;
				case ZOMBIE:
					for(int k=0;k<m_zombie_list.size();k++){
						if(m_zombie_list[k] == task){
							flag = true;
							break;
						}
					}
					if(!flag){
						ok=false;
						cout<<"Task in zombie not in the list "<<" i "<<i<<" j "<<j <<endl;
					}
					break;
			}
			state = ZOMBIE;
			for(int k=0; k<task->m_thread_list.size();k++){
				thread = task->m_thread_list[k];
				if( thread->m_state > state ){
					state = thread->m_state;
				}
				flag = false;
				switch (thread->m_state){
					case READY:
					case WAITING:
						for(int l=0;l<task->m_thread_scheduler->m_ready_thread_list.size();l++){
							if(task->m_thread_scheduler->m_ready_thread_list[l] == thread){
								flag = true;
								break;
							}
						}
						if(!flag){
							ok=false;
							cout<<"Thread in Ready not in the list "<<" i "<<i<<" j "<<j <<" k "<<k<<endl;
						}
						break;
					case ZOMBIE:
						for(int l=0;l<task->m_thread_scheduler->m_zombie_thread_list.size();l++){
							if(task->m_thread_scheduler->m_zombie_thread_list[l] == thread){
								flag = true;
								break;
							}
						}
						if(!flag){
							ok=false;
							cout<<"Thread in Zombie not in the list "<<" i "<<i<<" j "<<j<<" k "<<k<<endl;
						}
						break;
					default:
						for(int l=0;l<task->m_thread_scheduler->m_noready_thread_list.size();l++){
							if(task->m_thread_scheduler->m_noready_thread_list[l] == thread){
								flag = true;
								break;
							}
						}
						if(!flag){
							ok=false;
							cout<<"Thread not in the default list with status: "<<thread->m_state <<" i "<<i<<" j "<<j<<" k "<<k<<endl;
						}
				}
			}
			
			if(state != task->m_state){
				ok=false;
				cout<<"Task state ("<< task->m_state<< ") does not correspond with the higher thread status ("<<state<<")"<<" i "<<i<<" j "<<j<<endl;
			}

			for(int k=0;k<task->m_thread_scheduler->m_ready_thread_list.size();k++){
				if(task->m_thread_scheduler->m_ready_thread_list[k]->m_state != READY && task->m_thread_scheduler->m_ready_thread_list[k]->m_state != WAITING ){
					ok=false;
					cout<<"Wrong thread in ready list. Status: "<<task->m_thread_scheduler->m_ready_thread_list[k]->m_state<<" i "<<i<<" j "<<j<<" k "<<k<<endl;
				}
			}
			for(int k=0;k<task->m_thread_scheduler->m_zombie_thread_list.size();k++){
				if(task->m_thread_scheduler->m_zombie_thread_list[k]->m_state != ZOMBIE ){
					ok=false;
					cout<<"Wrong thread in zombie list "<<" i "<<i<<" j "<<j<<" k "<<k<<endl;
				}
			}
		}
	}
	if(!ok){
		uc_print_backtrace();
	}
	assert(ok);
}

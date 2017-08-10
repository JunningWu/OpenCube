//==========================================================================
//	uc_sched.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié ene 16 2008
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

#include <errno.h>
#include "rtos/api/posix/uc_sched.h"
#include "rtos/kernel/uc_thread_class.h"
#include "rtos/kernel/uc_os_defines.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/api/posix/uc_posix.h"

extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;

int UC_posix_class::uc_sched_setparam(pid_t pid, const struct uc_sched_param *param) {
	UC_process_class *process;
	UC_task_class *task;
	int policy;

	if (param == NULL) {
		return EINVAL;
	}
	if (pid == 0) {
		process = qt_parent_process;
	}
	else {
		process = qt_parent_rtos->get_process(pid);
	}

	if (process == NULL) {
		errno = ESRCH;
		return -1;
	}

	policy = sched_getscheduler(pid);
	if (param->sched_priority > sched_get_priority_max(policy) || param->sched_priority < sched_get_priority_min(policy)) {
		errno = EINVAL;
		return -1;
	}

	task = process->m_task_list[0];
	if(task == NULL){
		errno= EPERM;
		return -1;
	}

	task->m_schedparam = *param;
	return 0;
}

int UC_posix_class::uc_sched_getparam(pid_t pid, struct uc_sched_param *param){
	UC_process_class *process;
	UC_task_class *task;

	if (param == NULL) {
		return EINVAL;
	}

	if (pid == 0) {
		process = qt_parent_process;
	}
	else {
		process = qt_parent_rtos->get_process(pid);
	}

	if (process == NULL) {
		return -1;
	}

	task = process->m_task_list[0];
	if (task == NULL) {
		errno = EPERM;
		return -1;
	}

	*param = task->m_schedparam;
	return 0;
}

int UC_posix_class::uc_sched_getscheduler(pid_t pid) {
	UC_process_class *process;
	UC_task_class *task;

	if (pid == 0) {
		process = qt_parent_process;
	}
	else {
		process = qt_parent_rtos->get_process(pid);
	}

	if (process == NULL) {
		return -1;
	}

	task = process->m_task_list[0];
	if (task == NULL) {
		errno = EPERM;
		return -1;
	}

	return task->m_policy;
}

int UC_posix_class::uc_sched_setscheduler(pid_t pid, int policy, const struct uc_sched_param *param){
	UC_process_class *process;
	UC_task_class *task;

	if (param == NULL) {
		errno = EINVAL;
		return -1;
	}

	if (pid == 0) {
		process = qt_parent_process;
	}
	else {
		process = qt_parent_rtos->get_process(pid);
	}

	if (process == NULL) {
		errno = ESRCH;
		return -1;
	}

	if (param->sched_priority > sched_get_priority_max(policy) || param->sched_priority < sched_get_priority_min(policy)) {
		errno = EINVAL;
		return -1;
	}

	task = process->m_task_list[0];
	if (task == NULL) {
		errno = EPERM;
		return -1;
	}

	int ret = task->m_policy;
	task->m_policy = policy;
	task->m_schedparam = *param;
	return ret;
}

int UC_posix_class::uc_sched_yield() {
	// This function only returns 0 because it must be called directly from user code.
	// After the return, the thread is moved from SUPER_USER to WAITING.
	// No SUPER_USER function must call uc_sched_yield.
	return 0;
}

int UC_posix_class::uc_sched_get_priority_max(int policy) {
	switch (policy) {
		case 1:
			return PTHREAD_MAX_PRIORITY_1;
		case 2:
			return PTHREAD_MAX_PRIORITY_2;
		case 3:
			return PTHREAD_MAX_PRIORITY_3;
		default:
			errno = EINVAL;
			return -1;
	}
}

int UC_posix_class::uc_sched_get_priority_min(int policy){
	switch (policy) {
		case 1:
			return PTHREAD_MIN_PRIORITY_1;
		case 2:
			return PTHREAD_MIN_PRIORITY_2;
		case 3:
			return PTHREAD_MIN_PRIORITY_3;
		default:
			errno = EINVAL;
			return -1;
	}
}

/**
 * \brief Return the time slice size of the process of the corresponding pid.\nThe time corresponds to the task 0.
 *
 * \param pid process id to get the interval
 * \param tp struct where the interval is returned
 * \return 0 if ok. -1 otherwise.
 */

int UC_posix_class::uc_sched_rr_get_interval(pid_t pid, struct timespec *tp){

	UC_process_class *process;
	double time;

	if (tp == NULL) {
		return EINVAL;
	}
	if (pid == 0) {
		process = qt_parent_process;
	}
	else {
		process = qt_parent_rtos->get_process(pid);
	}

	if (process == NULL) {
		errno = ESRCH;
		return -1;
	}

	if (process->m_task_list.size() == 0) {
		errno = ESRCH;
		return -1;
	}

	time = NICE_TO_TICKS(process->m_task_list[0]->get_nice());
	time /= TICK_TIMER_HZ;
	tp->tv_sec = (int)time;
	time -= tp->tv_sec;
	tp->tv_nsec = (int)(time*1e9);
	return 0;
}

/**
 * \brief Set the processor mask of all tasks of the corresponding process
 *
 * The affinity mask is actually a per-task attribute that can be adjusted
 * independently for each of the task in a process.
 * The  value  returned  from a call to gettid(2) can be passed in the argument pid.
 * Specifying pid as 0 will set the attribute for the current task, and passing
 * the value returned from a call to getpid(2) will set the attribute for the main
 * task of the process. Threads of a single task always have the same mask, since 
 * mask is associanted to task, not to threads.
 *
 * \param pid processor id
 * \param len mask length
 * \param masl processor mask
 * \return 0 if OK. -1 otherwise.
 */
int UC_posix_class::uc_sched_setaffinity(pid_t pid, unsigned int len, unsigned long *mask){
	UC_process_class *process;
	UC_thread_class *thread;
	unsigned long new_mask;
	int i;

	if (pid < 0) {
		errno = ESRCH;
		return -1;
	}

	switch (len) {
		case 1:
			new_mask = (*mask) & 0xff;
			break;
		case 2:
			new_mask = (*mask) & 0xffff;
			break;
		case 3:
			new_mask = (*mask) & 0xffffff;
			break;
		case 4:
			new_mask = (*mask) & 0xffffffff;
			break;
		default:
			errno = EINVAL;
			return -1;
	}

	if (pid == 0) {
		qt_parent_rtos->get_current_cpu()->m_current_task->m_cpu_mask = new_mask;
		return 0;
	}
	else {
		process = qt_parent_rtos->get_process(pid);
		if(process != NULL){
			process->m_task_list[0]->m_cpu_mask = new_mask;
			return 0;
		}
		else{
			thread = qt_parent_rtos->get_thread(pid);
			if(thread != NULL){
				thread->m_parent_task->m_cpu_mask = new_mask;
				return 0;
			}
		}
	}

	// If not process or thread found with this pid/tid
	errno = ESRCH;
	return -1;
}

/**
 * \brief Get the processor mask of the first task of the corresponding process
 *
 * \param pid processor id
 * \param len mask length
 * \param masl processor mask
 * \return 0 if OK. -1 otherwise.
 */
int UC_posix_class::uc_sched_getaffinity(pid_t pid, unsigned int len, unsigned long *mask){
	UC_process_class *process;
	UC_thread_class *thread;
	unsigned long get_mask;

	if (pid < 0) {
		errno = ESRCH;
		return -1;
	}

	if (pid == 0) {
		get_mask = qt_parent_rtos->get_current_cpu()->m_current_task->m_cpu_mask;
	}
	else {
		process = qt_parent_rtos->get_process(pid);
		if(process != NULL){
			get_mask = process->m_task_list[0]->m_cpu_mask;
		}
		else{
			thread = qt_parent_rtos->get_thread(pid);
			if(thread != NULL){
				get_mask = thread->m_parent_task->m_cpu_mask;
			}
			else{
				// If not process or thread found with this pid/tid
				errno = ESRCH;
				return -1;
			}
		}
	}


	switch (len) {
		case 1:
			*mask = get_mask & 0xff;
			break;
		case 2:
			*mask = get_mask & 0xffff;
			break;
		case 3:
			*mask = get_mask & 0xffffff;
			break;
		case 4:
			*mask = get_mask & 0xffffffff;
			break;
		default:
			errno = EINVAL;
			return -1;
	}

	return 0;
}

struct uc_sched_param * UC_posix_class::uc_schedparam_init(){
	struct uc_sched_param *param;
	param = new uc_sched_param;
	param->sched_priority = NZERO;
	return param;
}

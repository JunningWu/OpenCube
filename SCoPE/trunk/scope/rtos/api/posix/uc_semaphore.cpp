//==========================================================================
//	uc_semaphore.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: jue ene 17 2008
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
#include "rtos/api/posix/uc_semaphore.h"
#include "rtos/api/posix/uc_posix.h"
// #include <stdarg.h>
#include <fcntl.h>

extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;

int uc_sem_class::unblock(UC_thread_class *thread) {
	counter++;
	vector<UC_thread_class *>::iterator it_thread;
	for (it_thread = blocked_threads.begin(); it_thread != blocked_threads.end(); it_thread++) {
		if (*it_thread == thread) {
			blocked_threads.erase(it_thread);
			break;
		}
	}
	return 0;
}

int UC_posix_class::uc_sem_close(uc_sem_t *sem_id){
	uc_sem_class *sem;
	if ((sem = qt_parent_rtos->find_sem(*sem_id)) == NULL) {
		 return -1;
	}

	vector<uc_sem_t *>::iterator it_sem_id;
	for (it_sem_id = qt_parent_process->Semaphores_List.begin(); it_sem_id != qt_parent_process->Semaphores_List.end(); it_sem_id++) {
		if (*it_sem_id == sem_id) {
			qt_parent_process->Semaphores_List.erase(it_sem_id);
			break;
		}
	}

	vector<uc_sem_class *>::iterator it_sem;
	for (it_sem = qt_parent_rtos->Semaphores_List.begin(); it_sem != qt_parent_rtos->Semaphores_List.end(); it_sem++) {
		if (*it_sem == sem) {
			qt_parent_rtos->Semaphores_List.erase(it_sem);
			break;
		}
	}
	return 0;
}

int UC_posix_class::uc_sem_destroy(uc_sem_t *sem_id){
	uc_sem_class *sem;
	if ((sem = qt_parent_rtos->find_sem(*sem_id)) == NULL) {
		return -1;
	}
	if (sem->open_sem.size() == 0) {
		vector<uc_sem_t *>::iterator it_sem_id;
		for (it_sem_id = qt_parent_process->Semaphores_List.begin(); it_sem_id != qt_parent_process->Semaphores_List.end(); it_sem_id++) {
			if (*it_sem_id == sem_id) {
				qt_parent_process->Semaphores_List.erase(it_sem_id);
				break;
			}
		}
	}

	vector<uc_sem_class *>::iterator it_sem;
	for (it_sem = qt_parent_rtos->Semaphores_List.begin(); it_sem != qt_parent_rtos->Semaphores_List.end(); it_sem++) {
		if (*it_sem == sem) {
			qt_parent_rtos->Semaphores_List.erase(it_sem);
			break;
		}
	}
	return 0;
}

int UC_posix_class::uc_sem_getvalue(uc_sem_t *sem_id, int *val){
	uc_sem_class *sem;
	if ((sem = qt_parent_rtos->find_sem(*sem_id)) == NULL) {
		return -1;
	}
	*val = sem->counter;
	return 0;
}

int UC_posix_class::uc_sem_init(uc_sem_t *sem_id, int pshared, int value){
	uc_sem_class *sem;

	if (value > SEM_VALUE_MAX) {
		errno = EINVAL;
		return -1;
	}

//	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
/*
	if(thread!=NULL && thread->in_fork){
		thread->semaphore_list_fork.extract_first_element(*sem_id);
		thread->semaphore_list_fork.add_element(*sem_id);
		return 0;
	}
*/
	sem = new uc_sem_class;
	sem->pshared = pshared;
	sem->counter = value;
	sem->name = NULL;
	sem->sem_id = qt_parent_rtos->m_sem_id_max++;
	sem->parent_process = qt_parent_process;
	*sem_id = sem->sem_id;

	qt_parent_rtos->Semaphores_List.push_back(sem);
// 	if(UC_PERFIDIX_Kernel.simulation_started){
		qt_parent_process->Semaphores_List.push_back(sem_id);
//		thread->m_semaphore_list_fork.push_back(*sem_id);
// 	}
	return 0;
}

uc_sem_t *UC_posix_class::uc_sem_open(const char *name, int flags){
	uc_sem_class *sem;
	int exists = 0;

//	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
/*
	if(thread!=NULL && thread->in_fork){
		int *sem_id= new int;
		thread->semaphore_list_fork.extract_first_element(*sem_id);
		thread->semaphore_list_fork.add_element(*sem_id);
		return sem_id;
	}
*/
	vector<uc_sem_class *>::iterator it_sem;
	for (it_sem = qt_parent_rtos->Semaphores_List.begin(); it_sem != qt_parent_rtos->Semaphores_List.end(); it_sem++) {
		if (strcmp((*it_sem)->name, name) == 0) {
			exists = 1;
			sem = *it_sem;
			break;
		}
	}

	if(exists) {
		if (flags & O_EXCL) {
			errno = EEXIST;
			return SEM_FAILED;
		}
		else {
			return &sem->sem_id;
		}
	}

	if (flags & O_CREAT) {
		sem = new uc_sem_class;
		sem->counter = 0;
		sem->pshared = 1;
		sem->sem_id = qt_parent_rtos->m_sem_id_max++;
		sem->parent_process = qt_parent_process;
		sem->name = new char[strlen(name)+1];
		strcpy(sem->name, name);
		qt_parent_rtos->Semaphores_List.push_back(sem);
//		thread->m_semaphore_list_fork.push_back(sem->sem_id);
		return &sem->sem_id;
	}
	errno = ENOENT;
	return SEM_FAILED;
}

uc_sem_t *UC_posix_class::uc_sem_open(const char *name, int flags, int mode, int value){
	uc_sem_class *sem;
	int exists = 0;

//	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
/*
	if(thread!=NULL && thread->in_fork){
		int *sem_id= new int;
		thread->semaphore_list_fork.extract_first_element(*sem_id);
		thread->semaphore_list_fork.add_element(*sem_id);
		return sem_id;
	}
*/
	vector<uc_sem_class *>::iterator it_sem;
	for (it_sem = qt_parent_rtos->Semaphores_List.begin(); it_sem != qt_parent_rtos->Semaphores_List.end(); it_sem++) {
		if (strcmp((*it_sem)->name, name) == 0) {
			exists = 1;
			sem = *it_sem;
			break;
		}
	}

	if (exists) {
		if (flags & O_EXCL) {
			errno = EEXIST;
			return SEM_FAILED;
		}
		else {
			return &sem->sem_id;
		}
	}

	if (flags & O_CREAT) {
		if (value > SEM_VALUE_MAX) {
			errno = EINVAL;
			return SEM_FAILED;
		}
		sem = new uc_sem_class;
		sem->counter = value;
		sem->pshared = 1;
		sem->sem_id = qt_parent_rtos->m_sem_id_max++;
		sem->parent_process = qt_parent_process;
		sem->name = new char[strlen(name)+1];
		strcpy(sem->name, name);
		qt_parent_rtos->Semaphores_List.push_back(sem);
//		thread->m_semaphore_list_fork.push_back(sem->sem_id);
		return &sem->sem_id;
	}
	errno = ENOENT;
	return SEM_FAILED;
}

int UC_posix_class::uc_sem_post(uc_sem_t *sem_id){
/*
	if(uc_get_curr_thread()->in_fork) return 0;
*/
	uc_sem_class *sem;
	if ((sem = qt_parent_rtos->find_sem(*sem_id)) == NULL) {
		errno = ENOENT;
		return -1;
	}

	if (sem == NULL) {
		return -1;
	}

	sem->counter++;
	while (sem->blocked_threads.size() != 0) {
		sem->counter++;
		sem->blocked_threads[0]->set_microcos_unblocker(UC_thread_class::SEMAPHORE, *sem_id);
		sem->blocked_threads[0]->unblock();
	}

	return 0;
}

int UC_posix_class::uc_sem_timedwait(uc_sem_t *sem_id, const struct timespec *abs_time){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
/*
	if (thread->in_fork) return 0;
*/
	uc_sem_class *sem;
	if ((sem = qt_parent_rtos->find_sem(*sem_id)) == NULL) {
		errno = ENOENT;
		return -1;
	}

	if (abs_time->tv_sec < 0) {
		errno = EINVAL;
		return -1;
	}

	if (abs_time->tv_nsec < 0 || abs_time->tv_nsec >= 1000000000) {
		errno = EINVAL;
		return -1;
	}

	if (sem == NULL) {
		return -1;
	}

	while (sem->counter <= 0) {
		sem->counter--;
		thread->block(&sem->blocked_threads, abs_time);

		if (thread->m_timeout_expired) {
			sem->counter++;
			errno = ETIMEDOUT;
			return -1;
		}
/*
		if (thread->m_signal_executed) {
			errno = EINTR;
			return -1;
		}
*/
	}
	sem->counter--;

	return 0;
}

int UC_posix_class::uc_sem_trywait(uc_sem_t *sem_id){
/*
	if(uc_get_curr_thread()->in_fork) return 0;
*/
	if (sem_id == NULL) {
		errno = EINVAL;
		return -1;
	}

	uc_sem_class *sem;
	if ((sem = qt_parent_rtos->find_sem(*sem_id)) == NULL) {
		errno = ENOENT;
		return -1;
	}

	if (sem->counter <= 0) {
		errno = EAGAIN;
		return -1;
	}

	sem->counter--;
	return 0;
}

int UC_posix_class::uc_sem_unlink(const char *name){
	uc_sem_class *sem;
	int exists = 0;

	vector<uc_sem_class *>::iterator it_sem;
	for (it_sem = qt_parent_rtos->Semaphores_List.begin(); it_sem != qt_parent_rtos->Semaphores_List.end(); it_sem++) {
		if (strcmp((*it_sem)->name, name) == 0) {
			exists = 1;
			sem = *it_sem;
			break;
		}
	}

	if (exists) {
		vector<uc_sem_t *>::iterator it_sem_id;
		for (it_sem_id = qt_parent_process->Semaphores_List.begin(); it_sem_id != qt_parent_process->Semaphores_List.end(); it_sem_id++) {
			if (*it_sem_id == &sem->sem_id) {
				qt_parent_process->Semaphores_List.erase(it_sem_id);
				break;
			}
		}

		UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
		vector<UC_thread_class *>::iterator it_thread;
		for (it_thread = sem->open_sem.begin(); it_thread != sem->open_sem.end(); it_thread++) {
			if (*it_thread == thread) {
				sem->open_sem.erase(it_thread);
				break;
			}
		}
		return 0;
	}
	errno = ENOENT;
	return -1;
}

int UC_posix_class::uc_sem_wait(uc_sem_t *sem_id){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	uc_sem_class *sem;
	if ((sem = qt_parent_rtos->find_sem(*sem_id)) == NULL) {
		errno = ENOENT;
		return -1;
	}

	if (sem == NULL) {
		return -1;
	}

	while (sem->counter <= 0) {
		sem->counter--;
		thread->block(&sem->blocked_threads);

		if (thread->m_signal_executed) {
			errno = EINTR;
			return -1;
		}
	}
	sem->counter--;
	return 0;
}

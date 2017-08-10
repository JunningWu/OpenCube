//==========================================================================
//	uc_pthread_rwlock.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié ene 9 2008
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
#include "rtos/api/posix/uc_pthread_rwlock.h"
#include "rtos/api/posix/uc_pthread_rwlock_class.h"
#include "rtos/api/posix/uc_posix.h"
#include "rtos/kernel/uc_rtos_class.h"

extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;

uc_pthread_rwlock_class * UC_posix_class::get_uc_pthread_rwlock_t(int id){
	vector<uc_pthread_rwlock_class *>::iterator it_rwlock;
	for (it_rwlock = qt_parent_process->m_rwlock_list.begin(); it_rwlock != qt_parent_process->m_rwlock_list.end(); it_rwlock++) {
		if ((*it_rwlock)->id == id) {
			return *it_rwlock;
		}
	}
	return NULL;
}

struct lockdata * UC_posix_class::uc_get_rwlock_lockling_element(uc_pthread_rwlock_class *lock, UC_thread_class *thread){
	vector<struct lockdata *>::iterator it_lockdata;
	for (it_lockdata = lock->locking.begin(); it_lockdata != lock->locking.end(); it_lockdata++) {
		if ((*it_lockdata)->thread == qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread) {
			return *it_lockdata;
		}
	}
	return NULL;
}

int UC_posix_class::uc_pthread_rwlockattr_init(uc_pthread_rwlockattr_t *attr){
	attr->pshared = UC_PTHREAD_PROCESS_PRIVATE;
	return 0;
}

int UC_posix_class::uc_pthread_rwlockattr_destroy(uc_pthread_rwlockattr_t *attr) {
	return 0;
}

int UC_posix_class::uc_pthread_rwlockattr_getpshared(const uc_pthread_rwlockattr_t *attr, int *num){
	*num = UC_PTHREAD_PROCESS_PRIVATE;
	return 0;
}

int UC_posix_class::uc_pthread_rwlockattr_setpshared(uc_pthread_rwlockattr_t *attr, int num){
	if (num == UC_PTHREAD_PROCESS_PRIVATE) {
		return 0;
	}
	return -1;
}

int UC_posix_class::uc_pthread_rwlock_init(uc_pthread_rwlock_t *id_lock, const uc_pthread_rwlockattr_t *attr){
	if (id_lock == NULL) {
		return EINVAL;
	}
	if (id_lock->init != 0) {
		return EBUSY;
	}

	uc_pthread_rwlock_class *lock = new uc_pthread_rwlock_class();
	id_lock->id = qt_parent_process->m_rwlock_max++;
	id_lock->init = 1;
	lock->id = id_lock->id;
	if (attr == NULL) {
		lock->pshared.pshared = UC_PTHREAD_PROCESS_PRIVATE;
	}
	else{
		if (attr->pshared > 1 || attr->pshared < 0) {
			return EINVAL;
		}
		lock->pshared = *attr;
	}
	lock->locking.clear();
	lock->rdlock = false;
	lock->init = 1;
	lock->wrlock = false;
	qt_parent_process->m_rwlock_list.push_back(lock);
	return 0;
}

int UC_posix_class::uc_pthread_rwlock_destroy(uc_pthread_rwlock_t *id_lock){
	if (id_lock->init == 0) {
		return EINVAL;
	}
	uc_pthread_rwlock_class *lock = get_uc_pthread_rwlock_t(id_lock->id);
	if (lock == NULL) {
		return EINVAL;
	}
	if (lock->rdlock || lock->wrlock) {
		return EBUSY;
	}

	vector<uc_pthread_rwlock_class *>::iterator it_lock;
	for (it_lock = qt_parent_process->m_rwlock_list.begin(); it_lock != qt_parent_process->m_rwlock_list.end(); it_lock++) {
		if (*it_lock == lock) {
			qt_parent_process->m_rwlock_list.erase(it_lock);
			break;
		}
	}

	id_lock->init=0;
	return 0;
}

int UC_posix_class::uc_pthread_rwlock_rdlock(uc_pthread_rwlock_t *id_lock){
	int priority;
	bool continue_loop;

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	if (thread->m_in_rwlock > 0) {
		return EDEADLK;
	}

	if (id_lock == NULL) {
		return EINVAL;
	}

	if (id_lock->init == 0) {
		return EINVAL;
	}

	uc_pthread_rwlock_class *lock = get_uc_pthread_rwlock_t(id_lock->id);

	if (lock == NULL) {
		return EINVAL;
	}

	struct lockdata *ldata = NULL;
	if (lock->locking.size() != 0) {
		ldata = lock->locking[0];
	}

	if (ldata != NULL) {
		if (ldata->thread == thread && lock->wrlock) {
			return EDEADLK;
		}
	}

	while (true) {
		continue_loop = false;
		uc_pthread_testcancel();
		while (lock->wrlock != 0) {
			thread->block(&lock->blocked_threads);
		}

		priority = thread->m_parent_task->m_thread_scheduler->goodness(thread);

		vector<UC_thread_class *>::iterator it_thread;
		for (it_thread = lock->blocked_threads.begin(); it_thread != lock->blocked_threads.end(); it_thread++) {
			if (priority <= (*it_thread)->m_parent_task->m_thread_scheduler->goodness(*it_thread)) {
				thread->block(&lock->blocked_threads);
				continue_loop = true;
				break;
			}
		}

		if (continue_loop == false) {
			break;
		}
	}

	uc_pthread_testcancel();
	ldata = uc_get_rwlock_lockling_element(lock, thread);

	if (ldata == NULL) {
		ldata = new struct lockdata;
		ldata->counter = 0;
		ldata->thread = thread;
		lock->locking.push_back(ldata);
	}

	ldata->counter++;
	lock->rdlock = true;
	thread->m_in_rwlock--;
	thread->m_rwlock_holds.push_back(lock);

	return 0;
}

int UC_posix_class::uc_pthread_rwlock_tryrdlock(uc_pthread_rwlock_t *id_lock){
	if (id_lock == NULL) {
		return EINVAL;
	}

	if (id_lock->init != 0) {
		return EINVAL;
	}

	uc_pthread_rwlock_class *lock = get_uc_pthread_rwlock_t(id_lock->id);
	if (lock == NULL) {
		return EINVAL;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if (thread->m_in_rwlock > 0) {
		return EDEADLK;
	}

	if (lock->wrlock != 0) {
		return EBUSY;
	}

	int priority = thread->m_parent_task->m_thread_scheduler->goodness(thread);

	vector<UC_thread_class *>::iterator it_thread;
	for (it_thread = lock->blocked_threads.begin(); it_thread != lock->blocked_threads.end(); it_thread++) {
		if (priority <= (*it_thread)->m_parent_task->m_thread_scheduler->goodness(*it_thread)) {
			return EBUSY;
		}
	}

	struct lockdata *ldata = uc_get_rwlock_lockling_element(lock, thread);

	if (ldata == NULL) {
		ldata = new struct lockdata;
		ldata->counter = 0;
		ldata->thread = thread;
		lock->locking.push_back(ldata);
	}

	thread->m_in_rwlock--;
	ldata->counter++;
	lock->rdlock = true;
	thread->m_rwlock_holds.push_back(lock);

	return 0;
}

int UC_posix_class::uc_pthread_rwlock_timedrdlock(uc_pthread_rwlock_t *id_lock, const struct timespec *abs_time){
	bool continue_loop = false;

	if (id_lock == NULL) {
		return EINVAL;
	}

	if (id_lock->init == 0) {
		return EINVAL;
	}

	uc_pthread_rwlock_class *lock = get_uc_pthread_rwlock_t(id_lock->id);
	if (lock == NULL) {
		return EINVAL;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	if (thread->m_in_rwlock > 0) {
		return EDEADLK;
	}

	struct lockdata *ldata = NULL;
	if (lock->locking.size() != 0) {
		ldata = lock->locking[0];
	}

	if (ldata != NULL) {
		if (ldata->thread == thread && lock->wrlock) {
			return EDEADLK;
		}
	}

	while (true) {
		uc_pthread_testcancel();
		while (lock->wrlock != 0) {
			thread->block(&lock->blocked_threads, abs_time);

			vector<UC_thread_class *>::iterator it_thread;
			for (it_thread = lock->blocked_threads.begin(); it_thread != lock->blocked_threads.end(); it_thread++) {
				if (*it_thread == thread) {
					lock->blocked_threads.erase(it_thread);
					break;
				}
			}

			if (thread->m_timeout_expired == true) {
				return ETIMEDOUT;
			}
		}

		int priority = thread->m_parent_task->m_thread_scheduler->goodness(thread);

		vector<UC_thread_class *>::iterator it_thread;
		for (it_thread = lock->blocked_threads.begin(); it_thread != lock->blocked_threads.end(); it_thread++) {
			if (priority <= (*it_thread)->m_parent_task->m_thread_scheduler->goodness(*it_thread)) {
				thread->block(&lock->blocked_threads, abs_time);

				vector<UC_thread_class *>::iterator it_thread2;
				for (it_thread2 = lock->blocked_threads.begin(); it_thread2 != lock->blocked_threads.end(); it_thread2++) {
					if (*it_thread2 == thread) {
						lock->blocked_threads.erase(it_thread2);
						break;
					}
				}

				if(thread->m_timeout_expired == true) {
					return ETIMEDOUT;
				}

				continue_loop = true;
				break;
			}
		}

		if(continue_loop == false) {
			break;
		}
	}

	uc_pthread_testcancel();
	ldata = uc_get_rwlock_lockling_element(lock, thread);

	if (ldata == NULL) {
		ldata = new struct lockdata;
		ldata->counter = 0;
		ldata->thread = thread;
		lock->locking.push_back(ldata);
	}

	ldata->counter++;
	lock->rdlock = true;
	thread->m_in_rwlock--;
	thread->m_rwlock_holds.push_back(lock);

	return 0;
}


int UC_posix_class::uc_pthread_rwlock_wrlock(uc_pthread_rwlock_t *id_lock){
	if (id_lock == NULL) {
		return EINVAL;
	}

	if (id_lock->init == 0) {
		return EINVAL;
	}

	uc_pthread_rwlock_class *lock = get_uc_pthread_rwlock_t(id_lock->id);
	if (lock == NULL) {
		return EINVAL;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	if (thread->m_in_rwlock < 0) {
		return EDEADLK;
	}

	struct lockdata *ldata = NULL;
	if (lock->locking.size() != 0) {
		ldata = lock->locking[0];
	}

	if (ldata != NULL) {
		if (ldata->thread == thread) {
			return EDEADLK;
		}
	}

	while (true) {
		uc_pthread_testcancel();
		ldata = NULL;
		if (lock->rdlock != 0) {
			thread->block(&lock->blocked_threads);

			vector<UC_thread_class *>::iterator it_thread;
			for (it_thread = lock->blocked_threads.begin(); it_thread != lock->blocked_threads.end(); it_thread++) {
				if (*it_thread == thread) {
					lock->blocked_threads.erase(it_thread);
					break;
				}
			}

			continue;
		}

		if (lock->locking.size() != 0) {
			ldata = lock->locking[0];
		}
		else {
			ldata = NULL;
		}

		if (ldata != NULL) {
			if (ldata->thread!=thread) {
				thread->block(&lock->blocked_threads);

				vector<UC_thread_class *>::iterator it_thread;
				for (it_thread = lock->blocked_threads.begin(); it_thread != lock->blocked_threads.end(); it_thread++) {
					if (*it_thread == thread) {
						lock->blocked_threads.erase(it_thread);
						break;
					}
				}
				continue;
			}
		}
		break;
	}

	if (ldata == NULL) {
		ldata = new struct lockdata;
		ldata->counter = 0;
		ldata->thread = thread;
		lock->locking.push_back(ldata);
	}

	ldata->counter++;
	lock->wrlock = true;
	thread->m_in_rwlock++;
	thread->m_rwlock_holds.push_back(lock);

	return 0;
}

int UC_posix_class::uc_pthread_rwlock_trywrlock(uc_pthread_rwlock_t *id_lock){
	if (id_lock == NULL) {
		return EINVAL;
	}

	if (id_lock->init == 0) {
		return EINVAL;
	}

	uc_pthread_rwlock_class *lock = get_uc_pthread_rwlock_t(id_lock->id);
	if (lock == NULL) {
		return EINVAL;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	if (thread->m_in_rwlock < 0) {
		return EDEADLK;
	}

	if (lock->rdlock != 0) {
		return EBUSY;
	}

	struct lockdata *ldata = NULL;
	if (lock->locking.size() != 0) {
		ldata = lock->locking[0];
	}

	if (ldata != NULL) {
		if (ldata->thread != thread) {
			return EBUSY;
		}
	}

	if (ldata == NULL) {
		ldata = new struct lockdata;
		ldata->counter = 0;
		ldata->thread = thread;
		lock->locking.push_back(ldata);
	}

	ldata->counter++;
	lock->wrlock = true;
	thread->m_in_rwlock++;
	thread->m_rwlock_holds.push_back(lock);

	return 0;
}

int UC_posix_class::uc_pthread_rwlock_timedwrlock(uc_pthread_rwlock_t *id_lock, const struct timespec *abs_time){
	if (id_lock == NULL) {
		return EINVAL;
	}

	if (id_lock->init == 0) {
		return EINVAL;
	}

	uc_pthread_rwlock_class *lock = get_uc_pthread_rwlock_t(id_lock->id);
	if (lock == NULL) {
		return EINVAL;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	if (thread->m_in_rwlock < 0) {
		return EDEADLK;
	}

	struct lockdata *ldata = NULL;
	if (lock->locking.size() != 0) {
		ldata = lock->locking[0];
	}

	if (ldata != NULL) {
		if (ldata->thread == thread) {
			return EDEADLK;
		}
	}

	while (true) {
		uc_pthread_testcancel();
		ldata = NULL;
		if (lock->rdlock != 0) {
			thread->block(&lock->blocked_threads, abs_time);

			vector<UC_thread_class *>::iterator it_thread;
			for (it_thread = lock->blocked_threads.begin(); it_thread != lock->blocked_threads.end(); it_thread++) {
				if (*it_thread == thread) {
					lock->blocked_threads.erase(it_thread);
					break;
				}
			}

			if (thread->m_timeout_expired == true) {
				return ETIMEDOUT;
			}

			continue;
		}

		if (lock->locking.size() != 0) {
			ldata = lock->locking[0];
		}
		else {
			ldata = NULL;
		}

		if (ldata != NULL) {
			if (ldata->thread != thread) {
				thread->block(&lock->blocked_threads, abs_time);

				vector<UC_thread_class *>::iterator it_thread;
				for (it_thread = lock->blocked_threads.begin(); it_thread != lock->blocked_threads.end(); it_thread++) {
					if (*it_thread == thread) {
						lock->blocked_threads.erase(it_thread);
						break;
					}
				}

				if (thread->m_timeout_expired == true) {
					return ETIMEDOUT;
				}

				continue;
			}
		}
		break;
	}

	if (ldata == NULL) {
		ldata = new struct lockdata;
		ldata->counter = 0;
		ldata->thread = thread;
		lock->locking.push_back(ldata);
	}

	ldata->counter++;
	lock->wrlock = true;
	thread->m_in_rwlock++;
	thread->m_rwlock_holds.push_back(lock);

	return 0;
}

int UC_posix_class::uc_pthread_rwlock_unlock(uc_pthread_rwlock_t *id_lock){
	if (id_lock == NULL) {
		return EINVAL;
	}

	if (id_lock->init == 0) {
		return EINVAL;
	}

	uc_pthread_rwlock_class *lock = get_uc_pthread_rwlock_t(id_lock->id);
	if (lock == NULL) {
		return EINVAL;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	struct lockdata *ldata = uc_get_rwlock_lockling_element(lock, thread);

	if (ldata == NULL){
		return EPERM;
	}

	ldata->counter--;

	vector<uc_pthread_rwlock_class *>::iterator it_lock;
	for (it_lock = thread->m_rwlock_holds.begin(); it_lock != thread->m_rwlock_holds.end(); it_lock++) {
		if (*it_lock == lock) {
			thread->m_rwlock_holds.erase(it_lock);
			break;
		}
	}

	if (ldata->counter < 0) {
		return -1;
	}

	if (ldata->counter > 0) {
		return 0;
	}

	vector<struct lockdata *>::iterator it_lockdata;
	for (it_lockdata = lock->locking.begin(); it_lockdata != lock->locking.end(); it_lockdata++) {
		if (*it_lockdata == ldata) {
			lock->locking.erase(it_lockdata);
			break;
		}
	}

	if (lock->locking.size() == 0) {
		lock->rdlock = false;
		lock->wrlock = false;
		while (lock->blocked_threads.size() != 0) {
			lock->blocked_threads[0]->unblock();
		}
	}

	if (thread->m_in_rwlock > 0) {
		thread->m_in_rwlock--;
	}
	else {
		thread->m_in_rwlock++;
	}

	return 0;
}


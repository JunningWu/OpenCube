//==========================================================================
//	uc_pthread_mutex_class.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar ene 8 2008
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

#include "rtos/api/posix/uc_pthread_mutex_class.h"
#include "rtos/kernel/uc_process_class.h"
#include "rtos/kernel/uc_rtos_class.h"
#include <errno.h>

extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;

uc_pthread_mutex_class::uc_pthread_mutex_class() : m_owner( 0 ), num_locks(0)
{
	type_of_channel = 2;
	channel_priority = 0;
}

uc_pthread_mutex_class::uc_pthread_mutex_class( const char* name_ )	: m_owner( 0 ), num_locks( 0 )
{
	type_of_channel = 2;
	channel_priority = 0;
}

uc_pthread_mutex_class::uc_pthread_mutex_class(int type, int protocol, int pshared) {
	type_of_channel = 2;
	channel_priority = 0;
	mutexattr = new uc_pthread_mutexattr_t;
	num_locks = 0;
	m_owner = NULL;
	mutexattr->type = type;
	mutexattr->protocol = protocol;
	mutexattr->pshared = pshared;
	type_of_channel = 2;
	channel_priority = 0;
}

void uc_pthread_mutex_class::update_mutex_priority(UC_thread_class *attr) {
	int priority;
	if (mutexattr->protocol == PTHREAD_PRIO_NONE) {
		return;
	}
	priority = attr->m_priority;
	if (channel_priority < priority) {
		channel_priority = priority;
		if (mutexattr->protocol == PTHREAD_PRIO_PROTECT) {
			channel_priority = mutexattr->prioceiling;
		}
		if (m_owner != NULL) {
			m_owner->m_channel_priority = channel_priority;
		}
	}
}

void uc_pthread_mutex_class::restore_process_priority() {
	int priority;
	if (mutexattr->protocol == PTHREAD_PRIO_NONE) {
		return;
	}
	if (mutexattr->protocol == PTHREAD_PRIO_PROTECT) {
		channel_priority = mutexattr->prioceiling;
		return;
	}
	channel_priority = 0;

	vector<UC_thread_class *>::iterator it_thread;
	for (it_thread = blocked_threads.begin(); it_thread != blocked_threads.end(); it_thread++) {
		priority = (*it_thread)->m_parent_task->m_thread_scheduler->goodness(*it_thread);
		if (channel_priority < priority) {
			channel_priority = priority;
		}
	}
}

int uc_pthread_mutex_class::sc_lock(const struct timespec * abs_timeout){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	if ( in_use() ) {
		switch (mutexattr->type) {
			case PTHREAD_MUTEX_ERRORCHECK:
				if (m_owner != thread) {
					update_mutex_priority(thread);
					thread->block(&blocked_threads, abs_timeout);
					if (thread->m_timeout_expired == true) {
						return ETIMEDOUT;
					}
					return 1;
				}
				else {
					return EDEADLK;
				}
			case PTHREAD_MUTEX_RECURSIVE:
				if (m_owner != thread) {
					update_mutex_priority(thread);
					thread->block(&blocked_threads, abs_timeout);
					if (thread->m_timeout_expired == true) {
						return ETIMEDOUT;
					}
					return 1;
				}
				else {
					num_locks++;
					return 0;
				}
			default:
				update_mutex_priority(thread);
				thread->block(&blocked_threads, abs_timeout);
				if (thread->m_timeout_expired == true) {
					return ETIMEDOUT;
				}
				return 1;
		}
	}

	m_owner = thread;
	num_locks++;
	update_mutex_priority(thread);
	return 0;
}

int uc_pthread_mutex_class::sc_lock(){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if ( in_use() ) {
		switch (mutexattr->type) {
			case PTHREAD_MUTEX_ERRORCHECK:
				if (m_owner != thread) {
					update_mutex_priority(thread);
					thread->block(&blocked_threads);
					return 1;
				}
				else {
					return EDEADLK;
				}
			case PTHREAD_MUTEX_RECURSIVE:
				if (m_owner != thread) {
					update_mutex_priority(thread);
					thread->block(&blocked_threads);
					return 1;
				}
				else {
					num_locks++;
					return 0;
				}
			default:
				update_mutex_priority(thread);
				thread->block(&blocked_threads);
				return 1;
		}
	}

	m_owner = thread;
	num_locks++;
	update_mutex_priority(thread);
	return 0;
}

// returns -1 if mutex could not be locked
int uc_pthread_mutex_class::sc_trylock() {
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if( in_use() ) {
		if (m_owner != thread || mutexattr->type != PTHREAD_MUTEX_RECURSIVE) {
			return EBUSY;
		}
		else {
			num_locks++;
			return 0;
		}
	}

	num_locks++;
	m_owner = thread;
	return 0;
}


// returns -1 if mutex was not locked by caller

int uc_pthread_mutex_class::unblock(){
	int flag;
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if ( m_owner != thread ) {
		return EPERM;
	}

	if (num_locks == 1 || mutexattr->type != PTHREAD_MUTEX_RECURSIVE) {
		m_owner = NULL;
		num_locks = 0;

		while (blocked_threads.size() != 0) {
			blocked_threads[0]->unblock();
		}
	}
	else {
		num_locks--;
	}

	return 0;
}

int uc_pthread_mutex_class::lock() {
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	int ret = 1;

	while (ret == 1) {
		ret = sc_lock();
// 		if (ret == 1) {
// 			vector<UC_thread_class *>::iterator it_thread;
// 			for (it_thread = blocked_threads.begin(); it_thread != blocked_threads.end(); it_thread++) {
// 				if (*it_thread == thread) {
// 					blocked_threads.erase(it_thread);
// 					break;
// 				}
// 			}
// 		}
	}

	return ret;
}

int uc_pthread_mutex_class::timedlock(const struct timespec * abs_timeout){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	int ret = 1;

	while (ret == 1) {
		ret = sc_lock(abs_timeout);
// 		if (ret == 1 || ret == ETIMEDOUT) {
// 			vector<UC_thread_class *>::iterator it_thread;
// 			for (it_thread = blocked_threads.begin(); it_thread != blocked_threads.end(); it_thread++) {
// 				if (*it_thread == thread) {
// 					blocked_threads.erase(it_thread);
// 					break;
// 				}
// 			}
// 		}
	}

	return ret;
}

int uc_pthread_mutex_class::trylock(){
	return sc_trylock();
}

int uc_pthread_mutex_class::unlock(){
	restore_process_priority();
	return unblock();
}

int uc_pthread_mutex_class::empty(){
	if (in_use()) {
		return 0;
	}
	if (blocked_threads.size() > 0) {
		return 0;
	}
	return 1;
}

UC_thread_class *uc_pthread_mutex_class::get_owner() {
	return m_owner;
}

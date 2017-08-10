//==========================================================================
//	uc_posix_c.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar nov 20 2007
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

#include <cstdarg>
#include <fcntl.h>

#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/api/posix/uc_stdlib.h"
#include "rtos/api/posix/uc_pthread.h"
#include "rtos/api/posix/uc_semaphore.h"
#include "rtos/api/posix/uc_mqueue.h"
#include "rtos/api/posix/uc_signal.h"
#include "rtos/api/posix/uc_time.h"
#include "rtos/api/posix/uc_sched.h"
#include "rtos/api/posix/uc_unistd.h"
#include "rtos/api/posix/sys_uc_time.h"
#include "rtos/api/posix/sys_uc_types.h"
#include "rtos/api/posix/sys_uc_wait.h"

extern unsigned long long uc_segment_time;
// Auxiliary functions

/**
 * \brief The POSIX object path
 */
#define POSIX qt_parent_rtos->POSIX

extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;

#define USER_TO_SUPER_USER() \
	bool change_state = false; \
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;\
	posix_user_to_superuser(thread, change_state);
/**
 * \brief Change the thread state from USER to SUPER_USER
 */
void posix_user_to_superuser(UC_thread_class *thread, bool &change_state){
	if (thread->m_state == USER) {
		change_state = true;
/*		if(qt_parent_rtos->get_current_cpu()->m_icache != NULL){
			qt_parent_rtos->get_current_cpu()->m_icache->flush();
		}
		if(qt_parent_rtos->get_current_cpu()->m_dcache != NULL){
			qt_parent_rtos->get_current_cpu()->m_dcache->flush();
		}*/
		thread->set_next_state(USER, SUPER_USER);
		stp_yield();
	}
}

#define SUPER_USER_TO_WAITING() \
	posix_super_user_to_waiting(thread,change_state);
/**
 * \brief Change the thread state from SUPER_USER to USER
 */
void posix_super_user_to_waiting(UC_thread_class *thread, bool change_state){
	if (change_state == true) {
		thread->set_next_state(SUPER_USER, WAITING);
		stp_yield();
	}
}

/***********************/
/*** POSIX functions ***/
/***********************/

// These functions must start with the USER_TO_SUPER_USER() macro
// and finish with the SUPER_USER_TO_WAITING() macro.
// The implementation of the POSIX functions are in the
// UC_posix_class class. Thus, to execute them they must be
// called through the correct path, that is loaded in the
// POSIX macro. (To call them: POSIX->posix_function).
// This is the user interface.
// This way of work splits the C++ implementation from the C
// interface and the state switches from the real implementation
// to avoid double state switches when POSIX to POSIX calls are done.

void UC_eat_time(unsigned long long time) {
	USER_TO_SUPER_USER();
	POSIX->eat_time(time);
	// By default, the USER_TO_SUPER_USER is only executed from USER state.
	// This function needs to wait the annotated time no matter what.
	if (change_state == false) {
		thread->set_next_state(SUPER_USER, WAITING);
		stp_yield();
		thread->set_next_state(USER, SUPER_USER);
		thread->update_state();
	}
	SUPER_USER_TO_WAITING();
}

void UC_thread_constraints_check_start() {
	USER_TO_SUPER_USER();
	POSIX->thread_constraints_check_start();
	SUPER_USER_TO_WAITING();
}

void UC_thread_constraints_check_end(long long min, long long max) {
	USER_TO_SUPER_USER();
	POSIX->thread_constraints_check_end(min, max);
	SUPER_USER_TO_WAITING();
}

int uc_pthread_create(uc_pthread_t *__thread_arg, const uc_pthread_attr_t *__attr,
					   void *(*__start_routine) (void *), void *__arg, const char *func_name) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_create(__thread_arg, __attr, __start_routine, __arg, func_name);
	SUPER_USER_TO_WAITING();
	return ret;
}



/*
int uc_pthread_atfork(void (*prepare)(void), void (*parent)(void),void (*child)(void))
{
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_atfork(prepare, parent, child);
	SUPER_USER_TO_WAITING();
	return ret;
}
*/
int uc_pthread_once(uc_pthread_once_t *once_control, void (*init_routine)(void)){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_once(once_control, init_routine);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_init(uc_pthread_attr_t *attr){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_init(attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_destroy(uc_pthread_attr_t *attr){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_destroy(attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_setdetachstate(uc_pthread_attr_t *attr, int detachstate){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_setdetachstate(attr, detachstate);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_getdetachstate(const uc_pthread_attr_t *attr, int *detachstate){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_getdetachstate(attr, detachstate);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_setschedpolicy(uc_pthread_attr_t *attr, int policy){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_setschedpolicy(attr, policy);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_getschedpolicy(const uc_pthread_attr_t *attr, int *policy){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_getschedpolicy(attr, policy);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_setschedparam(uc_pthread_attr_t *attr,const struct uc_sched_param *param){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_setschedparam(attr, param);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_getschedparam(const uc_pthread_attr_t *attr,struct uc_sched_param *param){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_getschedparam(attr, param);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_setinheritsched(uc_pthread_attr_t *attr, int inherit){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_setinheritsched(attr, inherit);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_getinheritsched(const uc_pthread_attr_t *attr, int *inherit){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_getinheritsched(attr, inherit);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_setscope(uc_pthread_attr_t *attr, int scope){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_setscope(attr, scope);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_getscope(const uc_pthread_attr_t *attr, int *scope){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_getscope(attr, scope);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_setstack(uc_pthread_attr_t *attr, void *stack, size_t stacksize){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_setstack(attr, stack, stacksize);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_getstack(uc_pthread_attr_t *attr, void **stack, size_t *stacksize){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_getstack(attr, stack, stacksize);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_setstackaddr(uc_pthread_attr_t *attr, void *stack){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_setstackaddr(attr, stack);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_getstackaddr(uc_pthread_attr_t *attr, void **stack){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_getstackaddr(attr, stack);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_setstacksize(uc_pthread_attr_t *attr, size_t stacksize){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_setstacksize(attr, stacksize);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_getstacksize(uc_pthread_attr_t *attr, size_t *stacksize){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_getstacksize(attr, stacksize);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_setschedparam(uc_pthread_t target_thread, int policy, const struct uc_sched_param *param){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_setschedparam(target_thread, policy, param);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_getschedparam(uc_pthread_t target_thread, int *policy, struct uc_sched_param *param){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_getschedparam(target_thread, policy, param);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_setschedprio(uc_pthread_t target_thread, int priority){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_setschedprio(target_thread, priority);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_getschedprio(uc_pthread_t target_thread, int *priority){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_getschedprio(target_thread, priority);
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_pthread_t uc_pthread_self(){
	USER_TO_SUPER_USER();
	uc_pthread_t ret = POSIX->uc_pthread_self();
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_detach (uc_pthread_t tid){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_detach (tid);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_equal (uc_pthread_t __thread1, uc_pthread_t __thread2){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_equal(__thread1, __thread2);
	SUPER_USER_TO_WAITING();
	return ret;
}

void uc_pthread_exit (void *__retval) {
	USER_TO_SUPER_USER();
	POSIX->uc_pthread_exit(__retval);
	SUPER_USER_TO_WAITING();
}

int uc_pthread_join (uc_pthread_t __th, void **__thread_return){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_join(__th, __thread_return);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_setcancelstate(int state, int *oldstate){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_setcancelstate(state, oldstate);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_setcanceltype(int type, int *oldtype){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_setcanceltype(type, oldtype);
	SUPER_USER_TO_WAITING();
	return ret;
}

void uc_pthread_cleanup_push(void (*routine)(void*), void *arg){
	USER_TO_SUPER_USER();
	POSIX->uc_pthread_cleanup_push(routine, arg);
	SUPER_USER_TO_WAITING();
}

void uc_pthread_cleanup_pop (int execute){
	USER_TO_SUPER_USER();
	POSIX->uc_pthread_cleanup_pop(execute);
	SUPER_USER_TO_WAITING();
}

int uc_pthread_cancel(uc_pthread_t p_thread){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_cancel(p_thread);
	SUPER_USER_TO_WAITING();
	return ret;
}

void uc_pthread_testcancel(){
	USER_TO_SUPER_USER();
	POSIX->uc_pthread_testcancel();
	SUPER_USER_TO_WAITING();
}

int uc_pthread_key_create(uc_pthread_key_t *key_id, void (*destructor)(void*)){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_key_create(key_id, destructor);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_key_delete(uc_pthread_key_t key_id){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_key_delete(key_id);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_setspecific(uc_pthread_key_t key_id, void *value){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_setspecific(key_id, value);
	SUPER_USER_TO_WAITING();
	return ret;
}

void *uc_pthread_getspecific(uc_pthread_key_t key){
	USER_TO_SUPER_USER();
	void *ret = POSIX->uc_pthread_getspecific(key);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_getcpuclockid(uc_pthread_t thread_id, clockid_t *clock_id){
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_getcpuclockid(thread_id, clock_id);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_getguardsize(const uc_pthread_attr_t *attr, size_t *guardsize) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_getguardsize(attr, guardsize);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_attr_setguardsize(uc_pthread_attr_t *attr, size_t guardsize) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_attr_setguardsize(attr, guardsize);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_cond_init(uc_pthread_cond_t *cond, uc_pthread_condattr_t *cond_attr) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_cond_init(cond, cond_attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_cond_destroy(uc_pthread_cond_t *cond) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_cond_destroy(cond);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_cond_signal(uc_pthread_cond_t *cond) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_cond_signal(cond);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_cond_broadcast(uc_pthread_cond_t *cond) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_cond_broadcast(cond);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_cond_timedwait(uc_pthread_cond_t *cond, uc_pthread_mutex_t *mutex,const struct timespec *abstime) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_cond_timedwait(cond, mutex, abstime);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_cond_wait(uc_pthread_cond_t *cond, uc_pthread_mutex_t *mutex) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_cond_wait(cond, mutex);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_condattr_init(uc_pthread_condattr_t *attr) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_condattr_init(attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_condattr_destroy(uc_pthread_condattr_t *attr) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_condattr_destroy(attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_condattr_getpshared(const uc_pthread_condattr_t *attr, int *pshared) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_condattr_getpshared(attr, pshared);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_condattr_setpshared(uc_pthread_condattr_t *attr, int pshared) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_condattr_setpshared(attr, pshared);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_condattr_getclock(const uc_pthread_condattr_t *attr,clockid_t *clock_id) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_condattr_getclock(attr, clock_id);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_condattr_setclock(uc_pthread_condattr_t *attr,clockid_t clock_id) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_condattr_setclock(attr, clock_id);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutex_init(uc_pthread_mutex_t *mutex,const uc_pthread_mutexattr_t *mutexattr) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutex_init(mutex, mutexattr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutex_lock(uc_pthread_mutex_t *mutex) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutex_lock(mutex);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutex_unlock(uc_pthread_mutex_t *mutex) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutex_unlock(mutex);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutex_trylock(uc_pthread_mutex_t *mutex) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutex_trylock(mutex);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutex_destroy(uc_pthread_mutex_t *mutex) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutex_destroy(mutex);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutex_getprioceiling(const uc_pthread_mutex_t *mutex, int *restrict) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutex_getprioceiling(mutex, restrict);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutex_setprioceiling(const uc_pthread_mutex_t *mutex, int prioceiling, int *restrict) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutex_setprioceiling(mutex, prioceiling, restrict);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutex_timedlock(uc_pthread_mutex_t *mutex,const struct timespec* timelock) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutex_timedlock(mutex, timelock);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutexattr_init(uc_pthread_mutexattr_t *attr) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutexattr_init(attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutexattr_destroy(uc_pthread_mutexattr_t *attr) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutexattr_destroy(attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutexattr_getprioceiling(const uc_pthread_mutexattr_t *attr, int* restrict) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutexattr_getprioceiling(attr, restrict);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutexattr_getprotocol(const uc_pthread_mutexattr_t *attr, int* restrict) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutexattr_getprotocol(attr, restrict);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutexattr_getpshared(const uc_pthread_mutexattr_t *attr, int* restrict) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutexattr_getpshared(attr, restrict);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutexattr_gettype(const uc_pthread_mutexattr_t *attr, int* restrict) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutexattr_gettype(attr, restrict);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutexattr_setprioceiling( uc_pthread_mutexattr_t *attr, int value) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutexattr_setprioceiling(attr, value);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutexattr_setprotocol( uc_pthread_mutexattr_t *attr, int value) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutexattr_setprotocol(attr, value);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutexattr_setpshared( uc_pthread_mutexattr_t *attr, int value) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutexattr_setpshared(attr, value);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_mutexattr_settype( uc_pthread_mutexattr_t *attr, int value) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_mutexattr_settype(attr, value);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlockattr_init(uc_pthread_rwlockattr_t *attr) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlockattr_init(attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlockattr_destroy(uc_pthread_rwlockattr_t *attr) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlockattr_destroy(attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlockattr_getpshared(const uc_pthread_rwlockattr_t *attr, int *num) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlockattr_getpshared(attr, num);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlockattr_setpshared(uc_pthread_rwlockattr_t *attr, int num) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlockattr_setpshared(attr, num);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlock_init(uc_pthread_rwlock_t *id_lock, const uc_pthread_rwlockattr_t *attr) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlock_init(id_lock, attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlock_destroy(uc_pthread_rwlock_t *id_lock) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlock_destroy(id_lock);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlock_rdlock(uc_pthread_rwlock_t *id_lock) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlock_rdlock(id_lock);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlock_tryrdlock(uc_pthread_rwlock_t *id_lock) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlock_tryrdlock(id_lock);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlock_timedrdlock(uc_pthread_rwlock_t *id_lock, const struct timespec *abs_time) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlock_timedrdlock(id_lock, abs_time);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlock_wrlock(uc_pthread_rwlock_t *id_lock) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlock_wrlock(id_lock);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlock_trywrlock(uc_pthread_rwlock_t *id_lock) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlock_trywrlock(id_lock);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlock_timedwrlock(uc_pthread_rwlock_t *id_lock, const struct timespec *abs_time) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlock_timedwrlock(id_lock, abs_time);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_rwlock_unlock(uc_pthread_rwlock_t *id_lock) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_rwlock_unlock(id_lock);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_spin_lock(uc_pthread_spinlock_t *lock) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_spin_lock(lock);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_spin_trylock(uc_pthread_spinlock_t *lock) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_spin_trylock(lock);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_spin_destroy(uc_pthread_spinlock_t *lock) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_spin_destroy(lock);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_spin_init(uc_pthread_spinlock_t *lock, int pshared) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_spin_init(lock, pshared);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_spin_unlock(uc_pthread_spinlock_t *lock) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_spin_unlock(lock);
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_mqd_t uc_mq_open(const char *name, int flags, ... ) {
	int mode;
	uc_mq_attr *attr;
	va_list vl;
	uc_mqd_t ret;

	USER_TO_SUPER_USER();

	if(flags & O_CREAT) {
		va_start(vl,flags);
		mode=va_arg(vl,int);
		attr=va_arg(vl,uc_mq_attr*);
		va_end(vl);
		ret = POSIX->uc_mq_open(name, flags, mode, attr);
	}
	else {
		ret = POSIX->uc_mq_open(name, flags);		
	}

	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_mq_close(uc_mqd_t mqd_id) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_mq_close(mqd_id);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_mq_notify(uc_mqd_t mqd_id, const struct uc_sigevent *ev) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_mq_notify(mqd_id, ev);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_mq_getattr(uc_mqd_t mqd_id, struct uc_mq_attr *attr) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_mq_getattr(mqd_id, attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_mq_setattr(uc_mqd_t mqd_id, const struct uc_mq_attr *attr, struct uc_mq_attr *old_attr) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_mq_setattr(mqd_id, attr, old_attr);
	SUPER_USER_TO_WAITING();
	return ret;
}

ssize_t uc_mq_receive(uc_mqd_t mqd_id, char *buffer, size_t size, void *msg_prio) {
	USER_TO_SUPER_USER();
	ssize_t ret = POSIX->uc_mq_receive(mqd_id, buffer, size, msg_prio);
	SUPER_USER_TO_WAITING();
	return ret;
}

ssize_t uc_mq_timedreceive(uc_mqd_t mqd_id, char * buffer, size_t size, void *msg_prio, const struct timespec *abs_time) {
	USER_TO_SUPER_USER();
	ssize_t ret = POSIX->uc_mq_timedreceive(mqd_id, buffer, size, msg_prio, abs_time);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_mq_send(uc_mqd_t mqd_id, const char *buffer, size_t size, int msg_prio) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_mq_send(mqd_id, buffer, size, msg_prio);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_mq_timedsend(uc_mqd_t mqd_id, const char *buffer, size_t size, int msg_prio, const struct timespec *abs_time) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_mq_timedsend(mqd_id, buffer, size, msg_prio, abs_time);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_mq_unlink(const char *name) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_mq_unlink(name);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sched_setparam(int pid, const struct uc_sched_param *param) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sched_setparam(pid, param);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sched_getparam(int pid, struct uc_sched_param *param) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sched_getparam(pid, param);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sched_getscheduler(int pid) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sched_getscheduler(pid);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sched_setscheduler(int pid, int policy, const struct uc_sched_param *param) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sched_setscheduler(pid, policy, param);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sched_yield() {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sched_yield();
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sched_get_priority_max(int policy) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sched_get_priority_max(policy);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sched_get_priority_min(int policy) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sched_get_priority_min(policy);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sched_rr_get_interval(int pid, struct timespec *tp) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sched_rr_get_interval(pid, tp);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sched_setaffinity(int pid, unsigned int len, unsigned long *mask) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sched_setaffinity(pid, len, mask);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sched_getaffinity(int pid, unsigned int len, unsigned long *mask) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sched_getaffinity(pid, len, mask);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sem_close(uc_sem_t *sem_id) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sem_close(sem_id);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sem_destroy(uc_sem_t *sem_id) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sem_destroy(sem_id);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sem_getvalue(uc_sem_t *sem_id, int *val) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sem_getvalue(sem_id, val);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sem_init(uc_sem_t *sem_id, int pshared, int value) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sem_init(sem_id, pshared, value);
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_sem_t *uc_sem_open(const char *name, int flags, ...) {
	int mode,value;
	va_list vl;
	uc_sem_t *ret;

	USER_TO_SUPER_USER();

	if(flags & O_CREAT) {
		va_start(vl,flags);
		mode=va_arg(vl,int);
		value=va_arg(vl,int);
		va_end(vl);
		ret = POSIX->uc_sem_open(name, flags, mode, value);
	}
	else {
		ret = POSIX->uc_sem_open(name, flags);
	}

	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sem_post(uc_sem_t *sem_id) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sem_post(sem_id);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sem_timedwait(uc_sem_t *sem_id, const struct timespec *abs_time) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sem_timedwait(sem_id, abs_time);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sem_trywait(uc_sem_t *sem_id) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sem_trywait(sem_id);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sem_unlink(const char *name) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sem_unlink(name);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sem_wait(uc_sem_t *sem_id) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sem_wait(sem_id);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_shm_open(const char *name, int oflag, mode_t mode) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_shm_open(name, oflag, mode);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_ftruncate(int fildes, off_t length) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_ftruncate(fildes, length);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_shm_unlink(const char *name) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_shm_unlink(name);
	SUPER_USER_TO_WAITING();
	return ret;
}

void *uc_mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off) {
	USER_TO_SUPER_USER();
	void *ret = POSIX->uc_mmap(addr, len, prot, flags, fildes, off);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_munmap(void *addr, size_t len) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_munmap(addr, len);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_kill(uc_pid_t a, int b) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_kill(a, b);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_killpg(uc_pid_t a, int b) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_killpg(a, b);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_kill(uc_pthread_t a, int b) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_kill(a, b);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pthread_sigmask(int a, const uc_sigset_t *b, uc_sigset_t *c) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pthread_sigmask(a, b, c);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_raise(int a) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_raise(a);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigaction(int a, const struct uc_sigaction *b, struct uc_sigaction *c) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigaction(a, b, c);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigaddset(uc_sigset_t *a, int b) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigaddset(a, b);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigdelset(uc_sigset_t *a, int b) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigdelset(a, b);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigemptyset(uc_sigset_t *a) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigemptyset(a);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigfillset(uc_sigset_t *a) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigfillset(a);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigismember(const uc_sigset_t *a, int b) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigismember(a, b);
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_sighandler_t uc_signal(int sig, uc_sighandler_t handler) {
	USER_TO_SUPER_USER();
	uc_sighandler_t ret = POSIX->uc_signal(sig, handler);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigpending(uc_sigset_t *a) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigpending(a);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigprocmask(int a, const uc_sigset_t *b, uc_sigset_t *c) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigprocmask(a, b, c);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigqueue(uc_pid_t a, int b, const union uc_sigval c) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigqueue(a, b, c);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigsuspend(const uc_sigset_t *a) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigsuspend(a);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigtimedwait(const uc_sigset_t *a, uc_siginfo_t *b, const struct timespec *c) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigtimedwait(a, b, c);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigwait(const uc_sigset_t *a, int *b) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigwait(a, b);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigwaitinfo(const uc_sigset_t *a, uc_siginfo_t *b) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigwaitinfo(a, b);
	SUPER_USER_TO_WAITING();
	return ret;
}

void (*uc_bsd_signal(int sig, void (*handler)(int)))(int) {
	USER_TO_SUPER_USER();
	void (*ret)(int) = POSIX->uc_bsd_signal(sig, handler);
	SUPER_USER_TO_WAITING();
	return ret;
}

void (*uc_sigset(int sig, void (*disp)(int)))(int) {
	USER_TO_SUPER_USER();
	void (*ret)(int) = POSIX->uc_sigset(sig, disp);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sighold(int a) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sighold(a);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigignore(int a) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigignore(a);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigpause(int a) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigpause(a);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_sigrelse(int a) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_sigrelse(a);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_siginterrupt(int a, int b) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_siginterrupt(a, b);
	SUPER_USER_TO_WAITING();
	return ret;
}

clock_t uc_clock() {
	USER_TO_SUPER_USER();
	clock_t ret = POSIX->uc_clock();
	SUPER_USER_TO_WAITING();
	return ret;
}

time_t uc_time(time_t *time) {
	USER_TO_SUPER_USER();
	time_t ret = POSIX->uc_time(time);
	SUPER_USER_TO_WAITING();
	return ret;
}

double uc_difftime(time_t time1, time_t time0) {
	USER_TO_SUPER_USER();
	double ret = POSIX->uc_difftime(time1, time0);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_nanosleep(const struct timespec *nseconds, struct timespec *rem) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_nanosleep(nseconds, rem);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_clock_getcpuclockid(int pid, clockid_t *clockid) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_clock_getcpuclockid(pid, clockid);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_clock_getres(clockid_t clock_id, struct timespec *rqtp) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_clock_getres(clock_id, rqtp);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_clock_gettime(clockid_t clock_id, struct timespec *rqtp) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_clock_gettime(clock_id, rqtp);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_clock_settime(clockid_t clock_id, const struct timespec *rqtp) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_clock_settime(clock_id, rqtp);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *rqtp, struct timespec *rmtp) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_clock_nanosleep(clock_id, flags, rqtp, rmtp);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_timer_create(clockid_t clockid, struct uc_sigevent *s_ev, timer_t *timer) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_timer_create(clockid, s_ev, timer);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_timer_delete(timer_t timer) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_timer_delete(timer);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_timer_gettime(timer_t time, struct itimerspec * spec) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_timer_gettime(time, spec);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_timer_getoverrun(timer_t timer) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_timer_getoverrun(timer);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_timer_settime(timer_t timer, int flags, const struct itimerspec *i_spec, struct itimerspec *o_spec) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_timer_settime(timer, flags, i_spec, o_spec);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_mkstemp(char *tmp_char) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_mkstemp(tmp_char);
	SUPER_USER_TO_WAITING();
	return ret;
}

void uc_abort() {
	USER_TO_SUPER_USER();
	POSIX->uc_abort();
	SUPER_USER_TO_WAITING();
}

int uc_system(const char* buf) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_system(buf);
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_pid_t uc_getpid() {
	USER_TO_SUPER_USER();
	uc_pid_t ret = POSIX->uc_getpid();
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_gid_t uc_getgid() {
	USER_TO_SUPER_USER();
	uc_gid_t ret = POSIX->uc_getgid();
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_pid_t uc_getppid() {
	USER_TO_SUPER_USER();
	uc_pid_t ret = POSIX->uc_getppid();
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_pid_t uc_getpgid(uc_pid_t pid) {
	USER_TO_SUPER_USER();
	uc_pid_t ret = POSIX->uc_getpgid(pid);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_setpgid(uc_pid_t pid, uc_pid_t pgid) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_setpgid(pid, pgid);
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_pid_t uc_getpgrp() {
	USER_TO_SUPER_USER();
	uc_pid_t ret = POSIX->uc_getpgrp();
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_setpgrp() {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_setpgrp();
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_setuid(uc_uid_t uid) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_setuid(uid);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_getuid() {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_getuid();
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_gid_t uc_getegid() {
	USER_TO_SUPER_USER();
	uc_gid_t ret = POSIX->uc_getegid();
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_setegid(uc_gid_t ee) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_setegid(ee);
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_uid_t uc_geteuid() {
	USER_TO_SUPER_USER();
	uc_uid_t ret = POSIX->uc_geteuid();
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_seteuid(uc_uid_t ee) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_seteuid(ee);
	SUPER_USER_TO_WAITING();
	return ret;
}

long uc_gethostid() {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_gethostid();
	SUPER_USER_TO_WAITING();
	return ret;
}

// int uc_open(const char *camino, int flags) {
// 	USER_TO_SUPER_USER();
// 	int ret = POSIX->uc_open(camino, flags);
// 	SUPER_USER_TO_WAITING();
// 	return ret;
// }
// 
// int uc_open(const char *camino, int flags, mode_t modo) {
// 	USER_TO_SUPER_USER();
// 	int ret = POSIX->uc_open(camino, flags, modo);
// 	SUPER_USER_TO_WAITING();
// 	return ret;
// }

int uc_open(const char *camino, int flags, ...) {
	va_list arguments;
	mode_t modo;
	int ret;
	USER_TO_SUPER_USER();
	if (flags & O_CREAT != 0) {
		va_start(arguments, flags);
		modo = va_arg(arguments, mode_t);
		va_end(arguments);
		ret = POSIX->uc_open(camino, flags, modo);
	}
	else {
		ret = POSIX->uc_open(camino, flags);
	}
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_creat(const char *camino, mode_t modo) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_creat(camino, modo);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pipe(int filedes[2]) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pipe(filedes);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_close(int fd) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_close(fd);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_write(int fd, const void *buf, size_t len) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_write(fd, buf, len);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_write(int fd, const void *buf, size_t len, off_t offset) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_write(fd, buf, len, offset);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_read(int fd, void *buf, size_t len) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_read(fd, buf, len);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pread(int fd, void *buf, size_t count, off_t offset) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pread(fd, buf, count, offset);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pwrite(int fd, const void *buf, size_t count, off_t offset) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pwrite(fd, buf, count, offset);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_nice(int inc) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_nice(inc);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_usleep(unsigned long int useconds) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_usleep(useconds);
	SUPER_USER_TO_WAITING();
	return ret;
}

unsigned int uc_sleep(unsigned int seconds) {
	USER_TO_SUPER_USER();
	unsigned int ret = POSIX->uc_sleep(seconds);
	SUPER_USER_TO_WAITING();
	return ret;
}

unsigned int uc_alarm(unsigned int seconds) {
	USER_TO_SUPER_USER();
	unsigned int ret = POSIX->uc_alarm(seconds);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_pause() {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_pause();
	SUPER_USER_TO_WAITING();
	return ret;
}

void uc__exit(int status) {
	USER_TO_SUPER_USER();
	POSIX->uc__exit(status);
	SUPER_USER_TO_WAITING();
}

void uc_exit(int status) {
	USER_TO_SUPER_USER();
	POSIX->uc_exit(status);
	SUPER_USER_TO_WAITING();
}

int uc_gettimeofday(struct uc_timeval *tv, void *none) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_gettimeofday(tv, none);
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_pid_t uc_wait(int *ret_val) {
	USER_TO_SUPER_USER();
	uc_pid_t ret = POSIX->uc_wait(ret_val);
	SUPER_USER_TO_WAITING();
	return ret;
}

uc_pid_t uc_waitpid (uc_pid_t __pid, int *__stat_loc, int __options) {
	USER_TO_SUPER_USER();
	uc_pid_t ret = POSIX->uc_waitpid(__pid, __stat_loc, __options);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_select(n, readfds, writefds, exceptfds, timeout);
	SUPER_USER_TO_WAITING();
	return ret;
}

int uc_smp_processor_id() {
	USER_TO_SUPER_USER();
	int ret = POSIX->uc_smp_processor_id();
	SUPER_USER_TO_WAITING();
	return ret;
}


/******************************/
/*** End of POSIX functions ***/
/******************************/

#undef USER_TO_SUPER_USER
#undef SUPER_USER_TO_USER
#undef POSIX

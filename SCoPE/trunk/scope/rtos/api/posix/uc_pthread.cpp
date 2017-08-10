//==========================================================================
//	uc_pthread.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar dic 4 2007
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
#include "rtos/api/posix/uc_pthread.h"
#include "rtos/api/posix/uc_pthread_class.h"
#include "rtos/api/posix/uc_posix.h"
#include "rtos/api/posix/uc_pthread_rwlock_class.h"
#include "rtos/qt_interface/uc_stp.h"

extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;
/*
uc_pthread_cond_t uc_pthread_cond_init::init_cond() {
	uc_pthread_cond_t cond;
	cond.id = qt_parent_rtos->m_cond_id_max++;
	return cond;
}

uc_pthread_mutex_t uc_pthread_mutex_init::init_mutex(){
	uc_pthread_mutex_t mutex;
	mutex.id = qt_parent_rtos->m_mutex_id_max++;
	return mutex;
}
*/
int UC_posix_class::uc_pthread_create(uc_pthread_t *__thread_arg, const uc_pthread_attr_t *__attr,
									  void *(*__start_routine) (void *), void *__arg, const char *func_name)
{
	UC_thread_class *thread = uc_create_thread(qt_parent_rtos->get_current_cpu()->m_current_task, __attr, __start_routine, __arg, func_name);
	*__thread_arg = thread->m_tid;

	if (thread->m_tid > 0) {
		return 0;
	}
	else {
		return thread->m_tid;
	}
}

int UC_posix_class::uc_pthread_once(uc_pthread_once_t *once_control, void (*init_routine)(void)){
	if (once_control == NULL) {
		 return -1;
	}

	if (init_routine == NULL) {
		 return -1;
	}

	if (*once_control) {
		 init_routine();
	}

	(*once_control) = 0;
	return 0;
}

int UC_posix_class::uc_pthread_attr_init(uc_pthread_attr_t *attr){
	if (attr == NULL) {
		return -1;
	}
	struct uc_sched_param *schedparam = uc_schedparam_init();

	attr->detachstate = 	UC_PTHREAD_CREATE_JOINABLE;
	attr->schedpolicy = 	SCHED_OTHER;
	attr->schedparam =   	*schedparam;
	attr->inheritsched = 	PTHREAD_EXPLICIT_SCHED;
	attr->scope = 			PTHREAD_SCOPE_PROCESS;
	attr->stackaddr = 		NULL;
	attr->stacksize = 		PTHREAD_STACK_SIZE;
	attr->cancelstate = 	UC_PTHREAD_CANCEL_ENABLE;
	attr->canceltype = 		UC_PTHREAD_CANCEL_ASYNCHRONOUS;

	delete schedparam;

	return 0;
}

int UC_posix_class::uc_pthread_attr_destroy(uc_pthread_attr_t *attr){
	if (attr == NULL) {
		 return -1;
	}
	return 0;
}

int UC_posix_class::uc_pthread_attr_setdetachstate(uc_pthread_attr_t *attr, int detachstate){
	if (attr == NULL) {
		 return -1;
	}

	if (detachstate < 0 || detachstate > 3) {
		 return EINVAL;
	}

	attr->detachstate = detachstate;
	return 0;
}

int UC_posix_class::uc_pthread_attr_getdetachstate(const uc_pthread_attr_t *attr, int *detachstate){
	if (attr == NULL) {
		 return EINVAL;
	}

	if (detachstate == NULL) {
		 return EINVAL;
	}

	*detachstate= attr->detachstate;
	return 0;
}

int UC_posix_class::uc_pthread_attr_setschedpolicy(uc_pthread_attr_t *attr, int policy){
	if (attr == NULL) {
		return EINVAL;
	}

	if (policy < 0) {
		 return EINVAL;
	}

	if (policy > UC_MAX_SCHED_POLICY) {
		return EINVAL;
	}

	attr->schedpolicy = policy;
	return 0;
}


int UC_posix_class::uc_pthread_attr_getschedpolicy(const uc_pthread_attr_t *attr, int *policy){
	if (attr == NULL) {
		return EINVAL;
	}

	if (policy == NULL) {
		return EINVAL;
	}

	*policy= attr->schedpolicy;
	return 0;
}

int UC_posix_class::uc_pthread_attr_setschedparam(uc_pthread_attr_t *attr,const struct uc_sched_param *param){
	struct uc_sched_param *param2;
	if (attr == NULL) {
		return EINVAL;
	}

	if (param->sched_priority > PTHREAD_MAX_PRIORITY || param->sched_priority < 0) {
		return ENOTSUP;
	}

// 	param2 = new struct uc_sched_param;
// 	*param2 = *param;
// 	attr->schedparam = *param2;
	attr->schedparam = *param;
	return 0;
}

int UC_posix_class::uc_pthread_attr_getschedparam(const uc_pthread_attr_t *attr,struct uc_sched_param *param){
	if (attr == NULL) {
		return EINVAL;
	}

	if (param == NULL) {
		return EINVAL;
	}

	*param= attr->schedparam;
	return 0;
}

int UC_posix_class::uc_pthread_attr_setinheritsched(uc_pthread_attr_t *attr, int inherit){
	if (attr == NULL) {
		return EINVAL;
	}

	if (inherit < 0 || inherit > 3) {
		return EINVAL;
	}

	attr->inheritsched = inherit;
	return 0;
}

int UC_posix_class::uc_pthread_attr_getinheritsched(const uc_pthread_attr_t *attr, int *inherit){
	if (attr == NULL) {
		return EINVAL;
	}

	if (inherit == NULL) {
		return EINVAL;
	}

	*inherit= attr->inheritsched;
	return 0;
}

int UC_posix_class::uc_pthread_attr_setscope(uc_pthread_attr_t *attr, int scope){
	if (attr == NULL) {
		return EINVAL;
	}

	if (scope < 0 || scope > 1) {
		return ENOTSUP;
	}

	attr->scope = scope;
	return 0;
}

int UC_posix_class::uc_pthread_attr_getscope(const uc_pthread_attr_t *attr, int *scope){
	if (attr == NULL) {
		return EINVAL;
	}

	if (scope == NULL) {
		return EINVAL;
	}

	*scope= attr->scope;
	return 0;
}

int UC_posix_class::uc_pthread_attr_setstack(uc_pthread_attr_t *attr, void *stack, size_t stacksize){
	cout << "Stack management not allowded: setstack" << endl;
	return 0;
}

int UC_posix_class::uc_pthread_attr_getstack(uc_pthread_attr_t *attr, void **stack, size_t *stacksize){
	cout << "Stack management not allowded: getstack" << endl;
	return 0;
}

int UC_posix_class::uc_pthread_attr_setstackaddr(uc_pthread_attr_t *attr, void *stack){
	cout << "Stack management not allowded: setstackaddr" << endl;
	return 0;
}

int UC_posix_class::uc_pthread_attr_getstackaddr(uc_pthread_attr_t *attr, void **stack){
	cout << "Stack management not allowded: getstackaddr" << endl;
	return 0;
}

int UC_posix_class::uc_pthread_attr_setstacksize(uc_pthread_attr_t *attr, size_t stacksize){
	cout << "Stack management not allowded: setstacksize" << endl;
	return 0;
}

int UC_posix_class::uc_pthread_attr_getstacksize(uc_pthread_attr_t *attr, size_t *stacksize){
	cout << "Stack management not allowded: getstacksize" << endl;
	return 0;
}

int UC_posix_class::uc_pthread_setschedparam(uc_pthread_t target_thread, int policy, const struct uc_sched_param *param){
	UC_thread_class *thread;
	UC_task_class *task;
// 	struct uc_sched_param *s_param;
// 	s_param = new struct uc_sched_param;

	if (param == NULL) {
		 return -1;
	}

	if (target_thread == 0) {
		thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	}
	else {
		thread = uc_get_thread(target_thread);
	}

	task = thread->m_parent_task;

	/*
		Move the task policy to the thread most rescrictive policy: RR > FIFO > OTHER
	*/
	if(task->m_policy < policy ){
		task->m_policy = policy;
	}
	if(task->m_schedparam.sched_priority < param->sched_priority ){
		task->m_schedparam.sched_priority = param->sched_priority;
	}

	thread->m_attribs.schedpolicy = policy;
	thread->m_attribs.schedparam = *param;

	return 0;
}

int UC_posix_class::uc_pthread_getschedparam(uc_pthread_t target_thread, int *policy, struct uc_sched_param *param){
	UC_thread_class *thread;

	if (policy == NULL) {
		return -1;
	}

	if (param == NULL) {
		return -1;
	}

	if (target_thread == 0) {
		thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	}
	else {
		thread = uc_get_thread(target_thread);
	}

	*policy = thread->m_attribs.schedpolicy;
	*param = thread->m_attribs.schedparam;
	return 0;
}

int UC_posix_class::uc_pthread_setschedprio(uc_pthread_t target_thread, int priority){
	UC_thread_class *thread;

	if (priority < 0 || priority > PTHREAD_MAX_PRIORITY) {
		return EINVAL;
	}

	if (target_thread == 0) {
		thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	}
	else {
		thread = uc_get_thread(target_thread);
	}

	if (thread == NULL) {
		return ESRCH;
	}

	if(thread->m_parent_task->m_schedparam.sched_priority < priority ){
		thread->m_parent_task->m_schedparam.sched_priority = priority;
	}


	thread->m_attribs.schedparam.sched_priority = priority;
	return 0;
}

int UC_posix_class::uc_pthread_getschedprio(uc_pthread_t target_thread, int *priority){
	UC_thread_class *thread;

	if (priority == NULL) {
		return -1;
	}

	if (target_thread == 0) {
		thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	}
	else {
		thread = uc_get_thread(target_thread);
	}

	*priority = thread->m_attribs.schedparam.sched_priority;
	return 0;
}

uc_pthread_t UC_posix_class::uc_pthread_self(){
	return qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread->m_tid;
}

int UC_posix_class::uc_pthread_detach(uc_pthread_t tid){
	vector<UC_thread_class *> *thread_list = &qt_parent_rtos->get_current_cpu()->m_current_task->m_thread_list;
	vector<UC_thread_class *>::iterator it_thread;

	for (it_thread = thread_list->begin(); it_thread != thread_list->end(); it_thread++) {
		if ((*it_thread)->m_tid == tid) {
			if ((*it_thread)->m_attribs.detachstate == UC_PTHREAD_CREATE_DETACHED) {
				return EINVAL;
			}
			(*it_thread)->m_attribs.detachstate = UC_PTHREAD_CREATE_DETACHED;
			return 0;
		}
	}

	return ESRCH;
}

int UC_posix_class::uc_pthread_equal(uc_pthread_t __thread1, uc_pthread_t __thread2){
	return (__thread1 == __thread2);
}

void UC_posix_class::uc_pthread_exit(void *__retval) {
	void (*routine)(void *);
	void *arg;
	uc_pthread_rwlock_class *rwlock;

	UC_thread_class *current_thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	current_thread->m_attribs.cancelstate = UC_PTHREAD_FINISHED;

	uc_pthread_key_struct *key;
	for (int i = 0; i < qt_parent_process->m_key_list.size(); /*nothing*/) {
		key = qt_parent_process->m_key_list[i];
		arg = uc_pthread_getspecific(key->key_id);
		if (arg != NULL) {
			uc_pthread_setspecific(key->key_id, NULL);
			if (key->destructor != NULL) {
				(key->destructor)(arg);
			}
		}
		// Maybe destroyed
		if (key == qt_parent_process->m_key_list[i]) {
			i++;
		}
	}

	while (current_thread->m_lifo_cancel_func.size() != 0) {
		routine = current_thread->m_lifo_cancel_func.top();
		arg = current_thread->m_lifo_cancel_args.top();
		current_thread->m_lifo_cancel_func.pop();
		current_thread->m_lifo_cancel_args.pop();

		if (routine != NULL) {
			(routine)(arg);
		}
	}

	while (current_thread->m_rwlock_holds.size() != 0) {
		struct lockdata *ldata;
		UC_thread_class *thread;
		rwlock = current_thread->m_rwlock_holds[0];
		current_thread->m_rwlock_holds.erase(current_thread->m_rwlock_holds.begin());
		ldata = uc_get_rwlock_lockling_element(rwlock, current_thread);
		ldata->counter--;
		if (ldata->counter == 0) {
			vector<struct lockdata *>::iterator it_ldata;
			for (it_ldata = rwlock->locking.begin(); it_ldata != rwlock->locking.end(); it_ldata++) {
				if (*it_ldata == ldata) {
					rwlock->locking.erase(it_ldata);
					break;
				}
			}
			if (rwlock->locking.size() == 0) {
				rwlock->rdlock = false;
				rwlock->wrlock = false;
				while (rwlock->blocked_threads.size() != 0) {
					thread = rwlock->blocked_threads[0];
					if (thread->m_state != ZOMBIE) {
						thread->unblock();
					}
					else {
						rwlock->blocked_threads.erase(rwlock->blocked_threads.begin());
					}
				}
			}
		}
	}
	current_thread->m_in_rwlock = 0;


	// FIXME
// 	int ret,dynamic;
// 	int ppid;

// 	UC_thread_class *current_thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

// 	dynamic=current_thread->is_dynamic;
// 	ret=uc_ch_exec_mod_SU_ZB();
	current_thread->m_end_value = __retval;

// 	if(ret){current_thread->pr_table->exit_code=(int)(int*)end_value;}

	UC_qt_end_this_thread(current_thread);
}

int UC_posix_class::uc_pthread_join(uc_pthread_t __th, void **__thread_return){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;
	for (it_task = qt_parent_process->m_task_list.begin(); it_task != qt_parent_process->m_task_list.end(); it_task++) {
		for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
			if ((*it_thread)->m_tid == __th) {
				if ((*it_thread)->m_attribs.detachstate == UC_PTHREAD_CREATE_DETACHED) {
					return EINVAL;
				}

				thread->block(&(*it_thread)->m_blocked_thread_list);

				if (__thread_return != NULL) {
					*__thread_return = (*it_thread)->m_end_value;
				}

				return 0;
			}
		}
	}

	return ESRCH;
}

int UC_posix_class::uc_pthread_setcancelstate(int state, int *oldstate){
	if (state < 0 || state > 4) {
		 return EINVAL;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if (oldstate != NULL) {
		*oldstate = thread->m_attribs.cancelstate;
	}

	thread->m_attribs.cancelstate = state;
	return 0;
}

int UC_posix_class::uc_pthread_setcanceltype(int type, int *oldtype){
	if (type < 0 || type > 4) {
		return EINVAL;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if (oldtype != NULL) {
		*oldtype = thread->m_attribs.canceltype;
	}

	thread->m_attribs.canceltype = type;
	return 0;
}

void UC_posix_class::uc_pthread_cleanup_push(void (*routine)(void*), void *arg){
	if (routine == NULL) {
		return;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	thread->m_lifo_cancel_func.push(routine);
	thread->m_lifo_cancel_args.push(arg);
}

void UC_posix_class::uc_pthread_cleanup_pop (int execute){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	void (*routine)(void*) = thread->m_lifo_cancel_func.top();
	void *arg = thread->m_lifo_cancel_args.top();
	thread->m_lifo_cancel_func.pop();
	thread->m_lifo_cancel_args.pop();

	if (routine == NULL) {
		return;
	}

	if (arg == NULL) {
		return;
	}

	if (execute != 0) {
		routine(arg);
	}
}

int UC_posix_class::uc_pthread_cancel(uc_pthread_t tid){
	UC_thread_class *thread = uc_get_thread(tid);
	if (thread == NULL) {
		return ESRCH;
	}

	if (thread->m_attribs.cancelstate == UC_PTHREAD_CANCEL_DISABLE) {
		return 0;
	}

	if(thread->m_attribs.cancelstate == UC_PTHREAD_CANCELED || thread->m_attribs.cancelstate == UC_PTHREAD_FINISHED ) {
		return 0;
	}

	thread->m_attribs.cancelstate = UC_PTHREAD_CANCELED;

	if(thread->m_attribs.canceltype == UC_PTHREAD_CANCEL_ASYNCHRONOUS) {
		thread->unblock();
	}

	return 0;
}

void UC_posix_class::uc_pthread_testcancel(){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	if (thread->m_attribs.cancelstate == UC_PTHREAD_CANCEL_DISABLE) {
		return;
	}

	if (thread->m_attribs.canceltype == UC_PTHREAD_CANCEL_DEFERRED){
		return;
	}

	if (thread->m_attribs.cancelstate == UC_PTHREAD_CANCELED) {
		qt_parent_process->m_error_code = 1;
		uc_pthread_exit((void *)-1);
	}
}

int UC_posix_class::uc_pthread_key_create(uc_pthread_key_t *key_id, void (*destructor)(void*)){
	uc_pthread_key_struct *key;

	if (qt_parent_process->m_key_list.size() >= PTHREAD_KEYS_MAX) {
		return EAGAIN;
	}

	key = new uc_pthread_key_struct;
	key->key_id = qt_parent_rtos->m_key_elements_counter++;
	*key_id = key->key_id;
	key->destructor = destructor;
	key->list.clear();
	qt_parent_process->m_key_list.push_back(key);
	return 0;
}

int UC_posix_class::uc_pthread_key_delete(uc_pthread_key_t key_id){
	vector<uc_pthread_key_struct *>::iterator it_key;
	for (it_key = qt_parent_process->m_key_list.begin(); it_key != qt_parent_process->m_key_list.end(); it_key++) {
		if ((*it_key)->key_id == key_id) {
			delete (*it_key);
			*it_key = NULL;
			qt_parent_process->m_key_list.erase(it_key);
			return 0;
		}
	}
	return -1;
}

int UC_posix_class::uc_pthread_setspecific(uc_pthread_key_t key_id, void *value){
	uc_pthread_t thread = uc_pthread_self();

	vector<uc_pthread_key_struct *>::iterator it_key;
	for (it_key = qt_parent_process->m_key_list.begin(); it_key != qt_parent_process->m_key_list.end(); it_key++) {
		if ((*it_key)->key_id == key_id) {
			bool create_element = true;
			vector<struct uc_key_element *>::iterator it_element;
			for (it_element = (*it_key)->list.begin(); it_element != (*it_key)->list.end(); it_element++) {
				if ((*it_element)->tid == thread) {
					create_element = false;
					break;
				}
			}

			if (create_element == true) {
				(*it_key)->list.push_back(new struct uc_key_element);
				it_element = (*it_key)->list.end();
				it_element--;
				(*it_element)->tid = thread;
			}

			if (value == NULL) {
				delete (*it_element);
				(*it_key)->list.erase(it_element);
				return 0;
			}

			(*it_element)->value = value;
			return 0;
		}
	}

	return -1;
}

void *UC_posix_class::uc_pthread_getspecific(uc_pthread_key_t key_id){
	uc_pthread_t thread = uc_pthread_self();

	vector<uc_pthread_key_struct *>::iterator it_key;
	for (it_key = qt_parent_process->m_key_list.begin(); it_key != qt_parent_process->m_key_list.end(); it_key++) {
		if ((*it_key)->key_id == key_id) {
			vector<struct uc_key_element *>::iterator it_element;
			for (it_element = (*it_key)->list.begin(); it_element != (*it_key)->list.end(); it_element++) {
				if ((*it_element)->tid == thread) {
					return (*it_element)->value;
				}
			}

			return NULL;
		}
	}

	return NULL;
}

int UC_posix_class::uc_pthread_getcpuclockid(uc_pthread_t thread_id, clockid_t *clock_id){
	if( qt_parent_rtos->m_tic_timer->get_clock_pthread(thread_id, clock_id) == -1 ) {
		return ESRCH;
	}

	return 0;
}

int UC_posix_class::uc_pthread_attr_getguardsize(const uc_pthread_attr_t *attr, size_t *guardsize) {
	return -1;
}

int UC_posix_class::uc_pthread_attr_setguardsize(uc_pthread_attr_t *attr, size_t guardsize) {
	return -1;
}

int UC_posix_class::uc_pthread_cond_init(uc_pthread_cond_t *cond, uc_pthread_condattr_t *cond_attr){
	uc_pthread_cond_class *cond_c;
	int cond_id;

	if (cond == NULL) {
		errno = EINVAL;
		return -1;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	cond_c = new uc_pthread_cond_class;
	cond_c->cond_id = qt_parent_rtos->m_cond_id_max++;
	cond_c->parent_process = qt_parent_process;

	cond->id = cond_c->cond_id;

	qt_parent_rtos->m_cond_list.push_back(cond_c);
	qt_parent_process->m_cond_list.push_back(cond);

//	thread->m_semaphore_list_fork.push_back(cond_id);
// FIXME: Semaphore list in the thread. Is it correct/required ????

	thread->m_semaphore_list.push_back(cond_id);

	uc_pthread_condattr_t *attr = new uc_pthread_condattr_t;

	if (cond_attr != NULL) {
		*attr = *cond_attr;
	}
	else {
		uc_pthread_condattr_init(attr);
	}

	cond_c->attr = attr;
	return 0;
}

int UC_posix_class::uc_pthread_cond_destroy(uc_pthread_cond_t *cond){
	if (cond == NULL) {
		return EINVAL;
	}

	uc_pthread_cond_class *cond_c = qt_parent_rtos->find_cond(cond->id);
	if (cond_c == NULL) {
		return EINVAL;
	}

	if (cond_c->blocked_threads.size() > 0) {
		return EBUSY;
	}

	if (cond_c->open_cond.size() == 0) {
		vector<uc_pthread_cond_t *>::iterator it_cond;
		for (it_cond = qt_parent_process->m_cond_list.begin(); it_cond != qt_parent_process->m_cond_list.end(); it_cond++) {
			if (*it_cond == cond) {
				qt_parent_process->m_cond_list.erase(it_cond);
				break;
			}
		}
		vector<uc_pthread_cond_class *>::iterator it_cond_c;
		for (it_cond_c = qt_parent_rtos->m_cond_list.begin(); it_cond_c != qt_parent_rtos->m_cond_list.end(); it_cond_c++) {
			if (*it_cond_c == cond_c) {
				qt_parent_rtos->m_cond_list.erase(it_cond_c);
				break;
			}
		}
	}
	return 0;
}

int UC_posix_class::uc_pthread_cond_signal(uc_pthread_cond_t *cond) {
	if (cond == NULL) {
		return EINVAL;
	}

	uc_pthread_cond_class *cond_c = qt_parent_rtos->find_cond(cond->id);
	if(cond_c == NULL) {
		return EINVAL;
	}

	if (cond_c->blocked_threads.size() != 0) {
		cond_c->blocked_threads[0]->unblock();
	}

	return 0;
}

int UC_posix_class::uc_pthread_cond_broadcast(uc_pthread_cond_t *cond){
	if (cond == NULL) {
		return EINVAL;
	}

	uc_pthread_cond_class *cond_c = qt_parent_rtos->find_cond(cond->id);
	if (cond_c == NULL) {
		return EINVAL;
	}

	cond_c->unblock();
	cond_c->blocked_threads.clear();

	return 0;
}

int UC_posix_class::uc_pthread_cond_timedwait(uc_pthread_cond_t *cond, uc_pthread_mutex_t *mutex,
											  const struct timespec *abs_timeout)
{
	if (cond == NULL) {
		return EINVAL;
	}

	uc_pthread_cond_class *cond_c = qt_parent_rtos->find_cond(cond->id);
	if (cond_c == NULL) {
		return EINVAL;
	}

	if (mutex == NULL) {
		return EINVAL;
	}

	uc_pthread_mutex_class *mutex_c = qt_parent_rtos->find_mutex(mutex->id);
	if (mutex_c == NULL) {
		return EINVAL;
	}

	uc_pthread_testcancel();

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	mutex_c->unblock();

	thread->block(&cond_c->blocked_threads, abs_timeout);

	uc_pthread_testcancel();
	if (thread->m_timeout_expired == true) {
		return ETIMEDOUT;
	}

	mutex_c->lock();

	return 0;
}

int UC_posix_class::uc_pthread_cond_wait(uc_pthread_cond_t *cond, uc_pthread_mutex_t *mutex) {
	if (cond == NULL) {
		return EINVAL;
	}

	uc_pthread_cond_class *cond_c = qt_parent_rtos->find_cond(cond->id);
	if (cond_c == NULL) {
		return EINVAL;
	}

	if (mutex == NULL) {
		return EINVAL;
	}

	uc_pthread_mutex_class *mutex_c = qt_parent_rtos->find_mutex(mutex->id);
	if (mutex_c == NULL) {
		return EINVAL;
	}

	uc_pthread_testcancel();

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	mutex_c->unblock();

	thread->block(&cond_c->blocked_threads);

	uc_pthread_testcancel();

	mutex_c->lock();

	return 0;
}


int UC_posix_class::uc_pthread_condattr_init(uc_pthread_condattr_t *attr){
	if (attr == NULL) {
		return EINVAL;
	}
	attr->pshared = PTHREAD_PROCESS_PRIVATE;
	attr->clk_id = CLOCK_REALTIME;
	return 0;
}

int UC_posix_class::uc_pthread_condattr_destroy(uc_pthread_condattr_t *attr){
	if (attr == NULL) {
		return EINVAL;
	}
	return 0;
}

int UC_posix_class::uc_pthread_condattr_getpshared(const uc_pthread_condattr_t *attr, int *pshared){
	if (attr == NULL) {
		return EINVAL;
	}
	if (pshared == NULL) {
		return EINVAL;
	}

	*pshared = attr->pshared;
	return 0;
}

int UC_posix_class::uc_pthread_condattr_setpshared(uc_pthread_condattr_t *attr, int pshared){
	if (attr == NULL) {
		return EINVAL;
	}
	attr->pshared = pshared;
	return 0;
}

int UC_posix_class::uc_pthread_condattr_getclock(const uc_pthread_condattr_t *attr, clockid_t *clock_id){
	if (attr == NULL) {
		return EINVAL;
	}
	if (clock_id == NULL) {
		return EINVAL;
	}

	*clock_id = attr->clk_id;
	return 0;
}

int UC_posix_class::uc_pthread_condattr_setclock(uc_pthread_condattr_t *attr, clockid_t clock_id){
	if (attr == NULL) {
		return EINVAL;
	}
	if (clock_id != CLOCK_REALTIME && clock_id != CLOCK_MONOTONIC) {
		return EINVAL;
	}

	attr->clk_id = clock_id;
	return 0;
}

int UC_posix_class::uc_pthread_mutex_init(uc_pthread_mutex_t *mutex,const uc_pthread_mutexattr_t *mutexattr){
	uc_pthread_mutex_class *mutex_c;
	int mutex_id;

	if (mutex == NULL) {
		 errno = EINVAL;
		 return -1;
	}

//	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	mutex_c = new uc_pthread_mutex_class;
	mutex_c->mutex_id = qt_parent_rtos->m_mutex_id_max++;
	mutex_c->parent_process = qt_parent_process;
	mutex->id=mutex_c->mutex_id;

	qt_parent_rtos->m_mutex_list.push_back(mutex_c);

// 	if(UC_PERFIDIX_Kernel.simulation_started){
		qt_parent_process->m_mutex_list.push_back(mutex);
//		thread->m_semaphore_list_fork.push_back(mutex_id);
// 	}

	uc_pthread_mutexattr_t *attr = new uc_pthread_mutexattr_t;

	if (mutexattr != NULL) {
		*attr = *mutexattr;
	}
	else {
		uc_pthread_mutexattr_init(attr);
	}

	mutex_c->mutexattr = attr;
	return 0;
}

int UC_posix_class::uc_pthread_mutex_lock(uc_pthread_mutex_t *mutex){
	uc_pthread_mutex_class *mutex_c;

	if (mutex == NULL) {
		errno = EINVAL;
		return -1;
	}

	if((mutex_c = qt_parent_rtos->find_mutex(mutex->id)) == NULL) {
		return -1;
	}

	if (mutex_c->mutexattr == NULL) {
		return -1;
	}

	return mutex_c->lock();
}

int UC_posix_class::uc_pthread_mutex_timedlock(uc_pthread_mutex_t *mutex, const struct timespec *abs_timeout){
	uc_pthread_mutex_class *mutex_c;

	if (mutex == NULL) {
		errno = EINVAL;
		return -1;
	}

	if ((mutex_c = qt_parent_rtos->find_mutex(mutex->id)) == NULL) {
		 return -1;
	}

	if (mutex_c->mutexattr == NULL) {
		return EINVAL;
	}

	if (abs_timeout == NULL) {
		return EINVAL;
	}

	if (abs_timeout->tv_nsec >= 1000000000) {
		return EINVAL;
	}

	if (abs_timeout->tv_nsec < 0) {
		return EINVAL;
	}

	return mutex_c->timedlock(abs_timeout);
}

int UC_posix_class::uc_pthread_mutex_unlock(uc_pthread_mutex_t *mutex){
	uc_pthread_mutex_class *mutex_c;

	if (mutex == NULL) {
		errno = EINVAL;
		return -1;
	}

	if ((mutex_c = qt_parent_rtos->find_mutex(mutex->id)) == NULL) {
		 return -1;
	}

	if (mutex_c->mutexattr == NULL) {
		return -1;
	}

	return mutex_c->unlock();
}

int UC_posix_class::uc_pthread_mutex_trylock(uc_pthread_mutex_t *mutex){
	uc_pthread_mutex_class *mutex_c;

	if (mutex == NULL) {
		 errno = EINVAL;
		 return -1;
	}

	if ((mutex_c = qt_parent_rtos->find_mutex(mutex->id)) == NULL) {
		 return -1;
	}

	if (mutex_c->mutexattr == NULL) {
		return -1;
	}

	return mutex_c->trylock();
}

int UC_posix_class::uc_pthread_mutex_destroy(uc_pthread_mutex_t *mutex){
	uc_pthread_mutex_class *mutex_c;

	if (mutex == NULL) {
		 errno = EINVAL;
		 return -1;
	}

	if ((mutex_c = qt_parent_rtos->find_mutex(mutex->id)) == NULL) {
		return -1;
	}

	if (mutex_c->mutexattr == NULL) {
		return EINVAL;
	}

	if (!mutex_c->empty()) {
		return EBUSY;
	}

	vector<uc_pthread_mutex_t *>::iterator it_mutex;
	for (it_mutex = qt_parent_process->m_mutex_list.begin(); it_mutex != qt_parent_process->m_mutex_list.end(); it_mutex++) {
		if (*it_mutex == mutex) {
			qt_parent_process->m_mutex_list.erase(it_mutex);
			break;
		}
	}

	vector<uc_pthread_mutex_class *>::iterator it_mutex_c;
	for (it_mutex_c = qt_parent_rtos->m_mutex_list.begin(); it_mutex_c != qt_parent_rtos->m_mutex_list.end(); it_mutex_c++) {
		if (*it_mutex_c == mutex_c) {
			qt_parent_rtos->m_mutex_list.erase(it_mutex_c);
			break;
		}
	}

	return 0;
}


int UC_posix_class::uc_pthread_mutex_getprioceiling(const uc_pthread_mutex_t *mutex, int *ceiling){
	uc_pthread_mutex_class *mutex_c;

	if (mutex == NULL) {
		 errno = EINVAL;
		 return -1;
	}

	if ((mutex_c = qt_parent_rtos->find_mutex(mutex->id)) == NULL) {
		return -1;
	}

	if (mutex_c->mutexattr == NULL) {
		return -1;
	}

	*ceiling = mutex_c->mutexattr->prioceiling;
	return 0;
}

int UC_posix_class::uc_pthread_mutex_setprioceiling(const uc_pthread_mutex_t *mutex, int prioceiling, int *ceiling){
	uc_pthread_mutex_class *mutex_c;

	if (mutex == NULL) {
		 errno = EINVAL;
		 return -1;
	}

	if ((mutex_c = qt_parent_rtos->find_mutex(mutex->id)) == NULL) {
		 return -1;
	}

	if (mutex_c->mutexattr == NULL) {
		return 1;
	}

	*ceiling = mutex_c->mutexattr->prioceiling;
	mutex_c->mutexattr->prioceiling = prioceiling;
	return 0;
}

int UC_posix_class::uc_pthread_mutexattr_init(uc_pthread_mutexattr_t *attr){
	if (attr == NULL) {
		return -1;
	}

	attr->type = PTHREAD_MUTEX_DEFAULT;
	attr->protocol = PTHREAD_PRIO_NONE;
	attr->pshared = PTHREAD_PROCESS_PRIVATE;
	attr->prioceiling = PTHREAD_MIN_PRIORITY_2;
	return 0;
}

int UC_posix_class::uc_pthread_mutexattr_destroy(uc_pthread_mutexattr_t *attr){
	if (attr == NULL) {
		return EINVAL;
	}
	return 0;
}

int UC_posix_class::uc_pthread_mutexattr_getprioceiling(const uc_pthread_mutexattr_t *attr, int* ceiling){
	if (attr == NULL) {
		return EINVAL;
	}
	if (ceiling == NULL) {
		return EINVAL;
	}
	*ceiling = attr->prioceiling;
	return 0;
}

int UC_posix_class::uc_pthread_mutexattr_getprotocol(const uc_pthread_mutexattr_t *attr, int* ceiling){
	if (attr == NULL) {
		return EINVAL;
	}
	if (ceiling == NULL) {
		return EINVAL;
	}
	*ceiling = attr->protocol;
	return 0;
}

int UC_posix_class::uc_pthread_mutexattr_getpshared(const uc_pthread_mutexattr_t *attr, int* ceiling){
	if (attr == NULL) {
		return EINVAL;
	}
	if (ceiling == NULL) {
		return EINVAL;
	}
	*ceiling = attr->pshared;
	return 0;
}

int UC_posix_class::uc_pthread_mutexattr_gettype(const uc_pthread_mutexattr_t *attr, int* ceiling){
	if (attr == NULL) {
		return EINVAL;
	}
	if (ceiling == NULL) {
		return EINVAL;
	}
	*ceiling = attr->type;
	return 0;
}

int UC_posix_class::uc_pthread_mutexattr_setprioceiling( uc_pthread_mutexattr_t *attr, int value){
	if (attr == NULL) {
		return 1;
	}
	attr->prioceiling = value;
	return 0;
}

int UC_posix_class::uc_pthread_mutexattr_setprotocol( uc_pthread_mutexattr_t *attr, int value){
	if (attr == NULL) {
		return 1;
	}
	attr->protocol = value;
	return 0;
}

int UC_posix_class::uc_pthread_mutexattr_setpshared( uc_pthread_mutexattr_t *attr, int value){
	if (attr == NULL) {
		return 1;
	}
	attr->pshared = value;
	return 0;
}

int UC_posix_class::uc_pthread_mutexattr_settype( uc_pthread_mutexattr_t *attr, int value){
	if (attr == NULL) {
		return 1;
	}
	if (value < 0) {
		return EINVAL;
	}
	attr->type = value;
	return 0;
}

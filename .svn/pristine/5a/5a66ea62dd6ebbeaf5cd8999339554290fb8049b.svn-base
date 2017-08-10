//==========================================================================
//	uc_pthread.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: vie nov 9 2007
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

#ifndef UC_PTHREAD_H
#define UC_PTHREAD_H

#include "uc_sched.h"
#include "sys_uc_types.h"
/*#include "rtos/api/posix/uc_sched.h"
#include "rtos/api/posix/sys_uc_types.h"*/
// #include <vector>
// using namespace std;

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/*#include "rtos/api/posix/uc_pthread_rwlock.h"
#include "rtos/api/posix/uc_pthread_spin_lock.h"*/
#include "uc_pthread_rwlock.h"
#include "uc_pthread_spin_lock.h"

/**********************************************************
//
//  BAR - not implemented
// #define PTHREAD_BARRIER_SERIAL_THREAD 	0
//
///////////////////////////////////////////////////////////
//
//  XSI - not implemented
//  #define PTHREAD_MUTEX_DEFAILT, PTHREAD_MUTEX_ERRORCHECK,
//	PTHREAD_MUTEX_NORMAL,PTHREAD_MUTEX_RECURSIVE
//
***********************************************************/

#undef PTHREAD_CANCEL_ASYNCHRONOUS
#undef PTHREAD_CANCEL_DEFERRED
enum {  UC_PTHREAD_CANCEL_ASYNCHRONOUS,
  UC_PTHREAD_CANCEL_DEFERRED};

#define PTHREAD_CANCEL_ASYNCHRONOUS UC_PTHREAD_CANCEL_ASYNCHRONOUS
#define PTHREAD_CANCEL_DEFERRED UC_PTHREAD_CANCEL_DEFERRED

#undef PTHREAD_CANCEL_ENABLE
#undef PTHREAD_CANCEL_DISABLE
enum {  UC_PTHREAD_CANCEL_ENABLE,
  UC_PTHREAD_CANCEL_DISABLE,UC_PTHREAD_CANCELED,UC_PTHREAD_FINISHED};

#define PTHREAD_CANCEL_ENABLE UC_PTHREAD_CANCEL_ENABLE
#define PTHREAD_CANCEL_DISABLE UC_PTHREAD_CANCEL_DISABLE

#undef PTHREAD_CANCELED
#define PTHREAD_CANCELED ((void *) -1)


#undef PTHREAD_SPIN_LOCKED
#undef PTHREAD_SPIN_UNLOCKED
#ifdef __parisc__
#define PTHREAD_SPIN_LOCKED 0
#define PTHREAD_SPIN_UNLOCKED 1
#else
#define PTHREAD_SPIN_LOCKED 1
#define PTHREAD_SPIN_UNLOCKED 0
#endif

// #undef PTHREAD_COND_INITIALIZER
// #define PTHREAD_COND_INITIALIZER  uc_pthread_cond_init::init_cond()


#undef PTHREAD_CREATE_JOINABLE
#undef PTHREAD_CREATE_DETACHED
enum{  UC_PTHREAD_CREATE_JOINABLE,
  UC_PTHREAD_CREATE_DETACHED};

#define PTHREAD_CREATE_JOINABLE UC_PTHREAD_CREATE_JOINABLE
#define PTHREAD_CREATE_DETACHED UC_PTHREAD_CREATE_DETACHED

#undef PTHREAD_EXPLICIT_SCHED
#undef PTHREAD_INHERIT_SCHED
enum{  UC_PTHREAD_EXPLICIT_SCHED,
  UC_PTHREAD_INHERIT_SCHED};

#define PTHREAD_EXPLICIT_SCHED UC_PTHREAD_EXPLICIT_SCHED
#define PTHREAD_INHERIT_SCHED UC_PTHREAD_INHERIT_SCHED


enum
{
  UC_PTHREAD_PRIO_NONE,  // sin defensa de prioridad
#define PTHREAD_PRIO_NONE UC_PTHREAD_PRIO_NONE
  UC_PTHREAD_PRIO_INHERIT, // herencia simple
#define PTHREAD_PRIO_INHERIT UC_PTHREAD_PRIO_INHERIT
  UC_PTHREAD_PRIO_PROTECT  // techo inmediato
#define PTHREAD_PRIO_PROTECT UC_PTHREAD_PRIO_PROTECT
};

#undef PTHREAD_PROCESS_PRIVATE
#undef PTHREAD_PROCESS_SHARED
enum{  UC_PTHREAD_PROCESS_PRIVATE,
  UC_PTHREAD_PROCESS_SHARED};

#define PTHREAD_PROCESS_PRIVATE UC_PTHREAD_PROCESS_PRIVATE
#define PTHREAD_PROCESS_SHARED UC_PTHREAD_PROCESS_SHARED

/* for completeness */
#undef PTHREAD_SCOPE_PROCESS
#undef PTHREAD_SCOPE_SYSTEM
enum{  UC_PTHREAD_SCOPE_SYSTEM,
  UC_PTHREAD_SCOPE_PROCESS};

#define PTHREAD_SCOPE_SYSTEM UC_PTHREAD_SCOPE_SYSTEM
#define PTHREAD_SCOPE_PROCESS UC_PTHREAD_SCOPE_PROCESS

enum{  UC_PTHREAD_MUTEX_NORMAL,UC_PTHREAD_MUTEX_ERRORCHECK,
  UC_PTHREAD_MUTEX_RECURSIVE,UC_PTHREAD_MUTEX_DEFAULT};
#define PTHREAD_MUTEX_NORMAL UC_PTHREAD_MUTEX_NORMAL
#define PTHREAD_MUTEX_ERRORCHECK UC_PTHREAD_MUTEX_ERRORCHECK
#define PTHREAD_MUTEX_RECURSIVE UC_PTHREAD_MUTEX_RECURSIVE
#define PTHREAD_MUTEX_DEFAULT UC_PTHREAD_MUTEX_DEFAULT

// #undef PTHREAD_MUTEX_INITIALIZER
// #define PTHREAD_MUTEX_INITIALIZER uc_pthread_mutex_init::init_mutex()

/**********************************************/
#undef PTHREAD_STACK_SIZE
#define PTHREAD_STACK_SIZE	16384

#undef PTHREAD_THREADS_MAX
#define PTHREAD_THREADS_MAX	128

#undef MAX_SPIN_COUNT
#define MAX_SPIN_COUNT		50
#undef SPIN_SLEEP_DURATION
#define SPIN_SLEEP_DURATION	2000001

#undef PTHREAD_KEYS_MAX
#define PTHREAD_KEYS_MAX	20
#undef PTHREAD_DESTRUCTOR_ITERATIONS
#define PTHREAD_DESTRUCTOR_ITERATIONS 10

/*************************************************************************/
// 	In library pthread.h

//	Attributes for threads.

// Initialize attributes object with default attributes:
// detachstate          == PTHREAD_JOINABLE
// scope                == PTHREAD_SCOPE_SYSTEM
// inheritsched         == PTHREAD_EXPLICIT_SCHED
// schedpolicy          == SCHED_OTHER
// schedparam           == sched_priority == NZERO
// stackaddr            == unset
// stacksize            == 0
//

#undef PTHREAD_ONCE_INIT
#define PTHREAD_ONCE_INIT 1

typedef int uc_pthread_once_t;

typedef struct uc_pthread_attr
{
  uc_pthread_t	pthread_id;
  int		detachstate;
  int		schedpolicy;
  struct uc_sched_param schedparam;
  int		inheritsched;
  int		scope;
  void *	stackaddr;
  unsigned long stacksize;
  int		cancelstate;
  int		canceltype;
} uc_pthread_attr_t;

typedef struct {
  int pshared;
  int id;
  clockid_t clk_id;
} uc_pthread_condattr_t;

typedef struct {
	int id;
	int initialized;
} uc_pthread_cond_t;

typedef struct {
	int id;
	int initialized;
} uc_pthread_mutex_t;

typedef struct {
  int type;
  int protocol;
  int pshared;
  int prioceiling;
} uc_pthread_mutexattr_t;

struct uc_key_element{
	uc_pthread_t tid;
	void *value;
};

typedef  int uc_pthread_key_t;

/**************************************************************************/

/* FORK */

// int uc_pthread_atfork(void (*prepare)(void), void (*parent)(void),void (*child)(void));

int uc_pthread_attr_init(uc_pthread_attr_t *attr);
int uc_pthread_attr_destroy(uc_pthread_attr_t *attr);

int uc_pthread_attr_setdetachstate(uc_pthread_attr_t *attr, int detachstate);
int uc_pthread_attr_getdetachstate(const uc_pthread_attr_t *attr, int *detachstate);
int uc_pthread_attr_setschedpolicy(uc_pthread_attr_t *attr, int policy);
int uc_pthread_attr_getschedpolicy(const uc_pthread_attr_t *attr, int *policy);
int uc_pthread_attr_setschedparam(uc_pthread_attr_t *attr,const struct uc_sched_param *param);
int uc_pthread_attr_getschedparam(const uc_pthread_attr_t *attr,struct uc_sched_param *param);

int uc_pthread_attr_setinheritsched(uc_pthread_attr_t *attr, int inherit);
int uc_pthread_attr_getinheritsched(const uc_pthread_attr_t *attr, int *inherit);
int uc_pthread_attr_setscope(uc_pthread_attr_t *attr, int scope);
int uc_pthread_attr_getscope(const uc_pthread_attr_t *attr, int *scope);

int uc_pthread_attr_setstack(uc_pthread_attr_t *attr, void *stack, size_t stacksize);
int uc_pthread_attr_getstack(uc_pthread_attr_t *attr, void **stack, size_t *stacksize);
int uc_pthread_attr_setstackaddr(uc_pthread_attr_t *attr, void *stack);
int uc_pthread_attr_getstackaddr(uc_pthread_attr_t *attr, void **stack);
int uc_pthread_attr_setstacksize(uc_pthread_attr_t *attr, size_t stacksize);
int uc_pthread_attr_getstacksize(uc_pthread_attr_t *attr, size_t *stacksize);

// XSI extension guardsize not implemented

int uc_pthread_attr_getguardsize(const uc_pthread_attr_t *attr, size_t *guardsize);
int uc_pthread_attr_setguardsize(uc_pthread_attr_t *attr, size_t guardsize);

/* CANCEL */
int uc_pthread_cancel(uc_pthread_t thread);

// #undef pthread_cleanup_push
void uc_pthread_cleanup_push(void (*routine)(void*), void *arg);

// #undef pthread_cleanup_pop
void uc_pthread_cleanup_pop (int execute);

/* Conditions */

int uc_pthread_cond_init(uc_pthread_cond_t *cond, uc_pthread_condattr_t *cond_attr);
int uc_pthread_cond_destroy(uc_pthread_cond_t *cond);
int uc_pthread_cond_signal(uc_pthread_cond_t *cond);
int uc_pthread_cond_broadcast(uc_pthread_cond_t *cond);
int uc_pthread_cond_timedwait(uc_pthread_cond_t *cond, uc_pthread_mutex_t *mutex,const struct timespec *abstime);
int uc_pthread_cond_wait(uc_pthread_cond_t *cond, uc_pthread_mutex_t *mutex);

int uc_pthread_condattr_init(uc_pthread_condattr_t *attr);
int uc_pthread_condattr_destroy(uc_pthread_condattr_t *attr);
int uc_pthread_condattr_getpshared(const uc_pthread_condattr_t *attr, int *pshared);
int uc_pthread_condattr_setpshared(uc_pthread_condattr_t *attr, int pshared);
int uc_pthread_condattr_getclock(const uc_pthread_condattr_t *attr,clockid_t *clock_id);
int uc_pthread_condattr_setclock(uc_pthread_condattr_t *attr,clockid_t clock_id);

/* THREADS */
int uc_pthread_create (uc_pthread_t *__threadarg,const uc_pthread_attr_t *__attr,
		void *(*__start_routine) (void *),void *__arg,const char *func_name);
int uc_pthread_detach (uc_pthread_t __th);
int uc_pthread_equal (uc_pthread_t __thread1, uc_pthread_t __thread2);
void uc_pthread_exit (void *__retval);
int uc_pthread_join (uc_pthread_t __th, void **__thread_return);

int uc_pthread_setschedparam(uc_pthread_t target_thread, int policy,
			  const struct uc_sched_param *param);
int uc_pthread_getschedparam(uc_pthread_t target_thread, int *policy,
			  struct uc_sched_param *param);
int uc_pthread_setschedprio(uc_pthread_t target_thread, int priority);
int uc_pthread_getschedprio(uc_pthread_t target_thread, int *priority);

/* thread specific variables */
int uc_pthread_key_create(uc_pthread_key_t *key, void (*destructor)(void*));
int uc_pthread_key_delete(uc_pthread_key_t key);
int uc_pthread_setspecific(uc_pthread_key_t key, void *value);
void *uc_pthread_getspecific(uc_pthread_key_t key);

int uc_pthread_getcpuclockid(uc_pthread_t thread_id, clockid_t *clock_id);

/* Mutexes */
int uc_pthread_mutex_init(uc_pthread_mutex_t *mutex,const uc_pthread_mutexattr_t *mutexattr);
int uc_pthread_mutex_lock(uc_pthread_mutex_t *mutex);
int uc_pthread_mutex_unlock(uc_pthread_mutex_t *mutex);
int uc_pthread_mutex_trylock(uc_pthread_mutex_t *mutex);
int uc_pthread_mutex_destroy(uc_pthread_mutex_t *mutex);

int uc_pthread_mutex_getprioceiling(const uc_pthread_mutex_t *, int *);
int uc_pthread_mutex_setprioceiling(const uc_pthread_mutex_t *mutex, int prioceiling, int *);
int uc_pthread_mutex_timedlock(uc_pthread_mutex_t *mutex,const struct timespec* timelock);

int uc_pthread_mutexattr_init(uc_pthread_mutexattr_t *attr);
int uc_pthread_mutexattr_destroy(uc_pthread_mutexattr_t *attr);

int uc_pthread_mutexattr_getprioceiling(const uc_pthread_mutexattr_t *attr, int* );
int uc_pthread_mutexattr_getprotocol(const uc_pthread_mutexattr_t *attr, int* );
int uc_pthread_mutexattr_getpshared(const uc_pthread_mutexattr_t *attr, int* );
int uc_pthread_mutexattr_gettype(const uc_pthread_mutexattr_t *attr, int* );

int uc_pthread_mutexattr_setprioceiling( uc_pthread_mutexattr_t *attr, int value);
int uc_pthread_mutexattr_setprotocol( uc_pthread_mutexattr_t *attr, int value);
int uc_pthread_mutexattr_setpshared( uc_pthread_mutexattr_t *attr, int value);
int uc_pthread_mutexattr_settype( uc_pthread_mutexattr_t *attr, int value);

/****************************************************************

	Linux but not POSIX

int pthread_mutexattr_getkind_np(const pthread_mutexattr_t *attr, int *kind);
int pthread_mutexattr_setkind_np(pthread_mutexattr_t *attr, int kind);


int __pthread_once(pthread_once_t* once_control, void (*init_routine)(void));

*****************************************************************/

/* ONCE */
int uc_pthread_once(uc_pthread_once_t* once_control, void (*init_routine)(void));
uc_pthread_t uc_pthread_self();

/* CANCEL */
int uc_pthread_setcancelstate(int state, int *oldstate);
int uc_pthread_setcanceltype(int type, int *oldtype);
void uc_pthread_testcancel();


/***************** DEFINES FOR USER CODE *********************/

#ifdef EXAMPLE

#define pthread_create(a,b,c,d) uc_pthread_create(a,b,(void *(*)(void *))c,d,#c)

#endif // EXAMPLE

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

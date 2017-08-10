//==========================================================================
//	uc_stp.cpp
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

#define SC_INCLUDE_DYNAMIC_PROCESSES

#define QT_STKALIGN QUICKTHREADS_STKALIGN
#define QT_SP QUICKTHREADS_SP
#define QT_ARGS QUICKTHREADS_ARGS
#define QT_ABORT QUICKTHREADS_ABORT
#define QT_BLOCK QUICKTHREADS_BLOCK

#include <setjmp.h>

#include "rtos/qt_interface/copyright.h"
#include "rtos/qt_interface/qt.h"
#include "rtos/qt_interface/stp.h"
#include "rtos/qt_interface/uc_stp.h"

#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/kernel/uc_process_class.h"
#include "rtos/kernel/uc_task_class.h"
#include "rtos/kernel/uc_thread_class.h"
#include "rtos/utils/uc_backtrace.h"
#include "rtos/api/posix/uc_posix.h"

__thread UC_rtos_class *qt_parent_rtos;
__thread UC_process_class *qt_parent_process;

#ifndef NULL
#define NULL	0
#endif

#define STP_STKSIZE (0x100000)

/* `alignment' must be a power of 2. */
#define STP_STKALIGN(sp, alignment) \
	((void *)((((qt_word_t)(sp)) + (alignment) - 1) & ~((alignment)-1)))


/* The notion of a thread is merged with the notion of a queue.
   Thread stuff: thread status (sp) and stuff to use during
   (re)initialization.  Queue stuff: next thread in the queue
   (next). */

struct stp_t
{
	qt_t *sp;              /* QuickThreads handle. */
	void *sto;             /* `malloc'-allocated stack. */
	struct stp_t *next;    /* Next thread in the queue. */
};


/* A queue is a circular list of threads.  The queue head is a
   designated list element.  If this is a uniprocessor-only
   implementation we can store the `main' thread in this, but in a
   multiprocessor there are several `heavy' threads but only one run
   queue.  A fancier implementation might have private run queues,
   which would lead to a simpler (trivial) implementation */

typedef struct stp_q_t
{
	stp_t t;
	stp_t *tail;
}
stp_q_t;


/* Helper functions. */

extern void *malloc(unsigned size);
extern void perror(char const *msg);
extern void free(void *sto);

void * xmalloc(unsigned size)
{
	void *sto;

	sto = malloc(size);
	if (!sto)
	{
		perror("malloc");
		exit(1);
	}
	return (sto);
}

/* Queue access functions. */

static void stp_qinit(stp_q_t *q)
{
	q->t.next = q->tail = &q->t;
}


static stp_t * stp_qget(stp_q_t *q)
{
	stp_t *t;

	t = q->t.next;
	q->t.next = t->next;
	if (t->next == &q->t)
	{
		if (t == &q->t)
		{		/* If it was already empty .. */
			return (NULL);		/* .. say so. */
		}
		q->tail = &q->t;		/* Else now it is empty. */
	}
	return (t);
}


static void stp_qput(stp_q_t *q, stp_t *t)
{
	q->tail->next = t;
	t->next = &q->t;
	q->tail = t;
}


/* Thread routines. */

static __thread stp_q_t stp_global_runq;	/* A queue of runable threads. */
static __thread stp_t stp_global_main;		/* Thread for the process. */
static __thread stp_t *stp_global_curr;		/* Currently-executing thread. */

static void *stp_starthelp(qt_t *old, void *ignore0, void *ignore1);
static void stp_only(void *pu, void *pt, qt_userf_t *f);
static void *stp_aborthelp(qt_t *sp, void *old, void *null);
static void *stp_yieldhelp(qt_t *sp, void *old, void *blockq);

void stp_init()
{
	stp_qinit(&stp_global_runq);
}

// Main function of the qt's scheduler.
// It has been modified to sync with the library simulation
void stp_start()
{
	stp_t *next;
	UC_process_class *pp;

	while (true)
	{
		next = stp_qget(&stp_global_runq);
		if (next == NULL)
		{
			if (qt_parent_process->m_stp_destroy == true)
			{
				pthread_mutex_unlock(&qt_parent_rtos->m_rtos_mutex);
				break;
			}
			pthread_mutex_unlock(&qt_parent_rtos->m_rtos_mutex);
			pthread_mutex_lock  (&qt_parent_process->m_process_mutex);
			next = qt_parent_process->m_stp;
			if (next == NULL)
			{
				break; // No more threads to schedule, destroying the process
			}
		}
		stp_global_curr = next;
		QT_BLOCK(stp_starthelp, 0, 0, next->sp);
	}
}


static void * stp_starthelp(qt_t *old, void *ignore0, void *ignore1)
{
	stp_global_main.sp = old;
	stp_qput(&stp_global_runq, &stp_global_main);
	/* return (garbage); */
}


void stp_create(stp_userf_t *f, void *pu)
{
	stp_t *t;
	void *sto;

	t = (stp_t*)xmalloc(sizeof(stp_t));
	t->sto = xmalloc(STP_STKSIZE);
	sto = STP_STKALIGN(t->sto, QT_STKALIGN);
	t->sp = QT_SP(sto, STP_STKSIZE - QT_STKALIGN);
	t->sp = QT_ARGS(t->sp, pu, t, (qt_userf_t *)f, stp_only);
	stp_qput(&stp_global_runq, t);
}


static void stp_only(void *pu, void *pt, qt_userf_t *f)
{
	stp_global_curr = (stp_t *)pt;
	(*(stp_userf_t *)f)(pu);
	stp_abort();
	/* NOTREACHED */
}


void stp_abort(void)
{
	stp_t *old, *newthread;

	newthread = stp_qget(&stp_global_runq);
	old = stp_global_curr;
	stp_global_curr = newthread;
	QT_ABORT(stp_aborthelp, old, (void *)NULL, newthread->sp);
}


static void * stp_aborthelp(qt_t *sp, void *old, void *null)
{
	free(((stp_t *)old)->sto);
	free(old);
	/* return (garbage); */
}


void stp_yield()
{
	stp_t *old, *newthread;
	UC_thread_class * thread;

	newthread = stp_qget(&stp_global_runq);
	old = stp_global_curr;
	stp_global_curr = newthread;

//#if DEBUG //FIXME: qt_parent_rtos->get_current_cpu() is probably wrong (at startup?).
	if(uc_backtrace_required() != 0){
	//	if(qt_parent_process != NULL && qt_parent_process->m_current_task!= NULL){
		if(qt_parent_rtos->get_current_cpu() != NULL){
			thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
			uc_save_backtrace(thread);
		}
	}

//#endif


	QT_BLOCK(stp_yieldhelp, old, &stp_global_runq, newthread->sp);

	thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if (thread->m_attribs.cancelstate == UC_PTHREAD_CANCELED) {
		qt_parent_process->m_error_code = 1;
		qt_parent_rtos->POSIX->uc_pthread_testcancel();
	}

	if(thread->m_signal_received && !thread->m_waiting_processor_in_signal
		  && thread->m_in_signal < MAX_CHAINED_SIGNALS)
	{
		qt_parent_process->signals_control.execute_signals(thread);
	}
}


static void * stp_yieldhelp(qt_t *sp, void *old, void *blockq)
{
 	((stp_t *)old)->sp = sp;
	// Erased, the queue of threads is controlled by the library scheduler
// 	stp_qput((stp_q_t *)blockq, (stp_t *)old);
	/* return (garbage); */
}

/**
 * \brief Starts an user function
 *
 * This function is called by UC_qt_wrapper to sync itself with the rtos simulator
 * and start the correspondent user function
 * \param void_thread The pointer to its UC_thread_class
 */
void UC_user_code_wrapper(void *void_thread) {
	UC_thread_class *thread = (UC_thread_class *)void_thread;
	int ret_jmp;

	// FIXME: Try/catch solution does not work with C sw code (only C++)
	
	if((ret_jmp=setjmp(thread->setjmp_buf))==0){

//	try {
		thread->init_thread(stp_global_curr);
		thread->m_initial_function(thread->m_initial_function_args);
	}
//	catch (char const *message) {
	else{
		/*Thread exited. Nothing to do */
	}
	thread->destroy_thread();
}

/**
 * \brief Creates the stp thread
 *
 * Initializes the global variables and the qt library and, creates the stp thread
 * \param void_thread Pointer to its UC_thread_class
 * \return Returns NULL
 */
void *UC_qt_wrapper(void *void_thread) {
	UC_thread_class *thread = (UC_thread_class *)void_thread;
	qt_parent_process = thread->m_parent_task->m_parent_process;
	qt_parent_rtos = qt_parent_process->m_parent_rtos;

	stp_init();

	stp_create (UC_user_code_wrapper, void_thread);

	stp_start();

	qt_parent_process->destroy_process();

	return NULL;
}

/**
 * \brief Destroy the thread in SCoPE
 *
 * Raises an exception which is caught at the UC_user_code_wrapper function
 * \param void_thread Pointer to its UC_thread_class
 * \return It should never return
 */
void UC_qt_end_this_thread(void *void_thread) {

	UC_thread_class *thread = (UC_thread_class *)void_thread;

	// FIXME: Temporal solution. Search for a better one ...
	// Throwing functions is not possible with SW code in C. only in C++

	/*throw "Thread exit";*/

	// solution A: Return to the OS SW code manager through a lonjmp call
	longjmp(thread->setjmp_buf,1);

	/*When solution A is active this is dead code*/

	// solution B: Abort the thread in qt
	// Copying the end of UC_user_code_wrapper
	thread->destroy_thread();
	// Destroying the thread in qt
	stp_abort();
	/* NOTREACHED */
		
}

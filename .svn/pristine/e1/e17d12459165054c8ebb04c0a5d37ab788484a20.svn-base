//==========================================================================
//	uc_thread_class.h
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

#ifndef UC_THREAD_CLASS_H
#define UC_THREAD_CLASS_H

#include <stack>
#include <setjmp.h>
#include "rtos/qt_interface/stp.h"
#include "rtos/kernel/uc_enums.h"
#include "rtos/api/posix/uc_pthread.h"
#include "rtos/kernel/uc_signals_control.h"
#include "rtos/drivers/uc_hal_sw_defines.h"

using namespace std;

class UC_task_class;
class UC_cpu_class;
class uc_pthread_rwlock_class;

class UC_thread_class{
public:
	enum microcos_unblocker {UNKNOWN, SEMAPHORE, MQUEUE}; ///< Enum of possible MicroCOS multievent unblockers

    UC_thread_class(UC_task_class *task);

    ~UC_thread_class();

	void init_thread(stp_t *stp);

	void destroy_thread();

	bool set_next_state(UC_process_states initial_state, UC_process_states final_state);
	bool update_state();

	void *qt_wrapper(void *);
	void *user_code_wrapper(void *);

	void annotate_user_time(unsigned long long time, unsigned long long instr);
	void annotate_rtos_time(unsigned long long time, unsigned long long instr);

	void block(vector<UC_thread_class *> *block_list);
	bool block(vector<UC_thread_class *> *block_list, const struct timespec *abs_timeout);
	void block();
	bool block(const struct timespec *abs_timeout);
	void unblock();
	void set_microcos_unblocker(microcos_unblocker unblocker, int id);

	/**
	 * \brief Pointer to the qt simple thread structure
	 *
	 * This pointer is necessary to schedule the qt threads. It is copied
	 * to the parent process object and the POSIX thread awaked.
	 */
	stp_t *m_stp;

	void *(*m_initial_function)(void *); ///< Function to be executed when this thread starts
	void *m_initial_function_args; ///< Arguments of the m_initial_function

	UC_task_class *m_parent_task; ///< The parent task

	UC_process_states m_state; ///< The state of the thread
	UC_process_states m_next_state; ///< The next state of the thread
	UC_process_states m_preempt_state; ///< The next state when remaining time due to a preemption finishes

	uc_pthread_attr_t m_attribs; ///< Thread's attributes

	int m_RR_counter; ///< Round Robin counter

	int m_cpu; ///< The number of cpu in which it is executing
	UC_cpu_class *m_current_cpu;  ///< The cpu in which was scheduled last time. It is an unsafe value

	bool m_kernel_mode; ///< Specify whether the thread is in kernel mode

	int m_priority; ///< The thread's priority

	unsigned long long m_parser_time; ///< Nanoseconds executed by this thread
	unsigned long long m_parser_instr; ///< Instructions executed by this thread
	unsigned long long m_switches_time; ///< Nanoseconds executed by the switches

	unsigned int m_tid; ///< The thread id number

	char *m_func_name; ///< The thread's name

	stack<void (*)(void *)> m_lifo_cancel_func;
	stack<void *> m_lifo_cancel_args;
	void * m_end_value;

	/**
	 * \brief The list of threads waiting the end of this thread
	 *
	 * It is used commonly for the pthread_join. The calling thread is blocked in this list
	 * and awaked when this thread is cancelled or finishs
	 */
	vector<UC_thread_class *> m_blocked_thread_list;

	/**
	 * \brief The list of pointers to the lists in which this thread is blocked
	 *
	 * These lists belongs to semaphores, mutexes, pthread_join...
	 */
	vector< vector<UC_thread_class *> * > m_blocked_in_list;

	vector<int> m_semaphore_list;

	int m_channel_priority;

	bool m_timeout_expired;

	int m_in_rwlock;

	vector<uc_pthread_rwlock_class *> m_rwlock_holds;

	vector<int> m_semaphore_list_fork;

	int m_signal_executed;
	int m_signal_received;
	int m_in_signal;
	int m_waiting_processor_in_signal;
	int m_is_waiting_signal;
	int m_waiting_pid;

	UC_signals_data signals;

	int MicroCOS_flags_ready;
	microcos_unblocker m_microcos_unblocker;
	int m_microcos_unblocker_id;

	int wait_queue_state;

	int m_num_backtrace;	///< Number of strings of the backtrace list
	char **m_backtrace;		///< Strings of the thread backtrace
	void **m_backtrace_p;	///< Pointers to the thread backtrace functions

	uc_cpu_bus_transfer_info_t m_bus_transfer_info;		///< Info for IO bus transfers
	vector<uc_cpu_bus_transfer_info_t *> m_bus_transfer_list;///< List of required bus transfers (IO, caches, ...)

	/**
	 * When the thread awakes after a wait with timeout,
	 * the remaining time until the timeout expires
	 */
	struct timespec m_timeout_remaining_time;

	bool m_timeout_blocked;	///< True if the thread is blocked with timeout

	unsigned long long m_time_created;	///< The time when the thread was created

	long long m_time_constraint;	///< Stores a time to check specification constraints
	unsigned long long m_user_time;		///< The time the thread was executed in user mode

	jmp_buf setjmp_buf;
private:

	void create_gui_packet();
};

UC_thread_class* uc_get_current_thread();
unsigned long uc_get_curr_clock_period();
unsigned long uc_get_base_clock_period();
#endif

//==========================================================================
//	uc_signals_control.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun ene 21 2008
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

#ifndef _UC_SIGNALS_CONTROL_H
#define _UC_SIGNALS_CONTROL_H

#include "rtos/api/posix/uc_signal.h"
#include "rtos/api/posix/uc_sleep_channel.h"

class UC_process_class;
class UC_thread_class;

class UC_signals_data{
public:
	uc_sigset_t mask;
	uc_sigset_t pend_signals;
	uc_sigset_t wait_signals;
	uc_siginfo_t signals_info[NSIG];
	vector<uc_siginfo_t> rt_signals_info[SIGRTMAX-SIGRTMIN+1];
	int is_waiting_signal;
	UC_signals_data();
};

class UC_signals_control{
public:
	UC_signals_control();
	UC_signals_control(UC_process_class *parent);
	UC_signals_control operator =(UC_signals_control signals);

	int signal_thread(uc_pthread_t thread_id, const uc_siginfo_t *info);
	int signal_thread(uc_pthread_t thread_id,const struct uc_sigevent *info);
	int signal_process(const uc_siginfo_t *info);
	int signal_process(const struct uc_sigevent *info);

	int thread_sigmask(UC_thread_class *thread,	int how, const uc_sigset_t *set, uc_sigset_t *old_set);
	bool sigpending(UC_thread_class *thread, uc_sigset_t *set);
	int set_action(int signal, const struct uc_sigaction *action, struct uc_sigaction *old_action);

	int signal_wait(UC_thread_class *thread, const uc_sigset_t *set, uc_siginfo_t *info);
	int signal_wait(const uc_sigset_t *set, uc_siginfo_t *info);

//  Returns the number of the signal that slept the thread.
//  If the timeout was consumed returns -1
	int signal_timedwait(UC_thread_class *thread, const uc_sigset_t *set, uc_siginfo_t *info, const struct timespec *time);

	int execute_signals(UC_thread_class *thread);
	void set_parent_process(UC_process_class *parent);

	struct uc_sigaction *signals_action[_NSIG];
	int sigaction_loaded[_NSIG];
	bool sigstop();

	int in_signal;
	void copy(UC_signals_control parent, UC_process_class *curr_pr_table);

private:
	UC_signals_data signals;

	int verify_signal_thread(UC_thread_class *thread_element, int signal);

	UC_process_class *pr_table;
	uc_sigset_t sigset_flag;
	uc_sigset_t sigset_mask;

// 	UC_sleep_channel block_element;
};

#endif

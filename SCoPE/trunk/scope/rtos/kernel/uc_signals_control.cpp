//==========================================================================
//	uc_signals_control.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar ene 22 2008
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
#include "rtos/kernel/uc_signals_control.h"
#include "rtos/kernel/uc_process_class.h"
#include "rtos/kernel/uc_os_defines.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/api/posix/uc_time.h"
#include "rtos/api/posix/uc_unistd.h"

extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;

UC_signals_data::UC_signals_data(){
	for (int i = 0; i < _NSIG; i++) {
		mask.signal[i] = false;
		pend_signals.signal[i] = false;
		wait_signals.signal[i] = false;
	}

	is_waiting_signal = 0;
}

UC_signals_control::UC_signals_control(){
	struct uc_sigaction action;
	action.sa_handler = SIG_DFL;
	action.sa_flags = SA_NOCLDSTOP;

	for (int i = 0; i < _NSIG; i++) {
		signals_action[i] = new struct uc_sigaction;
		*signals_action[i] = action;
		sigaction_loaded[i] = 0;
	}
}

UC_signals_control::UC_signals_control(UC_process_class *parent) {
	struct uc_sigaction action;
	action.sa_handler = SIG_DFL;
	action.sa_flags = SA_NOCLDSTOP;

	for (int i = 0; i < _NSIG; i++) {
		signals_action[i] = new struct uc_sigaction;
		*signals_action[i] = action;
		sigaction_loaded[i] = 0;
	}
	pr_table = parent;
}

UC_signals_control UC_signals_control::operator =(UC_signals_control signals) {
	for (int i = 0; i < _NSIG; i++) {
		signals_action[i] = signals.signals_action[i];
		sigaction_loaded[i] = signals.sigaction_loaded[i];
	}
	sigset_flag = signals.sigset_flag;
	sigset_mask = signals.sigset_mask;
	return *this;
}

//  Returns the number of the signal that slept the thread
int UC_signals_control::signal_wait(UC_thread_class *thread, const uc_sigset_t *set, uc_siginfo_t *info) {
	return signal_timedwait(thread, set, info, NULL);
}

int UC_signals_control::signal_wait(const uc_sigset_t *set, uc_siginfo_t *info) {
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	return signal_timedwait(thread, set, info, NULL);
}

void UC_signals_control::set_parent_process(UC_process_class *parent) {
	pr_table = parent;
}

int UC_signals_control::signal_thread(uc_pthread_t thread_id, const uc_siginfo_t *info){
	uc_siginfo_t *info_signal;
	int signal;

	vector<UC_thread_class *>::iterator it_thread;
	vector<UC_task_class *>::iterator it_task;
	for (it_task = pr_table->m_task_list.begin(); it_task != pr_table->m_task_list.end(); it_task++) {
		for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
			if ((*it_thread)->m_tid == thread_id) {
				signal = info->si_signo;
				if (signal >= _NSIG || signal < 0) {
					return EINVAL;
				}
				if (info == NULL) {
					return EINVAL;
				}
				if (signal == 0) {
					return 0;
				}

				(*it_thread)->signals.pend_signals.signal[signal] = true;
				if (signal < SIGRTMIN) {
					(*it_thread)->signals.signals_info[signal] = *info;
				}
				else {
					info_signal = new uc_siginfo_t;
					*info_signal = *info;
					(*it_thread)->signals.rt_signals_info[signal-SIGRTMIN].push_back(*info_signal);
				}
				return verify_signal_thread((*it_thread), signal);
			}
		}
	}

	return ESRCH;
}

int UC_signals_control::verify_signal_thread(UC_thread_class *thread_element, int signal){
	if (signal == 0) {
		return 0;
	}

	if (signal == SIGSTOP || signal == SIGTSTP || signal == SIGTTIN || signal == SIGTTOU) {
		thread_element->signals.pend_signals.signal[SIGCONT] = 0;
		signals.pend_signals.signal[SIGCONT] = 0;
	}

	if (signal == SIGCONT) {
		thread_element->signals.pend_signals.signal[SIGSTOP] = 0;
		thread_element->signals.pend_signals.signal[SIGTSTP] = 0;
		thread_element->signals.pend_signals.signal[SIGTTIN] = 0;
		thread_element->signals.pend_signals.signal[SIGTTOU] = 0;
		signals.pend_signals.signal[SIGSTOP] = 0;
		signals.pend_signals.signal[SIGTSTP] = 0;
		signals.pend_signals.signal[SIGTTIN] = 0;
		signals.pend_signals.signal[SIGTTOU] = 0;
	}

	if (thread_element->signals.wait_signals.signal[signal]) {
		thread_element->unblock();
		return 0;
	}

	if (!thread_element->signals.mask.signal[signal]) {
		 if (thread_element->m_priority < INTERRUPT_PRIORITY_THRESHOLD) {
 		 	if (signal == SIGSTOP || signal == SIGTSTP || signal == SIGTTIN || signal == SIGTTOU) {
				thread_element->signals.pend_signals.signal[SIGCONT] = 0;
				signals.pend_signals.signal[SIGCONT] = 0;
			}
			else if (signal == SIGCONT) {
				thread_element->signals.pend_signals.signal[SIGSTOP] = 0;
				thread_element->signals.pend_signals.signal[SIGTSTP] = 0;
				thread_element->signals.pend_signals.signal[SIGTTIN] = 0;
				thread_element->signals.pend_signals.signal[SIGTTOU] = 0;
				signals.pend_signals.signal[SIGSTOP] = 0;
				signals.pend_signals.signal[SIGTSTP] = 0;
				signals.pend_signals.signal[SIGTTIN] = 0;
				signals.pend_signals.signal[SIGTTOU] = 0;
			}
			thread_element->m_signal_received = 1;

			thread_element->unblock();
		}
	}
	return 0;
}

int UC_signals_control::signal_thread(uc_pthread_t thread_id, const struct uc_sigevent *info) {
	if (info == NULL) {
		return EINVAL;
	}

	if (info->sigev_notify == SIGEV_NONE) {
		return 0;
	}

	if (info->sigev_notify == SIGEV_SIGNAL) {
		uc_siginfo_t siginfo;
		siginfo.si_signo = info->sigev_signo;
		siginfo.si_code = info->si_code;
		siginfo.timer = info->timer;
		siginfo.si_pid = qt_parent_rtos->POSIX->uc_getpid();
		siginfo.si_value = info->sigev_value;
		signal_thread(thread_id, &siginfo);
		return 0;
	}

	uc_pthread_t pthread_id;
	struct uc_sigevent info2 = *info;
	if (info->sigev_notify == SIGEV_THREAD) {
		qt_parent_rtos->POSIX->uc_pthread_create(&pthread_id, info->sigev_notify_attributes, (void *(*)(void *))info->sigev_notify_function, (void *)(info2.sigev_value.sival_ptr),"signal_thread");
		return 0;
	}
	return EINVAL;
}

int UC_signals_control::signal_process(const uc_siginfo_t *info){
	int flag,found = 0;
	uc_siginfo_t *info_signal;
	int signal = info->si_signo;
	if (signal >= _NSIG || signal < 0) {
		return EINVAL;
	}
	if (info == NULL) {
		return EINVAL;
	}
	if (signal == 0) {
		return 0;
	}

	if (signal < SIGRTMIN) {
		signals.signals_info[signal] = *info;
	}
	else {
		info_signal = new uc_siginfo_t;
		*info_signal = *info;
		signals.rt_signals_info[signal-SIGRTMIN].push_back(*info_signal);
	}
	signals.pend_signals.signal[signal] = true;

	if (signal == SIGSTOP || signal == SIGTSTP || signal == SIGTTIN || signal == SIGTTOU) {
		signals.pend_signals.signal[SIGCONT] = 0;
	}
	if (signal == SIGCONT) {
		signals.pend_signals.signal[SIGSTOP] = 0;
		signals.pend_signals.signal[SIGTSTP] = 0;
		signals.pend_signals.signal[SIGTTIN] = 0;
		signals.pend_signals.signal[SIGTTOU] = 0;
	}

	// Awake threads waiting this signal in sigwait, pause, ...
	vector<UC_thread_class *>::iterator it_thread;
	vector<UC_task_class *>::iterator it_task;
	for (it_task = pr_table->m_task_list.begin(); it_task != pr_table->m_task_list.end(); it_task++) {
		for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
// 			if (signal == SIGCONT && !(*it_thread)->signals.mask.signal[SIGCONT]) {
// 				if ((*it_thread)->m_blocking_channel == &block_element){
// 					(*it_thread)->signals.pend_signals.signal[SIGSTOP] = 0;
// 					(*it_thread)->signals.pend_signals.signal[SIGTSTP] = 0;
// 					(*it_thread)->signals.pend_signals.signal[SIGTTIN] = 0;
// 					(*it_thread)->signals.pend_signals.signal[SIGTTOU] = 0;

// 					(*it_thread)->unblock();
// 					return 0;
// 				}
// 			}
			if ((*it_thread)->signals.wait_signals.signal[info->si_signo]) {
				found = 1;
				if (info->si_signo == SIGCONT) {
					(*it_thread)->signals.pend_signals.signal[SIGSTOP] = 0;
					(*it_thread)->signals.pend_signals.signal[SIGTSTP] = 0;
					(*it_thread)->signals.pend_signals.signal[SIGTTIN] = 0;
					(*it_thread)->signals.pend_signals.signal[SIGTTOU] = 0;
				}

				(*it_thread)->unblock();
			}
		}
	}

	if (found) {
		return 0;
	}

	// If there is no threads waiting for the signal, it awakes all threads (not masked)
	for (it_task = pr_table->m_task_list.begin(); it_task != pr_table->m_task_list.end(); it_task++) {
		for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
			if (!(*it_thread)->signals.mask.signal[info->si_signo] && (*it_thread)->m_state != ZOMBIE) {
				if ((*it_thread)->m_priority < INTERRUPT_PRIORITY_THRESHOLD) {
					(*it_thread)->m_signal_received = 1;
					if (info->si_signo == SIGCONT) {
						(*it_thread)->signals.pend_signals.signal[SIGSTOP] = 0;
						(*it_thread)->signals.pend_signals.signal[SIGTSTP] = 0;
						(*it_thread)->signals.pend_signals.signal[SIGTTIN] = 0;
						(*it_thread)->signals.pend_signals.signal[SIGTTOU] = 0;
					}
					(*it_thread)->unblock();
				}
			}
		}
	}
	return 0;
}

int UC_signals_control::signal_process(const struct uc_sigevent *info){
	if (info == NULL) {
		return EINVAL;
	}
	if (info->sigev_notify == SIGEV_NONE) {
		return 0;
	}

	if (info->sigev_notify == SIGEV_SIGNAL) {
		uc_siginfo_t siginfo;
		siginfo.si_signo = info->sigev_signo;
		siginfo.si_code = info->si_code;
		siginfo.timer = info->timer;
		siginfo.si_pid = qt_parent_rtos->POSIX->uc_getpid();
		siginfo.si_value = info->sigev_value;
		signal_process(&siginfo);
		return 0;
	}

	uc_pthread_t pthread_id;
	struct uc_sigevent info2 = *info;
	if (info->sigev_notify == SIGEV_THREAD) {
		qt_parent_rtos->POSIX->uc_pthread_create(&pthread_id, info->sigev_notify_attributes, (void *(*)(void *))info->sigev_notify_function, (void *)(info2.sigev_value.sival_ptr), "signal_process");
		return 0;
	}
	return EINVAL;
}

int UC_signals_control::thread_sigmask(UC_thread_class *thread,	int how, const uc_sigset_t *in_set, uc_sigset_t *old_set){
	if (old_set != NULL) {
		*old_set = thread->signals.mask;
	}
	if (in_set == NULL) {
		return 0;
	}
	uc_sigset_t *set = new uc_sigset_t;
	*set = *in_set;
	set->signal[SIGKILL] = 0;
	set->signal[SIGSTOP] = 0;
	switch (how) {
		case SIG_SIGSETFLAG:
			for (int i = 0; i < _NSIG; i++) {
				if ((!set->signal[i]) && thread->signals.mask.signal[i]) {
					sigset_mask.signal[i] = 1;
					break;
				}
			}
			break;
		case SIG_BLOCK:
			for (int i = 0; i < _NSIG; i++) {
				thread->signals.mask.signal[i] |= set->signal[i];
			}
			break;
		case SIG_SETMASK:
			for (int i = 0; i < _NSIG; i++) {
				if (thread->signals.mask.signal[i] && !set->signal[i]) {
					thread->signals.mask.signal[i] = 0;
					verify_signal_thread(thread, i);
				}
				else {
					thread->signals.mask.signal[i] = set->signal[i];
				}
			}
			break;
		case SIG_UNBLOCK:
			for (int i = 0; i < _NSIG; i++) {
				if (thread->signals.mask.signal[i] && set->signal[i]) {
					thread->signals.mask.signal[i] = 0;
					verify_signal_thread(thread, i);
				}
				else {
					thread->signals.mask.signal[i] &= (!set->signal[i]);
				}
			}
			break;
		default:
			return EINVAL;
	}
	return 0;
}

bool UC_signals_control::sigpending(UC_thread_class *thread, uc_sigset_t *set){
	bool ret = false;
	uc_sigset_t sset;
	if (thread != NULL) {
		for (int i = 0; i < _NSIG; i++) {
			sset.signal[i] = signals.pend_signals.signal[i] || thread->signals.pend_signals.signal[i];
			if(sset.signal[i]) {
				ret = true;
			}
		}
	}
	else {
		for(int i = 0; i < _NSIG; i++) {
			sset.signal[i] = signals.pend_signals.signal[i];
			if (sset.signal[i]) {
				ret = true;
			}
	   }
	}
	if (set != NULL) {
		*set = sset;
	}
	return ret;
}

bool UC_signals_control::sigstop(){
	bool ret = true;

	if (signals.pend_signals.signal[SIGSTOP]) {
		return true;
	}
	vector<UC_thread_class *>::iterator it_thread;
	vector<UC_task_class *>::iterator it_task;
	for (it_task = pr_table->m_task_list.begin(); it_task != pr_table->m_task_list.end(); it_task++) {
		for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
			ret &= (*it_thread)->signals.pend_signals.signal[SIGSTOP];
		}
	}
	return ret;
}

int UC_signals_control::set_action(int signal, const struct uc_sigaction *action, struct uc_sigaction *old_action){
	if (signal >= _NSIG || signal < 0) {
		return EINVAL;
	}

	if (action == NULL) {
		return EINVAL;
	}
	if (old_action != NULL) {
		*old_action = *signals_action[signal];
	}

	if (action != NULL) {
		if (signal == SIGKILL || signal == SIGSTOP) {
			return EINVAL;
		}
		*signals_action[signal] = *action;
		sigaction_loaded[signal] = 1;
	}
	sigset_flag.signal[signal] = 0;
	return 0;
}

int UC_signals_control::execute_signals(UC_thread_class *thread){
	int signal,found=0,ret=-1,flag,is_process;
	UC_process_states state;
	UC_thread_class *thread_element;
	UC_process_class *process_element;
	uc_sigset_t prev_flags;

	if (thread->m_waiting_processor_in_signal) {
		return -1;
	}

	if (thread->m_in_signal >= MAX_CHAINED_SIGNALS) {
		return -1;
	}
	if (thread->m_in_signal == MAX_CHAINED_SIGNALS-1) {
		printf("WARNING: Maximum number of chained signals reached\n");
	}

	thread->m_signal_received = 0;

	while (true) {
		if (thread->signals.pend_signals.signal[SIGKILL]) {
// 			thread->m_state = SUPER_USER;
			thread->m_parent_task->m_parent_process->m_error_code = 1;
			qt_parent_rtos->POSIX->uc_pthread_exit((void*)SIGKILL);
		}
		if(signals.pend_signals.signal[SIGKILL]){
// 			thread->m_state = SUPER_USER;
			thread->m_parent_task->m_parent_process->m_error_code = 1;
			qt_parent_rtos->POSIX->uc_exit(SIGKILL);
		}

		if (thread->signals.pend_signals.signal[SIGSTOP]) {
			ret = 0;
// 			state = thread->m_state;
// 			thread->m_state = SUPER_USER;
			if (!(signals_action[SIGSTOP]->sa_flags & SA_NOCLDSTOP)) {
				uc_siginfo_t siginfo;
				siginfo.si_signo = SIGCHLD;
				siginfo.si_pid = thread->m_parent_task->m_parent_process->m_pid;
				signal_thread(thread->m_parent_task->m_parent_process->m_ppid, &siginfo);
			}
			while (true) {
				thread->signals.pend_signals.signal[SIGSTOP] = 1;

				vector<UC_process_class *>::iterator it_process;
				for (it_process = qt_parent_rtos->m_process_list.begin(); it_process != qt_parent_rtos->m_process_list.end(); it_process++) {
					if ((*it_process)->m_pid == pr_table->m_ppid) {
						vector<UC_thread_class *>::iterator it_thread;
						vector<UC_task_class *>::iterator it_task;
						for (it_task = (*it_process)->m_task_list.begin(); it_task != (*it_process)->m_task_list.end(); it_task++) {
							for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
								if ((*it_thread)->m_waiting_pid == pr_table->m_pid) {
									(*it_thread)->unblock();
								}
							}
						}
						break;
					}
				}

// 				uc_ch_exec_mod_SU_BQ(NULL,&thread->auto_block);
				thread->block();

				if (!thread->signals.pend_signals.signal[SIGSTOP]) {
					break;
				}
			}
// 			thread->m_state = state;
			continue;
		}

		if (signals.pend_signals.signal[SIGSTOP]) {
			ret = 0;
// 			state = thread->m_state;
// 			thread->state = SUPER_USER;
			if (!(signals_action[SIGSTOP]->sa_flags & SA_NOCLDSTOP)) {
				uc_siginfo_t siginfo;
				siginfo.si_signo = SIGCHLD;
				siginfo.si_pid = thread->m_parent_task->m_parent_process->m_pid;
				signal_thread(thread->m_parent_task->m_parent_process->m_ppid, &siginfo);
			}
			while (true) {
				thread->signals.pend_signals.signal[SIGSTOP] = 1;
				thread->block();

				if (!thread->signals.pend_signals.signal[SIGSTOP]) {
					break;
				}
			}
// 			thread->m_state = state;
			continue;
		}

		found = 0;
		for (signal = 0; signal < _NSIG; signal++) {
			if (thread->signals.mask.signal[signal]) {
				continue;
			}
			if (thread->signals.pend_signals.signal[signal]) {
				is_process = 0;
				thread->signals.pend_signals.signal[signal] = false;
				found = 1;
				ret = 0;
				if (signal == SIGSTOP || signal == SIGTSTP || signal == SIGTTIN || signal == SIGTTOU) {
					thread->signals.pend_signals.signal[signal] = 0;
					thread->signals.pend_signals.signal[SIGCONT] = 0;
				}
				if (signal == SIGCONT) {
					thread->signals.pend_signals.signal[signal] = 0;
					thread->signals.pend_signals.signal[SIGSTOP] = 0;
					thread->signals.pend_signals.signal[SIGTSTP] = 0;
					thread->signals.pend_signals.signal[SIGTTIN] = 0;
					thread->signals.pend_signals.signal[SIGTTOU] = 0;
				}
				break;
			}
			else if (signals.pend_signals.signal[signal]) {
				is_process = 1;
				signals.pend_signals.signal[signal] = false;
				if (signal == SIGSTOP || signal == SIGTSTP || signal == SIGTTIN || signal == SIGTTOU) {
					thread->signals.pend_signals.signal[signal] = 0;
					thread->signals.pend_signals.signal[SIGCONT] = 0;
				}
				if (signal == SIGCONT) {
					thread->signals.pend_signals.signal[signal] = 0;
					thread->signals.pend_signals.signal[SIGSTOP] = 0;
					thread->signals.pend_signals.signal[SIGTSTP] = 0;
					thread->signals.pend_signals.signal[SIGTTIN] = 0;
					thread->signals.pend_signals.signal[SIGTTOU] = 0;
				}
				found = 2;
				ret = 0;
				break;
			}
		}

		if (!found) {
			break;
		}

		thread->m_in_signal++;
		in_signal++;
		thread->m_waiting_processor_in_signal = 1;

		uc_siginfo_t info;
		if (signal < SIGRTMIN) {
			if (found == 1) {
				info = thread->signals.signals_info[signal];
			}
			else {
				info = signals.signals_info[signal];
			}
		}
		else {
			if (found == 1) {
				info = (thread->signals.rt_signals_info[signal-SIGRTMIN])[0];
				thread->signals.rt_signals_info[signal-SIGRTMIN].erase(thread->signals.rt_signals_info[signal-SIGRTMIN].begin());
			}
			else {
				info = (signals.rt_signals_info[signal-SIGRTMIN])[0];
				signals.rt_signals_info[signal-SIGRTMIN].erase(signals.rt_signals_info[signal-SIGRTMIN].begin());
			}
		}

		prev_flags = thread->signals.mask;
		if (sigaction_loaded[signal]) {
			thread->signals.mask = signals_action[signal]->sa_mask;
			if (signals_action[signal]->sa_flags & SA_NODEFER) {
				thread->signals.mask.signal[signal] = 0;
			}
			else {
				thread->signals.mask.signal[signal] = 1;
			}
		}
		if (signals_action[signal]->sa_flags & SA_SIGINFO) {
	    	if (found == 2) {
				if (signal < SIGRTMIN) {
					if (thread->m_state == SUPER_USER) {
						thread->set_next_state(SUPER_USER, WAITING);
						thread->update_state();
						thread->set_next_state(WAITING, USER);
						thread->update_state();

						thread->m_waiting_processor_in_signal = 0;
						if (info.si_code == SI_TIMER && info.timer != NULL) {
							info.timer->signal_overrun = info.timer->signal_counter-1;
							info.timer->signal_counter=0;
						}
						signals_action[signal]->sa_sigaction(signal, &signals.signals_info[signal], NULL);

						thread->set_next_state(USER, SUPER_USER);
						thread->update_state();
					}
					else {
						thread->m_waiting_processor_in_signal = 0;
						if (info.si_code == SI_TIMER && info.timer != NULL) {
							info.timer->signal_overrun = info.timer->signal_counter-1;
							info.timer->signal_counter=0;
						}
						signals_action[signal]->sa_sigaction(signal, &signals.signals_info[signal], NULL);
					}
				}
				else {
					if (signals.rt_signals_info[signal-SIGRTMIN].size() > 0) {
						signals.pend_signals.signal[signal] = true;
					}
					if (thread->m_state == SUPER_USER) {
						thread->set_next_state(SUPER_USER, WAITING);
						thread->update_state();
						thread->set_next_state(WAITING, USER);
						thread->update_state();

						thread->m_waiting_processor_in_signal = 0;
						if (info.si_code == SI_TIMER && info.timer != NULL) {
							info.timer->signal_overrun = info.timer->signal_counter - 1;
							info.timer->signal_counter = 0;
						}
						signals_action[signal]->sa_sigaction(signal, &info, NULL);

						thread->set_next_state(USER, SUPER_USER);
						thread->update_state();
					}
					else {
						thread->m_waiting_processor_in_signal = 0;
						if (info.si_code == SI_TIMER && info.timer != NULL) {
							info.timer->signal_overrun = info.timer->signal_counter - 1;
							info.timer->signal_counter = 0;
						}
						signals_action[signal]->sa_sigaction(signal, &info, NULL);
					}
				}
	    	}
	    	else {
				if (signal < SIGRTMIN) {
					if (thread->m_state == SUPER_USER) {
						thread->set_next_state(SUPER_USER, WAITING);
						thread->update_state();
						thread->set_next_state(WAITING, USER);
						thread->update_state();

						thread->m_waiting_processor_in_signal = 0;
						if (info.si_code == SI_TIMER && info.timer != NULL) {
							info.timer->signal_overrun = info.timer->signal_counter - 1;
							info.timer->signal_counter = 0;
						}
						signals_action[signal]->sa_sigaction(signal, &thread->signals.signals_info[signal], NULL);

						thread->set_next_state(USER, SUPER_USER);
						thread->update_state();
					}
					else {
						thread->m_waiting_processor_in_signal = 0;
						if (info.si_code == SI_TIMER && info.timer != NULL) {
							info.timer->signal_overrun = info.timer->signal_counter - 1;
							info.timer->signal_counter = 0;
						}
						signals_action[signal]->sa_sigaction(signal, &thread->signals.signals_info[signal], NULL);
					}
				}
				else {
					if (thread->signals.rt_signals_info[signal-SIGRTMIN].size() > 0) {
						thread->signals.pend_signals.signal[signal] = true;
					}
					if (thread->m_state == SUPER_USER) {
						thread->set_next_state(SUPER_USER, WAITING);
						thread->update_state();
						thread->set_next_state(WAITING, USER);
						thread->update_state();

						thread->m_waiting_processor_in_signal = 0;
						if (info.si_code == SI_TIMER && info.timer != NULL) {
							info.timer->signal_overrun = info.timer->signal_counter - 1;
							info.timer->signal_counter = 0;
						}
						signals_action[signal]->sa_sigaction(signal, &info, NULL);

						thread->set_next_state(USER, SUPER_USER);
						thread->update_state();
					}
					else {
						thread->m_waiting_processor_in_signal = 0;
						if (info.si_code == SI_TIMER && info.timer != NULL) {
							info.timer->signal_overrun = info.timer->signal_counter - 1;
							info.timer->signal_counter = 0;
						}
						signals_action[signal]->sa_sigaction(signal, &info, NULL);
					}
				}
			}
		}
		else if	(signals_action[signal]->sa_handler != NULL) {
			if (signals_action[signal]->sa_handler != SIG_DFL) {
				if (thread->m_state == SUPER_USER) {
					thread->set_next_state(SUPER_USER, WAITING);
					thread->update_state();
					thread->set_next_state(WAITING, USER);
					thread->update_state();

					thread->m_waiting_processor_in_signal = 0;
					if (info.si_code == SI_TIMER && info.timer != NULL) {
						info.timer->signal_overrun = info.timer->signal_counter - 1;
						info.timer->signal_counter = 0;
					}
					if (sigset_mask.signal[signal]) {
						thread->signals.mask.signal[signal] = 1;
					}
					signals_action[signal]->sa_handler(signal);
					if (sigset_mask.signal[signal]) {
						thread->signals.mask.signal[signal] = 0;
						sigset_mask.signal[signal] = 0;
					}

					thread->set_next_state(USER, SUPER_USER);
					thread->update_state();
				}
				else {
					thread->m_waiting_processor_in_signal = 0;
					if (info.si_code == SI_TIMER && info.timer != NULL) {
						info.timer->signal_overrun = info.timer->signal_counter - 1;
						info.timer->signal_counter = 0;
					}
					if (sigset_mask.signal[signal]) {
						thread->signals.mask.signal[signal] = 1;
					}
					signals_action[signal]->sa_handler(signal);
					if (sigset_mask.signal[signal]) {
						thread->signals.mask.signal[signal] = 0;
						sigset_mask.signal[signal] = 0;
					}
				}
			}
			else {
    			thread->signals.mask = prev_flags;
				thread->m_waiting_processor_in_signal = 0;
				if (signal == SIGSTOP || signal == SIGTSTP || signal == SIGTTIN || signal == SIGTTOU) {
					thread->signals.pend_signals.signal[signal] = true;
	//				state=thread->state;
	//				thread->state=P_blocked;
					while (true) {
						thread->block();
						if (!thread->signals.pend_signals.signal[SIGSTOP] && !thread->signals.pend_signals.signal[SIGTSTP] &&
							!thread->signals.pend_signals.signal[SIGTTIN] && !thread->signals.pend_signals.signal[SIGTTOU] &&
							!signals.pend_signals.signal[SIGSTOP] && !signals.pend_signals.signal[SIGTSTP] &&
							!signals.pend_signals.signal[SIGTTIN] && !signals.pend_signals.signal[SIGTTOU] )
						{
							break;
						}
					}
// 					thread->m_state = state;
				}
				else if (signal == SIGCONT) {
					if (info.si_code == SI_TIMER && info.timer != NULL) {
						info.timer->signal_overrun = info.timer->signal_counter - 1;
						info.timer->signal_counter = 0;
					}
					thread->m_signal_received = 0;
					return 1;
				}
				else if (signal < SIGSTKFLT && signal > SIGCONT) {
					signals.pend_signals.signal[signal] = true;
					thread->m_parent_task->m_parent_process->m_exit_signal = signal;
					if (info.si_code == SI_TIMER && info.timer != NULL) {
						info.timer->signal_overrun = info.timer->signal_counter - 1;
						info.timer->signal_counter = 0;
					}
					if (is_process) {
						qt_parent_rtos->POSIX->uc_exit(0);
					}
					else {
						qt_parent_rtos->POSIX->uc_pthread_exit(NULL);
					}
				}
			}
    	}
    	thread->signals.mask = prev_flags;
 		in_signal--;
		thread->m_in_signal--;
		thread->m_signal_executed = 1;
   }

    // If it is in a sigwait function the rest of signals has to be resended
    // It is possible that several signals was only accepted by this thread
    // Thus when the thread cleans the first one, the rest can remain blocked
    // However they should be delivered to other threads.
    // To avoid that, process signals are resended

	if (thread->signals.is_waiting_signal) {
	    for (signal = 0; signal < _NSIG; signal++) {
			if (signals.pend_signals.signal[signal]) {
				vector<UC_thread_class *>::iterator it_thread;
				vector<UC_task_class *>::iterator it_task;
				for (it_task = pr_table->m_task_list.begin(); it_task != pr_table->m_task_list.end(); it_task++) {
					for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
						if (!(*it_thread)->signals.mask.signal[signal]) {
							if ((*it_thread)->m_priority < INTERRUPT_PRIORITY_THRESHOLD) {
								(*it_thread)->m_in_signal = 1;
								(*it_thread)->unblock();
							}
						}
					}
				}
			}
		}
	}
	return ret;
}

int UC_signals_control::signal_timedwait(UC_thread_class *thread, const uc_sigset_t *set, uc_siginfo_t *info_sig, const struct timespec *time){
	int signal,flag;
	uc_siginfo_t *info;
	struct timespec abs_time,abs_time2;
	UC_thread_class *thread_element;
	if (info_sig != NULL) {
		info = info_sig;
	}
	else {
		info = new uc_siginfo_t;
	}
	thread->signals.wait_signals = *set;
	thread->m_is_waiting_signal = true;
	thread_element = thread;
	while (true) {
		for (int i = 0; i < _NSIG; i++) {
			if (set->signal[i] && thread->signals.pend_signals.signal[i]) {
				thread->signals.pend_signals.signal[i] = false;
				if (i < SIGRTMIN) {
					*info = thread_element->signals.signals_info[i];
				}
				else {
					*info = (thread_element->signals.rt_signals_info[i-SIGRTMIN])[0];
					thread_element->signals.rt_signals_info[i-SIGRTMIN].erase(thread_element->signals.rt_signals_info[i-SIGRTMIN].begin());
				}
				thread->m_is_waiting_signal = false;
				return i;
			}
			signal = i;
			if(set->signal[i] && signals.pend_signals.signal[i]){
 		      	if (signal == SIGSTOP || signal == SIGTSTP || signal == SIGTTIN || signal == SIGTTOU) {
					thread_element->signals.pend_signals.signal[SIGCONT] = 0;
				}
				if (signal == SIGCONT) {
					thread_element->signals.pend_signals.signal[SIGSTOP] = 0;
					thread_element->signals.pend_signals.signal[SIGTSTP] = 0;
					thread_element->signals.pend_signals.signal[SIGTTIN] = 0;
					thread_element->signals.pend_signals.signal[SIGTTOU] = 0;
				}
				signals.pend_signals.signal[i] = false;
				if (i < SIGRTMIN) {
					*info = signals.signals_info[i];
				}
				else {
					*info = (signals.rt_signals_info[signal-SIGRTMIN])[0];
					signals.rt_signals_info[signal-SIGRTMIN].erase(signals.rt_signals_info[signal-SIGRTMIN].begin());
				}
				thread->m_is_waiting_signal = false;
				return i;
			}
		}
		for (signal = 0; signal < _NSIG; signal++) {
		    if (signals.pend_signals.signal[signal]) {
				vector<UC_thread_class *>::iterator it_thread;
				vector<UC_task_class *>::iterator it_task;
				for (it_task = pr_table->m_task_list.begin(); it_task != pr_table->m_task_list.end(); it_task++) {
					for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
						if (!(*it_thread)->signals.mask.signal[signal]) {
							if ((*it_thread)->m_priority < INTERRUPT_PRIORITY_THRESHOLD) {
								(*it_thread)->unblock();
							}
						}
					}
				}
		    	}
		}

		if (time != NULL) {
/*			if(time->tv_sec == 0 && time->tv_nsec == 0){
				thread->m_is_waiting_signal = false;
				errno = EAGAIN;
				return -1;
			}*/
			qt_parent_rtos->m_tic_timer->clock_gettime(CLOCK_MONOTONIC, &abs_time);
			abs_time = *time + abs_time;
			
			if (thread->block(&abs_time)) {
				thread->m_is_waiting_signal = false;
				errno = EAGAIN;
				return -1;
			}
		}
		else {
			thread->block();
		}
	}

}

void UC_signals_control::copy(UC_signals_control parent, UC_process_class *curr_pr_table) {
	for (int i = 0; i < _NSIG; i++) {
		*signals_action[i] = *parent.signals_action[i];
		sigaction_loaded[i] = parent.sigaction_loaded[i];
	}
	signals = parent.signals;
	sigset_flag = parent.sigset_flag;
	sigset_mask = parent.sigset_mask;
	pr_table = curr_pr_table;
}

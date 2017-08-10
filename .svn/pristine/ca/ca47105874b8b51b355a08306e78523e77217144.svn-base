//==========================================================================
//	uc_signal.cpp
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

#include <errno.h>
#include "rtos/api/posix/uc_signal.h"
#include "rtos/api/posix/uc_posix.h"

extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;

void uc_signal_default(int signal){
	if (signal > 3) {
		return;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if (signal == 3) {
		thread->unblock();
		return;
	}

	if (signal < 29) {
		uc_pthread_cancel(thread->m_tid);
	}
}

void uc_signal_ignore(int signal){
}

void uc_signal_error(int signal){
}

int UC_posix_class::uc_sigemptyset(uc_sigset_t *set){
	if (set == NULL) {
		return -1;
	}

	for (int i = 0; i < _NSIG; i++) {
		set->signal[i] = false;
	}

	return 0;
}

int UC_posix_class::uc_sigfillset(uc_sigset_t *set){
	if (set == NULL) {
		return  -1;
	}

	for (int i = 0; i < _NSIG; i++) {
		set->signal[i] = true;
	}

	return 0;
}

int UC_posix_class::uc_sigaddset(uc_sigset_t *set, int i){
	if (set == NULL) {
		return -1;
	}

	if (i >= _NSIG) {
		return -1;
	}

	set->signal[i] = true;
	return 0;
}

int UC_posix_class::uc_sigdelset(uc_sigset_t *set, int i){
	if (set == NULL) {
		return -1;
	}

	if (i >= _NSIG || i < 0) {
		return -1;
	}

	set->signal[i] = false;
	return 0;
}

int UC_posix_class::uc_sigismember(const uc_sigset_t *set, int i){
	if (set == NULL) {
		return -1;
	}

	if (i >= _NSIG || i < 0) {
		return -1;
	}

	return set->signal[i];
}

int UC_posix_class::uc_kill(uc_pid_t pid, int signal){
	int ret=0,flag,exists=0,perm=0;
	UC_process_class *process;

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;

	if (signal >= _NSIG || signal < 0) {
		errno = EINVAL;
		return -1;
	}
	uc_siginfo_t *info = new uc_siginfo_t;
	info->si_signo = signal;
	info->si_pid = uc_getpid();
	info->si_code = SI_USER;

	if (pid == 0) {
		pid = -qt_parent_process->m_pgrp;
	}
	else if (pid == -1) {
		exists = 1;
		vector<UC_process_class *>::iterator it_process;
		for (it_process = qt_parent_rtos->m_process_list.begin(); it_process != qt_parent_rtos->m_process_list.end(); it_process++) {
			if (!uc_has_permission(qt_parent_process, *it_process)) {
				continue;
			}
			perm = 1;
			if (signal == 0) {
				continue;
			}
			ret = (*it_process)->signals_control.signal_process(info);
		}
	}
	else if (pid > 0) {
		process	= qt_parent_rtos->get_process(pid);
		if (process == NULL) {
			delete info;
			errno = ESRCH;
			return -1;
		}
		if (!uc_has_permission(qt_parent_process, process)) {
			delete info;
			errno = EPERM;
			return -1;
		}
		if (signal == 0) {
			delete info;
			return 0;
		}
		ret = process->signals_control.signal_process(info);
		exists = 1;
		perm = 1;
	}
	else {
		vector<UC_process_class *>::iterator it_process;
		for (it_process = qt_parent_rtos->m_process_list.begin(); it_process != qt_parent_rtos->m_process_list.end(); it_process++) {
			if ((*it_process)->m_pgrp == -pid) {
				exists = 1;
				if (!uc_has_permission(qt_parent_process, *it_process)) {
					continue;
				}
				perm = 1;
				if (signal == 0) {
					continue;
				}
				ret = (*it_process)->signals_control.signal_process(info);
			}
		}
	}

	delete info;
	if (!exists) {
		errno = ESRCH;
		return -1;
	}
	if (!perm) {
		errno = EPERM;
		return -1;
	}

	return ret;
}

int UC_posix_class::uc_killpg(uc_pid_t grp, int signal){
	if (grp > 1) {
		return uc_kill(-grp, signal);
	}

	if (grp == 0) {
		return uc_kill(-uc_getgid(), signal);
	}

	errno = ESRCH;
	return -1;
}

int UC_posix_class::uc_pthread_kill(uc_pthread_t tid, int signal){
	uc_siginfo_t *info = new uc_siginfo_t;
	info->si_pid = uc_getpid();
	info->si_signo = signal;
	info->si_code = SI_USER;
	if (signal >= _NSIG || signal < 0) {
		return  EINVAL;
	}
	int ret = qt_parent_process->signals_control.signal_thread(tid, info);
	delete info;

	return ret;
}

int UC_posix_class::uc_pthread_sigmask(int how, const uc_sigset_t *set, uc_sigset_t *old_set){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	return qt_parent_process->signals_control.thread_sigmask(thread, how, set, old_set);
}

int UC_posix_class::uc_sigprocmask(int how, const uc_sigset_t *set, uc_sigset_t *old_set){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	int ret = qt_parent_process->signals_control.thread_sigmask(thread, how, set, old_set);
	if (ret != 0) {
		errno = ret;
		return -1;
	}

	return 0;
}

int UC_posix_class::uc_raise(int signal){
	uc_siginfo_t *info = new uc_siginfo_t;
	info->si_pid = uc_getpid();
	info->si_signo = signal;
	info->si_code = SI_USER;
	if (signal >= _NSIG || signal < 0) {
		errno = EINVAL;
		return -1;
	}
	int ret = qt_parent_process->signals_control.signal_thread(uc_pthread_self(), info);
	delete info;
	return ret;
}

int UC_posix_class::uc_sigqueue(uc_pid_t pid, int signal, const union uc_sigval value){
	if (signal >= _NSIG || signal < 0) {
		errno = EINVAL;
		return -1;
	}

	UC_process_class *process = qt_parent_rtos->get_process(pid);

	if (process == NULL) {
		errno = ESRCH;
		return -1;
	}

	if (process->m_task_list.size() == 0) {
		errno = EPERM;
		return -1;
	}

	uc_siginfo_t *info = new uc_siginfo_t;
	info->si_signo = signal;
	info->si_value = value;
	info->si_code = SI_USER;
	info->si_pid = uc_getpid();
	int ret = process->signals_control.signal_process(info);
	delete info;
	return ret;
}

int UC_posix_class::uc_sigaction(int signal, const struct uc_sigaction *action, struct uc_sigaction *old_action){
	if (action != NULL) {
		struct uc_sigaction action2 = *action;
		action2.sa_mask.signal[SIGKILL] = 0;
		action2.sa_mask.signal[SIGSTOP] = 0;
		return qt_parent_process->signals_control.set_action(signal, &action2, old_action);
	}
	else {
		return qt_parent_process->signals_control.set_action(signal, NULL, old_action);
	}
}

int UC_posix_class::uc_sigpending(uc_sigset_t *set){
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	qt_parent_process->signals_control.sigpending(thread, set);
	return 0;
}

int UC_posix_class::uc_sigsuspend(const uc_sigset_t *set){
	uc_sigset_t *old_set = new uc_sigset_t;

	if (set == NULL) {
		return -1;
	}

	uc_pthread_testcancel();

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	qt_parent_process->signals_control.thread_sigmask(thread, SIG_SETMASK, set, old_set);

	while (!thread->m_signal_executed){
		thread->block();
	}

	uc_pthread_testcancel();
	qt_parent_process->signals_control.thread_sigmask(thread, SIG_SETMASK, old_set, NULL);
	delete old_set;
	errno = EINTR;
	return -1;
}

int UC_posix_class::uc_sigtimedwait(const uc_sigset_t *set, uc_siginfo_t *info,	const struct timespec *time){
	uc_pthread_testcancel();
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	int ret = qt_parent_process->signals_control.signal_timedwait(thread, set, info, time);
	uc_pthread_testcancel();
	return ret;
}

int UC_posix_class::uc_sigwait(const uc_sigset_t *set, int *signal){
	uc_siginfo_t *info = new uc_siginfo_t;
	uc_pthread_testcancel();
	int ret = uc_sigwaitinfo(set, info);
	uc_pthread_testcancel();
	if (signal != NULL) {
		*signal = ret;
	}

	delete info;
	if (ret >= 0) {
		return 0;
	}
	return -1;
}

int UC_posix_class::uc_sigwaitinfo(const uc_sigset_t *set, uc_siginfo_t *info){
	uc_pthread_testcancel();
	int ret = qt_parent_process->signals_control.signal_wait(set, info);
	uc_pthread_testcancel();
	return ret;
}

int UC_posix_class::uc_siginterrupt(int sig, int flag) {
	struct uc_sigaction act;
	uc_sigaction(sig, NULL, &act);
	if (flag) {
		act.sa_flags &= ~SA_RESTART;
	}
	else {
		act.sa_flags |= SA_RESTART;
	}
	return uc_sigaction(sig, &act, NULL);
}

uc_sighandler_t UC_posix_class::uc_signal(int sig, uc_sighandler_t handler)
{
    struct uc_sigaction new_action;
    struct uc_sigaction old_action;

    uc_sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    new_action.sa_handler = handler;

    int err = uc_sigaction(sig, &new_action, &old_action);

    if(err != 0) {
    	errno = err;
        return SIG_ERR;
    }
    else {
		return old_action.sa_handler;
	}
}

void (*UC_posix_class::uc_bsd_signal(int sig, void (*func)(int)))(int)
{
	struct uc_sigaction act, oact;
	act.sa_handler = func;
	act.sa_flags = SA_RESTART;
	uc_sigemptyset(&act.sa_mask);
	uc_sigaddset(&act.sa_mask, sig);
	if (uc_sigaction(sig, &act, &oact) == -1) {
		return NULL;
	}
	return(oact.sa_handler);
}

void (*UC_posix_class::uc_sigset(int sig, void (*handler)(int)))(int){
	uc_sigset_t set;
	if (handler == SIG_HOLD) {
		 uc_sighold(sig);
		 return SIG_HOLD;
	}
	uc_sigemptyset(&set);
	uc_sigaddset(&set,sig);
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	qt_parent_process->signals_control.thread_sigmask(thread, SIG_SIGSETFLAG, &set, NULL);
	return uc_signal(sig, handler);
}

int UC_posix_class::uc_sighold(int signal){
	uc_sigset_t set;
	if (uc_sigprocmask(SIG_SETMASK, NULL, &set) < 0) {
		return -1;
	}
	if (uc_sigaddset(&set, signal) < 0) {
		return -1;
	}
	return uc_sigprocmask(SIG_BLOCK, &set, NULL);
}

int UC_posix_class::uc_sigignore(int signal){
	struct uc_sigaction act;
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	int ret = uc_sigaction(signal, &act, NULL);
	if (ret != 0) {
		errno = ret;
		return -1;
	}
	return 0;
}

int UC_posix_class::uc_sigpause(int signal){
	uc_sigset_t set;

	uc_pthread_testcancel();
	if (signal < 0 || signal > _NSIG) {
		errno = EINVAL;
		return -1;
	}

	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	int ret = qt_parent_process->signals_control.thread_sigmask(thread, SIG_SETMASK, NULL, &set);

	if (ret < 0) {
		errno = EINVAL;
		return -1;
	}

	int val = set.signal[signal];
	set.signal[signal] = 0;
	qt_parent_process->signals_control.thread_sigmask(thread, SIG_SETMASK, &set, NULL);

	while (!thread->m_signal_executed) {
		thread->block();
	}

	set.signal[signal]=val;
	qt_parent_process->signals_control.thread_sigmask(thread, SIG_SETMASK, &set, NULL);

	errno = EINTR;
	return -1;
}

int UC_posix_class::uc_sigrelse(int signal){
	uc_sigset_t set;
	if (uc_sigemptyset(&set) < 0) {
		return -1;
	}
	if (uc_sigaddset(&set, signal) < 0) {
		return -1;
	}
	return uc_sigprocmask(SIG_UNBLOCK, &set, NULL);
}

int UC_posix_class::uc_has_permission(UC_process_class *current_process, UC_process_class *process) {
	if (current_process == NULL || process == NULL) {
		return 0;
	}
	if (current_process->m_uid  == 0)return 1;
	if (current_process->m_euid == 0)return 1;
	if (current_process->m_uid  == process->m_uid) return 1;
	if (current_process->m_uid  == process->m_euid)return 1;
	if (current_process->m_euid == process->m_uid) return 1;
	if (current_process->m_euid == process->m_euid)return 1;
	return 0;
}


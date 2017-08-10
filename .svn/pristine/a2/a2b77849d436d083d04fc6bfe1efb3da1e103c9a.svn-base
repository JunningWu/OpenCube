//==========================================================================
//	uc_signal.h
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

#ifndef _UC_SIGNAL_H
#define _UC_SIGNAL_H

#include <signal.h>
//#include "rtos/api/posix/sys_uc_types.h"
#include "sys_uc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef SIGEV_NONE
#undef SIGEV_SIGNAL
#undef SIGEV_THREAD

#define SIGEV_NONE 0
#define SIGEV_SIGNAL 1
#define SIGEV_THREAD 2

#undef NSIG
#undef _NSIG
#undef SIGRTMIN
#undef SIGRTMAX

#define NSIG		32
#define _NSIG		64
#define SIGRTMIN	NSIG
#define SIGRTMAX	(_NSIG-1)

typedef void (*uc_sighandler_t)(int);

typedef int uc_sig_atomic_t;

// It should be defined in "sys/types.h"
typedef uc_sighandler_t uc_sig_t;

union uc_sigval {
	int sival_int;
	void *sival_ptr;
};

typedef struct uc_pthread_attr uc_pthread_attr_t;

#undef sigev_notify_function
#undef sigev_notify_attributes
struct uc_sigevent{
	int	sigev_notify;
	int sigev_signo;
	int si_code;
	union uc_sigval sigev_value;
	void (*sigev_notify_function)(union uc_sigval);
	uc_pthread_attr_t *sigev_notify_attributes;
	struct uc_timer_data *timer;
};

typedef struct{
// 	bool signal[_NSIG];
	int signal[_NSIG];
} uc_sigset_t;

typedef struct uc_sigstack{
	void *ss_sp;
	int ss_onstack;
} uc_sigstack;

#undef si_pid
#undef si_uid
#undef si_addr
#undef si_status
#undef si_band
#undef si_value

typedef struct uc_siginfo {
	int si_signo;
	int si_errno;
	int si_code;
	uc_pid_t si_pid;		/* sender's pid */
	uc_uid_t si_uid;		/* sender's uid */
	void *si_addr;
	int si_status;
	long si_band;
	union uc_sigval si_value;
	struct uc_timer_data *timer;
} uc_siginfo_t;

#undef sa_handler
#undef sa_sigaction

struct uc_sigaction {
	uc_sighandler_t sa_handler;
	void (*sa_sigaction)(int, uc_siginfo_t*, void*);
	int sa_flags;
	uc_sigset_t sa_mask;
};

void uc_signal_default(int signal);
void uc_signal_ignore(int signal);
void uc_signal_error(int signal);

#undef SIG_DFL
#undef SIG_IGN
#undef SIG_ERR
#undef SIG_HOLD

// POSIX signals

#undef SIGCHLD
#undef SIGURG

#undef SIGCONT

#undef SIGABRT
#undef SIGBUS
#undef SIGFPE
#undef SIGILL
#undef SIGQUIT
#undef SIGSEGV
#undef SIGSYS
#undef SIGTRAP
#undef SIGXCPU
#undef SIGXFSZ

#undef SIGALRM
#undef SIGHUP
#undef SIGINT
#undef SIGKILL
#undef SIGPIPE
#undef SIGTERM
#undef SIGUSR1
#undef SIGUSR2
#undef SIGPOLL
#undef SIGPROF
#undef SIGVTALRM

#undef SIGSTOP
#undef SIGTSTP
#undef SIGTTIN
#undef SIGTTOU

// NON POSIX signals

#undef SIGIO
#undef SIGCLD
#undef SIGIOT
#undef SIGUNUSED

#undef SIGSTKFLT
#undef SIGWINCH
#undef SIGPWR


/* SA_FLAGS values: */

#undef SA_NOCLDSTOP
#undef SA_NOCLDWAIT
#undef SA_SIGINFO
#undef SA_RESTORER
#undef SA_ONSTACK
#undef SA_RESTART
#undef SA_INTERRUPT
#undef SA_NODEFER
#undef SA_RESETHAND

// ugh, historic Linux legacy, for gpm :-(
#undef SA_NOMASK
#undef SA_ONESHOT

// sigaltstack controls
#undef SS_ONSTACK
#undef SS_DISABLE

#undef SI_USER
#undef SI_QUEUE
#undef SI_TIMER
#undef SI_ASYNCIO
#undef SI_MESGQ


#undef MINSIGSTKSZ
#undef SIGSTKSZ

#undef SIG_BLOCK
#undef SIG_UNBLOCK
#undef SIG_SETMASK
#undef SIG_SIGSETFLAG

#define SIG_DFL (&uc_signal_default)	/* default signal handling */
#define SIG_IGN (&uc_signal_ignore)		/* ignore signal */
#define SIG_ERR (&uc_signal_error)		/* error return from signal */
#define SIG_HOLD ((uc_sighandler_t)-1 )	/* hold flag */

// POSIX signals

#define SIGCHLD		1
#define SIGURG		2

#define SIGCONT		3

#define SIGABRT		4
#define SIGBUS		5
#define SIGFPE		6
#define SIGILL		7
#define SIGQUIT		8
#define SIGSEGV		9
#define SIGSYS		10
#define SIGTRAP		11
#define SIGXCPU		12
#define SIGXFSZ		13

#define SIGALRM		14
#define SIGHUP		15
#define SIGINT		16
#define SIGKILL		17
#define SIGPIPE		18
#define SIGTERM		19
#define SIGUSR1		20
#define SIGUSR2		21
#define SIGPOLL		22
#define SIGPROF		23
#define SIGVTALRM	24

#define SIGSTOP		25
#define SIGTSTP		26
#define SIGTTIN		27
#define SIGTTOU		28

// NON POSIX signals

#define SIGIO		SIGPOLL
#define SIGCLD		SIGCHLD
#define SIGIOT		4 // SIGABORT ?
#define SIGUNUSED	10 // SYGSYS ?

#define SIGSTKFLT	29
#define SIGWINCH	30
#define SIGPWR		31


/* SA_FLAGS values: */
#define SA_NOCLDSTOP	0x00000001
#define SA_NOCLDWAIT	0x00000002 /* not supported yet */
#define SA_SIGINFO		0x00000004
#define SA_RESTORER		0x04000000
#define SA_ONSTACK		0x08000000
#define SA_RESTART		0x10000000
#define SA_INTERRUPT	0x20000000 /* dummy -- ignored */
#define SA_NODEFER		0x40000000
#define SA_RESETHAND	0x80000000

/* ugh, historic Linux legacy, for gpm :-( */
#define SA_NOMASK	SA_NODEFER
#define SA_ONESHOT	SA_RESETHAND

/* sigaltstack controls */
#define SS_ONSTACK	1
#define SS_DISABLE	2

#define SI_USER		1
#define SI_QUEUE	2
#define SI_TIMER	3
#define SI_ASYNCIO	4
#define SI_MESGQ	5


#define MINSIGSTKSZ	2048
#define SIGSTKSZ	8192

#define SIG_BLOCK	0		/* for blocking signals */
#define SIG_UNBLOCK	1		/* for unblocking signals */
#define SIG_SETMASK	2		/* for setting the signal mask */
#define SIG_SIGSETFLAG	3	/*  NOT POSIX */

int uc_kill(uc_pid_t, int);
int uc_killpg(uc_pid_t, int);
int uc_pthread_kill(uc_pthread_t, int);
int uc_pthread_sigmask(int, const uc_sigset_t *, uc_sigset_t *);
int uc_raise(int);
int uc_sigaction(int, const struct uc_sigaction *, struct uc_sigaction *);
int uc_sigaddset(uc_sigset_t *, int);
int uc_sigdelset(uc_sigset_t *, int);
int uc_sigemptyset(uc_sigset_t *);
int uc_sigfillset(uc_sigset_t *);

int uc_sigismember(const uc_sigset_t *, int);
uc_sighandler_t uc_signal(int sig, uc_sighandler_t handler);
int uc_sigpending(uc_sigset_t *);
int uc_sigprocmask(int, const uc_sigset_t *, uc_sigset_t *);
int uc_sigqueue(uc_pid_t, int, const union uc_sigval);
int uc_sigsuspend(const uc_sigset_t *);
int uc_sigtimedwait(const uc_sigset_t *, uc_siginfo_t *, const struct timespec *);
int uc_sigwait(const uc_sigset_t *, int *);
int uc_sigwaitinfo(const uc_sigset_t *, uc_siginfo_t *);

void (*uc_bsd_signal(int sig, void (*handler)(int)))(int);
void (*uc_sigset(int sig, void (*disp)(int)))(int);
int uc_sighold(int);
int uc_sigignore(int);
int uc_sigpause(int);
int uc_sigrelse(int);
int uc_siginterrupt(int, int);

/****************************************************************
//
//   XSI extensions not implemented
//

typedef struct sigaltstack {
  void *ss_sp;
  int ss_flags;
  size_t ss_size;
} stack_t;

struct sigstack{
  int ss_onstack;
  void *ss_sp;
}

int killpg(pid_t, int);
int sigaltstack(const stack_t *restrict, stack_t *restrict);
***************************************************************/

#ifdef EXAMPLE
#define signal(a,b) uc_signal(a,(sighandler_t)(void *)b)
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

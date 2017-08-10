//==========================================================================
//	uc_sched.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: vie ene 4 2008
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

#ifndef _UC_SCHED_H
#define _UC_SCHED_H

#include <sched.h>
// #include "uc_time.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Scheduling policies
 */

#define TIME_SLICE_TICS 5
#define TICK_TIME_INTERVAL 100

#undef SCHED_OTHER
#undef SCHED_FIFO
#undef SCHED_RR
//#undef SCHED_SPORADIC

#define UC_MAX_SCHED_POLICY		3
#define SCHED_OTHER		1
#define SCHED_FIFO		2
#define SCHED_RR		3
//#define SCHED_SPORADIC		4

struct uc_sched_param {
	int sched_priority;
	int sched_priority_old; // Actually only used by the DS2 uCOS version
	int sched_ss_low_priority;
	struct timespec sched_ss_period;
	struct timespec sched_ss_init_budget;
	int sched_ss_max_repl;
};

#define PTHREAD_MAX_PRIORITY 64
#define RTOS_PRIORITY 65
#define PTHREAD_MAX_PRIORITY_3 64
#define PTHREAD_MIN_PRIORITY_3 32
#define PTHREAD_MAX_PRIORITY_2 48
#define PTHREAD_MIN_PRIORITY_2 16
#define PTHREAD_MAX_PRIORITY_1 15
#define PTHREAD_MIN_PRIORITY_1 0

int uc_sched_setparam(int pid, const struct uc_sched_param *param);
int uc_sched_getparam(int pid, struct uc_sched_param *param);
int uc_sched_getscheduler(int pid);
int uc_sched_setscheduler(int pid, int policy, const struct uc_sched_param *param);
int uc_sched_yield();
int uc_sched_get_priority_max(int policy);
int uc_sched_get_priority_min(int policy);
int uc_sched_rr_get_interval(int pid, struct timespec *tp);
int uc_sched_setaffinity(int pid, unsigned int len, unsigned long *mask);
int uc_sched_getaffinity(int pid, unsigned int len, unsigned long *mask);

// NOT POSIX
struct uc_sched_param * uc_schedparam_init();

//  cloning flags:

#define CSIGNAL         0x000000ff      // signal mask to be sent at exit
#define CLONE_VM        0x00000100      // set if VM shared between processes
#define CLONE_FS        0x00000200      // set if fs info shared between processes
#define CLONE_FILES     0x00000400      // set if open files shared between processes
#define CLONE_SIGHAND   0x00000800      // set if signal handlers and blocked signals shared
#define CLONE_PID       0x00001000      // set if pid shared
#define CLONE_PTRACE    0x00002000      // set if we want to let tracing continue on the child too
#define CLONE_VFORK     0x00004000      // set if the parent wants the child to wake it up on mm_release
#define CLONE_PARENT    0x00008000      // set if we want to have the same parent as the cloner
#define CLONE_THREAD    0x00010000      // Same thread group?

#define CLONE_SIGNAL    (CLONE_SIGHAND | CLONE_THREAD)

#define cpu_set_t unsigned long

#undef CPU_ZERO
#undef CPU_SET
#define CPU_ZERO(cpu_set) *cpu_set=0;
#define CPU_SET(cpu, cpu_set) *cpu_set |= 1<<cpu;

/*********************************************************************************************/
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

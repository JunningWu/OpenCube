//==========================================================================
//	uc_os_defines.h
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

#ifndef _OS_PARAMETERS_H
#define _OS_PARAMETERS_H

//#define DIC_RTLINUX 1

// #undef errno
// #define errno qt_parent_rtos->m_os_errno

#undef _POSIX_THREAD_CPUTIME
#define _POSIX_THREAD_CPUTIME

// Clocks per memory access (Mean value)
#define UC_STANDARD_CPU_MEMORY_ACCESS_FACTOR 1

#define UC_PORT_TIME 370

#define KERNEL_PRIORITY 0
#define MAX_CHAINED_SIGNALS 10

#define __STDC_CONSTANT_MACROS
#define __STDC_FORMAT_MACROS
#define __STDC_LIMIT_MACROS

#define NGROUPS 1
// Processes with priority => INTERRUPT_PRIORITY_THRESHOLD cannot be awaked by signals
// Processes with priority < INTERRUPT_PRIORITY_THRESHOLD are awaked by signals
#define INTERRUPT_PRIORITY_THRESHOLD 10000

//#define TICK_TIMER_HZ 600
#define TICK_TIMER_HZ 100
/*
 * Scheduling quanta.
 *
 * NOTE! The unix "nice" value influences how long a process
 * gets. The nice value ranges from -20 to +19, where a -20
 * is a "high-priority" task, and a "+10" is a low-priority
 * task.
 *
 * We want the time-slice to be around 50ms or so, so this
 * calculation depends on the value of HZ.
 */

#undef NZERO
#define NZERO 0

#if TICK_TIMER_HZ < 200
#define TICK_SCALE(x)   ((x) >> 2)
#elif TICK_TIMER_HZ < 400
#define TICK_SCALE(x)   ((x) >> 1)
#elif TICK_TIMER_HZ < 800
#define TICK_SCALE(x)   (x)
#elif TICK_TIMER_HZ < 1600
#define TICK_SCALE(x)   ((x) << 1)
// #elif TICK_TIMER_HZ == 25000
// #define TICK_SCALE(x)   5
#else
#define TICK_SCALE(x)   ((x) << 2)
#endif


#define NICE_TO_TICKS(nice)     (TICK_SCALE(20-(nice))+1)

// IMPORTANT: PROC_CHANGE_PENALTY + CHANGE_MM_PENALTY +
//  + NICE_PENALTY * 41  + TIME_SLICE_PENALTY * TICK_SCALE(41)* 2 shuld be < 1000

#define TIME_SLICE_PENALTY  1  // weight of the remain time slice
#define PROC_CHANGE_PENALTY 1  // weight of the change of the CPU where task runs
#define CHANGE_MM_PENALTY   1  // weight of changing the memory map (process)
#define NICE_PENALTY        1  // weight of the nice value

#define SCHED_YIELD_PENALTY 0 // Value of the process tick counter after a yield() call
			      // It can be negative.

// #define CALL_ID_TYPES 10
// #define CALL_ID_FUNCS 10

/*******************************/
/*** CONFIGURATION: OS TIMES ***/
/*******************************/

#define CONTEXT_SWITCH_TIME			0000
#define THREAD_SWITCH_TIME			000
#define CONTEXT_SWITCH_INSTR		0
#define THREAD_SWITCH_INSTR			0
// #define ZOMBIE_SWITCH			0
#define TIC_TIMER					00000

#define SYSTEM_CALL				0
#define PTHREAD_CALL			0
#define MUTEX_CALL				0
#define COND_CALL				0
#define SEM_CALL				0
#define TIME_CALL				0
#define IO_CALL					0
#define CORBA_CALL				0

#define SLEEP_CALL				0
#define AWAKE_CALL				0
#define EXIT_CALL				0
#define WAIT_CALL				0
#define KILL_CALL				0
#define PARAM_CREATE			0
#define PROCESS_CREATE			0


#define THREAD_CREATE			0

//#define INIT_CALL_ID 0
#define LOCK_CALL_ID			0
#define TIMED_LOCK_CALL_ID		0
#define TRYLOCK_CALL_ID			0
#define UNLOCK_CALL_ID			0

#define COND_SIGNAL_CALL_ID		0
#define COND_WAIT_CALL_ID		0

#define POST_CALL_ID			0
#define SEM_WAIT_CALL_ID		0

#define CLOCK_CALL_ID			0

#define CREATE_IO_CALL			0
#define WRITE_IO_CALL			0
#define READ_IO_CALL			0

#define CORBA_WAIT				0
#define CORBA_NOTIFY			0



/*#define CONTEXT_SWITCH_TIME			10000
#define THREAD_SWITCH_TIME			1000
#define CONTEXT_SWITCH_INSTR		1350
#define THREAD_SWITCH_INSTR			135
// #define ZOMBIE_SWITCH			10
#define TIC_TIMER					100000
*/
/*** OLD ***/

// #define SYSTEM_CALL				10
// #define PTHREAD_CALL			10
// #define MUTEX_CALL				10
// #define COND_CALL				10
// #define SEM_CALL				10
// #define TIME_CALL				10
// #define IO_CALL					10
// #define CORBA_CALL				10
// 
// #define SLEEP_CALL				10
// #define AWAKE_CALL				10
// #define EXIT_CALL				10
// #define WAIT_CALL				10
// #define KILL_CALL				10
// #define PARAM_CREATE			10
// #define PROCESS_CREATE			10
// 
// 
// #define THREAD_CREATE			10
// 
// //#define INIT_CALL_ID 0
// #define LOCK_CALL_ID			10
// #define TIMED_LOCK_CALL_ID		10
// #define TRYLOCK_CALL_ID			10
// #define UNLOCK_CALL_ID			10
// 
// #define COND_SIGNAL_CALL_ID		10
// #define COND_WAIT_CALL_ID		10
// 
// #define POST_CALL_ID			10
// #define SEM_WAIT_CALL_ID		10
// 
// #define CLOCK_CALL_ID			10
// 
// #define CREATE_IO_CALL			10
// #define WRITE_IO_CALL			10
// #define READ_IO_CALL			10
// 
// #define CORBA_WAIT				10
// #define CORBA_NOTIFY			10


/*********************/
/*** HAL's defines ***/
/*********************/

#define MAX_HAL_ARGS_SIZE		1024
#define MAX_HAL_RETURN_SIZE		128


#define MAX_DEV 256

#define smp_wmb()

#endif


//==========================================================================
//	uc_time.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun ene 7 2008
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


#ifndef _UC_TIME_H
#define _UC_TIME_H
// #include <vector>
// using namespace std;
#include <time.h>
/*#include "rtos/api/posix/uc_signal.h"
#include "rtos/kernel/uc_os_defines.h"*/
#include "uc_signal.h"
#include "../../kernel/uc_os_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

// class UC_thread_class;
// class UC_process_class;

//A number used to convert the value returned by the clock ( ) function into seconds.
// #undef CLOCKS_PER_SEC
// #define CLOCKS_PER_SEC TICK_TIMER_HZ

// Flag indicating time is absolute. For functions taking timer objects,
// this refers to the clock associated with the timer.
#undef TIMER_ABSTIME
#define TIMER_ABSTIME  0x1000000

//The identifier of the system-wide realtime clock.
#undef CLOCK_REALTIME
#define CLOCK_REALTIME 0x1000001

// The identifier for the system-wide monotonic clock, which is defined as a
// clock whose value cannot be set via clock_settime( ) and which cannot
// have backward clock jumps. The maximum possible clock jump shall be
// implementation-defined.
#undef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0x1000008

//The identifier of the CPU-time clock associated with the thread making a clock( )
// or timer*( ) function call.
#undef CLOCK_THREAD_CPUTIME_ID
#define CLOCK_THREAD_CPUTIME_ID 0x1000002

//The identifier of the CPU-time clock associated with the process making a clock( )
// or timer*( ) function call.
#undef CLOCK_PROCESS_CPUTIME_ID
#define CLOCK_PROCESS_CPUTIME_ID 0x1000003

extern const struct timespec uc_infinite_time;
#define INF_TIME uc_infinite_time
extern const struct timespec uc_timespec_zero;
#define UC_TIMESPEC_ZERO uc_timespec_zero

clock_t uc_clock();
time_t uc_time(time_t *);
double uc_difftime(time_t time1, time_t time0);
int uc_nanosleep(const struct timespec *nseconds, struct timespec *rem);

//clock id of the CPU-time clock of the process
int uc_clock_getcpuclockid(int pid, clockid_t *clockid);

//clock resolution
int uc_clock_getres(clockid_t clock_id, struct timespec *rqtp);
//clock value
int uc_clock_gettime(clockid_t clock_id, struct timespec *rqtp);
// set clock value
int uc_clock_settime(clockid_t clock_id, const struct timespec *rqtp);

//clock nanosleep
int uc_clock_nanosleep(clockid_t clock_id, int flags,
					   const struct timespec *rqtp, struct timespec *rmtp);

int uc_timer_create(clockid_t clockid, struct uc_sigevent *s_ev, timer_t *timer);
int uc_timer_delete(timer_t timer);
int uc_timer_gettime(timer_t time, struct itimerspec * spec);
int uc_timer_getoverrun(timer_t timer);
int uc_timer_settime(timer_t timer, int flags, const struct itimerspec *i_spec,
					 struct itimerspec *o_spec);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

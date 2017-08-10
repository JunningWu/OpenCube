//==========================================================================
//	uc_time.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié ene 23 2008
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
#include "rtos/api/posix/uc_time.h"
#include "rtos/api/posix/uc_posix.h"
#include "rtos/kernel/uc_os_defines.h"

extern __thread UC_rtos_class *qt_parent_rtos;
extern long long uc_segment_time;

const struct timespec uc_infinite_time = {0x7fffffff,0x7fffffff};
const struct timespec uc_timespec_zero={0,0};

uc_time_variables_class uc_time_variables;

bool operator >(const struct timespec a, const struct timespec b){
	if (a.tv_sec > b.tv_sec) {
		return true;
	}
	if (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec) {
		return true;
	}
	return false;
}

bool operator <(const struct timespec a, const struct timespec b){
	if (a.tv_sec < b.tv_sec) {
		return true;
	}
	if (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec) {
		return true;
	}
	return false;
}

bool operator >=(const struct timespec a, const struct timespec b){
	if (a.tv_sec < b.tv_sec) {
		return false;
	}
	if (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec) {
		return false;
	}
	return true;
}

bool operator <=(const struct timespec a, const struct timespec b){
	if (a.tv_sec > b.tv_sec) {
		return false;
	}
	if (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec) {
		return false;
	}
	return true;
}

bool operator ==(const struct timespec a, const struct timespec b){
	if (a.tv_sec == b.tv_sec && a.tv_nsec == b.tv_nsec) {
		return true;
	}
	return false;
}

bool operator !=(const struct timespec a, const struct timespec b){
	if (a.tv_sec == b.tv_sec && a.tv_nsec == b.tv_nsec) {
		return false;
	}
	return true;
}

struct timespec operator -(const struct timespec a, const struct timespec b){
	struct timespec c;
	if (a < b) {
		cout << "timespec: negative time" << endl;
	}
	if (a.tv_nsec < b.tv_nsec) {
		c.tv_sec = a.tv_sec - b.tv_sec - 1;
		c.tv_nsec = (a.tv_nsec + (long)1e9) - b.tv_nsec;
	}
	else {
		c.tv_sec = a.tv_sec - b.tv_sec;
		c.tv_nsec = a.tv_nsec - b.tv_nsec;
	}
	return c;
}

struct timespec operator +(const struct timespec a, const struct timespec b){
	struct timespec c;
	c.tv_sec = a.tv_sec + b.tv_sec;
	c.tv_nsec = a.tv_nsec + b.tv_nsec;
	if( c.tv_nsec >= 1e9){
		c.tv_nsec -= (long)1e9;
		c.tv_sec ++;
	}
	return c;
}

double UC_posix_class::uc_difftime(time_t time1, time_t time0){
	return (time1 - time0);
}

clock_t UC_posix_class::uc_clock(){
	int time_,aux;
	struct timespec tt;

	qt_parent_rtos->m_tic_timer->clock_gettime(CLOCK_MONOTONIC,&tt);
	time_ = (int)((double)tt.tv_nsec*CLOCKS_PER_SEC/1e9);
	aux = uc_time_variables.since_epoch + tt.tv_sec;

	aux *= CLOCKS_PER_SEC;
	aux += time_;

	return (clock_t)aux;
}

time_t UC_posix_class::uc_time(time_t *t){
	time_t aux;
	struct timespec tt;

	qt_parent_rtos->m_tic_timer->clock_gettime(CLOCK_MONOTONIC,&tt);
	aux = uc_time_variables.since_epoch + tt.tv_sec;
	if (t != NULL) {
		*t=aux;
	}

	return aux;
}

int UC_posix_class::uc_nanosleep(const struct timespec *nseconds, struct timespec *rem){
/*
	if(thread->in_fork) return 0;
*/
	uc_pthread_testcancel();
	int ret = qt_parent_rtos->m_tic_timer->clock_nanosleep(CLOCK_REALTIME,0,nseconds,rem);

	if (ret != 0) {
		errno = ret;
		return -1;
	}
	return 0;
}

//clock id of the CPU-time clock of the process
int UC_posix_class::uc_clock_getcpuclockid(int pid, clockid_t *clockid){
	if (pid == 0) {
		pid = uc_getpid();
	}
	return qt_parent_rtos->m_tic_timer->get_clock_id(pid, clockid);
}

//clock resolution
int UC_posix_class::uc_clock_getres(clockid_t clock_id, struct timespec *rqtp){
	struct timespec st;
	if (clock_id != CLOCK_REALTIME) {
		if (qt_parent_rtos->m_tic_timer->get_clock(clock_id) == NULL) {
			errno = EINVAL;
			return -1;
		}
	}

	st.tv_sec = 0;
	st.tv_nsec = (int)1e9 / TICK_TIMER_HZ;
	if (rqtp != NULL) {
		*rqtp=st;
	}
	return 0;
}

//clock value
int UC_posix_class::uc_clock_gettime(clockid_t clock_id, struct timespec *rqtp){

// 	if(clock_id == CLOCK_REALTIME){
// 		long long time = sc_time_stamp().value()/1000;
// 		time+= uc_segment_time;
// 		rqtp->tv_nsec = time % 1000000000;
// 		rqtp->tv_sec = time / 1000000000;
// 		return 0;
// 	}

	int ret = qt_parent_rtos->m_tic_timer->clock_gettime(clock_id, rqtp);


	if (ret == -1) {
		errno = EINVAL;
	}
	return ret;
}

// set clock value
int UC_posix_class::uc_clock_settime(clockid_t clock_id, const struct timespec *rqtp){
/*
	if(thread->in_fork) return 0;
*/
	int ret = qt_parent_rtos->m_tic_timer->clock_settime(clock_id, rqtp);
	if (ret == -1) {
		errno = EINVAL;
	}
	return ret;
}

//clock nanosleep
int UC_posix_class::uc_clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *rqtp, struct timespec *rmtp){
/*
	if(thread->in_fork) return 0;
*/
	uc_pthread_testcancel();
	return qt_parent_rtos->m_tic_timer->clock_nanosleep(clock_id, flags,rqtp, rmtp);
}

int UC_posix_class::uc_timer_create(clockid_t clockid, struct uc_sigevent *s_ev, timer_t *timer){
/*
	if(thread->in_fork) return 0;
*/
	int ret = qt_parent_rtos->m_tic_timer->create_timer(clockid,s_ev,timer);
	if (ret == -1) {
		errno = EINVAL;
	}
	return ret;
}

int UC_posix_class::uc_timer_delete(timer_t timer){
	return qt_parent_rtos->m_tic_timer->delete_timer(timer);
}

int UC_posix_class::uc_timer_gettime(timer_t time, struct itimerspec * spec){
	int ret = qt_parent_rtos->m_tic_timer->get_time(time,spec);
	if (ret == -1) {
		errno = EINVAL;
	}
	return ret;
}

int UC_posix_class::uc_timer_getoverrun(timer_t timer){
	return qt_parent_rtos->m_tic_timer->get_overrun(timer);
}

int UC_posix_class::uc_timer_settime(timer_t timer, int flags, const struct itimerspec *i_spec, struct itimerspec *o_spec){
/*
	if(thread->in_fork) return 0;
*/
	int ret = qt_parent_rtos->m_tic_timer->set_time(timer,flags,i_spec,o_spec);
	if (ret == -1) {
		errno = EINVAL;
	}
	return ret;
}

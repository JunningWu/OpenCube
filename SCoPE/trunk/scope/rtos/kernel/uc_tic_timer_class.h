//==========================================================================
//	uc_tic_timer_class.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun dic 17 2007
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

#ifndef UC_TIC_TIMER_CLASS_H
#define UC_TIC_TIMER_CLASS_H

#include "systemc.h"
#include "rtos/api/posix/uc_signal.h"
#include "rtos/kernel/uc_cpu_class.h"

#include <vector>
using namespace std;
#include "rtos/api/posix/sys_uc_types.h"
#include "rtos/api/posix/uc_time.h"
#include "rtos/api/posix/uc_time_class.h"
#include "rtos/drivers/linux/uc_irqreturn.h"
// class UC_cpu_class;
class UC_thread_class;
class UC_process_class;

class UC_tic_timer_class : public sc_module {
public:
	UC_tic_timer_class(sc_module_name module_name, vector<UC_cpu_class *> *cpu_list);

    ~UC_tic_timer_class();

	static irqreturn_t tic_timer_interruption_handler(int irq, void *args, struct pt_regs *regs);

	int get_clock_pthread(uc_pthread_t pthread_id, clockid_t *clockid);



	int create_timer(clockid_t clockid, struct uc_sigevent *s_ev, timer_t *timer);
	int delete_timer(timer_t timer);
	int get_time(timer_t time, struct itimerspec * spec);
	int get_overrun(timer_t timer);
	int set_time(timer_t timer, int flags, const struct itimerspec *i_spec,
				 struct itimerspec *o_spec);

	int clock_nanosleep(clockid_t clock_id, int flags,
						const struct timespec *rqtp, struct timespec *rmtp);
// 	int put_timeout(const struct timespec *rqtp, struct timespec *rmtp,
// 					struct UC_Thread_U_Area *thread_element);
	int clock_gettime(clockid_t clock_id, struct timespec *rqtp);
	int clock_settime(clockid_t clock_id, const struct timespec *rqtp);

	void create_clock(UC_thread_class *thread);
	int destroy_clock(UC_thread_class *thread);
	void create_clock(UC_process_class *process);
	int destroy_clock(UC_process_class *process);

	int get_clock_id(pid_t pid, clockid_t *clockid);

// 	uc_timer_data* get_timer(timer_t timer);
	uc_clock_data* get_clock(clockid_t clock_id);

	void thread_destroyed(UC_thread_class *thread);
	void process_destroyed(UC_process_class *process);

	unsigned int get_MicroCOS_num_tics();
	void set_MicroCOS_num_tics(unsigned int tics);

	int timer_n;
	int clock_n;

	static struct timespec tics_2_timespec(const long long tics);
	static long long timespec_2_tics(const struct timespec tics);
	static long long timespec_2_tics_up(const struct timespec tics);

private:
	void tic_timer_clock();

	int add_clock_time(clockid_t clock_id,int num,int abs_value);

	void place_timer(uc_timer_data*, uc_clock_data*);
	void place_sleep(uc_nanosleep_data*, uc_clock_data*);
	int generate_signal(uc_timer_data *data);
	int generate_signal(uc_nanosleep_data *data);

	vector<UC_cpu_class *> *m_cpu_list; ///< The cpu's list to send the tic timer interruption

	vector<uc_clock_data *> m_clock_list; ///< List of clocks

	unsigned int m_num_tics; ///< Number of processed tics
	unsigned int MicroCOS_num_tics; ///< Number of processed tics. This number can be modified by the user

	vector<uc_timer_data*> timer_list;
// 	vector<uc_clock_data*> clock_list;
// 	sc_event evento;
	uc_clock_data system_clk;
	uc_clock_data monotonic_clk;

	struct timespec real_time_add_time;
};

#endif

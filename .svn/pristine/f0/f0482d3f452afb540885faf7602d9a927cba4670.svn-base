//==========================================================================
//	uc_tic_timer_class.cpp
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

#include <errno.h>

#include "rtos/kernel/uc_tic_timer_class.h"
#include "rtos/kernel/uc_os_defines.h"
#include "rtos/api/ucos/uc_microcos.h"

extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;

UC_tic_timer_class::UC_tic_timer_class(sc_module_name module_name, vector<UC_cpu_class *> *cpu_list) : sc_module(module_name)
{
	m_cpu_list = cpu_list;
	MicroCOS_num_tics = 0;
	SC_HAS_PROCESS(UC_tic_timer_class);
	SC_THREAD(tic_timer_clock);

	timer_n = 0;
	clock_n = 0;
	system_clk.clk_id = clock_n++;
	system_clk.counter = 0;
	monotonic_clk.clk_id = clock_n++;
	monotonic_clk.counter = 0;
	real_time_add_time.tv_sec = 0;
	real_time_add_time.tv_nsec = 0;
	m_num_tics = 0;
}


UC_tic_timer_class::~UC_tic_timer_class()
{
}

/**
 * \brief SC_THREAD that sends the tic timer interruption to a list of processors every 10 ms
 */
void UC_tic_timer_class::tic_timer_clock() {
	while (true) {
		wait(1000000/TICK_TIMER_HZ, SC_US);
		vector<UC_cpu_class *>::iterator it_cpu;
		for (it_cpu = m_cpu_list->begin(); it_cpu != m_cpu_list->end(); it_cpu++) {
			(*it_cpu)->send_irq(0);
		}
// 		(*m_cpu_list)[0]->send_irq(0);
	}
}

/**
 * \brief Handles the tic timer interruption
 *
 * This function has the structure of an interruption handler, but the parameters are ignored
 * \param irq The interruption number (0)
 * \param args The arguments for the interruption (NULL)
 * \param regs The processor's registers (NULL)
 * \return The interruption's return
 */
irqreturn_t UC_tic_timer_class::tic_timer_interruption_handler(int irq, void *args, struct pt_regs *regs) {
	if (qt_parent_rtos->get_current_cpu()->m_id != 0) {
		return 0;
	}

	UC_tic_timer_class *tic_timer = (UC_tic_timer_class *)args;

	tic_timer->m_num_tics++;
	tic_timer->MicroCOS_num_tics++;
	tic_timer->add_clock_time(CLOCK_REALTIME, 1, 0);
	tic_timer->add_clock_time(CLOCK_MONOTONIC, 1, 0);

	UC_cpu_class *cpu = qt_parent_rtos->get_current_cpu();
//	qt_parent_process->m_current_task is the handler itself. To reduce the RR_counter it is required to get the previous task
	UC_task_class *task = cpu->m_prev_task;

	if (task != cpu->m_idle_task && task != NULL && (task->m_state == WAITING || task->m_state == READY)) {
		UC_thread_class *thread = task->m_current_thread;

		if (thread->m_attribs.schedpolicy != SCHED_FIFO) {
			thread->m_RR_counter--;
		}

		if (task->m_policy != SCHED_FIFO) {
			task->m_RR_counter--;
		}

		thread->m_timeout_expired = 1;

		clockid_t clock_id;
		if (tic_timer->get_clock_id(qt_parent_process->m_pid, &clock_id) == 0) {
			tic_timer->add_clock_time(clock_id, 1, 0);
		}

		if (tic_timer->get_clock_pthread(thread->m_tid, &clock_id) == 0) {
			tic_timer->add_clock_time(clock_id, 1, 0);
		}
	}

	OSTimeTick();
}

/**
 * \brief Returns a clock from a given thread identifier
 *
 * \param pthread_id The thread identifier
 * \param clockid The pointer to return the thread's clock
 * \return Returns 0 if success or -1 if failure
 */
int UC_tic_timer_class::get_clock_pthread(uc_pthread_t pthread_id, clockid_t *clockid) {
	vector<uc_clock_data *>::iterator it_clock;
	for (it_clock = m_clock_list.begin(); it_clock != m_clock_list.end(); it_clock++) {
		if ((*it_clock)->thread != NULL) {
			if ((*it_clock)->thread->m_tid == pthread_id) {
				*clockid = (*it_clock)->clk_id;
				return 0;
			}
		}
	}

// 	errno = EINVAL;
	return -1;
}

int UC_tic_timer_class::create_timer(clockid_t clockid, struct uc_sigevent *s_ev, timer_t *timer) {
	uc_clock_data *new_clk;
	uc_timer_data *new_data;

	new_data = new uc_timer_data;
	new_data->clock_addr = get_clock(clockid);
	new_data->active = 0;
	new_data->process = qt_parent_process;
	new_data->timer_id = (timer_t)timer_n++;
	if (s_ev == NULL) {
		s_ev = new struct uc_sigevent;
		s_ev->sigev_notify = SIGEV_SIGNAL;
		s_ev->sigev_signo = SIGALRM;
		s_ev->sigev_value.sival_int = (int)new_data->timer_id;
	}

	new_data->event_data = *s_ev;
	if (clockid == CLOCK_THREAD_CPUTIME_ID) {
		if (get_clock_pthread(qt_parent_rtos->POSIX->uc_pthread_self(), &clockid) != 0) {
			printf("Calling thread's clock not found\n");
		}
	}

	if (clockid == CLOCK_PROCESS_CPUTIME_ID) {
		if (get_clock_id(qt_parent_rtos->POSIX->uc_getpid(), &clockid) != 0) {
			printf("Calling process' clock not found\n");
		}
	}

	if (CLOCK_REALTIME == clockid) {
		new_data->timer_id = (timer_t)((int)new_data->timer_id);
		new_clk = &system_clk;
	}
	else if (CLOCK_MONOTONIC == clockid) {
		new_data->timer_id = (timer_t)((int)new_data->timer_id);
		new_clk = &monotonic_clk;
	}
	else if (m_clock_list.size() != 0) {
		vector<uc_clock_data *>::iterator it_clk;
		for (it_clk = m_clock_list.begin(); it_clk != m_clock_list.end(); it_clk++) {
			if ((*it_clk)->clk_id == clockid) {
				new_clk = *it_clk;
				break;
			}
		}
		if (it_clk == m_clock_list.end()) {
			return -1;
		}
	}
	else {
		return -1;
	}

	*timer = new_data->timer_id;
	timer_list.insert(timer_list.begin(), new_data);

	new_data->clock_addr = new_clk;

	new_clk->list_t.push_back(new_data);
	return 0;
}

int UC_tic_timer_class::delete_timer(timer_t timer) {
	uc_timer_data *new_data;

	vector<uc_timer_data *>::iterator it_timer;
	if (timer_list.size() != 0) {
		for (it_timer = timer_list.begin(); it_timer != timer_list.end(); it_timer++) {
			if ((*it_timer)->timer_id == timer) {
				new_data = *it_timer;
				break;
			}
		}
		if (it_timer == timer_list.end()) {
			return -1;
		}
	}
	else {
		return -1;
	}

	new_data->active = 0;

	for (it_timer = new_data->clock_addr->list_t.begin(); it_timer != new_data->clock_addr->list_t.end(); it_timer++) {
		if (*it_timer == new_data) {
			new_data->clock_addr->list_t.erase(it_timer);
			break;
		}
	}

	for (it_timer = timer_list.begin(); it_timer != timer_list.end(); it_timer++) {
		if (*it_timer == new_data) {
			timer_list.erase(it_timer);
			break;
		}
	}

	return 0;
}

int UC_tic_timer_class::get_time(timer_t timer, struct itimerspec * spec) {
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 0;

	vector<uc_timer_data *>::iterator it_timer;
	for (it_timer = timer_list.begin(); it_timer != timer_list.end(); it_timer++) {
		if ((*it_timer)->timer_id == timer) {
			spec->it_value = tics_2_timespec((*it_timer)->next_expiration);
			spec->it_interval = (*it_timer)->i_spec.it_interval;
			return 0;
		}
	}

	return -1;
}

int UC_tic_timer_class::get_overrun(timer_t timer) {
	vector<uc_timer_data *>::iterator it_timer;
	for (it_timer = timer_list.begin(); it_timer != timer_list.end(); it_timer++) {
		if ((*it_timer)->timer_id == timer) {
			return (*it_timer)->signal_overrun;
		}
	}

	errno = EINVAL;
	return -1;
}

int UC_tic_timer_class::set_time(timer_t timer, int flags, const struct itimerspec *in_spec, struct itimerspec *o_spec) {
	uc_clock_data *new_clk;
	uc_timer_data *new_data;
	struct timespec ts;

	if (in_spec->it_interval.tv_sec < 0) {
		errno = EINVAL;
		return -1;
	}
	if (in_spec->it_interval.tv_nsec < 0) {
		errno = EINVAL;
		return -1;
	}
	if (in_spec->it_interval.tv_nsec >= 1000000000) {
		errno = EINVAL;
		return -1;
	}
	if (in_spec->it_value.tv_sec < 0) {
		errno = EINVAL;
		return -1;
	}
	if (in_spec->it_value.tv_nsec < 0) {
		errno = EINVAL;
		return -1;
	}
	if (in_spec->it_value.tv_nsec >= 1000000000) {
		errno = EINVAL;
		return -1;
	}

	ts.tv_sec = 0;
	ts.tv_nsec = 0;

	if (timer_list.size() != 0) {
		vector<uc_timer_data *>::iterator it_timer;
		for (it_timer = timer_list.begin(); it_timer != timer_list.end(); it_timer++) {
			if ((*it_timer)->timer_id == timer) {
				new_data = *it_timer;
				break;
			}
		}
		if (it_timer == timer_list.end()) {
			return -1;
		}
	}
	else {
		return -1;
	}

	new_clk = new_data->clock_addr;

	if (o_spec != NULL) {
		if ((flags & TIMER_ABSTIME) != 0) {
			o_spec->it_value = tics_2_timespec(new_data->next_expiration + new_clk->counter);
		}
		else {
			o_spec->it_value=tics_2_timespec(new_data->next_expiration);
		}
		o_spec->it_interval = new_data->i_spec.it_interval;
	}

	if (in_spec == NULL) {
		errno = EINVAL;
		return -1;
	}

	if (in_spec->it_value == ts) {
		new_data->active = 0;
		return 0;
	}

	new_data->i_spec.it_interval = in_spec->it_interval;
	new_data->i_spec.it_value = in_spec->it_value;
	new_data->active = 1;

	if (flags & TIMER_ABSTIME) {
	   	new_data->next_expiration = timespec_2_tics_up(in_spec->it_value);
		new_data->next_expiration -= new_clk->counter;
		new_data->abs_value = 1;
	}
	else {
		new_data->next_expiration = timespec_2_tics_up(in_spec->it_value);
		new_data->abs_value = 0;
	}

	place_timer(new_data, new_clk);
	return 0;
}

int UC_tic_timer_class::clock_nanosleep(clockid_t clk_id, int flags, const struct timespec *rqtp, struct timespec *rmtp) {
	clockid_t clock_id = clk_id;

	if (rqtp->tv_sec < 0) {
		return EINVAL;
	}
	if (rqtp->tv_nsec < 0) {
		return EINVAL;
	}
	if (rqtp->tv_nsec >= 1000000000) {
		return EINVAL;
	}

	if (clock_id == CLOCK_THREAD_CPUTIME_ID) {
		if (get_clock_pthread(qt_parent_rtos->POSIX->uc_pthread_self(), &clock_id) != 0) {
			printf("Calling thread's clock not found\n");
  		}
	}
	if (clock_id == CLOCK_PROCESS_CPUTIME_ID) {
		if (get_clock_id(qt_parent_rtos->POSIX->uc_getpid(), &clock_id) != 0) {
			printf("Calling process' clock not found\n");
		}
	}

	uc_clock_data *new_clk;
	uc_nanosleep_data *data = new uc_nanosleep_data;
	if (CLOCK_REALTIME == clock_id) {
		new_clk = &system_clk;
	}
	else if (CLOCK_MONOTONIC == clock_id) {
		new_clk = &monotonic_clk;
	}
	else if (m_clock_list.size() != 0) {
		vector<uc_clock_data *>::iterator it_clk;
		for (it_clk = m_clock_list.begin(); it_clk != m_clock_list.end(); it_clk++) {
			if ((*it_clk)->clk_id == clock_id) {
				new_clk = *it_clk;
				break;
			}
		}
		if (it_clk == m_clock_list.end()) {
			return EINVAL;
		}
	}
	else {
		return EINVAL;
	}

	data->clk = new_clk;
	data->thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	if (rqtp == NULL) {
		return EINVAL;
	}

	if (flags & TIMER_ABSTIME) {
	   	data->counter = timespec_2_tics_up(*rqtp);
		data->counter -= new_clk->counter;
		data->abs_value = 1;
	}
	else {
		data->counter = timespec_2_tics_up(*rqtp);
		data->abs_value = 0;
	}

	place_sleep(data, new_clk);

	UC_thread_class *thread_element = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
	while (data->counter > 0) {
		data->thread->block();

		if (thread_element->m_signal_executed) {
			if (rqtp != NULL && rmtp != NULL) {
				*rmtp = tics_2_timespec(data->counter);
			}
			return EINTR;
		}
	}

	vector<uc_nanosleep_data *>::iterator it_clk;
	for (it_clk = new_clk->list_c.begin(); it_clk != new_clk->list_c.end(); it_clk++) {
		if (*it_clk == data) {
			new_clk->list_c.erase(it_clk);
			break;
		}
	}

	if (rqtp != NULL && rmtp != NULL) {
		*rmtp = tics_2_timespec(data->counter);
	}

	return 0;
}

int UC_tic_timer_class::get_clock_id(uc_pid_t pid, clockid_t *clockid) {
	vector<uc_clock_data *>::iterator it_clk;
	for (it_clk = m_clock_list.begin(); it_clk != m_clock_list.end(); it_clk++) {
		if ((*it_clk)->process != NULL && (*it_clk)->process->m_pid == pid) {
			*clockid = (*it_clk)->clk_id;
			return 0;
		}
	}

	errno = EINVAL;
	return -1;
}

uc_clock_data* UC_tic_timer_class::get_clock(clockid_t c_id) {
	int flag;
	uc_clock_data* clock;
	clockid_t clock_id = c_id;

	if (clock_id == 0) {
		return &system_clk;
	}
	if (clock_id == CLOCK_REALTIME) {
		return &system_clk;
	}
	if (clock_id == CLOCK_MONOTONIC) {
		return &monotonic_clk;
	}
	if (clock_id == CLOCK_THREAD_CPUTIME_ID) {
		if ((flag = get_clock_pthread(qt_parent_rtos->POSIX->uc_pthread_self(), &clock_id)) != 0) {
			printf("Calling thread's clock not found\n");
		}
	}
	if (clock_id == CLOCK_PROCESS_CPUTIME_ID) {
		if ((flag = get_clock_id(qt_parent_rtos->POSIX->uc_getpid(), &clock_id)) != 0) {
			printf("Calling process' clock not found\n");
		}
	}

	vector<uc_clock_data *>::iterator it_clk;
	for (it_clk = m_clock_list.begin(); it_clk != m_clock_list.end(); it_clk++) {
		if (clock_id == (*it_clk)->clk_id) {
			return *it_clk;
		}
	}

	errno = EINVAL;
	return NULL;
}

int UC_tic_timer_class::clock_gettime(clockid_t clk_id, struct timespec  *rqtp){
	uc_clock_data *new_clk;

	clockid_t clock_id = clk_id;
	if (CLOCK_REALTIME == clock_id) {
		*rqtp = tics_2_timespec(system_clk.counter) + real_time_add_time;
		return 0;
	}
	if (CLOCK_MONOTONIC == clock_id) {
		*rqtp = tics_2_timespec(monotonic_clk.counter);
		return 0;
	}

	if (clock_id == CLOCK_THREAD_CPUTIME_ID) {
		if (get_clock_pthread(qt_parent_rtos->POSIX->uc_pthread_self(), &clock_id) != 0) {
			printf("Calling thread's clock not found\n");
  		}
	}
	if (clock_id == CLOCK_PROCESS_CPUTIME_ID) {
		if (get_clock_id(qt_parent_rtos->POSIX->uc_getpid(), &clock_id) != 0) {
			printf("Calling process' clock not found\n");
		}
	}

	vector<uc_clock_data *>::iterator it_clk;
	for (it_clk = m_clock_list.begin(); it_clk != m_clock_list.end(); it_clk++) {
		if ((*it_clk)->clk_id == clock_id) {
			*rqtp = tics_2_timespec((*it_clk)->counter);
			return 0;
		}
	}

	errno=EINVAL;
	return -1;
}

int UC_tic_timer_class::clock_settime(clockid_t clk_id, const struct timespec  *rqtp) {
	uc_clock_data *new_clk;
	clockid_t clock_id = clk_id;
	if (rqtp->tv_sec < 0) {
		errno = EINVAL;
		return -1;
	}
	if (rqtp->tv_nsec < 0) {
		errno = EINVAL;
		return -1;
	}
	if (rqtp->tv_nsec >= 1000000000) {
		errno = EINVAL;
		return -1;
	}
	if (CLOCK_REALTIME == clock_id) {
		add_clock_time(clock_id,timespec_2_tics(*rqtp) - system_clk.counter, 1);
		system_clk.counter = timespec_2_tics(*rqtp);
		return 0;
	}
	if (CLOCK_MONOTONIC == clock_id) {
		return -1;
	}

	if (clock_id == CLOCK_THREAD_CPUTIME_ID) {
		if (get_clock_pthread(qt_parent_rtos->POSIX->uc_pthread_self(), &clock_id) != 0) {
			printf("Calling thread's clock not found\n");
  		}
	}

	if (clock_id == CLOCK_PROCESS_CPUTIME_ID) {
		if (get_clock_id(qt_parent_rtos->POSIX->uc_getpid(), &clock_id) != 0) {
			printf("Calling process' clock not found\n");
		}
	}

	vector<uc_clock_data *>::iterator it_clk;
	for (it_clk = m_clock_list.begin(); it_clk != m_clock_list.end(); it_clk++) {
		if ((*it_clk)->clk_id == clock_id) {
			add_clock_time(clock_id, timespec_2_tics(*rqtp) - (*it_clk)->counter, 1);
			(*it_clk)->counter = timespec_2_tics(*rqtp);
			return 0;
		}
	}

	errno=EINVAL;
	return -1;
}

unsigned int UC_tic_timer_class::get_MicroCOS_num_tics() {
	return MicroCOS_num_tics;
}

void UC_tic_timer_class::set_MicroCOS_num_tics(unsigned int tics) {
	MicroCOS_num_tics = tics;
}

int UC_tic_timer_class::add_clock_time(clockid_t clock_id, int num, int abs_value) {
	uc_clock_data *get_clk;
	uc_timer_data *timer_data, *t_aux;
	uc_nanosleep_data *sleep_data;
	struct timespec ts;
	int ret = 0;
	vector<uc_clock_data *>::iterator it_clk;

	ts.tv_sec = 0;
	ts.tv_nsec = 0;

	if (CLOCK_REALTIME == clock_id) {
		get_clk = &system_clk;
	}
	else if (CLOCK_MONOTONIC == clock_id) {
		get_clk = &monotonic_clk;
	}
	else if (m_clock_list.size() != 0) {
		for (it_clk = m_clock_list.begin(); it_clk != m_clock_list.end(); it_clk++) {
			if ((*it_clk)->clk_id == clock_id) {
				get_clk = *it_clk;
				break;
			}
		}
		if (it_clk == m_clock_list.end()) {
			return -1;
		}
	}
	else {
		return -1;
	}

	get_clk->counter = get_clk->counter + num;

	vector<uc_timer_data *>::iterator it_timer;
	for (it_timer = get_clk->list_t.begin(); it_timer != get_clk->list_t.end(); it_timer++) {
		if ((*it_timer)->active && (!abs_value || (*it_timer)->abs_value)) {
			if ((*it_timer)->next_expiration < (long long)num) {
				(*it_timer)->next_expiration = 0;
			}
			else {
				(*it_timer)->next_expiration -= (long long)num;
			}
			ret = 1;
		}
	}

	vector<uc_nanosleep_data *>::iterator it_sleep;
	for (it_sleep = get_clk->list_c.begin(); it_sleep != get_clk->list_c.end(); it_sleep++) {
		if (!abs_value || (*it_sleep)->abs_value) {
			if ((*it_sleep)->counter < (unsigned long long)num) {
				(*it_sleep)->counter = 0;
			}
			else {
				(*it_sleep)->counter -= (unsigned long long)num;
			}
			ret = 1;
		}
	}

	while (get_clk->list_t.size() != 0) {
		timer_data = get_clk->list_t[0];
		if (timer_data->active && timer_data->next_expiration <= 0) {
			get_clk->list_t.erase(get_clk->list_t.begin());
			generate_signal(timer_data);
			if (timer_data->i_spec.it_interval != ts) {
				timer_data->next_expiration = timespec_2_tics_up(timer_data->i_spec.it_interval);
				place_timer(timer_data, get_clk);
				t_aux = timer_data;
				timer_data->abs_value = 0;
			}
		}
		else {
			break;
		}
	}

	while (get_clk->list_c.size() != 0) {
		sleep_data = get_clk->list_c[0];
		if (sleep_data->counter <= 0) {
			get_clk->list_c.erase(get_clk->list_c.begin());
			generate_signal(sleep_data);
		}
		else {
			break;
		}
	}

	return ret;
}

struct timespec UC_tic_timer_class::tics_2_timespec(const long long tics){
	struct timespec ts;
	double time = (double)tics;
	time /= TICK_TIMER_HZ;
	ts.tv_sec = (int)time;
	ts.tv_nsec = (int)((time - (double)ts.tv_sec) * 1e9);
	return ts;
}

long long UC_tic_timer_class::timespec_2_tics_up(const struct timespec tics){
	double time = tics.tv_sec * 1e9 + tics.tv_nsec;
	time *= (TICK_TIMER_HZ / 1e9);
	long long f = (long long)time;
	if ((time - (double)f) < 0) {
		f--;
	}
	if ((time - (double)f) > 0) {
		f++;
	}
	return f;
}

long long UC_tic_timer_class::timespec_2_tics(const struct timespec tics){
	double time = tics.tv_sec * 1e9 + tics.tv_nsec;
	time *= (TICK_TIMER_HZ / 1e9);
	return (long long)time;
}

void UC_tic_timer_class::place_sleep(uc_nanosleep_data* sleep, uc_clock_data* clock) {
	vector<uc_nanosleep_data *>::iterator it_clk;
	for (it_clk = clock->list_c.begin(); it_clk != clock->list_c.end(); it_clk++) {
		if (*it_clk == sleep) {
			clock->list_c.erase(it_clk);
			break;
		}
	}

	for (it_clk = clock->list_c.begin(); it_clk != clock->list_c.end(); it_clk++) {
		if ((*it_clk)->counter >= sleep->counter) {
			break;
		}
	}

	clock->list_c.insert(it_clk, sleep);
}

void UC_tic_timer_class::place_timer(uc_timer_data* timer, uc_clock_data* clock) {
	vector<uc_timer_data *>::iterator it_clk;
	for (it_clk = clock->list_t.begin(); it_clk != clock->list_t.end(); it_clk++) {
		if (*it_clk == timer) {
			clock->list_t.erase(it_clk);
			break;
		}
	}

	for (it_clk = clock->list_t.begin(); it_clk != clock->list_t.end(); it_clk++) {
		if ((*it_clk)->next_expiration >= timer->next_expiration) {
			break;
		}
	}

	clock->list_t.insert(it_clk, timer);
}

int UC_tic_timer_class::generate_signal(uc_timer_data *data) {
	data->signal_counter++;
	data->event_data.si_code=SI_TIMER;
	data->event_data.timer=data;
	data->process->signals_control.signal_process(&data->event_data);
	return 0;
}

int UC_tic_timer_class::generate_signal(uc_nanosleep_data *data) {
	data->thread->m_timeout_expired = 1;
	data->thread->unblock();
	return 0;
}

void UC_tic_timer_class::create_clock(UC_thread_class *pthread) {
	struct uc_clock_data *new_clk = new struct uc_clock_data;
	new_clk->clk_id = clock_n++;
	new_clk->counter = 0;
	new_clk->thread = pthread;
	new_clk->process = NULL;
	m_clock_list.push_back(new_clk);
}

void UC_tic_timer_class::create_clock(UC_process_class *process) {
	struct uc_clock_data *new_clk = new struct uc_clock_data;
	new_clk->clk_id = clock_n++;
	new_clk->counter = 0;
	new_clk->thread = NULL;
	new_clk->process = process;
	m_clock_list.push_back(new_clk);
}

int UC_tic_timer_class::destroy_clock(UC_thread_class *pthread){
	if (pthread == NULL) {
		return -1;
	}

	vector<uc_clock_data *>::iterator it_clk;
	for (it_clk = m_clock_list.begin(); it_clk != m_clock_list.end(); it_clk++) {
		if ((*it_clk)->thread == pthread) {
			delete (*it_clk);
			m_clock_list.erase(it_clk);
			return 0;
		}
	}

	return -1;
}

int UC_tic_timer_class::destroy_clock(UC_process_class *process){
	if (process == NULL) {
		return -1;
	}

	vector<uc_clock_data *>::iterator it_clk;
	for (it_clk = m_clock_list.begin(); it_clk != m_clock_list.end(); it_clk++) {
		if ((*it_clk)->process == process) {
			delete (*it_clk);
			m_clock_list.erase(it_clk);
			return 0;
		}
	}

	return -1;
}

void UC_tic_timer_class::thread_destroyed(UC_thread_class *thread) {
	struct uc_clock_data *clk;
	int cont_sleep;

	// Cleaning system_clk
	clk = &system_clk;
	for (cont_sleep = 0; cont_sleep < clk->list_c.size(); /*nothing*/) {
		if (clk->list_c[cont_sleep]->thread == thread) {
			clk->list_c.erase(clk->list_c.begin() + cont_sleep);
		}
		else {
			cont_sleep++;
		}
	}

	// Cleaning monotonic_clk
	clk = &monotonic_clk;
	for (cont_sleep = 0; cont_sleep < clk->list_c.size(); /*nothing*/) {
		if (clk->list_c[cont_sleep]->thread == thread) {
			clk->list_c.erase(clk->list_c.begin() + cont_sleep);
		}
		else {
			cont_sleep++;
		}
	}

	// Cleaning the list of clocks
	for (int cont_clk = 0; cont_clk < m_clock_list.size(); cont_clk++) {
		clk = m_clock_list[cont_clk];
		for (cont_sleep = 0; cont_sleep < clk->list_c.size(); /*nothing*/) {
			if (clk->list_c[cont_sleep]->thread == thread) {
				clk->list_c.erase(clk->list_c.begin() + cont_sleep);
			}
			else {
				cont_sleep++;
			}
		}
	}
}

void UC_tic_timer_class::process_destroyed(UC_process_class *process) {
	struct uc_clock_data *clk;
	int cont_sleep;

	// Cleaning system_clk
	clk = &system_clk;
	for (cont_sleep = 0; cont_sleep < clk->list_t.size(); /*nothing*/) {
		if (clk->list_t[cont_sleep]->process == process) {
			for (int cont_timer = 0; cont_timer < timer_list.size(); cont_timer++) {
				if (timer_list[cont_timer] == clk->list_t[cont_sleep]) {
					timer_list.erase(timer_list.begin() + cont_timer);
					break;
				}
			}
			clk->list_t.erase(clk->list_t.begin() + cont_sleep);
		}
		else {
			cont_sleep++;
		}
	}

	// Cleaning monotonic_clk
	clk = &monotonic_clk;
	for (cont_sleep = 0; cont_sleep < clk->list_t.size(); /*nothing*/) {
		if (clk->list_t[cont_sleep]->process == process) {
			for (int cont_timer = 0; cont_timer < timer_list.size(); cont_timer++) {
				if (timer_list[cont_timer] == clk->list_t[cont_sleep]) {
					timer_list.erase(timer_list.begin() + cont_timer);
					break;
				}
			}
			clk->list_t.erase(clk->list_t.begin() + cont_sleep);
		}
		else {
			cont_sleep++;
		}
	}

	// Cleaning the list of clocks
	for (int cont_clk = 0; cont_clk < m_clock_list.size(); cont_clk++) {
		clk = m_clock_list[cont_clk];
		for (cont_sleep = 0; cont_sleep < clk->list_t.size(); /*nothing*/) {
			if (clk->list_t[cont_sleep]->process == process) {
				for (int cont_timer = 0; cont_timer < timer_list.size(); cont_timer++) {
					if (timer_list[cont_timer] == clk->list_t[cont_sleep]) {
						timer_list.erase(timer_list.begin() + cont_timer);
						break;
					}
				}
				clk->list_t.erase(clk->list_t.begin() + cont_sleep);
			}
			else {
				cont_sleep++;
			}
		}
	}
}

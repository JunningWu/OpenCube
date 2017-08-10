//==========================================================================
//	uc_time_class.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: lun mar 31 2008
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

#ifndef UC_TIME_CLASS_H
#define UC_TIME_CLASS_H

#include <vector>
using namespace std;
class UC_thread_class;
class UC_process_class;

class uc_time_variables_class{
	public:
		time_t since_epoch;

		uc_time_variables_class(){
			since_epoch=0;
		}

};
extern uc_time_variables_class uc_time_variables;

struct uc_nanosleep_data {
	UC_thread_class *thread;
	struct uc_clock_data *clk;
	unsigned long long counter;
	int abs_value;
};

struct uc_clock_data {
	clockid_t clk_id;
	unsigned long long counter;
	UC_process_class *process;
	UC_thread_class *thread;
	vector<struct uc_timer_data*> list_t; // list of armed timers sorted
	vector<struct uc_nanosleep_data*> list_c; // list of sleeps sorted
};

struct uc_timer_data {
	timer_t timer_id;
	int active;

	UC_process_class *process;

	int signal_counter;
	int signal_overrun;

	struct uc_clock_data* clock_addr;
	struct uc_sigevent event_data;
	long long next_expiration;       // tics remaining, when 0 timer acts
	int abs_value;
	struct itimerspec i_spec;
};

bool operator >(const struct timespec a, const struct timespec b);
bool operator <(const struct timespec a, const struct timespec b);
bool operator >=(const struct timespec a, const struct timespec b);
bool operator <=(const struct timespec a, const struct timespec b);
bool operator ==(const struct timespec a, const struct timespec b);
bool operator !=(const struct timespec a, const struct timespec b);
struct timespec operator -(const struct timespec a, const struct timespec b);
struct timespec operator +(const struct timespec a, const struct timespec b);

#endif

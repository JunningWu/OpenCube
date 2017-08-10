//==========================================================================
//	uc_statistics_class.h
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

#ifndef UC_STATISTICS_CLASS_H
#define UC_STATISTICS_CLASS_H

#include <vector>

class UC_cpu_class;
class UC_rtos_class;
class UC_thread_class;

class UC_cpu_statistics_class{
public:
    UC_cpu_statistics_class();

    ~UC_cpu_statistics_class();

	void print_statistics(UC_cpu_class *cpu);

	long long m_thread_switches; ///< Number of thread switches
	long long m_context_switches; ///< Number of task switches
	long long m_running_time; ///< Number of ns that the cpu has been executing code
	long long m_num_instr; ///< Number of machine instructions executed by the cpu
	long long m_bus_time; ///< Time the cpu has been busy on bus accesses
	long long m_idle_time; ///< Time the cpu has been idle
	long long m_interrupts; ///< Number of interruptions received by the cpu
	long long m_bus_transfers; ///< Amount of bytes transferred through the bus

};

class UC_rtos_statistics_class{
public:
    struct thread_info{
	int m_tid;
	char *m_name;		///< Thread name
	unsigned long long m_user_time;	///< ns. Time the thread has been executed in user mode
	
    };
    struct process_info{
	int pid;
	long long start_time;	///< ps. The time when the process was created
	long long end_time;	///< ps. The time when the process was destroyed (-1 if alive)
	std::vector <struct thread_info*> m_threads;
    };

    unsigned long long m_rtos_time;

    std::vector <struct process_info*> m_processes;

    UC_rtos_statistics_class();

    ~UC_rtos_statistics_class();

    void print_statistics(UC_rtos_class *rtos);

    int process_creation(int pid);
    int process_destroy(int pid);

    int add_thread_user_time(UC_thread_class *thread, unsigned long long time);

    unsigned long long get_min_process_start_time();	///< Return the minimum process start time in ps
    unsigned long long get_max_process_end_time();	///< Return the minimum process start time in ps

};

#endif

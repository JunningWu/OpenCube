//==========================================================================
//	uc_statistics_class.cpp
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

#include <iostream>
#include "systemc.h"
#include "rtos/kernel/uc_statistics_class.h"
#include "rtos/kernel/uc_cpu_class.h"
#include "rtos/utils/uc_backtrace.h"

using namespace std;

UC_cpu_statistics_class::UC_cpu_statistics_class()
{
	m_thread_switches = 0;
	m_context_switches = 0;
	m_running_time = 0;
	m_num_instr = 0;
	m_bus_time = 0;
	m_idle_time = 0;
	m_interrupts = 0;
	m_bus_transfers = 0;
}


UC_cpu_statistics_class::~UC_cpu_statistics_class()
{
}

/**
 * \brief Prints in the standard output the statistics of the CPU
 */
void UC_cpu_statistics_class::print_statistics(UC_cpu_class *cpu) {
	float time;
	double core_energy;
	double icache_energy = 0;
	int icache_misses = 0;
	double dcache_energy = 0;
	int dcache_misses = 0;
	int dcache_vueltasmp=0;
	time = sc_time_stamp().value();
	core_energy = 2 * m_num_instr;
	if (cpu->m_icache != NULL) {
		icache_energy = 3 * m_num_instr + 40 * cpu->m_icache->m_total_misses;
		icache_misses = cpu->m_icache->m_total_misses;
	}
	if (cpu->m_dcache != NULL) {
 		dcache_energy = 3 * m_num_instr + 40 * cpu->m_dcache->m_dcache_misses;
 		dcache_misses = cpu->m_dcache->m_dcache_misses;
 		dcache_vueltasmp = cpu->m_dcache->m_dcache_rempl;
	}

	cout << "\tNumber of thread switches:  " << m_thread_switches << endl;
	cout << "\tNumber of context switches: " << m_context_switches << endl;
	cout << "\tRunning time: " << m_running_time << " ns" << endl;
	cout << "\tUse of cpu: " << (1000.0 * m_running_time / sc_time_stamp().value()) * 100.0 << "%" << endl;
	cout << "\tInstructions executed: " << m_num_instr << endl;
	cout << "\tInstruction cache misses: " << icache_misses << endl;
	cout << "\tData cache misses: " << dcache_misses << endl;
	cout << "\tData cache returns to mp: " << dcache_vueltasmp << endl;
	cout << "\tCore Energy: " << core_energy << " nJ" << endl;
	cout << "\tCore Power: " << 1000000.0 * core_energy / time << " mW" << endl;
	cout << "\tInstruction Cache Energy: " << icache_energy << " nJ" << endl;
	cout << "\tData Cache Energy: " << dcache_energy << " nJ" << endl;
	cout << "\tInstruction Cache Power: " << 1000000.0 * icache_energy / time << " mW" << endl;
	cout << "\tBus access time:  " << m_bus_time  << " ns" << endl;
	cout << "\tBus transfers:  " << m_bus_transfers  << " bytes" << endl;
	cout << "\tIdle time:  " << m_idle_time  << " ns" << endl;
	cout << "\tNumber of interrupts:  " << m_interrupts << endl;
}

UC_rtos_statistics_class::UC_rtos_statistics_class()
{
	m_rtos_time = 0;
}


UC_rtos_statistics_class::~UC_rtos_statistics_class()
{
}

/**
 * \brief Prints in the standard output the statistics of the RTOS
 */
void UC_rtos_statistics_class::print_statistics(UC_rtos_class *rtos) {
	int num_finished = 0;
	long double total_time = 0;
	long long time = sc_time_stamp().value();
	struct process_info *process;
	struct thread_info *thread;


	for(int i=0; i<m_processes.size(); i++){

		if( m_processes[i]->end_time > 0 ){
			num_finished ++;
			total_time += m_processes[i]->end_time - m_processes[i]->start_time;
		}
		else{
			total_time += time - m_processes[i]->start_time;
		}

	}
	cout << "RTOS:  " << rtos->get_id() << endl;
	cout << "\tNumber of m_processes created:  " << m_processes.size() << endl;
	cout << "\tNumber of m_processes destroyed:  " << num_finished << endl;
	cout << "\tMean process duration (process start - process end):  " << (total_time/m_processes.size()) * 1e-12 <<" sec"<< endl;
	cout << "\tLast SW execution time:  " << get_max_process_end_time() * 1e-12 <<" sec"<< endl;
	double rtos_time = m_rtos_time;
	double user_time = 0;

	for (int i = 0; i < m_processes.size(); i++) {
		process = m_processes[i];
		cout << "	Process PID: "<<process->pid<<endl;
		for (int j = 0; j < process->m_threads.size(); j++) {
			thread = process->m_threads[j];
			cout << "		Thread TID: "<<thread->m_tid;
			cout << ", name: "<<thread->m_name;
			cout << ", User time: "<<thread->m_user_time << " ns"<<endl;
			user_time += thread->m_user_time;
		}
	}
	cout << "\tTotal User time:  " << user_time / 1e9 <<" sec"<< endl;
	cout << "\tTotal Kernel time:  " << (rtos_time / 1e9) <<" sec"<< endl;


}


/**
 * \brief Add a new process to the statistics
 * \param pid number of the new process
 * \return 0 on success, -1 if the pid has been already used
 */
int UC_rtos_statistics_class::process_creation(int pid){
	struct process_info *info = new struct process_info;
	for(int i=0; i<m_processes.size(); i++){
		if( m_processes[i]->pid == pid ){
			cout<<"WARNING: pid has been already used"<<endl;
			return -1;
		}
	}

	info->pid = pid;
	info->start_time = sc_time_stamp().value();
	info->end_time = -1;
	m_processes.push_back(info);
	return 0;
}

/**
 * \brief Indicate that a process has finished
 * If it is there are no more active SW processes and the variable
 * uc_end_with_sw_destruction is active the simulation is finished
 *
 * \param pid number of the new process
 * \return 0 on success, -1 if the pid has not been created or has already been destroyed
 */
int UC_rtos_statistics_class::process_destroy(int pid){
	for(int i=0; i<m_processes.size(); i++){
		if( m_processes[i]->pid == pid ){
			if(m_processes[i]->end_time != -1){
				cout<<"WARNING: process has been already destroyed"<<endl;
				return -1;
			}
			m_processes[i]->end_time = sc_time_stamp().value();
			uc_check_end_with_sw_destruction();
			return 0;
		}
	}
	return -1;
}


/**
 * \brief return the time in ps when the first process started
 * \return minimum start time
 */
unsigned long long UC_rtos_statistics_class::get_min_process_start_time(){
	if(m_processes.size() == 0 ) return 0;
	return m_processes[0]->start_time;
}

/**
 * \brief return the time in ps when the last process finished. If any has not been destroyed return the current time
 * \return maximum process end time
 */
unsigned long long UC_rtos_statistics_class::get_max_process_end_time(){ ///< Return the minimum process start time in ps
	long long time = 0;
	for(int i=0; i<m_processes.size(); i++){
		if( m_processes[i]->end_time == -1 ){
			return sc_time_stamp().value();
		}
		if(m_processes[i]->end_time > time) time = m_processes[i]->end_time;
	}
	return time;
}

/**
 * \brief Add the user time to the thread statistics. If not a user process the time is added to kernel time
 * \param thread thread where the time must be added
 * \param time the time to add
 * \return true if the thread already was in the list, if not, and -1 if the process was not in the list (usually for interrupts)
 */
int UC_rtos_statistics_class::add_thread_user_time(UC_thread_class *thread, unsigned long long time){

	int pid, tid;
	pid = thread->m_parent_task->m_parent_process->m_pid;
	tid = thread->m_tid;

	for(int i=0;i<m_processes.size(); i++){
		if(m_processes[i]->pid != pid) continue;
		for(int j=0; j<m_processes[i]->m_threads.size();j++){
			if(m_processes[i]->m_threads[j]->m_tid != tid) continue;
			m_processes[i]->m_threads[j]->m_user_time += time;
			return 1;
		}

		// The process exists but the thread has not been created yet
		struct thread_info *thread_info = new struct thread_info;
		thread_info->m_tid = tid;
		thread_info->m_name = (char*) malloc(strlen(thread->m_func_name)+1);
		strcpy(thread_info->m_name, thread->m_func_name);
		thread_info->m_user_time = time;

		m_processes[i]->m_threads.push_back(thread_info);
		return 0;
	}
	m_rtos_time += time;
	return -1;
}

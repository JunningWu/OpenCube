//==========================================================================
//	uc_generate_execution_trace.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar 11 2009
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

#include "systemc.h"
#include "rtos/utils/uc_generate_execution_trace.h"
#include "rtos/kernel/uc_task_class.h"
#include "rtos/kernel/uc_rtos_class.h"

/**
 * \brief Constructor
 *
 */
generate_execution_trace::generate_execution_trace(){
	if(uc_backtrace_required() & 0x30) {
		file = fopen("trace.dat", "w");
		if(file == NULL){
			printf("Error: file trace.date open failed\n");
		}
	}
	else {
		file = NULL;
	}
}

/**
 * \brief Destructor
 *
 */
generate_execution_trace::~generate_execution_trace(){
	if(file!= NULL){
		fclose(file);
		file = NULL;
	}
}

/**
 * \brief Print a line indicating that a threads starts a USER execution
 *
 * \param thread Thread that starts the execution
 * \param cpu CPU number identifier where the thread runs
 */
void generate_execution_trace::print_start(UC_thread_class *thread, int cpu){
	if(uc_backtrace_required() & 0x10) {
		if(file == NULL){
			file = fopen("trace.dat", "w");
			if(file == NULL){
				printf("Error: file trace.date open failed\n");
			}
		}
		

		if(thread == NULL) return;
		if (thread->m_func_name == NULL) thread->m_func_name = "";
		if(thread->m_parent_task == NULL || thread->m_parent_task->m_parent_process == NULL){
			fprintf(file,"resume task: time:%f RTOS: %02d CPU: %03d PID: %03d TID: %03d State: %d name: %s \n",((double)(sc_time_stamp().value()))/1e12,-1,cpu, -1, thread->m_tid, thread->m_state, thread->m_func_name);			
		}else{
			fprintf(file,"resume task: time:%f RTOS: %02d CPU: %03d PID: %03d TID: %03d State: %d name: %s \n",((double)(sc_time_stamp().value()))/1e12,thread->m_parent_task->m_parent_process->m_parent_rtos->get_id(),cpu, thread->m_parent_task->m_parent_process->m_pid, thread->m_tid, thread->m_state, thread->m_func_name);
		}
	}
}

/**
 * \brief Print a line indicating that a threads end a USER execution
 *
 * \param thread Thread that ends the execution
 * \param cpu CPU number identifier where the thread runs
 */
void generate_execution_trace::print_end(UC_thread_class *thread, int cpu){
	if(uc_backtrace_required() & 0x10) {
		if(file == NULL){
			file = fopen("trace.dat", "w");
			if(file == NULL){
				printf("Error: file trace.date open failed\n");
			}
		}
		if(thread == NULL) return;
		if(thread->m_parent_task == NULL || thread->m_parent_task->m_parent_process == NULL){
			fprintf(file,"stop  task:  time:%f RTOS: %02d CPU: %03d PID: %03d TID: %03d State: %d name: %s \n",((double)(sc_time_stamp().value()))/1e12,-1,cpu, -1, thread->m_tid, thread->m_state, thread->m_func_name);			
		}else{
			fprintf(file,"stop  task:  time:%f RTOS: %02d CPU: %03d PID: %03d TID: %03d State: %d name: %s \n",((double)(sc_time_stamp().value()))/1e12,thread->m_parent_task->m_parent_process->m_parent_rtos->get_id(),cpu, thread->m_parent_task->m_parent_process->m_pid, thread->m_tid, thread->m_state, thread->m_func_name);
		}
	}
}

/**
 * \brief Print a line indicating that a threads starts a bus access
 *
 * \param thread Thread that starts the access
 * \param cpu CPU number identifier where the thread runs
 */
void generate_execution_trace::print_start_bus(UC_thread_class *thread, int cpu){
	if(uc_backtrace_required() & 0x20) {
		if(file == NULL){
			file = fopen("trace.dat", "w");
			if(file == NULL){
				printf("Error: file trace.date open failed\n");
			}
		}
		if(thread == NULL) return;
		if(thread->m_parent_task == NULL || thread->m_parent_task->m_parent_process == NULL){
			fprintf(file,"start bus access: time:%f RTOS: %02d CPU: %03d PID: %03d TID: %03d name: %s \n",((double)(sc_time_stamp().value()))/1e12,-1,cpu, -1, thread->m_tid,thread->m_func_name);			
		}else{
			fprintf(file,"start bus access: time:%f RTOS: %02d CPU: %03d PID: %03d TID: %03d name: %s \n",((double)(sc_time_stamp().value()))/1e12,thread->m_parent_task->m_parent_process->m_parent_rtos->get_id(),cpu, thread->m_parent_task->m_parent_process->m_pid, thread->m_tid,thread->m_func_name);
		}
	}
}

/**
 * \brief Print a line indicating that a threads end a bus access
 *
 * \param thread Thread that ends the access
 * \param cpu CPU number identifier where the thread runs
 */
void generate_execution_trace::print_end_bus(UC_thread_class *thread, int cpu){
	if(uc_backtrace_required() & 0x20) {
		if(file == NULL){
			file = fopen("trace.dat", "w");
			if(file == NULL){
				printf("Error: file trace.date open failed\n");
			}
		}
		if(thread == NULL) return;
		if(thread->m_parent_task == NULL || thread->m_parent_task->m_parent_process == NULL){
			fprintf(file,"end  bus  access: time:%f RTOS: %02d CPU: %03d PID: %03d TID: %03d name: %s \n",((double)(sc_time_stamp().value()))/1e12,-1,cpu, -1, thread->m_tid,thread->m_func_name);			
		}else{
			fprintf(file,"end  bus  access: time:%f RTOS: %02d CPU: %03d PID: %03d TID: %03d name: %s \n",((double)(sc_time_stamp().value()))/1e12,thread->m_parent_task->m_parent_process->m_parent_rtos->get_id(),cpu, thread->m_parent_task->m_parent_process->m_pid, thread->m_tid,thread->m_func_name);
		}
	}
}

generate_execution_trace execution_trace;


//==========================================================================
//	uc_backtrace.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié abr 2 2008
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

#include <execinfo.h>
#include <cxxabi.h>
#include "rtos/kernel/uc_thread_class.h"
#include "rtos/kernel/uc_process_class.h"
#include "rtos/kernel/uc_task_class.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/utils/uc_backtrace.h"
#include <stdio.h>
#include <unistd.h>

UC_thread_class *uc_last_executed_thread;

/**
 * \brief Function that return the string corresponding to a process state
 *
 * \param state State to be decoded
 * \return The corresponding string
 */

const char *uc_print_state(UC_process_states state){
	switch(state){
		case ZOMBIE: 	return "ZOMBIE";
		case CREATED: 	return "CREATED";
		case BLOCKED: 	return "BLOCKED";
		case READY: 	return "READY";
		case WAITING: 	return "WAITING";
		case USER: 		return "USER";
		case SUPER_USER: return "SUPER_USER";
//		default:	return "Unknown";
	}
	return "Unknown";
}

/**
 * \brief Function to save the backtrace of the current thread
 *
 * \param current_thread Pointer to the current thread
 */
void uc_save_backtrace(UC_thread_class *current_thread) {

	void *buffer[40];
	int deep, i;

	if(current_thread==NULL) return;

	if( uc_backtrace_required() == 0 ) return;

	if((deep=backtrace(buffer,40))<=0 || deep < 2) {
		printf("Error in backtrace\n");
	}
	else{
		// FIXME: Function names seems to be static and shuld not require to be free
/*		for(i=0;i<current_thread->m_num_backtrace;i++){
		free(current_thread->m_backtrace[i]);
	}
*/

		if(current_thread->m_num_backtrace > 0 && current_thread->m_backtrace!= NULL){
			free(current_thread->m_backtrace);
		}
		if(current_thread->m_num_backtrace > 0 && current_thread->m_backtrace_p!= NULL){
			free(current_thread->m_backtrace_p);
		}

		// First two elements are "uc_save_backtrace" and "stp_yield". They are not interesting
		current_thread->m_backtrace_p = (void**)malloc( (deep-2) * sizeof(void*));
		for ( i=0; i < (deep-2); i++ ){
			current_thread->m_backtrace_p[i] = buffer[i+2];
		}
		current_thread->m_num_backtrace=deep-2;

		if( (uc_backtrace_required() & 4) != 0){
			current_thread->m_backtrace=backtrace_symbols(&buffer[2],deep-2);
		}
	}
	uc_last_executed_thread = current_thread;
}

/**
 * \brief Function that returns the demangled function string from a backtrace string
 *
 * \param str Backtrace string
 * \return Demangled function name
 */
char *uc_get_demangled_backtrace_string(const char * str){

	char *name;
	char *begin = 0, *end = 0;
	if (str == NULL){ return ""; }

	// find the parentheses and address offset surrounding the mangled name
	begin = (char*)strchr(str,'(');
	end = (char*)strrchr(str,'+');
	if (begin && end){
		int stat;
		begin++;
		*end = '\0';
		name = abi::__cxa_demangle(begin,0,0,&stat);

		if (name == NULL)
		{
			*end = '+';
			name = (char*) malloc(strlen(str)+1);
			strcpy(name,str);
		}
	}
	else{
		name = (char*) malloc(strlen(str)+1);
		strcpy(name,str);
	}
	return name;
}

/**
 * \brief Function that print the backtrace of a thread
 *
 * \param thread Thread with the corresponding backtrate to print
 */
void uc_print_backtrace(UC_thread_class *thread) {

	void *buffer[1];
	int i;
	char **str;

	if(thread==NULL) return;

	buffer[0]=(void*)thread->m_initial_function;
	str = backtrace_symbols(buffer,1);

	fprintf(stderr,"\nThread %i in %s with %s state. Current backtrace:\n",thread->m_tid, uc_get_demangled_backtrace_string(str[0]), uc_print_state(thread->m_state));

	if(thread->m_num_backtrace > 0 && thread->m_backtrace == NULL){
		thread->m_backtrace=backtrace_symbols(thread->m_backtrace_p,thread->m_num_backtrace);
	}

	for(i=0;i<thread->m_num_backtrace;i++){
		fprintf(stderr,"\t%s\n",uc_get_demangled_backtrace_string(thread->m_backtrace[i]));
	}
}

/**
 * \brief Function that print the backtrace of all threads of a task
 *
 * \param task Task containing the threads which backtrace will be printed
 */
void uc_print_backtrace(UC_task_class *task) {
	int i;

	fprintf(stderr,"\ntask in %s state\n", uc_print_state(task->m_state));

	for(i=0;i<task->m_thread_list.size();i++){
		uc_print_backtrace(task->m_thread_list[i]);
	}

}

/**
 * \brief Function that print the backtrace of all threads of a process
 *
 * \param process Process containing the threads which backtrace will be printed
 */
void uc_print_backtrace(UC_process_class *process) {
	int i;

	fprintf(stderr,"\nprocess %i %s\n", process->m_pid, process->m_process_name);

	for(i=0;i<process->m_task_list.size();i++){
		uc_print_backtrace(process->m_task_list[i]);
	}

}

/**
 * \brief Function that print the backtrace of all rtos fo the system
 *
 */
void uc_print_backtrace(){
	UC_rtos_class *rtos;
	
	if(! uc_simulation_config.activate_backtrace & 1 ){
		return;
	}

	for(int i=0;i<uc_simulation_config.rtos_list->size();i++){
		rtos = (*uc_simulation_config.rtos_list)[i];
		fprintf(stderr,"\nReport of RTOS %i\n",i);
	
		for(int i=0;i<rtos->m_process_list.size();i++){
			uc_print_backtrace(rtos->m_process_list[i]);
		}
	}
}


/**
 * \brief Check if backtrace generation is required
 *
 * \return Backtrace requirement.
 */
int uc_backtrace_required(){
	return uc_simulation_config.activate_backtrace;
}


bool uc_activate_vcd_report(int level){
	
	UC_rtos_class *rtos;
	if(uc_backtrace_required() && 0x300){
		uc_simulation_config.vcd_file = sc_create_vcd_trace_file("execution");
	}

/*	for(int i = 0; i < uc_simulation_config.rtos_list.size(); i++){
		rtos = uc_simulation_config.rtos_list[i];
		for(int i = 0; i < rtos->processors_list.size(); i++){
			
		}
	}*/

}

/**
 * \brief Check if backtrace generation is required
 * 1(0001b): store backtrate point,   3(0011b): print final backtrate status,
 * 5(0101b): save backtrace strings, 15(1111b): print all backtrace()
 * 16 (1xxxxb): generate cpu trace file, 32(1xxxxxb): generate cpu-bus accesses trace file
 * (0x100): generate vcd trace with processors' state
 * !<(0x200): generate vcd trace with threads' state !! non operative >!
 * \param val New backtrace value
 * \return 0
 */
int uc_activate_backtrace(int val){
	uc_simulation_config.activate_backtrace = val;
	if(val && 0x100){
		uc_activate_vcd_report(val);
	}

	return 0;
}


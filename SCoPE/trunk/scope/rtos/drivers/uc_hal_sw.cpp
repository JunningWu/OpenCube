//==========================================================================
//	uc_hal_sw.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: jue nov 22 2007
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
#include <stdarg.h>

#include "rtos/kernel/uc_cpu_class.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "hal/uc_bus_data.h"

extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;
extern unsigned long long uc_segment_time;
extern unsigned long long uc_segment_instr;


/**
 * \brief Creates the stack to access to the bus
 *
 * Stores the function identifier and creates the stack for arguments. The cpu checks that
 * the identifier is not -1 and execute the HAL function. \n
 * This function is executed by the user code in a transparent way for the user.
 * \param cpu The current processor
 * \param ... The parameters of the HAL's function
 * \return
 */
int HAL_stack_creation(int function_id, ...) {
// 	UC_rtos_class *foo_rtos = qt_parent_rtos;
// 	UC_process_class *foo_process = qt_parent_process;
// 	UC_thread_class *thread = (*qt_parent_rtos)[qt_parent_process->m_cpu]->m_current_task->m_current_thread;
// 	thread->set_next_state(USER, SUPER_USER);
// 	thread->update_state();
// 	stp_yield();

	uc_cpu_bus_transfer_info_t *info = new uc_cpu_bus_transfer_info_t;
	int i;
	int *local_stack;

	UC_cpu_class *cpu = qt_parent_rtos->get_current_cpu();
	UC_thread_class *thread = cpu->m_current_task->m_current_thread;

	va_list arg_list;
	va_start(arg_list, function_id);

	thread->m_bus_transfer_info.m_HAL_function_id = function_id;
	thread->m_bus_transfer_info.m_HAL_done = 0;

	local_stack = (int*)thread->m_bus_transfer_info.m_HAL_arg_stack;

	local_stack[0] = (int)thread;
	for (int arg_cont = 1; arg_cont < 6; arg_cont++) {
		local_stack[arg_cont] = va_arg(arg_list, int);
	}
	va_end(arg_list);
	thread->annotate_user_time(uc_segment_time, uc_segment_instr);
	uc_segment_time = 0;
	uc_segment_instr = 0;

	while(thread->m_bus_transfer_info.m_HAL_done==0){

		thread->m_bus_transfer_list.push_back(&thread->m_bus_transfer_info);
	
		stp_yield();
	
/*	Element removed from the list in uc_cpu_class:bus_access.
		for(  i=0; i < thread->m_bus_transfer_list.size(); i++ ) {
			if(thread->m_bus_transfer_list[i]==(&thread->m_bus_transfer_info)){
				thread->m_bus_transfer_list.erase( thread->m_bus_transfer_list.begin()+i );
			}
		}*/

	}

	thread->m_bus_transfer_info.m_HAL_done = 0;

	return thread->m_bus_transfer_info.m_HAL_return;
}

/**
 * \brief Creates the stack to access to the bus
 *
 * Stores the function identifier and creates the stack for arguments. The cpu checks that
 * the identifier is not -1 and execute the HAL function. \n
 * This function is executed by the user code in a transparent way for the user.
 * \param cpu The current processor
 * \param function_id The identifier of the function
 * \param ... The parameters of the HAL's function
 * \return
 */
/*int HAL_stack_creation_cpu(int function_id, UC_cpu_class *cpu, ...) {
	UC_thread_class *thread = cpu->m_current_task->m_current_thread;

	va_list arg_list;
	va_start(arg_list, cpu);

	thread->m_bus_transfer_info.m_HAL_function_id = function_id;
	int *local_stack = (int*)thread->m_bus_transfer_info.m_HAL_arg_stack;

	local_stack[0] = (int)thread;
	for (int arg_cont = 1; arg_cont < 6; arg_cont++) {
		local_stack[arg_cont] = va_arg(arg_list, int);
	}

	va_end(arg_list);

	return 0;
}*/

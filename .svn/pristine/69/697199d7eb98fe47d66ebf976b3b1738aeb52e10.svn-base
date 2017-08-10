//==========================================================================
//	uc_cpu_class.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mar oct 30 2007
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

#ifndef UC_CPU_CLASS_H
#define UC_CPU_CLASS_H

#include "rtos/kernel/uc_rtos_class.h"
#include <vector>
using namespace std;

#include "rtos/kernel/uc_task_class.h"
#include "rtos/kernel/uc_process_class.h"
#include "rtos/kernel/uc_statistics_class.h"
#include "rtos/drivers/uc_irq_mask_class.h"
#include "rtos/drivers/uc_hal_sw_defines.h"
#include "rtos/kernel/uc_icache.h"
#include "rtos/kernel/uc_dcache.h"
#include "rtos/kernel/uc_gui_statistics.h"
#include "hal/uc_tlm_bus_sockets.h"
#include "hal/uc_master_base.h"
#include "systemc.h"
class uc_cpu_xml_if_class;
class UC_TLM_bus_class;


class UC_cpu_class : public UC_master_base {
public:
	UC_cpu_class(sc_module_name module_name, UC_rtos_class *rtos, int id,  const char* cpu_name);

	~UC_cpu_class();

	void new_process(void* (*function)(void *), void *args, const char *name = "", float optim = -1);

// 	void * HAL_stack_creation(int function_id, ...);

	void send_irq(int irq);

	static void * interrupt_wrapper(void *data);
	static void * interruption_handler(void *void_cpu);
	void bus_access(UC_thread_class *thread);
	int freeze_cpu(unsigned long long time); ///< Freeze the processor for the specified time (in ns)

	cpu_statistics get_statistics();

	UC_rtos_class *m_parent_rtos; ///< Pointer to the parent rtos
	UC_task_class *m_current_task; ///< Current executing task
	UC_task_class *m_idle_task; ///< Idle task, executed when no other is available
	UC_task_class *m_prev_task; ///< The previous task
	UC_process_class *m_idle_process; ///< Idle process, executed when no other is available

	int m_status; ///< CPU status: 0 idle, 1 running, 2 bus access

	int m_id; ///< Identifier of this processor

	UC_cpu_statistics_class *m_statistics; ///< Cpu statistics

	sc_event m_resume_cpu; ///< Event to interrupt the waiting of the cpu

	uc_cpu_bus_transfer_info_t *m_HAL_info; ///< HAL's transfer info

	unsigned short m_active_interruptions; ///< Array of raised and not handled interruptions. Max = 16 interruptions
	UC_process_class *m_interruptions_process; ///< The process that handles the interruptions threads
	UC_task_class *m_interruptions_task; ///< The task that handles the interruptions threads
	UC_thread_class *m_interruptions_thread; ///< The thread that creates the interruptions threads
	//UC_thread_class *m_preempted_thread; ///< The thread preempted by an interruption

	UC_process_class *m_dic_interruptions_process; ///< The process that handles the interruptions threads for the DIC
	UC_task_class *m_dic_interruptions_task; ///< The task that handles the interruptions threads for the DIC
	UC_thread_class *m_dic_interruptions_thread; ///< The thread that creates the interruptions threads for the DIC

	void load_cpu(void);
	UC_icache_class *m_icache;	///< The instruction cache
	UC_dcache_class *m_dcache;	///< The data cache

	bool m_irq_enabled; 		///< The flag that sets the interruptions enabled or disabled

	// Added an additional parameter to allow adapting the parser time to processors with different frequency
	long long m_parsed_clock;	///< The clock frequency supposed for the cpu when parsing(in Hz)
	long long m_current_clock;	///< The current clock frequency of the cpu (in Hz). Can be dynamicaly changed. if =0 parsed clock is used
	float m_cpi;			///< The cycles per instruction for this cpu
	float m_instr_energy;		///< The average energy per instruction

private:
	unsigned long long m_freeze_time; ///< Time the processor must be freezed. It will be waited at the first chance.
		
	void init();
	void execute_HAL_function();
	void wait_time(UC_thread_class *thread);
	vector<void* (*)(void *)> m_new_function_list; ///< List of functions to be executed when the simulation starts
	vector<void *> m_new_function_args_list; ///< List of arguments that corresponds with the functions in m_new_function_list
	vector<char *> m_new_function_name_list; ///< List of names that corresponds with the functions in m_new_function_list
	vector<float>  m_new_function_opt_list;  ///< List of optimization factors that corresponds with the functions in m_new_function_list
	UC_irq_mask_class m_irq_mask; ///< The interruption's mask

	uc_cpu_xml_if_class *m_xml_if;

};

#endif

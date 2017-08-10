//==========================================================================
//	uc_cpu_class.cpp
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

#include <stdarg.h>
#include "tinyxml.h"
#include "rtos/kernel/uc_cpu_class.h"
#include "rtos/kernel/uc_os_defines.h"
#include "rtos/drivers/uc_interrupt_info_class.h"
#include "rtos/api/posix/uc_posix_c.h"
#include "rtos/kernel/uc_cpu_xml_if.h"
#include "rtos/utils/uc_randn.h"
#include "hal/uc_tlm_bus_class.h"
#include "rtos/utils/uc_generate_execution_trace.h"

extern __thread UC_rtos_class *qt_parent_rtos;
extern __thread UC_process_class *qt_parent_process;
extern unsigned long long uc_segment_time;
extern unsigned long long uc_segment_instr;


// Eliminated: theorical cpi and frequency depends on the specific processor
//float UC_core_cpi = 0.0;

/**
 * \brief Constructor
 *
 * \param module_name module name for SystemC
 * \param rtos Pointer to its parent rtos
 * \param id CPU number identifier
 */
UC_cpu_class::UC_cpu_class(sc_module_name module_name, UC_rtos_class *rtos, int id, const char* cpu_name) : UC_master_base(module_name)
{
	m_parent_rtos = rtos;
	m_id = id;
	m_idle_process = new UC_process_class(m_parent_rtos, "idle");
	m_idle_task = new UC_task_class(m_idle_process);
	m_statistics = new UC_cpu_statistics_class();
	m_current_task = m_idle_task;

	m_active_interruptions = 0;
	m_irq_enabled = false;

	m_parsed_clock = 0;
	m_current_clock = 0;
	m_cpi = 0;

	m_status = 0;
	m_freeze_time = 0;

//	m_preempted_thread = NULL;
	m_icache = NULL;
	m_dcache = NULL;
	if(cpu_name == NULL){
		m_xml_if = new uc_cpu_xml_if_class("arm926t");
	}
	else{
		m_xml_if = new uc_cpu_xml_if_class(cpu_name);
	}
	m_prev_task = NULL;

	load_cpu();

//	m_port.bind(*this);

	SC_HAS_PROCESS(UC_cpu_class);

	SC_THREAD(init);

	if(uc_backtrace_required() && 0x100){

		char str[10];
		int index = 4;
		int id=m_id;
		strcpy(str,"cpu_");
		str[index++]=(id/100) + '0';
		id %= 100;
		str[index++]=(id/10) + '0';
		id %= 10;
		str[index++]=id + '0';
		str[index]='\0';
		sc_trace(uc_simulation_config.vcd_file,m_status, str);
	}

}

UC_cpu_class::~UC_cpu_class()
{
	cout << name() << " " << endl;
	m_statistics->print_statistics(this);
	if (m_icache != NULL) {
		cout << "\n\tTotal instruction miss transfers: " << m_icache->m_miss_transfers  << endl;
		delete m_icache;
	}
	if (m_dcache != NULL) {
		cout << "\n\tTotal data miss transfers: " << m_dcache->m_miss_transfers  << endl;
		delete m_dcache;
	}
}


/**
 * \brief Creates a list of functions to execute
 *
 * Inserts the new function into the list of new processes to be executed when
 * the simulation starts
 * \param function The function to be inserted into the list
 * \param args The arguments to be inserted into the list
 * \param optim Indicate the optimization factor provoked by the compiler optimizations. Default value -1(no optimization)
 */
void UC_cpu_class::new_process(void* (*function)(void *), void *args, const char *name, float optim)
{
	m_new_function_list.push_back(function);
	m_new_function_args_list.push_back(args);
	char *func_name = new char[strlen(name)+1];
	strcpy(func_name, name);
	m_new_function_name_list.push_back(func_name);
	m_new_function_opt_list.push_back(optim);
}

/**
 * \brief Creates the list of new processes, performs the reschedule/SystemC annotation loop
 *
 * This SC_THREAD executes the list of processes inserted before the simulation
 * starts in the correspondent processor. It creates and initializes the process
 * and thread members.\n
 * Then, it executes a loop with the reschedule and the time annotation functions.\n
 * If the current thread has initialized the bus access protocol, loading the
 * m_HAL_function_id the m_HAL_arg_stack variables, the cpu executes the correspondent
 * HAL function.
 */
void UC_cpu_class::init()
{

	m_interruptions_process = new UC_process_class(m_parent_rtos, "interruption_handler");
	m_parent_rtos->m_process_list.push_back(m_interruptions_process);

	m_interruptions_task = m_parent_rtos->POSIX->uc_pthread_create(m_interruptions_process, interruption_handler, (void*)this, "interruption_handler");
	m_interruptions_task->m_policy = SCHED_FIFO;
	m_interruptions_thread = m_interruptions_task->m_thread_list[0];
	m_interruptions_process->m_task_list[0]->m_cpu_mask = (1 << m_id);
	m_interruptions_process->m_task_list[0]->set_nice(-1000);
	m_interruptions_thread->m_attribs.schedpolicy = SCHED_FIFO;
	m_interruptions_thread->m_attribs.schedparam.sched_priority = 100;
	pthread_mutex_lock(&m_parent_rtos->m_rtos_mutex);

	if(m_parent_rtos->m_additional_interruption_manager != NULL){
		m_dic_interruptions_process = new UC_process_class(m_parent_rtos, "dic_interruptions_process");
		m_parent_rtos->m_process_list.push_back(m_dic_interruptions_process);

		m_dic_interruptions_task = m_parent_rtos->POSIX->uc_pthread_create(m_dic_interruptions_process, m_parent_rtos->m_additional_interruption_manager, (void*)this, "additional_interruption_handler");
		m_dic_interruptions_process->m_task_list[0]->m_cpu_mask = (1 << m_id);
		m_dic_interruptions_process->m_task_list[0]->set_nice(-1101);
		m_dic_interruptions_task->m_policy = SCHED_FIFO;
		m_dic_interruptions_thread = m_dic_interruptions_task->m_thread_list[0];
		pthread_mutex_lock(&m_parent_rtos->m_rtos_mutex);
	}

	UC_process_class *process;
	for (int i = 0; i < m_new_function_list.size(); i++) {
		process = new UC_process_class(m_parent_rtos, m_new_function_name_list[i], m_new_function_opt_list[i]);
		process->m_pid = m_parent_rtos->m_last_pid++;
		m_parent_rtos->m_statistics->process_creation(process->m_pid);
		m_parent_rtos->m_process_list.push_back(process);

		UC_task_class *task = m_parent_rtos->POSIX->uc_pthread_create(
				process,
				m_new_function_list[i],
				m_new_function_args_list[i],
				m_new_function_name_list[i]);

//	Modif 11-03-09: Process starts with all processor allocation valid
//		process->m_task_list[0]->m_cpu_mask = (1 << m_id);

		process->m_task_list[0]->m_cpu_mask = 0xffffffff;
		process->m_task_list[0]->m_cpu_mask >>= 32-m_parent_rtos->get_num_cpus();
		pthread_mutex_lock(&m_parent_rtos->m_rtos_mutex);

		delete (m_new_function_name_list[i]);
	}
	m_new_function_list.clear();
	m_new_function_args_list.clear();
	m_new_function_name_list.clear();
	m_new_function_opt_list.clear();

	UC_thread_class *thread = NULL;
	while (true) {

// Used when  the DIC real-time extension is activated.
// Checks if a real-time task has to be executed.
// Otherwise continues with normal execution
		if(m_parent_rtos->m_additional_scheduler != NULL){
			thread = m_parent_rtos->m_additional_scheduler(m_parent_rtos,this);

			if (thread == NULL) {
				thread = m_parent_rtos->m_task_scheduler->reschedule(this);
			}
		}
		else{
			thread = m_parent_rtos->m_task_scheduler->reschedule(this);
		}

		if (thread != NULL) { // (thread == NULL) ==> Idle task scheduled
			thread->m_current_cpu = this;
			execution_trace.print_start(thread,m_id);
			m_status = 1;

			#ifdef SEGMENT_MISS_ANNOTATION
			if(m_icache!=NULL)m_icache->mark_start_segment(this,thread);
			if(m_dcache!=NULL)m_dcache->mark_start_segment(this,thread);
			#endif

			if (thread->m_parser_time == 0) {
				m_parent_rtos->set_current_cpu(this);
				if(thread->m_state != SUPER_USER && thread->m_state != USER) {
//					printf("Warning. Parsing time in state = %d \n",thread->m_state);
//					assert(thread->m_state != SUPER_USER);
				}
				if(m_dcache!=NULL)uc_dcache_memory=m_dcache->m_memory;//LUMYLUMYLUMY
				pthread_mutex_unlock(&thread->m_parent_task->m_parent_process->m_process_mutex);
				pthread_mutex_lock(&m_parent_rtos->m_rtos_mutex);
				m_parent_rtos->set_current_cpu(NULL);
				thread->m_preempt_state = thread->m_next_state;
			}

			// sync_misses must be performed before bus_access
			// sync_misses moved to wait_time function
			//m_icache->sync_misses(this,thread);
			//m_dcache->sync_misses(this,thread);

			wait_time(thread);
			execution_trace.print_end(thread,m_id);

			// If m_parser_time!=0 the thread has been preempted and the bus access must be delayed
			if (thread->m_bus_transfer_list.size() > 0 && thread->m_parser_time == 0) {
				bus_access(thread);
			}
		}
		else {
			// Add the instructions executed by the idle process and cache misses
			sc_time init_hal_time = sc_time_stamp();
			m_status = 0;
			wait(m_resume_cpu);
			unsigned long long idle_time = (sc_time_stamp() - init_hal_time).value() / 1000;
			m_statistics->m_idle_time += idle_time;
/*			if(m_current_clock != 0){
				m_statistics->m_num_instr += (long long)((idle_time*m_current_clock) / 1e9) / m_cpi;
			}else{
				m_statistics->m_num_instr += (long long)((idle_time* m_parsed_clock) / 1e9) / m_cpi;
			}
*/			//FIXME: add a cache miss when loading the idle thread. Check stall cycles (use the mean stall for previous misses)
			if(m_icache != NULL){

				if(m_icache->m_total_misses != 0){
					m_icache->m_stall_cycles += (long long) m_icache->m_stall_cycles/m_icache->m_total_misses;
				}
				else{
					m_icache->m_stall_cycles+=10;
				}
// 				m_icache->m_total_misses++;
			}
		}
	}
}

void UC_cpu_class::bus_access(UC_thread_class *thread){
	int i;

	// Moves the thread to the ready list. If an interruption arrives while waiting
	// in the bus the task is preempted. If it is not in the list, it is not scheduled again.
	m_parent_rtos->set_current_cpu(this);

	while(thread->m_bus_transfer_list.size()>0){
		m_HAL_info = thread->m_bus_transfer_list[thread->m_bus_transfer_list.size()-1];
		thread->m_bus_transfer_list.pop_back();
		if(m_HAL_info->m_HAL_done) continue; // Transfer was already done

		execution_trace.print_start_bus(thread,m_id);

	
		sc_time init_hal_time = sc_time_stamp();
		m_status = 2;
		unsigned long long bus_time=0;
		switch(m_HAL_info->m_HAL_function_id){
			case UC_IO_READ_ID:
			case UC_IO_WRITE_ID:
			case UC_IO_IRQ_ID:
				execute_HAL_function();
				bus_time = (sc_time_stamp() - init_hal_time).value() / 1000;
			break;
			case UC_ICACHE_READ_ID:
			case UC_ICACHE_MEM_ID:
		// Add cache miss cycles for cache statistics
				execute_HAL_function();
				bus_time = (sc_time_stamp() - init_hal_time).value() / 1000;
				m_parent_rtos->set_current_cpu(this);
				m_icache->miss_time( bus_time );
			break;
			case UC_DCACHE_READ_ID:
			case UC_DCACHE_WRITE_ID:
			case UC_DCACHE_MEM_ID:
			case UC_DCACHE_WMEM_ID:
		// Add cache miss cycles for cache statistics
				execute_HAL_function();
				bus_time = (sc_time_stamp() - init_hal_time).value() / 1000;
				m_parent_rtos->set_current_cpu(this);
				m_dcache->miss_time( bus_time );
			break;
		}

	
		m_statistics->m_bus_time += bus_time;
		m_statistics->m_bus_transfers += m_HAL_info->m_HAL_return;
		if(thread->m_state == SUPER_USER){
			m_parent_rtos->m_statistics->m_rtos_time += bus_time;
		}
		else{
			thread->m_user_time += bus_time;
		}
		m_HAL_info->m_HAL_done = 1;

	
		execution_trace.print_end_bus(thread,m_id);
	}
	
	thread->m_bus_transfer_list.clear();
}


/**
 * \brief Waits an specify time or until an interruption is raised and updates the time variables
 *
 * \param thread A pointer to the thread that has to wait
 */
void UC_cpu_class::wait_time(UC_thread_class *thread) {
	float percent = 1.0, dcache_perc = 1.0, icache_perc = 1.0; //Using a time percent for instructions equivalency
	// Annotates the initial time
	unsigned long long initial_time,remaining_period;

	unsigned long long total_time, wait_time;
	unsigned long long total_instr;
	bool force_misses = false;
	
	// Checks for freeze time
	if (m_freeze_time != 0) {
		unsigned long long freeze = m_freeze_time;
		m_freeze_time = 0;
		// NOTE in delta cycle the interruption_handler can be updated to READY state

// FIXME: Decide if using event or not. 
/*		initial_time = sc_time_stamp().value() / 1000;*/
		wait(freeze, SC_NS/*, m_resume_cpu*/); // If no event is used, no preemtion must be modeled.
/*		m_freeze_time = sc_time_stamp().value() / 1000 - initial_time;*/
	}

/*	if (m_freeze_time == 0) {*/

	total_time = thread->m_switches_time + thread->m_parser_time;
	total_instr = thread->m_parser_instr;

	initial_time = sc_time_stamp().value() / 1000;

	wait_time = total_time;
	if(uc_get_max_wait_interval() > 0 && total_time > uc_get_max_wait_interval()){
		wait_time = uc_get_max_wait_interval();
		force_misses = true;
	}
	if(uc_get_sw_wait_period() > 0 ){
		remaining_period = uc_get_sw_wait_period() - (initial_time % uc_get_sw_wait_period());
		if(total_time > remaining_period){
			wait_time = remaining_period;
			force_misses = true;
		}
	}

	// Waits the switches time
	if (total_time != 0) {
		// NOTE in delta cycle the interruption_handler can be updated to READY state
		wait(wait_time, SC_NS, m_resume_cpu);
	}

	// Gets the running time and updates the time variables
	unsigned long long running_time = sc_time_stamp().value() / 1000 - initial_time;
	if (total_time != 0) {
		unsigned long long transfer_time=0, total_transfer_time=0;
		if(m_icache!=NULL){
			transfer_time = sc_time_stamp().value() / 1000 - m_icache->m_last_annotation;
			total_transfer_time = total_time - m_icache->m_last_annotation;
		}
		icache_perc = (float)transfer_time / (float)total_transfer_time;

		transfer_time=0;
		total_transfer_time=0;
		if(m_dcache!=NULL){
			transfer_time = sc_time_stamp().value() / 1000 - m_dcache->m_last_annotation;
			total_transfer_time = total_time - m_dcache->m_last_annotation;
		}
		dcache_perc = (float)transfer_time / (float)total_transfer_time;

		percent = (float)running_time / (float)total_time;
	}

	if(total_instr > 1e15 || percent > 1 || percent < 0) assert(0);

	unsigned long long running_instr = (long long)((float)total_instr * percent);
	m_statistics->m_running_time += running_time;
	m_statistics->m_num_instr += running_instr;
	thread->m_parser_time = total_time - running_time;
	thread->m_parser_instr = total_instr - running_instr;
	thread->m_switches_time = 0;

	if(force_misses){
		int n_misses ;
		if(m_icache!=NULL){
			n_misses = (int)(icache_perc*m_icache->m_annotate);
			m_icache->sync_misses(this,thread,n_misses);
		}
		if(m_dcache!=NULL){
			n_misses = (int)(dcache_perc*(m_dcache->m_annotate + m_dcache->m_write_annotate));
			m_dcache->sync_misses(this,thread,n_misses);
		}
	}

/*	}*/

 	if (thread->m_parser_time == 0) {
		thread->m_next_state = thread->m_preempt_state;
		thread->update_state();
	}
}

/**
 * \brief Creates the stack to access to the bus
 *
 * Stores the function identifier and creates the stack for arguments. The cpu checks that
 * the identifier is not -1 and execute the HAL function. \n
 * This function is executed by the user code in a transparent way for the user.
 * \param function_id The identifier of the function
 * \param ... The parameters of the HAL's function
 * \return
 */



/**
 * \brief Actives the execution of an interruption
 *
 * \param irq The irq to raise
 */
void UC_cpu_class::send_irq(int irq) {
	unsigned short irq_shift = (1 << irq);
#if 0
	// For debugging
	if (irq_shift & m_active_interruptions) {
		char msg[15];
		sprintf(msg, "IRQ lost: %d", irq);
		SC_REPORT_WARNING(msg, "");
	}
#endif
	m_active_interruptions |= irq_shift;
	m_statistics->m_interrupts ++;

	if(m_parent_rtos->m_additional_interruption_shield != NULL){
		if(m_parent_rtos->m_additional_interruption_shield(irq,m_parent_rtos)){
			// Interruptions enabled and not masked
			if ( (m_irq_enabled == true) && ((~m_irq_mask.current_mask() & irq_shift) > 0) ) {
				m_dic_interruptions_thread->set_next_state(BLOCKED, READY);
				m_dic_interruptions_thread->update_state();
				m_resume_cpu.notify(SC_ZERO_TIME);
			}
			return;
		}
	}

	// Interruptions enabled and not masked
	if ( (m_irq_enabled == true) && ((~m_irq_mask.current_mask() & irq_shift) > 0) ) {
		m_interruptions_thread->set_next_state(BLOCKED, READY);
		m_interruptions_thread->update_state();
		m_resume_cpu.notify(SC_ZERO_TIME);
	}
}

/**
 * \brief Handles the raised interruptions
 */
void * UC_cpu_class::interruption_handler(void * void_cpu) {
	char name[10];
	UC_cpu_class *cpu = (UC_cpu_class *)void_cpu;
	UC_thread_class *thread = cpu->m_interruptions_thread;
	cpu->m_irq_enabled = true;
	thread->set_next_state(USER, SUPER_USER);
	thread->update_state();

	/**** BOOT ****/
	if (cpu->m_id == 0) {
		cpu->m_parent_rtos->init_MicroCOS();
		cpu->m_parent_rtos->init_modules();
	}

	while (true) {
		thread->set_next_state(SUPER_USER, BLOCKED);
		thread->update_state(); // fix from DS2 feedback
		stp_yield();

		// Interruptions not enabled or masked
		if ( (cpu->m_irq_enabled == false) || ((~cpu->m_irq_mask.current_mask() & cpu->m_active_interruptions) == 0) ) {
			continue;
		}

		// Gets the interruption number
		unsigned short highest_interruption = cpu->m_active_interruptions;
		int interruption = 0;
		while (highest_interruption != 1) {
			highest_interruption >>= 1;
			interruption++;
		}
// 		cout << "IRQ" << interruption << endl;

		// Creates the needed class to call the interruption
		UC_interrupt_info_class *info = new UC_interrupt_info_class();
		*info = cpu->m_parent_rtos->m_idt[interruption];
		info->m_irq = interruption;
		info->m_cpu = cpu;

		// The last pid must not be change
		int max_pid = cpu->m_parent_rtos->m_last_pid;
		cpu->m_parent_rtos->m_last_pid = 0xffffffff;


		sprintf(name, "IRQ %d", interruption);
		UC_thread_class *new_thread = cpu->m_parent_rtos->POSIX->uc_create_thread(cpu->m_interruptions_task, NULL, interrupt_wrapper, (void*)info, (char*)name);

		cpu->m_interruptions_task->m_cpu = cpu->m_id;
		// Creates the thread in the interruption task
		int pid = qt_parent_process->m_pid;

		// Reloads the last pid
		cpu->m_parent_rtos->m_last_pid = max_pid;

		// The thread's priority is set depending on the interruption number
		new_thread->m_attribs.schedpolicy = SCHED_FIFO;
		new_thread->m_attribs.schedparam.sched_priority = interruption;
	}
	return NULL;
}

/**
 * \brief Executes the desired interruption's handler and controls the interruptions' mask
 *
 * \param data A struct with the needed parameters
 * \return The handler's return parameter
 */
void * UC_cpu_class::interrupt_wrapper(void *data) {
	void * return_pointer = NULL;
	UC_interrupt_info_class info = *(UC_interrupt_info_class *)data;

	delete ((UC_interrupt_info_class *)data);

	info.m_cpu->m_irq_mask.next_mask(~(0xffffff << (info.m_irq+1)));

	UC_thread_class *thread = info.m_cpu->m_current_task->m_current_thread;

	if (info.m_handler == NULL) {
		cout << "Interruption " << info.m_irq << " not managed, ";
		cout << "processor: " << info.m_cpu->m_id << ", time: " << sc_time_stamp() << endl;
	}
	else {
		// FIXME: Associate a constant time to all interrupt handlers

		// FIXME Interruptions flush the cache?
// 		if (info.m_cpu->m_icache != NULL) {
// 			info.m_cpu->m_icache->flush();
// 		}

		thread->set_next_state(USER, SUPER_USER);
		thread->update_state();

		return_pointer = (void*)info.m_handler(info.m_irq, info.m_args, info.m_regs);

		thread->set_next_state(SUPER_USER, WAITING);
		stp_yield();

/*		if (info.m_irq == 0){
			uc_segment_time += randn (1000, 0, 0) ;
		}
*/	}

	// Clear the interruption bit
	info.m_cpu->m_active_interruptions &= ~(1 << info.m_irq);

	info.m_cpu->m_irq_mask.previous_mask();

	if ((~info.m_cpu->m_irq_mask.current_mask() & info.m_cpu->m_active_interruptions) != 0) {
		info.m_cpu->m_interruptions_thread->set_next_state(BLOCKED, READY);
		info.m_cpu->m_interruptions_thread->update_state();
	}

	return return_pointer;
}

/**
 * \brief Creates a packet with the processor statistics
 *
 * \return The statistics of this processor
 */
cpu_statistics UC_cpu_class::get_statistics() {
	cpu_statistics stats;
	stats.running_time = m_statistics->m_running_time;
	stats.instructions = m_statistics->m_num_instr;
	if(m_icache != NULL){
		stats.instruction_misses = m_icache->m_total_misses;
	}
	if(m_dcache != NULL){
		stats.data_misses = m_dcache->m_total_misses;
	}
	return stats;
}


/**
 * \brief Loads the processor parameters from the processor library
 */
void UC_cpu_class::load_cpu(void) {
	m_xml_if->load_cpu(this);
}

/**
 * \brief Freeze the cpu for a certain time.
 *
 * \param time Time the processor must be freezed
 * \return 0 on default (OK). -1 when error
 */

int UC_cpu_class::freeze_cpu(unsigned long long time){
	m_freeze_time += time;
	return 0;
}

// Auxiliary functions
#define aligned_sizeof(a) ((sizeof(a)&0x3) ? (sizeof(a)&0xfffffffc)+4 : sizeof(a))
#define GET_ARG(ret, type, stack) { \
			ret = ((type*)stack)[0]; \
			stack = &((char*)stack)[aligned_sizeof(type)]; \
		}

/**
 * \brief Executes the loaded HAL function
 *
 * The user code has to execute a HAL function. That function has been loaded in the correspondent
 * variables by the HAL_stack_creation function and now can be executed. \n
 * This function gets the HAL function's arguments from the stack, run the required function and
 * stores the result in the return HAL variable.
 */
void UC_cpu_class::execute_HAL_function() {
	UC_thread_class *thread;
	void *local_stack = m_HAL_info->m_HAL_arg_stack;
	
//	uc_transfer_extension_t *extension = new uc_transfer_extension_t;
	uc_transfer_extension_t *extension = new uc_transfer_extension_t;
	extension->ts = 0;
	extension->hburst = 1;
	extension->width = 32;

	GET_ARG(thread, UC_thread_class*, local_stack);
	switch(m_HAL_info->m_HAL_function_id){
		case UC_IO_READ_ID:
		{
			ADDRESS a;
			DATA b;
			int c;
			GET_ARG(a, ADDRESS, local_stack);
			GET_ARG(b, DATA, local_stack);
			GET_ARG(c, int, local_stack);
			m_HAL_info->m_HAL_return = m_port->read(a, b, c, extension);
			break;
		}
		case UC_IO_WRITE_ID:
		{
			ADDRESS a;
			DATA b;
			int c;
			GET_ARG(a, ADDRESS, local_stack);
			GET_ARG(b, DATA, local_stack);
			GET_ARG(c, int, local_stack);
			m_HAL_info->m_HAL_return = m_port->write(a, b, c, extension);
			break;
		}
		case UC_ICACHE_MEM_ID:
		{
			int a;
			GET_ARG(a, int, local_stack);
			extension->ts = m_icache->m_last_annotation;
			extension->hburst = m_icache->m_line_size;
			if(m_parent_rtos->m_memory_addr!=(void*)-1){
				DATA addr=(DATA)malloc(a);
				m_HAL_info->m_HAL_return = m_port->read((ADDRESS)m_parent_rtos->m_memory_addr, addr, a, extension);
				free(addr);
			}
			else{
				m_HAL_info->m_HAL_return = m_port->send_to_memory(a, extension);
			}
			break;
		}
		case UC_DCACHE_MEM_ID:
		{
			int a;
			GET_ARG(a, int, local_stack);
			extension->ts = m_dcache->m_last_annotation;
			extension->hburst = m_dcache->m_line_size;
			if(m_parent_rtos->m_memory_addr!=(void*)-1){
				DATA addr=(DATA)malloc(a);
				m_HAL_info->m_HAL_return = m_port->read((ADDRESS)m_parent_rtos->m_memory_addr, addr, a, extension);
				free(addr);
			}
			else{
				m_HAL_info->m_HAL_return = m_port->send_to_memory(a, extension);
			}
			break;
		}
		case UC_DCACHE_WMEM_ID:
		{
			int a;
			GET_ARG(a, int, local_stack);
			extension->ts = m_dcache->m_last_annotation;
			extension->hburst = m_dcache->m_line_size;
			if(m_parent_rtos->m_memory_addr!=(void*)-1){
				DATA addr=(DATA)malloc(a);
				m_HAL_info->m_HAL_return = m_port->write((ADDRESS)m_parent_rtos->m_memory_addr, addr, a, extension);
				free(addr);
			}
			else{
				m_HAL_info->m_HAL_return = m_port->send_to_memory(a, extension);
			}
			break;
		}
		case UC_ICACHE_READ_ID:
		{
			ADDRESS a;
			DATA b;
			int c;
			GET_ARG(a, ADDRESS, local_stack);
			GET_ARG(b, DATA, local_stack);
			GET_ARG(c, int, local_stack);
			extension->ts = m_icache->m_last_annotation;
			extension->hburst = m_icache->m_line_size;
//			m_HAL_info->m_HAL_return = m_port->read(a, b, c, extension);
			if(m_parent_rtos->m_memory_addr!=(void*)-1){
				m_HAL_info->m_HAL_return = m_port->read((ADDRESS)m_parent_rtos->m_memory_addr, b, c, extension);
			}
			else{
				m_HAL_info->m_HAL_return = m_port->send_to_memory(c, extension);
			}
			break;
		}
		case UC_DCACHE_READ_ID:
		{
			ADDRESS a;
			DATA b;
			int c;
			GET_ARG(a, ADDRESS, local_stack);
			GET_ARG(b, DATA, local_stack);
			GET_ARG(c, int, local_stack);
			extension->ts = m_dcache->m_last_annotation;
			extension->hburst = m_dcache->m_line_size;
//			m_HAL_info->m_HAL_return = m_port->read(a, b, c, extension);
			if(m_parent_rtos->m_memory_addr!=(void*)-1){
				m_HAL_info->m_HAL_return = m_port->read((ADDRESS)m_parent_rtos->m_memory_addr, b, c, extension);
			}
			else{
				m_HAL_info->m_HAL_return = m_port->send_to_memory(c, extension);
			}
			break;
		}
		case UC_DCACHE_WRITE_ID:
		{
			ADDRESS a;
			DATA b;
			int c;
			GET_ARG(a, ADDRESS, local_stack);
			GET_ARG(b, DATA, local_stack);
			GET_ARG(c, int, local_stack);
			extension->ts = m_dcache->m_last_annotation;
			extension->hburst = m_dcache->m_line_size;
//			m_HAL_info->m_HAL_return = m_port->write(a, b, c, extension);
			if(m_parent_rtos->m_memory_addr!=(void*)-1){
				m_HAL_info->m_HAL_return = m_port->read((ADDRESS)m_parent_rtos->m_memory_addr, b, c, extension);
			}
			else{
				m_HAL_info->m_HAL_return = m_port->send_to_memory(c, extension);
			}
			break;
		}
		case UC_IO_IRQ_ID:
		{
			unsigned int a;
			unsigned int b;
			GET_ARG(a, unsigned int, local_stack);
			GET_ARG(b, unsigned int, local_stack);
			m_port->configure_irq_port(a, b);
			break;
		}
	}
	delete extension;
}

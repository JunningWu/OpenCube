//==========================================================================
//	uc_rtos_class.cpp
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

#include <errno.h>
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/qt_interface/stp.h"
#include "systemc.h"
#include "rtos/api/posix/uc_posix.h"
#include "rtos/drivers/uc_linux_module.h"
#include "rtos/utils/uc_backtrace.h"
#include "rtos/api/ucos/uc_microcos.h"

extern __thread UC_process_class *qt_parent_process;

#include <iostream>
using namespace std;

unsigned long long uc_segment_time = 0;
unsigned long long uc_segment_instr = 0;
int UC_rtos_class::static_rtos_id = 0;
unsigned int UC_rtos_class::m_gui_packet_ptr = 0;
uc_gui_packet UC_rtos_class::m_gui_packet_list[];
int UC_rtos_class::m_gui_fd = -1;

// Empty funtion to execute as IPI handler
irqreturn_t dummy_irq_function(int, void*, pt_regs*){}

UC_rtos_class::UC_rtos_class(int num_cpus, const char * cpu_name)
{
	m_this_rtos_id = static_rtos_id;
	static_rtos_id++;
	m_memory_addr=(void*)-1;

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&m_rtos_mutex, &attr);
	pthread_mutex_lock(&m_rtos_mutex);

	// Init API objects
	POSIX = new UC_posix_class(this);
	MicroCOS = new UC_microcos_class(this);

	// Init task scheduler
	m_task_scheduler = new UC_task_scheduler_class(this);

	m_idt = new UC_interrupt_info_class[16];
	// Init tic timer
	m_tic_timer = new UC_tic_timer_class(sc_gen_unique_name("tic_timer"), &m_cpu_list);
	m_idt[0].m_handler = UC_tic_timer_class::tic_timer_interruption_handler;
	m_idt[0].m_args = m_tic_timer;

	// Activate IPI (Inter-processor interruptions): Nothing to do, just awake it
	m_idt[1].m_handler = dummy_irq_function;
	m_idt[1].m_args = NULL;

	// Init the processors
	char name[30];
	for (int cpu = 0; cpu < num_cpus; cpu++) {
		sprintf(name, "processor_%d_rtos", cpu);
		UC_cpu_class *new_cpu = new UC_cpu_class(sc_gen_unique_name(name), this, cpu, cpu_name);
// 		new_cpu->load_cpu();
		m_cpu_list.push_back(new_cpu);
	}
	m_num_cpus = num_cpus;

	m_cond_id_max = 0;
	m_mutex_id_max = 0;
	m_mqd_id_max = 0;
	m_sem_id_max = 0;
	m_last_pgrp = 1;
	m_last_pid = 1;
	m_file_descriptor_max =1000;

	OSIntNesting = 0;
	MicroCOS_started = false;
	m_current_cpu = NULL;

	m_additional_scheduler = NULL;
	m_additional_interruption_manager = NULL;
	m_additional_interruption_shield = NULL;

	m_statistics = new UC_rtos_statistics_class();
	uc_simulation_config.rtos_list->push_back(this);
}

void UC_rtos_class::print_backtrace(){
	if ((uc_backtrace_required() & 2) != 0){
		fprintf(stderr,"\nReport of RTOS %i\n",m_this_rtos_id);
	
		for(int i=0;i<m_process_list.size();i++){
			uc_print_backtrace(m_process_list[i]);
		}
	}
}

UC_rtos_class::~UC_rtos_class()
{
	print_backtrace();
	for (int i = 0; i < m_process_list.size(); i++) {
		delete m_process_list[i];
	}

	m_statistics->print_statistics(this);
	for (int i = 0; i < m_cpu_list.size(); i++) {
		delete m_cpu_list[i];
	}

	int numbytes;
	void *bufw = pop_gui_packets(numbytes);
	if (numbytes != 0) {
		write(m_gui_fd, bufw, numbytes);
	}
	close(m_gui_fd);
}

/**
 * \brief Returns a pointer to the processor number 'cpu'
 *
 * The number of processors starts on 0
 * \param cpu The number of the processor to return
 * \return A pointer to the processor number 'cpu'
 */
UC_cpu_class * UC_rtos_class::get_processor(int cpu) {
	return (*this)[cpu];
}

/**
 * \brief Returns a pointer to the processor number 'cpu'
 *
 * The number of processors starts on 0
 * \param cpu The number of the processor to return
 * \return A pointer to the processor number 'cpu'
 */
UC_cpu_class * UC_rtos_class::operator [](int cpu) {
	if (cpu < 0 || cpu > m_num_cpus) {
		return NULL;
	}
	else {
		return m_cpu_list[cpu];
	}
}

/**
 * \brief Prints the process/task/thread tree
 */
void UC_rtos_class::rtos_debug() {
	vector<UC_process_class *>::iterator it_process;
	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;

	cout << "RTOS: " << this << endl;
	for (it_process = m_process_list.begin(); it_process != m_process_list.end(); it_process++) {
		cout << "\t Process: " << (*it_process) << endl;
		for (it_task = (*it_process)->m_task_list.begin(); it_task != (*it_process)->m_task_list.end(); it_task++) {
			cout << "\t\t Task: " << (*it_task) << ", state: " << (*it_task)->m_state << endl;
			for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
				cout << "\t\t\t Thread: " << (*it_thread) << ", state: " << (*it_thread)->m_state << endl;
			}
		}
	}
}

/**
 * \brief Finds a pthread cond
 *
 * \param id The pthread cond's identifier
 * \return The pthread cond
 */
uc_pthread_cond_class *UC_rtos_class::find_cond(int id) {
	vector<uc_pthread_cond_class *>::iterator it_cond;
	for (it_cond = m_cond_list.begin(); it_cond != m_cond_list.end(); it_cond++) {
		if ((*it_cond)->cond_id == id) {
			if ((*it_cond)->attr->pshared != 0) {
				return *it_cond;
			}
			if ((*it_cond)->parent_process != NULL && qt_parent_process != (*it_cond)->parent_process){
								//get_current_cpu()->m_current_task != (*it_cond)->parent->m_parent_task) {
				return NULL;
			}
			return (*it_cond);
		}
	}
	return NULL;
}


/**
 * \brief Finds a pthread mutex
 *
 * \param id The pthread mutex's identifier
 * \return The pthread mutex
 */
uc_pthread_mutex_class *UC_rtos_class::find_mutex(int id) {
	vector<uc_pthread_mutex_class *>::iterator it_mutex;
	for (it_mutex = m_mutex_list.begin(); it_mutex != m_mutex_list.end(); it_mutex++) {
		if ((*it_mutex)->mutex_id == id) {
			if ((*it_mutex)->mutexattr->pshared != 0) {
				return *it_mutex;
			}
			if ((*it_mutex)->parent_process != NULL && qt_parent_process != (*it_mutex)->parent_process){
								//get_current_cpu()->m_current_task != (*it_mutex)->parent->m_parent_task) {
				return NULL;
			}
			return (*it_mutex);
		}
	}
	return NULL;
}

/**
 * \brief Finds a mqueue object
 *
 * \param id The mqueue's identifier
 * \return The mqueue object
 */
uc_mqd_class *UC_rtos_class::find_mqd(int id) {
	vector<uc_mqd_class *>::iterator it_mqd;
	for (it_mqd = Mqueue_List.begin(); it_mqd != Mqueue_List.end(); it_mqd++) {
		if ((*it_mqd)->mqd_id == id) {
			return *it_mqd;
		}
	}
	errno = EINVAL;
	return NULL;
}

/**
 * \brief Finds a mqueue params object
 *
 * \param id The mqueue params' identifier
 * \param mqd The mqueue object
 * \return The mqueue params object
 */
uc_mqd_params *UC_rtos_class::find_mqd_params(int pid, uc_mqd_class *mqd) {
	vector<struct uc_mqd_params *>::iterator it_mqd;
	for (it_mqd = mqd->mqd_opened.begin(); it_mqd != mqd->mqd_opened.end(); it_mqd++) {
		if ((*it_mqd)->pid == pid) {
			return *it_mqd;
		}
	}
	return NULL;
}

/**
 * \brief Finds a process from its pid
 *
 * \param pid The process's identifier
 * \return The process
 */
UC_process_class * UC_rtos_class::get_process(pid_t pid) {
	vector<UC_process_class *>::iterator it_process;
	for (it_process = m_process_list.begin(); it_process != m_process_list.end(); it_process++) {
		if ((*it_process)->m_pid == pid) {
			return (*it_process);
		}
	}
	return NULL;
}

/**
 * \brief Finds a thread from its tid
 *
 * \param pid The thread's identifier
 * \return The thread
 */

UC_thread_class * UC_rtos_class::get_thread(pthread_t tid){
	vector<UC_process_class *>::iterator it_process;
	vector<UC_task_class *>::iterator it_task;
	vector<UC_thread_class *>::iterator it_thread;

	for (it_process = m_process_list.begin(); it_process != m_process_list.end(); it_process++) {
		for (it_task = (*it_process)->m_task_list.begin(); it_task != (*it_process)->m_task_list.end(); it_task++) {
			for (it_thread = (*it_task)->m_thread_list.begin(); it_thread != (*it_task)->m_thread_list.end(); it_thread++) {
				if ((*it_thread)->m_tid == tid) {
					return (*it_thread);
				}
			}
		}
	}
	return NULL;

}

/**
 * \brief Finds a semaphore object
 *
 * \param id The mqueue's identifier
 * \return The mqueue object
 */
uc_sem_class *UC_rtos_class::find_sem(int id) {
	vector<uc_sem_class *>::iterator it_sem;
	for (it_sem = Semaphores_List.begin(); it_sem != Semaphores_List.end(); it_sem++) {
		if ((*it_sem)->sem_id == id) {
			if ((*it_sem)->pshared != 0) {
				return *it_sem;
			}
			if (qt_parent_process != (*it_sem)->parent_process) {
				errno = EINVAL;
				return NULL;
			}
			return (*it_sem);
		}
	}
	return NULL;
}

/**
 * \brief Registers a new file
 *
 * \param file The file to register
 */
void UC_rtos_class::add_file(uc_file *file) {
	m_file_list.push_back(file);
}

/**
 * \brief Deletes a new file
 *
 * \param file The file to delete
 * \return 1 if success and 0 if failure
 */
int UC_rtos_class::delete_file(uc_file *file) {
	vector<uc_file *>::iterator it_file;
	for (it_file = m_file_list.begin(); it_file != m_file_list.end(); it_file++) {
		if (*it_file == file) {
			m_file_list.erase(it_file);
			return 1;
		}
	}
	return 0;
}

/**
 * \brief Assigns a file descriptor and registers a new file
 *
 * \param file The file to assign the file descriptor
 */
void UC_rtos_class::assign_file_descriptor(uc_file *file) {
	file->fd = m_file_descriptor_max++;
	add_file(file);
}

/**
 * \brief Gets a file from its identifier
 *
 * \param id The file's identifier to get
 * \return The pointer to the file or NULL if it is not found
 */
uc_file* UC_rtos_class::get_file(int id) {
	vector<uc_file *>::iterator it_file;
	for (it_file = m_file_list.begin(); it_file != m_file_list.end(); it_file++) {
		if ((*it_file)->fd == id) {
			return *it_file;
		}
	}
	return NULL;
}

/**
 * \brief Gets a file from its name
 *
 * \param name The file's name to get
 * \return The pointer to the file or NULL if it is not found
 */
uc_file* UC_rtos_class::get_file(const char *name) {
	vector<uc_file *>::iterator it_file;
	for (it_file = m_file_list.begin(); it_file != m_file_list.end(); it_file++) {
		if ((*it_file)->name!= NULL && strcmp((*it_file)->name, name) == 0) {
			return *it_file;
		}
	}
	return NULL;
}

void UC_rtos_class::kernel_module_init(uc_module_init_class module_init) {
	uc_linux_module *mod;
	for (int i = 0; i < m_module_list.size(); i++) {
		mod = m_module_list[i];
		if (strcmp(mod->m_module_file, module_init.m_file) == 0) {
			mod->init(module_init.m_init, module_init.m_name, module_init.m_version, module_init.m_descr, module_init.m_copyr);
			return;
		}
	}

	mod = new uc_linux_module(module_init.m_file);
	mod->init(module_init.m_init, module_init.m_name, module_init.m_version, module_init.m_descr, module_init.m_copyr);
	m_module_list.push_back(mod);
}

// void UC_rtos_class::kernel_module_exit(void (*exit)(void), char *file) {
// 	uc_linux_module *mod;
// 	for (int i = 0; i < m_module_list.size(); i++) {
// 		mod = m_module_list[i];
// 		if (strcmp(mod->m_module_file, file) == 0) {
// 			mod->set_exit(exit);
// 			return;
// 		}
// 	}
//
// 	mod = new uc_linux_module(file);
// 	mod->set_exit(exit);
// 	m_module_list.push_back(mod);
// }

void UC_rtos_class::init_modules() {
	for (int i = 0; i < m_module_list.size(); i++) {
		m_module_list[i]->m_init();
	}
}

// void UC_rtos_class::exit_modules() {
// 	uc_linux_module *mod;
// 	for (int i = 0; i < m_module_list.size(); i++) {
// 		mod = m_module_list[i];
// 		mod->m_exit();
// 		delete mod;
// 	}
// 	m_module_list.clear();
// }

/**
 * \brief Gets the host errno, not the simulated
 *
 * It is needed by functions that use host resources and
 * get an error return
 */
void UC_rtos_class::get_real_errno() {
	m_os_errno = errno;
}

/**
 * \brief Init the MicroCOS mutex
 */
void UC_rtos_class::init_MicroCOS() {
	uc_pthread_mutexattr_t MicroCOS_attr;
	POSIX->uc_pthread_mutexattr_init(&MicroCOS_attr);
	POSIX->uc_pthread_mutex_init(&MicroCOS_started_mutex, &MicroCOS_attr);
	POSIX->uc_pthread_mutex_lock(&MicroCOS_started_mutex);
}

/**
 * \brief Number of processors
 *
 * \return Number of processors
 */
int UC_rtos_class::get_num_cpus() {
	return m_num_cpus;
}

/**
 * \brief Gets the unique RTOS identifier
 *
 * \return the RTOS identifier
 */
int UC_rtos_class::get_id()
{
	return m_this_rtos_id;
}

/**
 * \brief Inserts a gui packet in a buffer for a later sending
 *
 * \param packet the gui packet
 */
void UC_rtos_class::push_gui_packet(uc_gui_packet packet)
{
// 	cout << "Packet " << m_gui_packet_ptr << endl;
// 	cout << packet.rtos_id << endl;
// 	cout << packet.tid << endl;
// 	cout << packet.time << endl;
// 	cout << packet.state << endl;
// 	cout << packet.name << endl;
// 	cout << endl;
	m_gui_packet_list[m_gui_packet_ptr] = packet;
	m_gui_packet_ptr++;

	if (m_gui_packet_ptr == MAX_GUI_BUFFER_SIZE) {
		int numbytes;
		void *bufw = pop_gui_packets(numbytes);
		write(m_gui_fd, bufw, numbytes);
	}
}

/**
 * \brief Gets the gui packet buffer and clears it
 *
 * \param bytes returns the number of bytes of the buffer
 * \return the gui packet buffer
 */
uc_gui_packet * UC_rtos_class::pop_gui_packets(int &bytes)
{
	bytes = m_gui_packet_ptr * sizeof(uc_gui_packet);
	m_gui_packet_ptr = 0;
	return m_gui_packet_list;
}

/**
 * \brief Get the current cpu. Only valid when running user code. Not for scheduling purposes
 *
 * \return Current cpu
 */
UC_cpu_class * UC_rtos_class::get_current_cpu(){
	return m_current_cpu;
}

/**
 * \brief Set the current cpu.
 *
 * \param cpu Current cpu
 */
void UC_rtos_class::set_current_cpu(UC_cpu_class *cpu){
	m_current_cpu = cpu;
}

/**
 * \brief Assign an additional schduling function. The original scheduler is executed only when this new scheduler returns NULL;
 *
 * \param additional_schedule new scheduling function
 */
void UC_rtos_class::set_additional_scheduler(UC_thread_class *(*additional_scheduler)(UC_rtos_class *, UC_cpu_class*)){
	m_additional_scheduler = additional_scheduler;
}

/**
 * \brief Assign a function to create an additional thread for interrupt management. It is called when the interruption shield function returns true
 *
 * \param additional_interruption_manager new interrupt handler
 */

void UC_rtos_class::set_additional_interrupt_manager(void * (*additional_interruption_manager)(void *)){
	m_additional_interruption_manager = additional_interruption_manager;
}

/**
 * \brief Assign a function to create an interrupt shield. It is called when an interruption arrives to decide if the function must be managed by the new manager or by the original one.
 *
 * \param additional_interruption_shield new interrupt shield function
 */

void UC_rtos_class::set_additional_interrupt_shield(bool (*additional_interruption_shield)(int , UC_rtos_class *)){
	m_additional_interruption_shield = additional_interruption_shield;
}

/**
 * \brief Assign a function to inform the additional scheduler that a thread has been destroyed.
 *
 * \param additional_destroy_thread additional destroy function
 */
void UC_rtos_class::set_additional_destroy_thread(int (*additional_destroy_thread)(UC_thread_class * ,UC_rtos_class *)){
	m_additional_destroy_thread = additional_destroy_thread;
}

/**
 * \brief Assign a function to create an interrupt shield. It is called when an interruption arrives to decide if the function must be managed by the new manager or by the original one.
 *
 * \param additional_change_status function to change the task status when there is an additional scheduler
 */
void UC_rtos_class::set_additional_change_status(int (*additional_change_status)(UC_thread_class *,UC_rtos_class *,	  UC_process_states, UC_process_states)){
	m_additional_change_status = additional_change_status;
}

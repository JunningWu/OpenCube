//==========================================================================
//	uc_rtos_class.h
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

#ifndef UC_RTOS_CLASS_H
#define UC_RTOS_CLASS_H

#include "systemc.h"
#include "rtos/kernel/uc_cpu_class.h"
#include "rtos/kernel/uc_task_scheduler_class.h"
#include "rtos/api/posix/uc_posix.h"
#include "rtos/drivers/uc_interrupt_info_class.h"
#include "rtos/kernel/uc_tic_timer_class.h"
#include "rtos/api/posix/uc_pthread_cond_class.h"
#include "rtos/api/posix/uc_pthread_mutex_class.h"
#include "rtos/api/posix/uc_mqueue.h"
#include "rtos/api/posix/uc_mqueue_class.h"
#include "rtos/api/posix/uc_semaphore.h"
#include "rtos/api/posix/uc_semaphore_class.h"
#include "rtos/kernel/uc_os_defines.h"
#include "rtos/drivers/uc_io.h"
#include "rtos/utils/uc_gui_interface.h"
#include "rtos/kernel/uc_statistics_class.h"



using namespace std;

class UC_cpu_class;
class UC_posix_class;
class UC_task_scheduler_class;
class UC_tic_timer_class;
class UC_interrupt_info_class;
class UC_process_class;
class uc_file;
class uc_linux_module;
class uc_module_init_class;
struct uc_driver_t;
struct uc_device_t;
struct uc_timer_list_t;
struct resource;
struct net_device;
class UC_microcos_class;

#define MAX_GUI_BUFFER_SIZE 1000

class UC_rtos_class {
public:
    UC_rtos_class(int num_cpus, const char* cpu_name = NULL);

    ~UC_rtos_class();

	int get_num_cpus();
	UC_cpu_class * get_processor(int cpu);
	UC_cpu_class * operator [](int cpu);
	int get_id();

	void rtos_debug();
	static void push_gui_packet(uc_gui_packet packet);
	static uc_gui_packet *pop_gui_packets(int &bytes);

	// Functions to include an additional system manager (e.g. RT Linux ext.)

	void set_additional_scheduler(UC_thread_class *(*additional_scheduler)(UC_rtos_class *, UC_cpu_class*));
	void set_additional_interrupt_manager(void * (*additional_interruption_manager)(void *));
	void set_additional_interrupt_shield(bool (*additional_interruption_shield)(int , UC_rtos_class *));
	void set_additional_destroy_thread(int (*m_additional_destroy_thread)(UC_thread_class * ,UC_rtos_class *));
	void set_additional_change_status(int (*m_additional_change_status)(UC_thread_class *,UC_rtos_class *, UC_process_states, UC_process_states));
//	void set_additional_check_domain(int (*m_additional_check_domain)(UC_thread_class * ,UC_rtos_class *));

	uc_pthread_cond_class *find_cond(int id);
	uc_pthread_mutex_class *find_mutex(int id);
	uc_mqd_class *find_mqd(int id);
	uc_mqd_params *find_mqd_params(int pid, uc_mqd_class *mqd);
	UC_process_class * get_process(pid_t pid);
	UC_thread_class * get_thread(pthread_t tid);
	uc_sem_class *find_sem(int id);

	uc_file* get_file(int id);
	uc_file* get_file(const char *name);
	void add_file(uc_file *file);
	int delete_file(uc_file *file);
	void assign_file_descriptor(uc_file *file);

	void kernel_module_init(uc_module_init_class module_init);
	void init_modules();

	void get_real_errno();

	void init_MicroCOS();

	UC_cpu_class * get_current_cpu();
	void set_current_cpu(UC_cpu_class *);
	void print_backtrace();

	vector<UC_process_class *> m_process_list; ///< List of process running on this rtos

	UC_posix_class *POSIX; ///< POSIX interface
	UC_microcos_class *MicroCOS; /// MicroCOS interface

	pthread_mutex_t m_rtos_mutex; ///< This is the mutex that blocks the simulation when a thread is running

	UC_task_scheduler_class *m_task_scheduler; ///< The task scheduler

	unsigned int m_last_pid; ///< The pid counter

	UC_interrupt_info_class *m_idt; ///< idt: interruption descriptor table

	UC_tic_timer_class *m_tic_timer; ///< The tic timer module

	int m_key_elements_counter;

	int m_cond_id_max;
	int m_mutex_id_max;
	int m_mqd_id_max;
	int m_sem_id_max;
	int m_last_pgrp;

	vector<uc_pthread_cond_class *> m_cond_list;
	vector<uc_pthread_mutex_class *> m_mutex_list;
	vector<uc_mqd_class *> Mqueue_List;
	vector<uc_sem_class *> Semaphores_List;
	vector<UC_process_class *> m_group_leaders;

	int OSIntNesting;
	bool MicroCOS_started;
	uc_pthread_mutex_t MicroCOS_started_mutex;

	uc_driver_t m_driver_list[MAX_DEV];
	vector<uc_driver_t *> m_miscdriver_list;
	vector<uc_device_t *> m_dev_list;
	vector<uc_timer_list_t *> m_timer_list_control;
	vector<resource *> port_resource_list;
	vector<resource *> mem_resource_list;
	vector<net_device *> m_netdevice_list;
	vector<uc_linux_module *> m_module_list;

	int m_os_errno;

	static int m_gui_fd; // file descriptor

	// Functions for scheduling extensions. Use set_additiona_... to set the functions. Example of use: Real-time extensions

	UC_thread_class *(*m_additional_scheduler)(UC_rtos_class *, UC_cpu_class*);
	void * (*m_additional_interruption_manager)(void *);
	bool (*m_additional_interruption_shield)(int , UC_rtos_class *);
	int (*m_additional_destroy_thread)(UC_thread_class * ,UC_rtos_class *);
	int (*m_additional_change_status)(UC_thread_class *,UC_rtos_class *,	  UC_process_states, UC_process_states);
//	int (*m_additional_check_domain)(UC_thread_class * ,UC_rtos_class *);

	UC_rtos_statistics_class *m_statistics;


void *m_memory_addr;

private:
	int m_num_cpus; ///< Number of cpus

	vector<UC_cpu_class *> m_cpu_list; ///< List of processors controlled by this rtos
	vector<uc_file *> m_file_list;

	int m_file_descriptor_max;

	static int static_rtos_id;
	int m_this_rtos_id;

	static uc_gui_packet m_gui_packet_list[MAX_GUI_BUFFER_SIZE];
	static unsigned int m_gui_packet_ptr;
	UC_cpu_class *m_current_cpu;

};

#endif

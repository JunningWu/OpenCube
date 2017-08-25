//==========================================================================
//	uc_ll_cache_bus_access.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: jue mar 13 2008
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

#include "rtos/low_level/uc_ll_cache_bus_access.h"
#include "rtos/kernel/uc_process_class.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/drivers/uc_hal_sw.h"
extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;
extern long long uc_segment_time;
extern long long uc_segment_instr;

/**
 * \brief Used to model the effect of instruction cache misses in the bus
 * \param size amount of information to transfer
 */
void uc_ll_icache_miss(int size) {
	int ret;

	// USER annotation
	if(qt_parent_rtos == NULL) return;
	UC_cpu_class * cpu = qt_parent_rtos->get_current_cpu();
	UC_thread_class *thread = cpu->m_current_task->m_current_thread;
	thread->annotate_user_time(uc_segment_time, uc_segment_instr);
	uc_segment_time = 0;
	uc_segment_instr = 0;
	cpu->m_icache->m_miss_transfers++;

	while (size > 0) {
		ret = send_to_memory(size);
		size -= ret;
	}
	if(thread->m_state != SUPER_USER && thread->m_state != USER) {
		printf("Error: thread provoking a miss in state = %d \n",thread->m_state);
		assert(thread->m_state == SUPER_USER);
	}
}

/**
 * \brief Used to model the effect of instruction cache misses in the bus
 * Used in the cpu_thread (SystemC) not during user code execution
 * \param size amount of information to transfer
 * \param addr address for the transfer
 */
void uc_ll_icache_miss(int size, void * addr) {
	int ret;

	// USER annotation
	UC_cpu_class * cpu = qt_parent_rtos->get_current_cpu();
	UC_thread_class *thread = cpu->m_current_task->m_current_thread;
	thread->annotate_user_time(uc_segment_time, uc_segment_instr);
	uc_segment_time = 0;
	uc_segment_instr = 0;
	cpu->m_icache->m_miss_transfers++;

	while (size > 0) {
		void* data = malloc(size);
		ret = icache_read(addr,data,size);
		free(data);
		size -= ret;
	}
	if(thread->m_state != SUPER_USER && thread->m_state != USER) {
		printf("Error: thread provoking a miss in state = %d \n",thread->m_state);
		assert(thread->m_state == SUPER_USER);
	}
}

/**
 * \brief Used to model the effect of data cache misses in the bus
 * Used in the cpu_thread (SystemC) not during user code execution
 * \param size amount of information to transfer
 * \param addr address for the transfer
 */
void uc_ll_dcache_miss(int size, void * addr) {
	int ret;

	// USER annotation
	UC_cpu_class * cpu = qt_parent_rtos->get_current_cpu();
	UC_thread_class *thread = cpu->m_current_task->m_current_thread;
	thread->annotate_user_time(uc_segment_time, uc_segment_instr);
	uc_segment_time = 0;
	uc_segment_instr = 0;
	cpu->m_dcache->m_miss_transfers++;

	while (size > 0) {
		void* data = malloc(size);
		ret = dcache_read(addr,data,size);
		free(data);
		size -= ret;
	}
	if(thread->m_state != SUPER_USER && thread->m_state != USER) {
		printf("Error: thread provoking a miss in state = %d \n",thread->m_state);
		assert(thread->m_state == SUPER_USER);
	}
}

/**
 * \brief Used to model the effect of data cache write in the bus
 * Used in the cpu_thread (SystemC) not during user code execution
 * \param size amount of information to transfer
 * \param addr address for the transfer
 */
void uc_ll_dcache_write(int size, void * addr) {
	int ret;

	// USER annotation
	UC_cpu_class * cpu = qt_parent_rtos->get_current_cpu();
	UC_thread_class *thread = cpu->m_current_task->m_current_thread;
	thread->annotate_user_time(uc_segment_time, uc_segment_instr);
	uc_segment_time = 0;
	uc_segment_instr = 0;
	cpu->m_dcache->m_miss_transfers++;

	while (size > 0) {
		void* data = malloc(size);
		ret = dcache_read(addr,data,size);
		free(data);
		size -= ret;
	}
	if(thread->m_state != SUPER_USER && thread->m_state != USER) {
		printf("Error: thread provoking a miss in state = %d \n",thread->m_state);
		assert(thread->m_state == SUPER_USER);
	}
}

/**
 * \brief Used to model the effect of data cache misses in the bus
 * Used in the cpu_thread (SystemC) not during user code execution
 * \param size amount of information to transfer
 */
void uc_ll_dcache_miss(int size) {
	int ret;

	// USER annotation
	UC_cpu_class * cpu = qt_parent_rtos->get_current_cpu();
	UC_thread_class *thread = cpu->m_current_task->m_current_thread;
	thread->annotate_user_time(uc_segment_time, uc_segment_instr);
	uc_segment_time = 0;
	uc_segment_instr = 0;
	cpu->m_dcache->m_miss_transfers++;

	while (size > 0) {
		ret = dcache_from_memory(size);
		size -= ret;
	}
	if(thread->m_state != SUPER_USER && thread->m_state != USER) {
		printf("Error: thread provoking a miss in state = %d \n",thread->m_state);
		assert(thread->m_state == SUPER_USER);
	}
}

/**
 * \brief Used to model the effect of data cache write in the bus
 * Used in the cpu_thread (SystemC) not during user code execution
 * \param size amount of information to transfer
 */
void uc_ll_dcache_write(int size) {
	int ret;

	// USER annotation
	UC_cpu_class * cpu = qt_parent_rtos->get_current_cpu();
	UC_thread_class *thread = cpu->m_current_task->m_current_thread;
	thread->annotate_user_time(uc_segment_time, uc_segment_instr);
	uc_segment_time = 0;
	uc_segment_instr = 0;
	cpu->m_dcache->m_miss_transfers++;

	while (size > 0) {
		ret = dcache_to_memory(size);
		size -= ret;
	}
	if(thread->m_state != SUPER_USER && thread->m_state != USER) {
		printf("Error: thread provoking a miss in state = %d \n",thread->m_state);
		assert(thread->m_state == SUPER_USER);
	}
}

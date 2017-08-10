//==========================================================================
//	uc_simulator_configuration.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: feb 2 2009
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

#include "rtos/utils/uc_simulator_configuration.h"
#include "rtos/kernel/uc_rtos_class.h"
#include "rtos/utils/uc_debug.h"
#include "rtos/utils/uc_backtrace.h"
#include "rtos/api/posix/uc_posix_c.h"

extern __thread UC_rtos_class *qt_parent_rtos;

#define USER_TO_SUPER_USER() \
	bool change_state = false; \
	UC_thread_class *thread = qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;\
	posix_user_to_superuser(thread, change_state);


uc_simulation_config_t uc_simulation_config;
static int simulation_config_initialized = 0;

uc_simulation_config_t::uc_simulation_config_t(){
	// IF rtos_list is != NULL means that has been already initialized. It hapens when inicializen in the executable and in a .so
	if(simulation_config_initialized != 0) return;
	simulation_config_initialized = 1;
	end_with_sw_destruction = 0;
	activate_backtrace = 0;
	rtos_list = new vector <UC_rtos_class*>();

	vcd_file = NULL;
	m_wait_period = 0;
	m_max_interval = 0;
	m_ic_granularity = 100;
	m_dc_granularity = 100;
	m_sigint = 0;

	uc_load_stop_sim_handler();
}
uc_simulation_config_t::~uc_simulation_config_t(){
	if(rtos_list != NULL){
/*		for(int i=0;i<(*rtos_list).size();i++){
			(*rtos_list)[i]->print_backtrace();
		}*/
		delete rtos_list;
		rtos_list = NULL;
	}
	if(vcd_file != NULL){	
		sc_close_vcd_trace_file(vcd_file);
		vcd_file = NULL;
	}
}

/**
 * \brief Get the data of the rtos id
 * \param id numbero of rtos to return
 * \return rtos info
 */
UC_rtos_class *uc_get_rtos_data(int id){
	for(int i=0; i<uc_simulation_config.rtos_list->size(); i++){
		if((*uc_simulation_config.rtos_list)[i]->get_id() == id) return (*uc_simulation_config.rtos_list)[i];
	}
	return NULL;
}


/**
 * \brief Check if there are no more active SW processes and the variable uc_end_with_sw_destruction is active the simulation is finished
 *
 * \return 0 if all SW processes has been destroyed, -1 otherwise
 */
int uc_check_end_with_sw_destruction(){
	for(int i=0; i<uc_simulation_config.rtos_list->size(); i++){
		for(int j=0; j<(*uc_simulation_config.rtos_list)[i]->m_statistics->m_processes.size(); j++){
			if( (*uc_simulation_config.rtos_list)[i]->m_statistics->m_processes[j]->end_time == -1 ){
				return -1;
			}
		}
	}
	if(uc_simulation_config.end_with_sw_destruction > 0){
		sc_stop();
	}
	return 0;
}

/**
 * \brief Update the value of the variable uc_end_with_sw_destruction
 * \param val new value for the variable
 * \return 0 
 */
int uc_activate_end_with_sw_destruction(int val){
	uc_simulation_config.end_with_sw_destruction = val;
	return 0;
}

/**
 * \brief Update the flag to indicate checking the system status coherence
 * \param activate Flag value: true activates checking, false disables checking
 */
void uc_set_check_system_status_flag(bool activate){
	uc_simulation_config.check_system_status = activate;
}

/**
 * \brief Get the flag to indicate checking the system status coherence
 * \return Flag value 
 */
bool uc_get_check_system_status_flag(){
	return uc_simulation_config.check_system_status;
}

/**
 * \brief Allows the user code to finish the simulation calling sc_stop
 */
void uc_user_sc_stop(){
	USER_TO_SUPER_USER();
	sc_stop();
}

/**
 * \brief Set the maximum interval a process can wait for a running time
 * \param time The time interval
 */
void uc_set_max_wait_interval(long long time){
	uc_simulation_config.m_max_interval = time;
}

/**
 * \brief Return the maximum interval a process can wait for a running time
 * \return Maximum interval time
 */
long long uc_get_max_wait_interval(){
	return uc_simulation_config.m_max_interval;
}

/**
 * \brief Set the maximum period for sw execution waits
 * \param time Time period
 */
void uc_set_sw_wait_period(long long time){
	uc_simulation_config.m_wait_period = time;
}

/**
 * \brief Return the maximum period for sw execution waits
 * \return Maximum period
 */
long long uc_get_sw_wait_period(){
	return uc_simulation_config.m_wait_period;
}

/**
 * \brief Set the maximum number of misses to accumulate in cache before per
 * \param time Time period
 */
void uc_set_ic_granularity(int num_lines){
	uc_simulation_config.m_ic_granularity = num_lines;
}
/**
 * \brief Return the maximum period for sw execution waits
 * \return Maximum period
 */
int uc_get_ic_granularity(){
	return uc_simulation_config.m_ic_granularity;
}

/**
 * \brief Set the maximum number of misses to accumulate in data cache before per
 * \param time Time period
 */
void uc_set_dc_granularity(int num_lines){
	uc_simulation_config.m_dc_granularity = num_lines;
}
/**
 * \brief Return the maximum period for data cache transfers
 * \return Maximum period
 */
int uc_get_dc_granularity(){
	return uc_simulation_config.m_dc_granularity;
}

//==========================================================================
//	uc_icache.cpp
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: jue ene 24 2008
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

/*!
 * \file uc_icache.cpp
 * \brief Methods for instruction cache model
 */

#include <iostream>
#include "rtos/kernel/uc_cpu_class.h"
#include "rtos/kernel/uc_thread_class.h"
#include "rtos/kernel/uc_icache.h"
#include "rtos/low_level/uc_ll_cache_bus_access.h"
#include "rtos/utils/uc_simulator_configuration.h"

using namespace std;
extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;


/*!
 * \fn UC_icache_class::UC_icache_class(int size, int assoc, int line_size)
 * \brief Contructor of instruction cache class
 * \param size The total size of cache in bytes
 * \param assoc The degree of associativity (lines per set)
 * \param line_size The line size in instructions (words)
 * \param instr_size The instruction size in bytes
 */
UC_icache_class::UC_icache_class(int size, int assoc, int line_size, int instr_size, UC_cpu_class *cpu) {
	m_enable = true;
	m_annotate = 0;
	m_data = 0;
	m_size = size;
	m_assoc = assoc;
	m_line_size = line_size;
	m_instr_size = instr_size;
	m_set_size = m_line_size * m_assoc;
	m_num_lines = m_size / (m_line_size * m_instr_size);
	m_cpu = cpu;

	// To model icache with 0 size
	if(m_num_lines == 0) m_num_sets = 1;
	else m_num_sets = m_num_lines / m_assoc;


	m_victim_line = (int *)malloc (sizeof(int)*m_num_sets);
	m_histogram = (int *)malloc (sizeof(int)*m_num_sets);
	// m_data is initialized in initialize_icache_size to simplify size changes

	initialize_icache_size(size);
	
/*	m_num_lines = m_size / (m_line_size * m_instr_size);
	m_num_sets = m_num_lines / m_assoc;
	m_set_size = m_line_size * m_assoc;
	m_total_misses = 0;
	for (int i=0 ; i<m_num_sets ; i++) {
		m_victim_line[i] = 0;
		for (int j=0 ; j<m_assoc ; j++) {
			m_data[i][j] = NULL;
		}
	}
	for (int i=0 ; i<m_line_size ; i++) {
		m_histogram[i] = 0;
	}
	initialize_icache(size);*/
}

/*!
 * \fn UC_icache_class::initialize_icache(int size)
 * \brief Function to initialize the cache. Used by the constructor.
 * Can be used to modify cache size before starting the simulation.
 *
 * \param size The total size of cache in bytes
 */
void UC_icache_class::initialize_icache_size(int size){
	int i, j;
	m_enable = true;
	m_annotate = 0;
	m_total_misses = 0;
	m_miss_transfers = 0;
	m_last_annotation = 0;

	// delete m_data lines if they has been previously created
	// they will be created regarding to the new cache size
	if(m_data){
		for (i=0 ; i<m_num_sets ; i++) {
			delete m_data[i];
		}
	}
	else{
		m_data = (uc_icache_line_t ***)malloc (sizeof(uc_icache_line_t **)*m_num_sets);
	}


	m_size = size;
	m_num_lines = m_size / (m_line_size * m_instr_size);

	// To model icache with 0 size. Avoid division by 0
	if(m_num_lines == 0) m_assoc = 64;
	else  m_assoc = m_num_lines / m_num_sets;
	

	for ( i=0 ; i<m_num_sets ; i++) {
		m_data[i] = (uc_icache_line_t **)malloc (sizeof(uc_icache_line_t *)*m_assoc);	//Reserva el espacio para el set i de la cache

		m_victim_line[i] = 0;	//pone que la siguiente linea a sustituir sea la 0
		for ( j=0 ; j<m_assoc ; j++) {
			m_data[i][j] = NULL;	//Vacia todos los huecos del set
		}
	}

	for ( i=0 ; i<m_num_sets ; i++) {
		m_histogram[i] = 0;	//vacia el histograma pero no se para que sirve el histograma
	}
	
	for (i=0 ; i<8 ; i++) {	//uno por cada set (con saber el tag del primero de la linea sabemos el de toda la linea)
		for (j=0 ; j<64 ; j++) {	//uno por cada linea del set
			m_tags[i][j] = -1;	//vacia los tag
		}
	}

}

/*!
 * \fn UC_icache_class::~UC_icache_class()
 * \brief Destructor of instruction cache class
 */
UC_icache_class::~UC_icache_class() {
}


/*!
 * \fn UC_icache_class::insert_line(uc_icache_line_t *line)
 * \brief Inserts a line into cache, replacing the victim if necessary
 */
void UC_icache_class::insert_line(uc_icache_line_t *line) {
	short num_set = line->num_set;	//numero del set al que va el dato
	uc_icache_line_t *victim = m_data[num_set][m_victim_line[num_set]];
	if (victim != NULL) {
		victim->hit = 0;
	}
	m_data[num_set][m_victim_line[num_set]] = line;
	line->hit = 1;
	m_victim_line[num_set] = (m_victim_line[num_set] + 1) % m_assoc;
	m_total_misses++;
	m_annotate++;
	//bus transfer:
	if (uc_get_ic_granularity() > 0 && m_annotate >= uc_get_ic_granularity()) {
		sc_time start_time = sc_time_stamp();
		uc_ll_icache_miss(m_line_size * m_instr_size * m_annotate/*,line->address*/);
		m_annotate = 0;
		m_last_annotation = start_time.value()/1000;
	}
}


/**
 * \fn UC_icache_class::search_tag(int tag, int iStart, int iEnd)
 * \brief Implements the search technique
 */
void UC_icache_class::search_tag(int tag, int iStart, int iEnd) {
	int i, j, victim;
	for (i = iStart ; i <= iEnd ; i++) {
		for (j=0 ; j<64 ; j++) {
			if (m_tags[i][j] == tag) {
				break;
			}
		}
		if (j == 64) {
			//Cache miss
			m_total_misses++;
			victim = m_victim_line[i];
			m_tags[i][victim] = tag;
			m_victim_line[i] = (m_victim_line[i] + 1) % 64;
		}
	}
}


/*!
 * \fn UC_icache_class::flush(void)
 * \brief Flush the instruction cache
 */
void UC_icache_class::flush(void) {
	int i, j;
	for (i=0 ; i<m_num_sets ; i++) {
		for (j=0 ; j<m_assoc ; j++) {
			if (m_data[i][j] != NULL) {
				m_data[i][j]->hit = 0;
				m_data[i][j] = NULL;
			}
		}
	}
}


void UC_icache_class::miss_time(unsigned long long time_ns) {
	if (m_cpu->m_current_clock != 0) {
		m_stall_cycles += (unsigned long long)((time_ns*m_cpu->m_current_clock) / 1e9);
	}
	else {
		m_stall_cycles += (unsigned long long)((time_ns*m_cpu->m_parsed_clock) / 1e9);
	}
}

/*!
 * \fn UC_icache_class::sync_misses(UC_cpu_class* cpu, UC_thread_class *thread)
 * \brief Send the misses accumulated to the bus
 * This function is called by the cpu execution loop, not by user code
 * Adds transfer information to the transfer stack, maintaining the previous information
 * 
 * \param cpu Processor associated to the cache
 * \param thread Current thread
 */
void UC_icache_class::sync_misses(UC_cpu_class* cpu, UC_thread_class *thread){

	if(m_annotate == 0) return;

	// Models the misses
	sc_time start_time = sc_time_stamp();
	uc_ll_icache_miss(m_line_size * m_instr_size * m_annotate);
	m_annotate = 0;
	m_last_annotation = start_time.value()/1000;

}

/*!
 * \fn UC_icache_class::sync_misses(UC_cpu_class* cpu, UC_thread_class *thread)
 * \brief Send the misses accumulated to the bus
 * This function is called by the cpu execution loop, not by user code
 * Forze performing bus transfers for pending misses. 
 * 
 * \param cpu Processor associated to the cache
 * \param thread Current thread
 * \param misses Number of misses to be performed and eliminted from the list
 */
void UC_icache_class::sync_misses(UC_cpu_class* cpu, UC_thread_class *thread, int misses){

	if(m_annotate == 0) return;
	if(misses == 0) return;

	// Models the misses
	if(m_annotate<misses) misses = m_annotate;
	sc_time start_time = sc_time_stamp();
	uc_ll_icache_miss(m_line_size * m_instr_size * misses);
	m_annotate -= misses;
	m_last_annotation = start_time.value()/1000;

}



/*!
 * \fn UC_icache_class::mark_start_segment(UC_cpu_class* cpu, UC_thread_class *thread)
 * \brief Update timming information when user segment starts
 * This function is called by the cpu execution loop, not by user code
 * If no previous misses are stored, updates the m_last_annotation time with the current time
 * 
 * \param cpu Processor associated to the cache
 * \param thread Current thread
 */
void UC_icache_class::mark_start_segment(UC_cpu_class* cpu, UC_thread_class *thread){
	if(m_annotate == 0)
		m_last_annotation = sc_time_stamp().value()/1000;
}

/****************************************************************************/
/*                   External functions to the cache object                 */
/****************************************************************************/

void ic_search_tag(int tag, int iStart, int iEnd) {
	UC_cpu_class *cpu;
	UC_icache_class *icache;
	//Get current cache and insert the block:
	cpu = qt_parent_rtos->get_current_cpu();
	icache = cpu->m_icache;
	icache->search_tag(tag, iStart, iEnd);
}


/*!
 * \fn ic_insert_line(uc_icache_line_t *line)
 * \brief Select the corresponding cache and inserts the line
 * This function is called directly by user code
 */
void ic_insert_line(uc_icache_line_t *line) {
	UC_cpu_class *cpu;
	UC_icache_class *icache;
	//Get current cache and insert the block:
	cpu = qt_parent_rtos->get_current_cpu();
	icache = cpu->m_icache;
	icache->insert_line(line);
}


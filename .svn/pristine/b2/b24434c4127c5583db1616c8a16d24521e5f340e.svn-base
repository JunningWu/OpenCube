//==========================================================================
//	uc_icache.h
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
 * \file uc_icache.h
 * \brief Instruction cache model
 */
#ifndef UC_ICACHE_H
#define UC_ICACHE_H

#include "rtos/kernel/uc_icache_line.h"
//#define IC_GRANULARITY 100

class UC_cpu_class;
class UC_thread_class;

/*!
 * \class UC_icache_class
 * \brief Class to model the instruction cache
 */
class UC_icache_class {
public:
	UC_icache_class(int size, int assoc, int line_size, int instr_size, UC_cpu_class *cpu);
	~UC_icache_class();

	void initialize_icache_size(int size);
	void insert_line(uc_icache_line_t *line);
	void search_tag(int tag, int iStart, int iEnd);
	void miss_time(unsigned long long time_ns);
	void flush(void);

	void sync_misses(UC_cpu_class* cpu, UC_thread_class *thread);
	void sync_misses(UC_cpu_class* cpu, UC_thread_class *thread, int misses);

	void mark_start_segment(UC_cpu_class* cpu, UC_thread_class *thread);
	int get_miss_size(){ return m_line_size*m_instr_size;} //!< Return the size of a miss (in bytes)
	int get_burst_size(){ return m_line_size;} //!< Return the size of a single burst (in words)
	
	unsigned long long m_total_misses;
	unsigned long long m_stall_cycles;

	UC_cpu_class *m_cpu;		//!< Processor where the cache is

	int m_annotate;			//!< Number of misses accumulated before bus annotation (for speed improvement)
	unsigned long long m_last_annotation;	//!< Time of the last bus annotation (for speed improvement)
	int m_line_size;		//!< Size of single cache line (in words) (parameter)
	int m_instr_size;		//!< The instruction size (in bytes)

	int m_miss_transfers;

private:
	//Common characteristics:
	int m_size;			//!< Total size of cache (in bytes) (parameter)
	int m_assoc;			//!< Degree of associativity (parameter)
	int m_num_lines;		//!< Total number of lines
	int m_num_sets;			//!< Total number of sets
	int m_set_size;			//!< Set size (in instructions)
	bool m_enable;			//!< If cache is enabled
	int *m_victim_line;		//!< The next victim line for each set
	int *m_histogram;		//!< The histogram of line sizes
	uc_icache_line_t ***m_data;	//!< The cache model. A matrix of line pointers
	int m_tags[8][64];		//!< Tags for Rosenstiel method


};


#endif

//==========================================================================
//	uc_dcache.h
//	Author: Luis Díaz
//	Date: feb 24 2010
//	Description:
//==========================================================================
//  For more information about SCOPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================

/*!
 * \file uc_dcache.h
 * \brief Data cache model
 */
#ifndef UC_DCACHE_H
#define UC_DCACHE_H

#include "rtos/kernel/uc_dcache_line.h"
//#define IC_GRANULARITY 100

class UC_cpu_class;
class UC_thread_class;

/*!
 * \class UC_dcache_class
 * \brief Class to model the data cache
 */
class UC_dcache_class {
public:
	UC_dcache_class(int size, int assoc, int line_size, int instr_size, UC_cpu_class *cpu);
	~UC_dcache_class();

	void initialize_dcache_size(int size);
// 	void read_access(int address);
// 	void write_access(int address);

	void miss_time(unsigned long long time_ns);
	void flush(void);

	void dc_insert_line(int address);

	void sync_misses(UC_cpu_class* cpu, UC_thread_class *thread);
	void sync_misses(UC_cpu_class* cpu, UC_thread_class *thread, int misses);

	void mark_start_segment(UC_cpu_class* cpu, UC_thread_class *thread);
	int get_miss_size(){ return m_line_size*m_instr_size;} //!< Return the size of a miss (in bytes)
	int get_burst_size(){ return m_line_size;} //!< Return the size of a single burst (in words)
	
	unsigned long long m_total_misses;
	unsigned long long m_stall_cycles;

	UC_cpu_class *m_cpu;		//!< Processor where the cache is

	int m_annotate;			//!< Number of misses accumulated before bus annotation (for speed improvement)
	int m_write_annotate;			//!< Number of misses accumulated before bus annotation (for speed improvement)
	unsigned long long m_last_annotation;	//!< Time of the last bus annotation (for speed improvement)
	int m_line_size;		//!< Size of single cache line (in words) (parameter)
	int m_instr_size;		//!< The instruction size (in bytes)

	int m_miss_transfers;

	int m_dcache_misses;		//!< Total dcache misses
	int m_dcache_rempl;		//!< Total dcache reemplaces with bit_dirty=1
	char *m_memory;			//!< Information for all the memory map (2exp(32-7))

private:
	//Common characteristics:
	int m_size;			//!< Total size of cache (in bytes) (parameter)
	int m_assoc;			//!< Degree of associativity (parameter)
	int m_num_lines;		//!< Total number of lines
	int m_num_sets;			//!< Total number of sets
	int m_set_size;			//!< Set size (in instructions)
	bool m_enable;			//!< If cache is enabled
	int m_dcache_pos;		//!< posicion en la que esta el dato en noestro modelo de cache. Solo valido si esta.
	int **dcache_data;		//!< Mi modelo de cache
	int *dcache_pos;

	int **dcache_order;		//!< Para llevar el orden en que han sido metidos los datos en Dcache para hacer luego los reemplazos.
};


#endif

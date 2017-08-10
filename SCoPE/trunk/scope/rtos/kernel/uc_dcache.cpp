//==========================================================================
//	uc_dcache.cpp
//	Author: Luis Díaz
//	Date: feb 24 2010
//	Description:
//==========================================================================
//  For more information about SCOPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================

//Caracteristicas de la DCACHE
//Politica de reemplazo FIFO el dato que lleva mas tiempo en cache es el que saco
//Al llevar a mp una linea modificada lleva toda la línea, no solo el dato cambiado (hay un bit dirty por línea no por dato)


/*!
 * \file uc_dcache.cpp
 * \brief Methods for data cache model
 */

#include <iostream>
#include "rtos/kernel/uc_cpu_class.h"
#include "rtos/kernel/uc_thread_class.h"
#include "rtos/kernel/uc_dcache.h"
#include "rtos/low_level/uc_ll_cache_bus_access.h"
#include "rtos/utils/uc_simulator_configuration.h"

using namespace std;
extern __thread UC_process_class *qt_parent_process;
extern __thread UC_rtos_class *qt_parent_rtos;


char *uc_dcache_memory;

/*!
 * \fn UC_dcache_class::UC_dcache_class(int size, int assoc, int line_size)
 * \brief Contructor of instruction cache class
 * \param size The total size of cache in bytes
 * \param assoc The degree of associativity (lines per set)
 * \param line_size The line size in instructions (words)
 * \param instr_size The instruction size in bytes
 */
UC_dcache_class::UC_dcache_class(int size, int assoc, int line_size, int instr_size, UC_cpu_class *cpu) {
	int i;
	m_enable = true;
	m_annotate = 0;
	m_write_annotate = 0;
	m_size = size;
	m_assoc = assoc;
	m_line_size = line_size;
	m_instr_size = instr_size;
	m_set_size = m_line_size * m_assoc;
	m_num_lines = m_size / (m_line_size * m_instr_size);
	m_cpu = cpu;
	m_memory = (char*) calloc(33554432,1);

	// To model dcache with 0 size
	if(m_num_lines == 0) m_num_sets = 1;
	else m_num_sets = m_num_lines / m_assoc;

	dcache_order=(int**) calloc (sizeof (int*) * m_num_sets,1);
	for(i=0;i<m_num_sets;i++){
		dcache_order[i]=(int*) calloc (sizeof (int) * m_set_size,1);
	}

	initialize_dcache_size(size);
}

/*!
 * \fn UC_dcache_class::initialize_dcache_size(int size)
 * \brief Function to initialize the dcache. Used by the constructor.
 * Can be used to modify cache size before starting the simulation.
 * \param size The total size of cache in bytes
 */
void UC_dcache_class::initialize_dcache_size(int size){
	int i;
	m_enable = true;
	m_annotate = 0;
	m_total_misses = 0;
	m_miss_transfers = 0;
	m_last_annotation = 0;
	m_dcache_misses=0;
	m_dcache_rempl=0;

	dcache_pos=(int*) calloc (sizeof (int) * m_num_sets,1);	//int dcache_pos[m_num_sets];
}

/*!
 * \fn UC_dcache_class::~UC_dcache_class()
 * \brief Destructor of instruction cache class
 */
UC_dcache_class::~UC_dcache_class() {
}


#define WORD_SIZE 	4
#define WORD_SIZE_LOG2 	2

#define LINE_SIZE 	8*WORD_SIZE
#define LINE_SIZE_LOG2	3+WORD_SIZE_LOG2

// Number of lines info stored in a char: 4 lines (bit dirty,bit in_cache)
#define NUM_LINES_FOR_IN_DMEM 		4 
#define NUM_LINES_FOR_IN_DMEM_LOG2 	2
#define MASK_FOR_IN_DMEM_LOG2 		0x3

#define TOTAL_LOG2_FOR_IN_DMEM 		NUM_LINES_FOR_IN_DMEM_LOG2+LINE_SIZE_LOG2

#define DMEM_IN_ARRAY(addr)		m_memory[((unsigned)addr)>>TOTAL_LOG2_FOR_IN_DMEM]

// Defines merged to minimize computational effort
// #define ADDR_NUM_IN_DMEM(addr)	(addr>>LINE_SIZE_LOG2) & MASK_FOR_IN_DMEM_LOG2
// #define BIT_NUM_IN_DMEM(addr)	ADDR_NUM_IN_DMEM(addr)<<1
// #define DIRTY_BIT_NUM_IN_DMEM(addr)	(ADDR_NUM_IN_DMEM(addr)<<1) | 1
#define BIT_NUM_IN_DMEM(addr)		((addr>>(LINE_SIZE_LOG2-1)) & (MASK_FOR_IN_DMEM_LOG2<<1))
#define DIRTY_BIT_NUM_IN_DMEM(addr)	((addr>>(LINE_SIZE_LOG2-1)) & (MASK_FOR_IN_DMEM_LOG2<<1) | 1)

#define BIT_IN_DMEM(addr)		((DMEM_IN_ARRAY(addr)) & (1 << BIT_NUM_IN_DMEM(addr)))
#define DIRTY_BIT_IN_DMEM(addr)		((DMEM_IN_ARRAY(addr)) & ((1 << DIRTY_BIT_NUM_IN_DMEM(addr))))

#define SET_BIT_IN_DMEM(addr)		((DMEM_IN_ARRAY(addr)) |= (1 << BIT_NUM_IN_DMEM(addr)))
#define SET_DIRTY_BIT_IN_DMEM(addr)	((DMEM_IN_ARRAY(addr)) |= (1 << DIRTY_BIT_NUM_IN_DMEM(addr)))

#define CLEAN_BIT_IN_DMEM(addr)		((DMEM_IN_ARRAY(addr)) &= ~(1 << BIT_NUM_IN_DMEM(addr)))
#define CLEAN_DIRTY_BIT_IN_DMEM(addr)	((DMEM_IN_ARRAY(addr)) &= ~(1 << DIRTY_BIT_NUM_IN_DMEM(addr)))

	//LUMY: Este comentario grande es para la nueva dcache
/**
 * \fn UC_dcache_class::dc_insert_line(int address_aux)
 * \brief Implements the search cache technique
 */
void UC_dcache_class::dc_insert_line(int address) {
	int set_num, addressaux, i;

	addressaux=address/(m_line_size*m_instr_size);
	set_num=addressaux%m_num_sets;	//set en el que tiene que ir el dato

	if(BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]])){//si esta
		if(DIRTY_BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]])){//y esta sucio 
			for(i=0;i<m_num_sets;i++){
				dcache_order[i]=(int*) calloc (sizeof (int) * m_set_size,1);
			}
			m_write_annotate ++;
			//TODO:LUMY:Hay que llevar la linea a mp (sumar el tiempo de llevarlo) Habria que implementar el write buffer
			if (uc_get_dc_granularity() > 0 && m_write_annotate >= uc_get_dc_granularity()) {
				sc_time start_time = sc_time_stamp();
				uc_ll_dcache_write( m_instr_size * m_write_annotate,(void*)address);
				m_write_annotate = 0;
				m_last_annotation = start_time.value()/1000;
			}
			m_dcache_rempl++;
		}
		CLEAN_BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]]);
		CLEAN_DIRTY_BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]]);
	}
	dcache_order[set_num][dcache_pos[set_num]]=address;
	dcache_pos[set_num]++;
	if (dcache_pos[set_num]==64){
		dcache_pos[set_num]=0;
	}
// 	uc_ll_cache_read(m_line_size * m_instr_size, m_cpu);	//TODO: LUMY No se xq falla mirar con Hector
	m_dcache_misses++;
	m_annotate++;

	if (uc_get_dc_granularity() > 0 && m_annotate >= uc_get_dc_granularity()) {
		sc_time start_time = sc_time_stamp();
		uc_ll_dcache_miss(m_line_size * m_instr_size * m_annotate,(void*)address);
		m_annotate = 0;
		m_last_annotation = start_time.value()/1000;
	}
	SET_BIT_IN_DMEM(address);
}

/**
 * \fn UC_dcache_class::read_access(int address)
 * \brief 
 */

/*void UC_dcache_class::read_access(int address) {
	int set_num, addressaux;
	//if(m_memory[((unsigned int) address)>>7]&(1<<((((unsigned int)address)>>4)&6))==0){
	if((BIT_IN_DMEM(address))== 0){//si no esta en la cache
		addressaux=address/(m_line_size*m_instr_size);
		set_num=addressaux%m_num_sets;	//set en el que tiene que ir el dato
		if(BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]])){//si esta
			if(DIRTY_BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]])){//y esta sucio 
				//TODO:LUMY:Hay que llevar la linea a mp (sumar el tiempo de llevarlo) Habria que implementar el write buffer
 				uc_ll_dcache_write(m_line_size * m_instr_size, (void*)dcache_order[set_num][dcache_pos[set_num]]);
				m_dcache_rempl++;
				m_write_annotate ++;
			}
			CLEAN_BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]]);
			CLEAN_DIRTY_BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]]);
		}
		dcache_order[set_num][dcache_pos[set_num]]=address;
		uc_ll_dcache_miss(m_line_size * m_instr_size, (void*)address);
		m_annotate++;
		dcache_pos[set_num]++;
		if (dcache_pos[set_num]==64){
			dcache_pos[set_num]=0;
		}
// 		uc_ll_cache_read(m_line_size * m_instr_size, m_cpu);	//TODO: LUMY No se xq falla mirar con Hector
		m_dcache_misses++;
		SET_BIT_IN_DMEM(address);
	}else{
	}
}*/

/**
 * \fn UC_dcache_class::write_access(int address)
 * \brief 
 */
/*void UC_dcache_class::write_access(int address) {
	int set_num, addressaux;
	if((BIT_IN_DMEM(address))== 0){
		addressaux=address/(m_line_size*m_instr_size);
		set_num=addressaux%m_num_sets;	//set en el que tiene que ir el dato
		if(BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]])){//si esta
			if(DIRTY_BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]])){//y esta sucio 
				//TODO:LUMY:Hay que llevar la linea a mp (sumar el tiempo de llevarlo) Habria que implementar el write buffer
//				uc_ll_dcache_write(m_line_size * m_instr_size, (void*) dcache_order[set_num][dcache_pos[set_num]]);
//				m_write_annotate ++;
//				m_dcache_rempl++;
			}
			CLEAN_BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]]);
			CLEAN_DIRTY_BIT_IN_DMEM(dcache_order[set_num][dcache_pos[set_num]]);
		}
		dcache_order[set_num][dcache_pos[set_num]]=address;
		uc_ll_dcache_miss(m_line_size * m_instr_size,(void*) address);
		m_annotate++;
		dcache_pos[set_num]++;
		if (dcache_pos[set_num]==64){
			dcache_pos[set_num]=0;
		}
// 		uc_ll_cache_read(m_line_size * m_instr_size, m_cpu);	//TODO: LUMY No se xq falla mirar con Hector
		m_dcache_misses++;
		SET_BIT_IN_DMEM(address);
	}
	//m_memory[((unsigned int)address)>>7]|=1<<(((((unsigned int)address)>>4)&6)|1);
	SET_DIRTY_BIT_IN_DMEM(address);
}*/

/****************************************************************************/
/*                   External functions to the cache object                 */
/****************************************************************************/

/*void dcache_read_access(void *address) {
	int dcache_hit;
	UC_cpu_class *cpu;
	UC_dcache_class *dcache;
	cpu = qt_parent_rtos->get_current_cpu();
	dcache = cpu->m_dcache;
	dcache->read_access((int)address);
}

void dcache_write_access(void *address) {
// 	int dcache_hit;
// 	UC_cpu_class *cpu;
// 	UC_dcache_class *dcache;
// 	cpu = qt_parent_rtos->get_current_cpu();
// 	dcache = cpu->m_dcache;
// 	dcache->write_access((int)address);
}*/

int dc_insert_line(void *address){	//LUMY: Este comentario grande es para la nueva dcache
	UC_cpu_class *cpu;
	UC_dcache_class *dcache;
	cpu = qt_parent_rtos->get_current_cpu();
	dcache = cpu->m_dcache;
	dcache->dc_insert_line((int)address);

}


/*!
 * \fn UC_dcache_class::sync_misses(UC_cpu_class* cpu, UC_thread_class *thread)
 * \brief Send the misses accumulated to the bus
 * This function is called by the cpu execution loop, not by user code
 * Adds transfer information to the transfer stack, maintaining the previous information
 * 
 * \param cpu Processor associated to the cache
 * \param thread Current thread
 */
void UC_dcache_class::sync_misses(UC_cpu_class* cpu, UC_thread_class *thread){

	if(m_annotate == 0 && m_write_annotate == 0) return;

	// Models the misses
	sc_time start_time = sc_time_stamp();
	if(m_write_annotate > 0 ){
		uc_ll_dcache_write(m_line_size * m_instr_size * m_write_annotate);
		m_write_annotate = 0;
	}
	if(m_annotate > 0 ){
		uc_ll_dcache_miss(m_line_size * m_instr_size * m_annotate);
		m_annotate = 0;
	}
	m_last_annotation = start_time.value()/1000;

}

/*!
 * \fn UC_dcache_class::sync_misses(UC_cpu_class* cpu, UC_thread_class *thread)
 * \brief Send the misses accumulated to the bus
 * This function is called by the cpu execution loop, not by user code
 * Forze performing bus transfers for pending misses. 
 * 
 * \param cpu Processor associated to the cache
 * \param thread Current thread
 * \param misses Number of misses to be performed and eliminted from the list
 */
void UC_dcache_class::sync_misses(UC_cpu_class* cpu, UC_thread_class *thread, int misses){
	int w_misses = misses;
	misses = 0;

	if(m_annotate == 0 && m_write_annotate == 0) return;
	if(misses == 0) return;

	// Models the misses
	if(m_write_annotate<w_misses){
		misses = w_misses-m_write_annotate;
		w_misses = m_write_annotate;
	}
	sc_time start_time = sc_time_stamp();
	uc_ll_dcache_miss(m_line_size * m_instr_size * misses);
	m_write_annotate -= w_misses;
	uc_ll_dcache_write(m_line_size * m_instr_size * misses);
	m_annotate -= misses;
	m_last_annotation = start_time.value()/1000;

}



/*!
 * \fn UC_dcache_class::mark_start_segment(UC_cpu_class* cpu, UC_thread_class *thread)
 * \brief Update timming information when user segment starts
 * This function is called by the cpu execution loop, not by user code
 * If no previous misses are stored, updates the m_last_annotation time with the current time
 * 
 * \param cpu Processor associated to the cache
 * \param thread Current thread
 */
void UC_dcache_class::mark_start_segment(UC_cpu_class* cpu, UC_thread_class *thread){
	if(m_annotate == 0 && m_write_annotate == 0)
		m_last_annotation = sc_time_stamp().value()/1000;
}

/*!
 * \fn UC_dcache_class::flush(void)
 * \brief Flush the instruction cache
 */
void UC_dcache_class::flush(void) {
	int i, j;

	// Write back disabled 
	//write_back();
	sync_misses(m_cpu,NULL);

	memset(m_memory,0,33554432);
	for(i=0;i<m_num_sets;i++){
		memset(dcache_order[i],0,sizeof (int) * m_set_size);
	}
	memset(dcache_pos,0,(sizeof (int)) * m_num_sets);	//int dcache_pos[m_num_sets];

}

void UC_dcache_class::miss_time(unsigned long long time_ns) {
	if (m_cpu->m_current_clock != 0) {
		m_stall_cycles += (unsigned long long)((time_ns*m_cpu->m_current_clock) / 1e9);
	}
	else {
		m_stall_cycles += (unsigned long long)((time_ns*m_cpu->m_parsed_clock) / 1e9);
	}
}

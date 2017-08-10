//==========================================================================
//
// Author(s): Hector Posadas
// Contributors:
//
// Modifications:
//
//==========================================================================
//
//  The following code is derived, directly or indirectly, from XML plug-in for SCoPE,
//  released June 30, 2008. Copyright (C) 2008 Univeristy of Cantabria(UC)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 2 of the License, or
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
//  For more information about SCoPE and the XML plug-in for SCoPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  the authors or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//
//==========================================================================


#include "uc_scope_gprof.h"
#include "rtos/kernel/uc_thread_class.h"
#include "rtos/kernel/uc_cpu_class.h"
#include "uc_time_annotation.h"

//#include "linux/time.h"
#include <cxxabi.h>

#ifndef CLOCK_REALTIME_HR
#define CLOCK_REALTIME_HR 0
#endif

extern __thread UC_rtos_class *qt_parent_rtos;
extern unsigned long long uc_segment_time;



uc_gprof_info_c uc_gprof_info;
struct timespec uc_timespec_substract(const struct timespec val1, const struct timespec val2){
	struct timespec new_val;
	new_val.tv_nsec = val1.tv_nsec - val2.tv_nsec;
	new_val.tv_sec = val1.tv_sec - val2.tv_sec;
	if( new_val.tv_nsec < 0 ){
		new_val.tv_sec--;
		new_val.tv_nsec += (int)1e9;
	}
	return new_val;
}
struct timespec uc_timespec_add(const struct timespec val1, const struct timespec val2){
	struct timespec new_val;
	new_val.tv_nsec = val1.tv_nsec + val2.tv_nsec;
	new_val.tv_sec = val1.tv_sec + val2.tv_sec;
	if( new_val.tv_nsec >= 1e9 ){
		new_val.tv_sec++;
		new_val.tv_nsec -= (int)1e9;
	}
	return new_val;
}

/** According to gcc documentation: called upon function enter */
void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
	std::vector<struct function_call_info*> *func_list;
	struct function_call_info* info;
	struct timespec segment_time;

	func_list = (*uc_gprof_info.thread_list)[uc_get_current_thread()->m_tid];

	if(func_list == NULL){
		func_list = new std::vector<struct function_call_info*>;
		(*uc_gprof_info.thread_list)[uc_get_current_thread()->m_tid] = func_list;
	}
	
	info = new struct function_call_info;
	if(clock_gettime(CLOCK_REALTIME_HR, &(info->m_host_time))){
		perror("Clock_gettime error ");
	}
// 	qt_parent_rtos->POSIX->uc_clock_gettime(CLOCK_REALTIME, &(info->m_estimated_time));
// 	segment_time.tv_sec = curr_sc_time / (long)1e9;
// 	segment_time.tv_nsec = curr_sc_time - segment_time.tv_sec * (long)1e9;
// 	info->m_estimated_time = uc_timespec_add(info->m_estimated_time, segment_time);

	unsigned long long curr_sc_time = uc_segment_time + uc_get_sc_time();
	info->m_estimated_time.tv_sec = curr_sc_time / (long)1e9;
	info->m_estimated_time.tv_nsec = curr_sc_time - info->m_estimated_time.tv_sec * (long)1e9;

	func_list->push_back(info);

}


/** According to gcc documentation: called upon function exit */
void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
	Dl_info info;
	std::vector<struct function_call_info*> *func_list;
	struct function_call_info* call_info;
	struct timespec new_time;
	struct timespec segment_time;
	int stat;

	func_list = (*uc_gprof_info.thread_list)[uc_get_current_thread()->m_tid];
	call_info = (*func_list)[func_list->size()-1];

	dladdr(this_fn, &info);
	clock_gettime(CLOCK_REALTIME_HR, &new_time);
	call_info->m_host_time = uc_timespec_substract(new_time, call_info->m_host_time);

// 	qt_parent_rtos->POSIX->uc_clock_gettime(CLOCK_REALTIME, &new_time);
// 
// 	if(new_time.tv_sec < call_info->m_estimated_time.tv_sec){
// 		printf("new_time < call_info\n");
// 		assert(0);
// 	}
// 	if(uc_segment_time < 0 || uc_segment_time > 1e12){
// 		printf("uc_segment_time error\n");
// 		assert(0);
// 	}
// 
// 	segment_time.tv_sec = uc_segment_time / (long)1e9;
// 	segment_time.tv_nsec = uc_segment_time - segment_time.tv_sec * (long)1e9;
// 	new_time = uc_timespec_add(new_time, segment_time);

	unsigned long long curr_sc_time = uc_segment_time + uc_get_sc_time();
	new_time.tv_sec = curr_sc_time / (long)1e9;
	new_time.tv_nsec = curr_sc_time - new_time.tv_sec * (long)1e9;

	call_info->m_estimated_time = uc_timespec_substract(new_time, call_info->m_estimated_time);

	assert(call_info->m_estimated_time.tv_sec >= 0);

 	fprintf(uc_gprof_info.output,"thread %d, func: %s, host_time: %u.%09d, estimated_time: %u.%09d \n", uc_get_current_thread()->m_tid, abi::__cxa_demangle(info.dli_sname,0,0,&stat), call_info->m_host_time.tv_sec, call_info->m_host_time.tv_nsec, call_info->m_estimated_time.tv_sec, call_info->m_estimated_time.tv_nsec);

	delete call_info;

	func_list->pop_back();

}


//==========================================================================
//
// Author(s): Hector Posadas, Juan Castillo, David Quijano
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


#ifndef UC_SCOPE_GPROF_H
#define UC_SCOPE_GPROF_H

#include <vector>
#include "time.h"
#include "stdio.h"
#include <dlfcn.h>

extern "C" void __attribute__((__no_instrument_function__))
__cyg_profile_func_enter(void *this_fn, void *call_site);

extern "C" void __attribute__((__no_instrument_function__))
__cyg_profile_func_exit(void *this_fn, void *call_site);

struct function_call_info{
	struct timespec m_host_time;
	struct timespec m_estimated_time;
};


class uc_gprof_info_c{
	public:

	FILE *output;

	std::vector<std::vector<struct function_call_info*> *> 	*thread_list;

	uc_gprof_info_c(){
		output = fopen("gmon.scope", "w");
		thread_list = new std::vector<std::vector<struct function_call_info*> *>(1000, (std::vector<struct function_call_info*> *)NULL);
	}
	uc_gprof_info_c(char *str){
		output = fopen(str, "w");
		thread_list = new std::vector<std::vector<struct function_call_info*> *>(1000, (std::vector<struct function_call_info*> *)NULL);
	}

	~uc_gprof_info_c(){
		if(output != NULL){
			fclose(output);
			output = NULL;
		}
		if( thread_list != NULL ){
			delete  thread_list;
			thread_list = NULL;
		}
	}

};

#endif

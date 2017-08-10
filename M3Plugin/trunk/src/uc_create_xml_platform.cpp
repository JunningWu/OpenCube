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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <ctype.h>
#include <vector>

#include "uc_load_xml.h"
#include "uc_create_xml_platform.h"
#include "xml_input.h"
#include "xml_if.h"
#include "xml_list.h"
#include "sc_scope.h"
#include "uc_unistd.h"

#include "uc_hal_sw.h"
#include "io_access/io_access.h"

#ifdef SMPSim
#include "hal/SMPSim.h"
#endif

#ifdef NO_XML_PLATFORM
/**
 * \brief Return the pointer to the current thread
 * \return Current thread pointer
 */
UC_thread_class* uc_get_current_thread(){
	return qt_parent_rtos->get_current_cpu()->m_current_task->m_current_thread;
}
#endif

extern char *executable_name;
extern long long uc_segment_time;

struct _xml_intermediate_info{
	void *function;
	int argc;
	char **argv;
	int priority;
	int policy;
	float optimization;
};

std::vector<UC_TLM_bus_class*> system_buses;

/**
 * \brief Obtain the pointer to a function from a dynamic library, knowing the function name and the file name. The function has to be 'extern "C"'
 *
 * \param file Name of the dynamic library where the function is
 * \param function Name of the function
 * \return The function pointer
 */
void *get_dynamic_function(char *file, char *function){
	void *handler = NULL;
	void *function_p;
	char *chr;
	int error_level = 0; // 1: incompatible a
	if(function == NULL) {
		printf("Error: functionality not assigned \n");
		return NULL;
	}
	if(file != NULL) {
		handler = dlopen(file, RTLD_NOW | RTLD_GLOBAL ) ;
		if(handler == NULL){
			perror("dlopen:");
			printf("Fatal error: opening library %s for function %s \n",file,function);
			char buffer[200];
			sprintf(buffer,"/lib/ld-linux.so.* %s",file);
			FILE *outp = popen (buffer,"r");
			while(fgets(buffer,200,outp)){
				printf("%s",buffer);
			}
			pclose(outp);
			printf("-- Use dltest or ldd for more information --\n");
			assert(0);
		}
	}
	if(handler == NULL){
		handler = dlopen(NULL, RTLD_NOW | RTLD_GLOBAL ) ;
	}
	if(handler == NULL) {
		printf("Error: fail loading static file symbols %s \n",dlerror());
//		exit(-1);
	}
	function_p = dlsym(handler, function) ;
	if(function_p == NULL) {
		char str[100] ;
		sprintf(str,"_Z%d%siPPc",strlen(function),function);
		function_p = dlsym(handler, str);
	}	
	if(function_p == NULL) {
		char str[100] ;
		sprintf(str,"_Z%d%siPPKc",strlen(function),function);
		function_p = dlsym(handler, str);
	}	
	if(function_p == NULL) {
		char str[100] ;
		error_level = 1;
		sprintf(str,"_Z%d%sv",strlen(function),function);
		function_p = dlsym(handler, str);
	}	
	if(function_p == NULL) {
		char str[100] ;
		error_level = 1;
		sprintf(str,"_Z%d%sPv",strlen(function),function);
		function_p = dlsym(handler, str);
		
		xml_scope_simulation_info.m_warnings++;
	}	

	if(function_p == NULL) {
		printf("Warning: unsafe loading of static function %s \n",function,dlerror());
//		exit(-1);
		error_level = 2;

		char str[1000];
		char str2[100];
		char num[100];
		int sec;
		char line[1000], addr[1000], type[1000], scope[1000], flag[1000], name[1000];

		char strm[100] ;
		sprintf(strm,"_Z%d%s",strlen(function),function);

		strcpy(str,"readelf -s ");
		strcat(str, executable_name);
		strcat(str, " | grep ");
		strcat(str,function);

//		printf("executing %s\n",str);
		FILE *f = popen(str,"r");
		if(f == NULL){
			printf("Internal Error: popen \n");
			exit(-1);
		}
		while(fgets(str2,1000,f)){
			sscanf(str2,"%s %s %s %s %s %s %d %s", line, addr, num, type, scope, flag, &sec, name);
// 			printf("from %s \n", str2);
// 			printf("readed %s %s %s %s %s %s %d %s\n", line, addr, num, type, scope, flag, &sec, name);

			if(strcmp(name, function) == 0) {
				int num=strtol(addr,NULL, 16);
				printf("\tSuccessful search - function found: %s 0x%x\n",function,num);
				printf("\tTry linking with '-Wl,--export-dynamic' next time\n");
				xml_scope_simulation_info.m_warnings++;
				
				return (void*)num;
			}
			if((chr=strstr(name, strm)) != 0 && chr == 0) {
				int num=strtol(addr,NULL, 16);

				chr += strlen(strm);
				if(strcmp(chr,"iPPc") == 0 || strcmp(chr,"iPPKc") == 0  || strcmp(chr,"v") == 0 ){

					printf("\tSuccessful search - function found: %s 0x%x\n",function,num);
					printf("\tTry linking with '-Wl,--export-dynamic' and declaring the function as 'extern \"C\"'\n");
					xml_scope_simulation_info.m_warnings++;
				}
				else {
					xml_scope_simulation_info.m_warnings++;
					printf("Warning: Incompatible arguments. The functions to be executed as SW components use to be declared as func(int argc, char **argv)\n. Funcion %s doesn't \n",function);
				}
				
				return (void*)num;
			}
			if(strstr(name, function) != 0 && strcmp(type,"FUNC")==0) {
				int num=strtol(addr,NULL, 16);
				printf("Warning: using function %s instead of function: %s. Addr: 0x%x\n",name, function,num);
				printf("\tTry linking with '-Wl,--export-dynamic' and declaring the function as 'extern \"C\"'\n");
				xml_scope_simulation_info.m_warnings++;
				
				return (void*)num;
			}
/*			printf("Invalid name %s from %s\n", name, function);*/
		}
		fclose(f);
		error_level = 3;
		printf("Error when loading static function %s: Function not found\n",function);
		printf("\tTry declaring the function as 'extern \"C\"'\n");
// 		printf("Executed %s\n",str);
// 		f = popen(str,"r");
// 		while(fgets(str2,1000,f)){ printf("%s\n",str2);}

		exit(-1);
	}
	if(error_level ==1 ){
		printf("Warning when loading \"%s\": Function found with incompatible arguments.\n  Dynamic functions expected as func(int argc, char **argv).\n",function);		
	}
	return function_p;
}

/**
 * \brief Obtain the pointer to a function from a the executable itself, knowing the function name and the file name. Tries with the name in C and C++ for gcc 3.x-4.x
 *
 * \param function Name of the function
 * \return The function pointer
 */
void *get_static_function(char *function){
	return get_dynamic_function(NULL, function);
}

/**
 * \brief Load the information required to execute a function from the sw_allocation struct, where all are strings, to the _xml_intermediate_info, as function poineter, priority, policy, arguments, ...
 *
 * \param info Structure where the infomation is stored
 * \param alloc Structure from where the information is read
 * \return 
 */
void load_task_info(struct _xml_intermediate_info *info, struct sw_allocation *alloc){
	char str1[200], *str2, *c;
	vector<char*> list;
	int i;
	
	// Function
	if(alloc->task->library && strstr(alloc->task->library,".so")) 
		info->function = get_dynamic_function(alloc->task->library, alloc->task->function);
	else if(alloc->task->file && strstr(alloc->task->file,".so")) 
		info->function = get_dynamic_function(alloc->task->file, alloc->task->function);
	else 
		info->function = get_static_function(alloc->task->function);
	
	// arguments
	c = alloc->args;
	str1[0]='\0';
//	if(c){
		while (c && sscanf(c,"%s",str1)){
			str2 = (char*) malloc(strlen(str1)+1);
			strcpy (str2,str1);
			list.push_back(str2);
			c = strstr(c ,str1);
			if(c[strlen(str1)] == '\0') break;
			c = &c[strlen(str1)+1];
			str1[0]='\0';
		}
//	}
	info->argv = (char **)malloc(sizeof(char*)* list.size());
	for(i = 0; i<list.size(); i++ ){
		info->argv[i] = list[i];
	}
	info->argc = list.size();

	if(alloc->policy){
		if (strcmp(alloc->policy,"SCHED_FIFO") == 0 ) info->policy = SCHED_FIFO;
		else if (strcmp(alloc->policy,"SCHED_RR") == 0 ) info->policy = SCHED_RR;
		else if (strcmp(alloc->policy,"SCHED_OTHER") == 0 ) info->policy = SCHED_OTHER;
	//	else if (strcmp(alloc->policy,"SCHED_SS")) info->policy = SCHED_SS;
		else info->policy = xml_convert_int_number(alloc->policy);
	}
	else {
		 info->policy = 0;
	}

	info->priority = xml_convert_int_number(alloc->priority);
	if(alloc->task->optimization == NULL){
		info->optimization=1;
	}
	else info->optimization = strtof (alloc->task->optimization, NULL);
}

struct _xml_taskload_info{
	int priority;
	int policy;
	int data_size;
	unsigned long int compute_time;
	timespec period;
};

/**
 * \brief Create the data structure to create a task load from the xml structure containing xml strings
 *
 * \param info Structure where the infomation is stored
 * \param alloc Structure from where the information is read
 * \return 
 */
void load_taskload_info(struct _xml_taskload_info *info, struct sw_allocation *alloc){

	unsigned long time;

	if(alloc->policy){
		if (strcmp(alloc->policy,"SCHED_FIFO") == 0 ) info->policy = SCHED_FIFO;
		else if (strcmp(alloc->policy,"SCHED_RR") == 0 ) info->policy = SCHED_RR;
		else if (strcmp(alloc->policy,"SCHED_OTHER") == 0 ) info->policy = SCHED_OTHER;
	//	else if (strcmp(alloc->policy,"SCHED_SS")) info->policy = SCHED_SS;
		else info->policy = xml_convert_int_number(alloc->policy);
	}
	else {
		 info->policy = 0;
	}

	info->priority = xml_convert_int_number(alloc->priority);

	info->data_size = xml_convert_int_number(alloc->task->data_size);
	time = xml_convert_int_number(alloc->task->period);
	info->period.tv_sec = time / 1000000000;
	info->period.tv_nsec = time % 1000000000;
	info->compute_time = xml_convert_int_number(alloc->task->compute_time);

}

/**
 * \brief Intermediate function used to launch a SW task. Read the data struct and starts the function indicated in the function pointer.
 *
 * \param data Structure where the infomation is stored
 * \return Return value of the called function
 */
void *_xml_intermediate_function(void* data){
	struct _xml_intermediate_info *info;
	void *ret;
	info = (struct _xml_intermediate_info *)data;
// FIXME:

	if(info->policy != 0 || info->priority!= 0){
		struct uc_sched_param param;
		param.sched_priority = info->priority;
		uc_sched_setscheduler(uc_getpid(), info->policy, &param);
	}

	ret = ((void* (*)(int, char**))info->function)(info->argc, info->argv);

// FIXME: Delete internal components
	delete info;
	return ret;
}

/**
 * \brief Function executed to simulate a task load.
 *
 * \param data Structure where the taskload infomation is stored
 * \return NULL, used for compatibility
 */
void *_xml_taskload_function(void* data){
	int size, ret;
	struct _xml_taskload_info *info;
	info = (struct _xml_taskload_info *)data;

	if(info->policy != 0 || info->priority!= 0){
		struct uc_sched_param param;
		param.sched_priority = info->priority;
		uc_sched_setscheduler(uc_getpid(), info->policy, &param);
	}

	while (info->period.tv_nsec > 0 || info->period.tv_sec > 0){
		uc_segment_time = info->compute_time;
		size = info->data_size;
       		while (size > 0) {
                	ret = send_to_memory(size);
                	size -= ret;
        	}
		uc_nanosleep(&info->period, NULL);
	}

	delete info;
	return NULL;
}



/**
 * \brief Instanciate the SW components (OS) described in the XML System Description file
 *
 * \return 
 */

void instanciate_sw_components(){
	int i;
	struct list_elem * elem;
	struct list_elem * list;

	elem = get_sw_instance_header();
	while(elem){ 

		struct sw_instance * instance = (struct sw_instance *) elem->elem;
		if(strcasecmp(instance->component->type, "OS") == 0 ){
			UC_rtos_class *rtos;
			int number_processors = 0;
	
			if(instance->hw_group != NULL){
				number_processors = xml_list_size(&instance->hw_group->list);
				rtos = new UC_rtos_class(number_processors, ((struct hw_instance*)(instance->hw_group->list->elem))->component->class_name);
				list = instance->hw_group->list;
				for( i=0 ; i< number_processors; i++){
					((struct hw_instance*)(list->elem))->scope_data = (*rtos)[i];
					list = list->next;
				}
			}
			else if(instance->hw_resource != NULL){
#ifdef SMPSim
				if(instance->hw_resource->component->type == "SMP"){
					SMPSim *smp = new SMPSim(xml_convert_int_number(instance->hw_resource->component->type_specific_2), instance->hw_resource->component->class_name);
					rtos = smp->getRTOS();
					instance->hw_resource->scope_data=smp;
				}
				else{
#endif
					number_processors = 1;
					rtos = new UC_rtos_class(number_processors, 	instance->hw_resource->component->class_name);
					instance->hw_resource->scope_data = (*rtos)[0];
#ifdef SMPSim
				}
#endif
			}
			else {
				printf("Error: SW component %s has not hw resource associated\n", instance->name);
				exit(-1);
			}
	
	
			rtos_list.push_back(rtos);
			if(xml_scope_simulation_info.m_debug_level == 1){
				gui_connector->new_rtos(rtos);
			}

			if(instance->memory!= NULL){
				rtos->m_memory_addr=(void*)xml_convert_int_number(instance->memory);
			}

		}
		elem = elem->next;
	}

}

struct xml_component_info *get_component_parameters(struct hw_instance *instance){
	struct xml_component_info *info;
	info = (struct xml_component_info *)calloc(sizeof(struct xml_component_info),1);

	info->name = instance->name;
	info->category = instance->component->category;
	info->type = instance->type;
	
	info->mem_size = (int) GET_INSTANCE_PARAMETER(instance,mem_size);
	info->latency = GET_INSTANCE_PARAMETER_UNIT(instance,latency,1e-9);
	info->frequency = GET_INSTANCE_PARAMETER_UNIT(instance,freq,1e6);
	info->width = GET_INSTANCE_PARAMETER(instance,width);
	info->static_power = GET_INSTANCE_PARAMETER_UNIT(instance,static_power,1e-3);
	info->active_energy = GET_INSTANCE_PARAMETER_UNIT(instance,active_energy,1e-9);
	info->read_energy = GET_INSTANCE_PARAMETER_UNIT(instance,read_energy,1e-3);
	info->write_energy = GET_INSTANCE_PARAMETER_UNIT(instance,write_energy,1e-3);
	info->read_size_energy = GET_INSTANCE_PARAMETER_UNIT(instance,read_size_energy,1e-3);
	info->write_size_energy = GET_INSTANCE_PARAMETER_UNIT(instance,write_size_energy,1e-3);
	info->component_specific = (instance != NULL) ? instance->component_specific : instance->component->component_specific;

	return info;
}

/**
 * \brief Instanciate the HW components described in the XML System Description file: processors, buses, memories, ...
 *
 * \return
 */
void instanciate_hw_components(){
#ifndef NO_XML_PLATFORM
	struct list_elem * elem;
	struct hw_instance *instance;
	int i, j;
	int type = 0;
	elem = get_hw_instance_header();
	while(elem){ 
		instance = (struct hw_instance *) elem->elem;

//		printf("Instantiating component %s\n",instance->name);

		if(strcasecmp(instance->component->category, "network") == 0 ){
			int x,y;
			if(instance->type_specific_1)
				x = xml_convert_int_number(instance->type_specific_1);
			else if(instance->component->type_specific_1)
				x = xml_convert_int_number(instance->component->type_specific_1);
			else
				x = xml_list_size(&instance->contain) + xml_list_size(&instance->connections) ;
			if(instance->type_specific_2)
				y = xml_convert_int_number(instance->type_specific_2);
			else if(instance->component->type_specific_2)
				y = xml_convert_int_number(instance->component->type_specific_2);
			else
				y = (xml_list_size(&instance->contain) + xml_list_size(&instance->connections))/x ;

			int numbers[x][y][1];
			
			if(instance->component->class_name != NULL)
				if(strcasecmp(instance->component->class_name,"sicosys")==0) type = 1;

			UC_NoC_Interface *simulator = new UC_NoC_Interface("simulator", 1, type);
			for(i = 0; i<y; i++){
				for(j = 0; j<x; j++){
					numbers[j][i][0] = j+i*x;
				}
			}
			simulator->set_structure((int ***)numbers,x,y,1);
			if(simulator_list.size()>0) {
				printf("Error: Current version does not accept several networks\n");
				exit(-1);
			}
			simulator_list.push_back(simulator);
			instance->scope_data = (void*) simulator;
			if(xml_scope_simulation_info.m_debug_level == 1){
				gui_connector->new_noc(simulator);
			}
		}
		else if(strcasecmp(instance->component->category, "bus") == 0  ){
			float speed;
			int width, mem_start;
			struct list_elem * list_elem;
			struct hw_connection *connection;
			struct hw_instance *inst2;
			UC_TLM_bus_class *hal;

// 			if(instance->freq) speed = xml_convert_int_number(instance->freq)/**1000000*/;
// 			else if(instance->component->freq) speed = xml_convert_int_number(instance->component->freq)/**1000000*/;
//			else 
			if((speed=GET_INSTANCE_PARAMETER_UNIT(instance,freq,1e6))==0)
				speed = 100000000;
			

			if(instance->width) width = xml_convert_int_number(instance->width);
			else if(instance->component->width) width = xml_convert_int_number(instance->component->width);
			else width = 8;

			// convert from Hz to bytes/sec
			speed *= width/8;

			mem_start = 0;
			list_elem = instance->contain;
			while(list_elem){
				inst2 = (struct hw_instance *) list_elem->elem;

				if(strcmp(inst2->component->category, "memory") == 0  ){
					mem_start = xml_convert_int_number(inst2->start_addr);
				}
				list_elem = list_elem->next;
			}
			list_elem = instance->connections;
			while(list_elem){
				connection = (struct hw_connection *) list_elem->elem;
				if(strcmp(connection->instance->component->category, "memory") == 0  ){
					mem_start = xml_convert_int_number(inst2->start_addr);
				}
				list_elem = list_elem->next;
			}

			if(instance->component->class_name != NULL && 
			   strcasecmp(instance->component->class_name, "generic_bus") != 0 &&
 			   strcasecmp(instance->component->class_name, "generic") != 0 &&
			   strcasecmp(instance->component->class_name, "default") != 0){
				UC_TLM_bus_class *(*init_func)(const char*,float,int,struct xml_component_info*);
				struct xml_component_info *info;

				init_func = (UC_TLM_bus_class * (*)(const char*,float,int,struct xml_component_info*))get_dynamic_function(instance->component->library,instance->component->init_func);
	
				if ( init_func == NULL){
					printf("Fail when loading initialization function for instance %s\n",instance->name);
				}
				info = get_component_parameters(instance);
				hal = init_func(sc_gen_unique_name(instance->name), speed, mem_start, info);
				free(info);

			}else{

				hal = new UC_TLM_bus_class(sc_gen_unique_name(instance->name), speed, mem_start);
			}
			instance->scope_data = (void*) hal;
			system_buses.push_back(hal);
			if(xml_scope_simulation_info.m_debug_level == 1){
				gui_connector->new_hal(hal);
			}
		}
		elem = elem->next;
	}

	elem = get_hw_instance_header();
	while(elem){ 
		instance = (struct hw_instance *) elem->elem;
		if(strcasecmp(instance->component->category, "network") == 0 ){;}
		else if(strcasecmp(instance->component->category, "bus") == 0  ){;}
#ifdef SMPSim
		else if(strcasecmp(instance->component->category, "SMP") == 0  ){
			list_elem *cache ;
			for( cache = instance->contain; cache != NULL; cache= cache->next ){
				if( strcmp(((struct hw_instance *)cache->elem)->component->category, "icache") == 0 ){
					unsigned int mem_size=0;
					if(((struct hw_instance *)cache->elem)->mem_size != NULL){
						mem_size = xml_convert_int_number(((struct hw_instance *)cache->elem)->mem_size);
					}
					else if(((struct hw_instance *)cache->elem)->component->mem_size != NULL){
						mem_size = xml_convert_int_number(((struct hw_instance *)cache->elem)->component->mem_size);
					}
					UC_cpu_class* proc;
					int num_procc = xml_convert_int_number(instance->component->type_specific_2);
					if(xml_convert_int_number(instance->type_specific_2)>0){
						num_procc = xml_convert_int_number(instance->type_specific_2);
					}
					for(int i = 0;i<num_procc;i++){
						proc = ((SMPSim*)instance->scope_data)->getCPU(i);
						if(proc->m_icache != NULL){
							proc->m_icache->initialize_icache_size(mem_size);
						}
						else{
						// FIXME: Set the associativity degree != than 64
							proc->m_icache = new UC_icache_class(mem_size,64,8,4,proc);
						}
					}
					((struct hw_instance *)cache->elem)->scope_data = proc->m_icache;

				}
			}
		}
#endif
		else if(strcasecmp(instance->component->category, "processor") == 0  ){
			int bus_priority = GET_INSTANCE_PARAMETER_UNIT(instance,bus_priority,1); 
			((UC_cpu_class*)instance->scope_data)->m_current_clock =  GET_INSTANCE_PARAMETER_UNIT(instance,freq,1e6);
			((UC_cpu_class*)instance->scope_data)->set_bus_priority(bus_priority);
// 			if(instance->freq){
// 				((UC_cpu_class*)instance->scope_data)->m_current_clock = xml_convert_int_number(instance->freq);
// 			}
// 			else if(instance->component->freq){
// 				((UC_cpu_class*)instance->scope_data)->m_current_clock = xml_convert_int_number(instance->component->freq);
// 			}

			// Manage ICache size
			list_elem *cache ;
			for( cache = instance->contain; cache != NULL; cache= cache->next ){
				if( strcmp(((struct hw_instance *)cache->elem)->component->category, "icache") == 0 ){
					unsigned int mem_size=0;
					if(((struct hw_instance *)cache->elem)->mem_size != NULL){
						mem_size = xml_convert_int_number(((struct hw_instance *)cache->elem)->mem_size);
					}
					else if(((struct hw_instance *)cache->elem)->component->mem_size != NULL){
						mem_size = xml_convert_int_number(((struct hw_instance *)cache->elem)->component->mem_size);
					}
					if(((UC_cpu_class*)instance->scope_data)->m_icache != NULL){
						((UC_cpu_class*)instance->scope_data)->m_icache->initialize_icache_size(mem_size);
					}
					else{
						// FIXME: Set the associativity degree != than 64
						((UC_cpu_class*)instance->scope_data)->m_icache = new UC_icache_class(mem_size,64,8,4,(UC_cpu_class*)instance->scope_data);
					}
					((struct hw_instance *)cache->elem)->scope_data = ((UC_cpu_class*)instance->scope_data)->m_icache;

				}
			}
		}
		else if(strcasecmp(instance->component->category, "bridge") == 0  ){
			printf("Error: Current version does not accept bridges\n");
			exit(-1);
		}
//		else if(strcasecmp(instance->component->category, "as_hw") == 0  ){
		else if((instance->component->class_name != NULL && strcmp("generic",instance->component->class_name)!=0) || strcasecmp(instance->component->category, "as_hw") == 0){
			unsigned int latency, size;
			struct xml_component_info *info;
			if(instance->mem_size) size = xml_convert_int_number(instance->mem_size);
			else if(instance->component->mem_size) size = xml_convert_int_number(instance->component->mem_size);
			else size = 100;

			// FIXME: the parameter required is speed(frequency) of latency??
			void *(*init_func)(const char*,int,int,int,struct xml_component_info*);
			init_func = (void * (*)(const char*,int,int,int,struct xml_component_info*))get_dynamic_function(instance->component->library,instance->component->init_func);

			if ( init_func == NULL){
				printf("Fail when loading initialization function for instance %s\n",instance->name);
			}

			info = get_component_parameters(instance);

			instance->scope_data = init_func(sc_gen_unique_name(instance->name), xml_convert_int_number(instance->start_addr), ((unsigned)xml_convert_int_number(instance->start_addr)) + size -1, xml_convert_int_number(instance->irq),info);
			free(info);

			int burst_size = GET_INSTANCE_PARAMETER_UNIT(instance,burst_size,1);
			if(burst_size)((UC_hw_if*)instance->scope_data)->set_burst_size(burst_size);

			if(instance->start_addr != NULL){
				uc_initialize_peripheral_access((int*)xml_convert_int_number(instance->start_addr), size);
			}
		}
		else if(strcasecmp(instance->component->category, "net_if") == 0  ){
			unsigned int latency, size;
			double freq;
			if(instance->mem_size) size = xml_convert_int_number(instance->mem_size);
			else if(instance->component->mem_size) size = xml_convert_int_number(instance->component->mem_size);
			else size = 100;

			latency = GET_INSTANCE_PARAMETER_UNIT(instance,latency,1e-9);
			if(latency == 0){
				freq=GET_INSTANCE_PARAMETER_UNIT(instance,freq,1e6); 
				if(freq>0)latency = (int)(1e9/freq);// Memory expects latency in ns, not seconds
			}
// 			if(instance->latency) latency = xml_convert_int_number(instance->latency);
// 			else if(instance->component->latency) latency = xml_convert_int_number(instance->component->latency);
// 			else latency = 0;

			// FIXME: the parameter required is speed(frequency) of latency??
			UC_hw_NoC *eth = new UC_hw_NoC(sc_gen_unique_name(instance->name), xml_convert_int_number(instance->start_addr), ((unsigned)xml_convert_int_number(instance->start_addr)) + size -1, xml_convert_int_number(instance->irq), latency, simulator_list[0]);
			instance->scope_data = (void*) eth;

			int burst_size = GET_INSTANCE_PARAMETER_UNIT(instance,burst_size,1);
			if(burst_size)((UC_hw_if*)instance->scope_data)->set_burst_size(burst_size);

			uc_initialize_peripheral_access((int*)xml_convert_int_number(instance->start_addr), size);
		}
		else if(strcasecmp(instance->component->category, "memory") == 0  ){
			unsigned int latency, size;
			double freq;
			if(instance->mem_size) size = xml_convert_int_number(instance->mem_size);
			else if(instance->component->mem_size) size = xml_convert_int_number(instance->component->mem_size);
			else size = 100;

			latency = GET_INSTANCE_PARAMETER_UNIT(instance,latency,1e-9);
			if(latency == 0){
				freq=GET_INSTANCE_PARAMETER_UNIT(instance,freq,1e6); 
				if(freq>0)latency = (int)(1e9/freq);// Memory expects latency in ns, not seconds
			}
// 			if(instance->latency) latency = xml_convert_int_number(instance->latency);
// 			else if(instance->component->latency) latency = xml_convert_int_number(instance->component->latency);
// 			else latency = 0;

			// FIXME: the parameter required is speed(frequency) of latency??
			UC_hw_memory *mem = new UC_hw_memory(sc_gen_unique_name(instance->name), xml_convert_int_number(instance->start_addr), ((unsigned)xml_convert_int_number(instance->start_addr)) + size -1, latency);
			instance->scope_data = (void*) mem;

			int burst_size = GET_INSTANCE_PARAMETER_UNIT(instance,burst_size,1);
			if(burst_size)((UC_hw_if*)instance->scope_data)->set_burst_size(burst_size);

		}
		else if(strcasecmp(instance->component->category, "dma") == 0  ){
			unsigned int size;
			if(instance->mem_size) size = xml_convert_int_number(instance->mem_size);
			else if(instance->component->mem_size) size = xml_convert_int_number(instance->component->mem_size);
			else size = 100;
			UC_hw_dma *dma = new UC_hw_dma(sc_gen_unique_name(instance->name), xml_convert_int_number(instance->start_addr), ((unsigned )xml_convert_int_number(instance->start_addr)) + size -1);
			int bus_priority = GET_INSTANCE_PARAMETER_UNIT(instance,bus_priority,1); 
			dma->set_bus_priority(bus_priority);
			instance->scope_data = (void*) dma;

			uc_initialize_peripheral_access((int*)xml_convert_int_number(instance->start_addr), size);

			int burst_size = GET_INSTANCE_PARAMETER_UNIT(instance,burst_size,1);
			if(burst_size)((UC_hw_if*)instance->scope_data)->set_burst_size(burst_size);

		}
		else if(strcasecmp(instance->component->category, "icache") == 0  ){
			// Additional information added to processors
		}
		else if(strcasecmp(instance->component->category, "dcache") == 0  ){
			// Additional information cannot be added to processors
			printf("Error: Current version does not accept dcaches\n");
			exit(-1);
		}
		else {
			printf("Error: HW component type %s not recognized. Component: %s\n", instance->component->category,instance->name);
			exit(-1);
		}
		elem = elem->next;
	}
#endif
}

/**
 * \brief Connect the HW components of the system as described in the XML System Description file
 *
 * Current version only connections to connect the network interface to the network
 *
 */
void connect_hw_components(){
#ifndef NO_XML_PLATFORM
	struct list_elem * elem;
	struct list_elem * list_elem;
	int i=0;
	
	struct hw_instance *instance;
	struct hw_connection *connection;
	struct hw_instance *hw;


	elem = get_hw_instance_header();
	int master, index;
	while(elem){ 
		master = 0;
		index = 0;
		hw = (struct hw_instance *) elem->elem;
		if( hw->contain){ 
			list_elem = hw->contain;
			while(list_elem){
				instance = (struct hw_instance *) list_elem->elem;

//				printf("Connecting component %s in %s\n",instance->name,hw->name );

				if(strcasecmp(instance->component->category, "network") == 0  ){
					printf("Error: Current version does not accept multiple sub-network connection\n");
					exit(-1);
				}
				else if(strcasecmp(instance->component->category, "bus") == 0  ){
					printf("Error: Current version does not accept multiple sub-bus connection\n");
					exit(-1);
				}
				else if(strcasecmp(instance->component->category, "processor") == 0 ) {
					if(strcmp(hw->component->category, "bus")){
						printf("Error: Current version does not accept connecting processors to any component which is not a bus\n");
						exit(-1);
					}
					if(instance->scope_data == NULL){
						printf("Error: Processor creation. Plase check the processor has an OS associated\n");
						exit(-1);
					}
					((UC_cpu_class*)instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
				}
#ifdef SMPSim
				else if(strcasecmp(instance->component->category, "SMP") == 0 ) {
					if(strcmp(hw->component->category, "bus")){
						printf("Error: Current version does not accept connecting processors to any component which is not a bus\n");
						exit(-1);
					}
					if(instance->scope_data == NULL){
						printf("Error: Processor creation. Plase check the processor has an OS associated\n");
						exit(-1);
					}
					((SMPSim*)instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
				}
#endif
				else if(strcasecmp(instance->component->category, "net_if") == 0  ){
					if(!strcmp(hw->component->category, "bus")){
						((UC_TLM_bus_class*)hw->scope_data)->bind((UC_hw_if*)instance->scope_data);
					}
/*					else{
						printf("Error: Current version does not accept connecting a net_if to any component which is not a bus\n");
						exit(-1);
					}*/
//					((UC_master_base*)instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);

				}
				else if(strcasecmp(instance->component->category, "icache") == 0  ){
					// Added directly to processors
				}
				else if(strcasecmp(instance->component->category, "dma") == 0  ){
					if(instance->master!= NULL && strcasecmp(instance->master,"master")==0){
						if(instance->component->master!= NULL && strcasecmp(instance->component->master,"both")==0)
							((UC_master_slave_base*)instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
						else
							((UC_master_base*)instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
					}
					else if(instance->master!= NULL && strcasecmp(instance->master,"slave")==0){
						if(instance->component->master!= NULL && strcasecmp(instance->component->master,"both")==0)
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_master_slave_base*)instance->scope_data);
						else
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_hw_if*)instance->scope_data);
						
					}
					else {
						if(instance->component->master!= NULL && strcasecmp(instance->component->master,"both")==0){
							((UC_master_slave_base*)instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_master_slave_base*)instance->scope_data);
						}
						else if(instance->component->master!= NULL && strcasecmp(instance->component->master,"master")==0)
							((UC_master_base*)instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
						else
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_hw_if*)instance->scope_data);

					}

				}
				else if(instance->scope_data != NULL  ){

					if(instance->master!= NULL && strcasecmp(instance->master,"master")==0){
						if(instance->component->master!= NULL && strcasecmp(instance->component->master,"both")==0)
							((UC_master_slave_base*)instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
						else
							((UC_master_base*)instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
					}
					else if(instance->master!= NULL && strcasecmp(instance->master,"slave")==0){
						if(instance->component->master!= NULL && strcasecmp(instance->component->master,"both")==0)
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_master_slave_base*)instance->scope_data);
						else
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_hw_if*)instance->scope_data);
						
					}
					else {
						if(instance->component->master!= NULL && strcasecmp(instance->component->master,"both")==0){
							((UC_master_slave_base*)instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_master_slave_base*)instance->scope_data);
						}
						else if(instance->component->master!= NULL && strcasecmp(instance->component->master,"master")==0)
							((UC_master_base*)instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
						else
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_hw_if*)instance->scope_data);
					}

				}
/*				else if(strcasecmp(instance->component->category, "memory") == 0  ){
					((UC_TLM_bus_class*)hw->scope_data)->bind((UC_hw_if*)instance->scope_data);
				}
				else if(strcasecmp(instance->component->category, "bridge") == 0  ){
					printf("Error: Current version does not accept bridges\n");
					exit(-1);
				}
				else if(strcasecmp(instance->component->category, "dcache") == 0  ){
					printf("Error: Current version does not accept dcaches\n");
					exit(-1);
				}
				else if(strcasecmp(instance->component->category, "as_hw") == 0  ){
					((UC_TLM_bus_class*)hw->scope_data)->bind((UC_hw_if*)instance->scope_data);
				}*/
				else {
					printf("Error: Fail when initializing a HW instance. Instance: %s\n", instance->component->category, instance->name);
					exit(-1);
				}
				list_elem = list_elem->next;
			}
		}

		
		if(strcmp(hw->component->category, "bus")==0){ 
			list_elem = hw->connections;
			while(list_elem){
				connection = (struct hw_connection *) list_elem->elem;

//				printf("Connecting link %s in %s\n",connection->name,hw->name );

				if(strcmp(connection->instance->component->category, "net_if") == 0  ){
					if(connection->master == NULL){
						((UC_TLM_bus_class*)hw->scope_data)->bind((UC_master_slave_base*)connection->instance->scope_data);
					}
					else if(strcasecmp(connection->master,"master")==0){
						((UC_master_base*)connection->instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
					}
					else if(strcasecmp(connection->master,"both")==0){
						((UC_master_slave_base*)connection->instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
						((UC_TLM_bus_class*)hw->scope_data)->bind((UC_master_slave_base*)connection->instance->scope_data);
					}
					else {
						((UC_TLM_bus_class*)hw->scope_data)->bind((UC_master_slave_base*)connection->instance->scope_data);
					}
				}
				else if(strcmp(connection->instance->component->category, "dma") == 0  ){
					if(connection->master!= NULL && strcasecmp(connection->master,"master")==0){
						if(connection->instance->component->master!= NULL && strcasecmp(connection->instance->component->master,"both")==0)
							((UC_master_slave_base*)connection->instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
						else
							((UC_master_base*)connection->instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
					}
					else if(connection->master!= NULL && strcasecmp(connection->master,"slave")==0){
						if(connection->instance->component->master!= NULL && strcasecmp(connection->instance->component->master,"both")==0)
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_master_slave_base*)connection->instance->scope_data);
						else
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_hw_if*)connection->instance->scope_data);
						
					}
					else {
						if(connection->instance->component->master!= NULL && strcasecmp(connection->instance->component->master,"both")==0){
							((UC_master_slave_base*)connection->instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_master_slave_base*)connection->instance->scope_data);
						}
						else if(connection->instance->component->master!= NULL && strcasecmp(connection->instance->component->master,"master")==0)
							((UC_master_base*)connection->instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
						else
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_hw_if*)connection->instance->scope_data);
					}
				}
				else if(strcasecmp(instance->component->category, "bridge") == 0  ){
					printf("Error: Current version does not accept bridges\n");
					exit(-1);
				}
				else if(instance->scope_data != NULL  ){
					if(connection->master!= NULL && strcasecmp(connection->master,"master")==0){
						if(connection->instance->component->master!= NULL && strcasecmp(connection->instance->component->master,"both")==0)
							((UC_master_slave_base*)connection->instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
						else
							((UC_master_base*)connection->instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
					}
					else if(connection->master!= NULL && strcasecmp(connection->master,"slave")==0){
						if(connection->instance->component->master!= NULL && strcasecmp(connection->instance->component->master,"both")==0)
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_master_slave_base*)connection->instance->scope_data);
						else
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_hw_if*)connection->instance->scope_data);
						
					}
					else {
						if(connection->instance->component->master!= NULL && strcasecmp(connection->instance->component->master,"both")==0){
							((UC_master_slave_base*)connection->instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_master_slave_base*)connection->instance->scope_data);
						}
						else if(connection->instance->component->master!= NULL && strcasecmp(connection->instance->component->master,"master")==0)
							((UC_master_base*)connection->instance->scope_data)->bind((UC_TLM_bus_class*)hw->scope_data);
						else
							((UC_TLM_bus_class*)hw->scope_data)->bind((UC_hw_if*)connection->instance->scope_data);
					}
				}
				else {
					printf("Error: HW component type %s cannot be liked using a CONECTION rule \n", instance->component->category);
					exit(-1);
				}
				list_elem = list_elem->next;

			}

		}
		elem = elem->next;
	}	

	for(i=0;i<system_buses.size();i++){
		system_buses[i]->generate_memory_map();
	}
#endif
}


/**
 * \brief Function executed to instanciate a SW task. If the task is a SW task or a taskload, a process is created in the corresponding thread. It if is a driver, it is adequately load
 *
 * \param 
 * \return
 */
void allocate_sw_tasks(){
	struct list_elem * elem;
	struct sw_allocation *alloc;
	struct _xml_intermediate_info *info;
	struct _xml_taskload_info *load_info;
	UC_cpu_class *cpu;
	elem = get_sw_allocation_header();
	while(elem){
		alloc = (struct sw_allocation *)elem->elem;
		if(alloc->hw_resource != NULL) {
			cpu = (UC_cpu_class *)alloc->hw_resource->scope_data;
		}
		else if(alloc->hw_group != NULL){
			 cpu = (UC_cpu_class *)((struct hw_instance*) alloc->hw_group->list->elem)->scope_data;
		}
		else if(alloc->sw_resource != NULL){
			if(alloc->sw_resource->hw_resource){
				cpu = (UC_cpu_class *)alloc->sw_resource->hw_resource->scope_data;
			}
			else {
				cpu = (UC_cpu_class *)((struct hw_instance*) alloc->sw_resource->hw_group->list->elem)->scope_data;
			}
		}

		if(strcasecmp(alloc->task->type,"SW") == 0){
			info = (struct _xml_intermediate_info *)malloc(sizeof(struct _xml_intermediate_info));
			load_task_info(info, alloc);
			cpu->new_process(_xml_intermediate_function, info, alloc->name, info->optimization);
		}
		else if(strcasecmp(alloc->task->type,"module") == 0 || strcasecmp(alloc->task->type,"driver") == 0){
			info = (struct _xml_intermediate_info *)malloc(sizeof(struct _xml_intermediate_info));
			load_task_info(info, alloc);
		        cpu->m_parent_rtos->kernel_module_init(((uc_module_init_class (*)())info->function)());
		}
		else if(strcasecmp(alloc->task->type,"taskload") == 0 || strcasecmp(alloc->task->type,"load") == 0){
			load_info = (struct _xml_taskload_info *)malloc(sizeof(struct _xml_taskload_info));
			load_taskload_info(load_info, alloc);
		        cpu->new_process(_xml_taskload_function, load_info);
		}

		elem = elem->next;
	}
}

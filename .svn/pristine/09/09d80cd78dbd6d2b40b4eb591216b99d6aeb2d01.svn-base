//==========================================================================
//
// Author(s): Hector Posadas
// Contributors:
// Description:   Header file
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
#ifndef _XML_IF_H_
#define _XML_IF_H_

#include "xml_component_info.h"

/* All structures start with: "name", "path", ["type"]*/

extern int xml_info_level;

double xml_convert_number(const char* str, float default_unit=1);
unsigned int xml_convert_int_number(const char* str, float default_unit=1);

#define GET_INSTANCE_PARAMETER(instance, param) (instance->param != NULL) ? xml_convert_number(instance->param) :  xml_convert_number(instance->component->param)
#define GET_INSTANCE_PARAMETER_UNIT(instance, param, unit) (instance->param != NULL) ? xml_convert_number(instance->param,unit) :  xml_convert_number(instance->component->param,unit)

struct hw_component{
	char *name;
	char *path;
	char *category;
	char *class_name;
	char *activation_type;
	char *freq;
	char *mem_size;
	char *type_specific_1;
	char *type_specific_2;
	char *area;
	char *static_power;
	char *active_energy;
	char *read_energy;
	char *read_size_energy;
	char *write_energy;
	char *write_size_energy;
	char *latency;
	char *width;
	char *init_func;
	char *library;
	char *component_specific;
	char *bus_priority;
	char *burst_size;
	char *master;
};

struct hw_instance{
	char *name;
	char *path;
	char *type;
	char *activation_type;
	char *class_name;
	char *latency;
	char *freq;
	char *mem_size;
	char *type_specific_1;
	char *type_specific_2;
	char *start_addr;
	char *irq;
	char *local_id;
	char *area;
	char *static_power;
	char *active_energy;
	char *read_energy;
	char *read_size_energy;
	char *write_energy;
	char *write_size_energy;
	char *width;
	char *bus_priority;
	char *burst_size;
	char *master;

	char *component_name;

	struct hw_component *component;
	int offset;
	struct list_elem * contain;
	struct list_elem * connections;
	void *scope_data;
	char *component_specific;
/*	char *rec_irq;
	char *port;
*/
};

struct hw_connection{
	char *name;
	char *path;
	char *activation_type;
	char *speed;
	char *latency;
	char *mem_size;
	char *type_specific_1;
	char *type_specific_2;
	char *start_addr;
	char *port;
	char *irq;
	char *rec_irq;
	char *local_id;
	char *master;

	char *instance_name;

	struct hw_instance *instance;
	int offset;
};

struct hw_group{
	char *name;
	char *path;
	struct list_elem * hw_names; // list of char *

	struct list_elem * list;
};

struct sw_component{
	char *name;
	char *path;
	char *type;
};

struct sw_instance{
	char *name;
	char *path;
	char *type;
	char *component_name;
	char *resource_name;
	char *memory;

	struct sw_component* component;
	struct hw_instance* hw_resource;
	struct hw_group* hw_group;
	int offset;
	void *scope_data;
};

struct sw_task{
	char *name;
	char *path;
	char *type;
	char *function;
	char *file;
	char *optimization;
	char *class_name;
	char *compute_time;
	char *period;
	char *data_size;
	char *init_time;
	char *fin_time;
	char *library;
};

struct sw_allocation{
	char *name;
	char *path;
	char *priority;
	char *policy;
	char *args;

	char *task_name;
	char *os_name;
	char *resource_name;

	struct sw_task* task;
	struct hw_instance* hw_resource;
	struct hw_group*    hw_group;
	struct sw_instance* sw_resource;
	int offset;
	void *scope_data;
};


int strcasecmp_null(const char *str1, const char *str2);
unsigned long int atoi_null(const char *str);

void xml_close_declare(char *path, const char *name);
int xml_declare_function(char *path, const char *name, const char **atts);


void *xml_load_hw_component(char *path, const char **atts);
void *xml_load_hw_instance(char *path,  const char **atts);
void *xml_load_hw_connection(char *path, const char **atts);
void *xml_load_hw_group(char *path, const char **atts);
void *xml_load_sw_component(char *path, const char **atts);
void *xml_load_sw_instance(char *path, const char **atts);
void *xml_load_exec_component(char *path, const char **atts);
void *xml_load_exec_instance(char *path, const char **atts);

void *xml_add_to_hw_group(char *path, const char **atts);
void xml_add_process_argument(char *path, const char **attr);

void xml_hw_component_string_convert(struct hw_component * component);
void xml_hw_instance_string_convert(struct hw_instance * instance);
void xml_hw_connection_string_convert(struct hw_connection * connection);
void xml_hw_group_string_convert(struct hw_group * group);
void xml_sw_instance_string_convert(struct sw_instance * instance);
void xml_sw_task_string_convert(struct sw_task * task);
void xml_sw_allocation_string_convert(struct sw_allocation * allocation);

void xml_connect_hw_instance(struct hw_instance * instance);
void xml_connect_hw_connection(struct hw_connection * connection);
void xml_connect_hw_group(struct hw_group * group);
void xml_connect_sw_instance(struct sw_instance* instance);
int xml_connect_sw_allocation(struct sw_allocation * allocation);

double xml_convert_number(const char* str, float default_unit);

#endif

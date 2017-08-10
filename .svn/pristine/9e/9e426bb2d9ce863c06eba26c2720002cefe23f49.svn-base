//==========================================================================
//
// Author(s): Hector Posadas
// Contributors:
// Description:   Decode the XML clauses from a XML System Description file
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "xml_if.h"
#include "xml_input.h"
#include "xml_list.h"
#include "xml_hierarchy.h"


/**
 * \brief Compares two strings checking if any is NULL.
 *
 * \param str1 String to be compared
 * \param str2 String to be compared
 * \return 1 if equal, 0 otherwise.
 */
int strcasecmp_null(const char *str1, const char *str2){
	if(str1 == NULL || str2 == NULL) return 0;
	return !strcasecmp(str1, str2);
}


/**
 * \brief Obtain a integer value from a string, checking if the string is null
 *
 * \param str The string to be converted
 * \return the value, or 0
 */
unsigned long int atoi_null(const char *str){
	if(str == NULL) return 0;
	return strtoul(str, NULL, 0);
}

/**
 * \brief Check the type of clause, and call the corresponding function to decode the arguments
 *
 * \param path Clause path
 * \param name Clause name
 * \param atts Clause attributes
 * \return 0 if ok, otherwise error
 */
int xml_declare_function(char *path, const char *name, const char **atts){

	if(strcasecmp_null(name,"Description"))	{
		return xml_chech_instance(0, atts);
	}
	else if(strcasecmp_null(name,"HW_Platform"))	{
		return xml_chech_instance(1, atts);
	}
	else if(strcasecmp_null(name,"HW_Components"))	{
		return xml_chech_instance(2, atts);
	}
	else if(strcasecmp_null(name,"HW_Component"))	{
		xml_load_hw_component(path, atts);
	}
	else if(strcasecmp_null(name,"HW_Architecture")){
		return xml_chech_instance(3, atts);
	}
	else if(strcasecmp_null(name,"HW_Instance"))	{
		xml_load_hw_instance(path, atts);
	}
	else if(strcasecmp_null(name,"HW_Connection"))	{
		xml_load_hw_connection(path, atts);
	}
	else if(strcasecmp_null(name,"Computing_Groups")){
		return xml_chech_instance(4, atts);
	}
	else if(strcasecmp_null(name,"Computing_Group")){
		xml_load_hw_group(path, atts);
	}
	else if(strcasecmp_null(name,"Computing_Resource")){
		xml_add_to_hw_group(path, atts);
	}
	else if(strcasecmp_null(name,"SW_Platform"))	{
		return xml_chech_instance(5, atts);
	}
	else if(strcasecmp_null(name,"SW_Components"))	{
		return xml_chech_instance(6, atts);
	}
	else if(strcasecmp_null(name,"SW_Component"))	{
		xml_load_sw_component(path, atts);
	}
	else if(strcasecmp_null(name,"SW_Architecture")){
		return xml_chech_instance(7, atts);
	}
	else if(strcasecmp_null(name,"SW_Instance"))	{
		xml_load_sw_instance(path, atts);
	}
	else if(strcasecmp_null(name,"Functionality"))	{
		return xml_chech_instance(8, atts);
	}
	else if(strcasecmp_null(name,"Exec_Component"))	{
		xml_load_exec_component(path, atts);
	}
	else if(strcasecmp_null(name,"Allocation"))	{
		return xml_chech_instance(9, atts);
	}
	else if(strcasecmp_null(name,"Exec_Instance"))	{
		xml_load_exec_instance(path, atts);
	}
	else if(strcasecmp_null(name,"Argument"))	{
		xml_add_process_argument(path, atts);
	}
	else if(strcasecmp_null(name,"Application")){
		return xml_chech_instance(10, atts);
	}
	else if(strcasecmp_null(name,"Repeat"))	{

	}
/*	else if(strcasecmp_null(name,"Implementation"))	{

	}*/
	else{
		printf("Error: XML Rule %s Unknown\n", name);
		exit(-1);
	}
	return 1;
}

/**
 * \brief Close a xml clause. 
 *
 * \param path Clause path
 * \param name Clause name
 * \return 
 */
void xml_close_declare(char *path, const char *name){

}

/**
 * \brief Decode the arguments of a HW_Component clause
 *
 * \param path Clause path
 * \param name Clause name
 * \return the created struct
 */
void *xml_load_hw_component(char *path, const char **attr){
	int i;
	struct hw_component * new_component = (struct hw_component *) calloc(sizeof(struct hw_component),1);
	new_component->path = (char*) calloc(strlen(path)+1,1);
	strcpy(new_component->path, path);
	new_component->class_name = NULL;
	
	for(i = 0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"name")){
			new_component->name = (char*) malloc(strlen(xml_string_index_convert(xml_string_index_convert(attr[i+1])))+5);
			strcpy(new_component->name,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"category")){
			new_component->category = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->category,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"class")||strcasecmp_null(attr[i],"type") || strcasecmp_null(attr[i],"proc_type")){
			new_component->class_name = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->class_name,xml_string_index_convert(attr[i+1]));
		} 
		else if(strcasecmp_null(attr[i],"activation_type")){
			new_component->activation_type = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->activation_type,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"frequency")){
			new_component->freq = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->freq,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"width")){
			new_component->width = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->width,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"area")){
			new_component->area = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->area,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"static_power")){
			new_component->static_power = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->static_power,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"active_energy")){
			new_component->active_energy = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->active_energy,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"read_energy")){
			new_component->read_energy = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->read_energy,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"write_energy")){
			new_component->write_energy = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->write_energy,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"read_size_energy")){
			new_component->read_size_energy = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->read_size_energy,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"write_size_energy")){
			new_component->write_size_energy = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->write_size_energy,xml_string_index_convert(attr[i+1]));
		}

		else if(strcasecmp_null(attr[i],"mem_size")){
			new_component->mem_size = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->mem_size,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"sets")){
			new_component->type_specific_1 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_1,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"lines")){
			new_component->type_specific_2 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_2,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"x_size")){
			new_component->type_specific_1 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_1,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"y_size")){
			new_component->type_specific_2 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_2,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"mem_type")){
			new_component->type_specific_1 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_1,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"init_func")){
			new_component->init_func = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->init_func,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"library")){
			new_component->library = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->library,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"latency")){
			new_component->latency = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->latency,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"bus_priority")){
			new_component->bus_priority = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->bus_priority,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"burst_size")){
			new_component->burst_size = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->burst_size,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"master_slave")){
			new_component->master = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->master,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"processors_num")){
			new_component->type_specific_2 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_2,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"component_specific")){
			new_component->component_specific = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->component_specific,xml_string_index_convert(attr[i+1]));
		}
#if 0
		else if(strcasecmp_null(attr[i],"cache")){
			new_component->type_specific_1 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_1,xml_string_index_convert(attr[i+1]));
		}
#endif		
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
	}

	create_hw_component(new_component);
	return new_component;
}

/**
 * \brief Decode the arguments of a HW_Instance clause
 *
 * \param path Clause path
 * \param name Clause name
 * \return the created struct
 */
void *xml_load_hw_instance(char *path, const char **attr){
	int i;
	char *str,*c;
	struct hw_instance * new_component = (struct hw_instance *) calloc(sizeof(struct hw_instance),1);
	new_component->path = (char*) calloc(strlen(path)+1,1);
	strcpy(new_component->path,path);
	
	for(i = 0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"name")){
			new_component->name = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+5);
			strcpy(new_component->name,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"frequency")){
			new_component->freq = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->freq,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"width")){
			new_component->width = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->width,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"area")){
			new_component->area = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->area,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"static_power")){
			new_component->static_power = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->static_power,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"active_energy")){
			new_component->active_energy = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->active_energy,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"read_energy")){
			new_component->read_energy = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->read_energy,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"write_energy")){
			new_component->write_energy = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->write_energy,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"read_size_energy")){
			new_component->read_size_energy = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->read_size_energy,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"write_size_energy")){
			new_component->write_size_energy = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->write_size_energy,xml_string_index_convert(attr[i+1]));
		}


		else if(strcasecmp_null(attr[i],"mem_size")){
			new_component->mem_size = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->mem_size,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"proc_type")){
			new_component->type_specific_2 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_2,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"sets")){
			new_component->type_specific_1 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_1,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"lines")){
			new_component->type_specific_2 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_2,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"mem_type")){
			new_component->type_specific_1 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_1,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"component")){
			str = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(str,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"start_addr")){
			new_component->start_addr = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->start_addr,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"irq")){
			new_component->irq = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->irq,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"local_id")){
			new_component->local_id = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->local_id,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"activation_type")){
			new_component->activation_type = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->activation_type,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"latency")){
			new_component->latency = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->latency,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"bus_priority")){
			new_component->bus_priority = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->bus_priority,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"burst_size")){
			new_component->burst_size = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->burst_size,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"master_slave")){
			new_component->master = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->master,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"processors_num")){
			new_component->type_specific_2 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_2,xml_string_index_convert(attr[i+1]));
		}
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
#if 0
		else if(strcasecmp_null(attr[i],"category")){
			new_component->type = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"class")){
			new_component->class_name = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->class_name,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"port")){
			new_component->port = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->port,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"rec_irq")){
			new_component->rec_irq = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->rec_irq,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"cache")){
			new_component->type_specific_1 = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type_specific_1,xml_string_index_convert(attr[i+1]));
		}
#endif
	}

	if(strchr(str,'@')){
		new_component->component = get_hw_component_by_path(str);
	}
	else{
		new_component->component = get_hw_component_by_name(str);
	}
	if(new_component->component == NULL){
		printf("Error: component %s not found\n",str);
		exit(-1);
	}
	new_component->offset = 0;
	create_hw_instance(new_component);

	free(str);

	return new_component;
}

/**
 * \brief Decode the arguments of a HW_Connection clause
 *
 * \param path Clause path
 * \param name Clause name
 * \return the created struct
 */
void *xml_load_hw_connection(char *path, const char **attr){
	int i;
	char *c,*str;
	struct hw_connection * new_component = (struct hw_connection *) calloc(sizeof(struct hw_connection),1);
	new_component->path = (char*) malloc(strlen(path)+1);
	strcpy(new_component->path,path);
	
	for(i = 0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"name")){
			new_component->name = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+5);
			strcpy(new_component->name,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"activation_type")){
			new_component->activation_type = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->activation_type,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"frequency")){
			new_component->speed = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->speed,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"latency")){
			new_component->latency = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->latency,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"mem_size")){
			new_component->mem_size = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->mem_size,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"instance")){
			str = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(str,xml_string_index_convert(attr[i+1]));
		}

		else if(strcasecmp_null(attr[i],"start_addr")){
			new_component->start_addr = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->start_addr,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"port")){
			new_component->port = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->port,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"irq")){
			new_component->irq = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->irq,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"rec_irq")){
			new_component->rec_irq = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->rec_irq,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"local_id")){
			new_component->local_id = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->local_id,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"master_slave")){
			new_component->master = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->master,xml_string_index_convert(attr[i+1]));
		}
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
	}

	if(strchr(str,'@')){
		new_component->instance = get_hw_instance_by_path(str);
	}
	else{
		new_component->instance = get_hw_instance_by_name(str);
	}
	if(new_component->instance == NULL){
		printf("Error: component %s not found\n",str);
		exit(-1);
	}
	new_component->offset = 0;
	create_hw_connection(new_component);

	free(str);

	return new_component;
}

/**
 * \brief Decode the arguments of a Computing_Group clause
 *
 * \param path Clause path
 * \param name Clause name
 * \return the created struct
 */
void *xml_load_hw_group(char *path, const char **attr){
	int i;
	struct hw_group * new_component = (struct hw_group *) calloc(sizeof(struct hw_group),1);
	new_component->path = (char*) malloc(strlen(path)+1);
	strcpy(new_component->path,path);
	
	for(i = 0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"name")){
			new_component->name = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+5);
			strcpy(new_component->name,xml_string_index_convert(attr[i+1]));
		}
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
	}

	create_hw_group(new_component);
	return new_component;
}


/**
 * \brief Decode the arguments of a Computing_Resource clause
 *
 * \param path Clause path
 * \param name Clause name
 * \return the created struct
 */
void *xml_add_to_hw_group(char *path, const char **attr){
	int i;
	struct hw_group * new_component;
	struct hw_instance * instance;
	char *str, *c;
	str = (char*) malloc(strlen(path)+1);
	strcpy(str, path);
	c = strrchr(str,'@');
	c[-1]='\0';

//	new_component = get_hw_group_by_path(str);
	new_component = (struct hw_group *) xml_list_last_element( get_hw_group_header() );

	if(new_component == NULL){
		printf("Error: component %s not found\n",str);
		exit(-1);
	}
	free(str);

	for(i = 0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"name")){
			str = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(str,xml_string_index_convert(attr[i+1]));
		}
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
	}
	
	if(strchr(str,'@')){
		instance = get_hw_instance_by_path(str);
	}
	else{
		instance = get_hw_instance_by_name(str);
	}
	if(instance == NULL){
		printf("Error: instance %s not found\n",str);
		exit(-1);
	}
	add_instance_to_hw_group(new_component, instance);

	free(str);

	return new_component;
}
/**
 * \brief Decode the arguments of a SW_Component clause
 *
 * \param path Clause path
 * \param name Clause name
 * \return the created struct
 */
void *xml_load_sw_component(char *path, const char **attr){
	int i;
	struct sw_component * new_component = (struct sw_component *) calloc(sizeof(struct sw_component),1);
	new_component->path = (char*) malloc(strlen(path)+1);
	strcpy(new_component->path,path);
	
	for(i = 0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"name")){
			new_component->name = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+5);
			strcpy(new_component->name,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"type")){
			new_component->type = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type,xml_string_index_convert(attr[i+1]));
		}
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
	}

	create_sw_component(new_component);
	return new_component;

}
/**
 * \brief Decode the arguments of a SW instance clause
 *
 * \param path Clause path
 * \param name Clause name
 * \return the created struct
 */
void *xml_load_sw_instance(char *path, const char **attr){
	int i;
	char *str,*c;
	struct sw_instance * new_component = (struct sw_instance *) calloc(sizeof(struct sw_instance),1);
	new_component->path = (char*) malloc(strlen(path)+1);
	strcpy(new_component->path,path);
	new_component->memory = NULL;
	
	for(i = 0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"name")){
			new_component->name = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+5);
			strcpy(new_component->name,xml_string_index_convert(attr[i+1]));
		}
#if 0
		else if(strcasecmp_null(attr[i],"type")){
			new_component->type = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type,xml_string_index_convert(attr[i+1]));
		}
#endif
		else if(strcasecmp_null(attr[i],"memory")){
			new_component->memory = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->memory,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"component")){
			if(strchr(xml_string_index_convert(attr[i+1]),'@')){
				new_component->component = get_sw_component_by_path(xml_string_index_convert(attr[i+1]));
			}
			else{
				new_component->component = get_sw_component_by_name(xml_string_index_convert(attr[i+1]));
			}
			if(new_component->component == NULL){
				printf("Error: component %s not found\n",xml_string_index_convert(attr[i+1]));
				exit(-1);
			}
		}
		else if(strcasecmp_null(attr[i],"hw_resource")){
			str = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(str,xml_string_index_convert(attr[i+1]));
		}
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
	}

	if(strchr(str,'@')){
		new_component->hw_resource = get_hw_instance_by_path(str);
		if(new_component->hw_resource == NULL){
			new_component->hw_group = get_hw_group_by_path(str);
		}
	}
	else{
		new_component->hw_resource = get_hw_instance_by_name(str);
		if(new_component->hw_resource == NULL){
			new_component->hw_group = get_hw_group_by_name(str);
		}
	}
	if(new_component->hw_resource == NULL && new_component->hw_group == NULL){
		printf("Error: component %s not found\n",str);
		exit(-1);
	}
	new_component->offset = 0;
	create_sw_instance(new_component);

	free(str);

	return new_component;
}
/**
 * \brief Decode the arguments of a Exec_Component clause
 *
 * \param path Clause path
 * \param name Clause name
 * \return the created struct
 */
void *xml_load_exec_component(char *path, const char **attr){
	int i;
	struct sw_task * new_component = (struct sw_task *) calloc(sizeof(struct sw_task),1);
	new_component->path = (char*) malloc(strlen(path)+1);
	strcpy(new_component->path,path);
	
	for(i = 0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"name")){
			new_component->name = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+5);
			strcpy(new_component->name,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"category")){
			new_component->type = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->type,xml_string_index_convert(attr[i+1]));
		}
#if 0
		else if(strcasecmp_null(attr[i],"class")){
			new_component->class_name = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->class_name,xml_string_index_convert(attr[i+1]));
		}
#endif
		else if(strcasecmp_null(attr[i],"function")){
			new_component->function = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->function,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"file")){
			new_component->file = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->file,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"library")){
			new_component->library = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->library,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"optimization")){
			new_component->optimization = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->optimization,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"compute_time")){
			new_component->compute_time = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->compute_time,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"period")){
			new_component->period = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->period,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"data_size")){
			new_component->data_size = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->data_size,xml_string_index_convert(attr[i+1]));
		}
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
	}

	if(strcasecmp_null(new_component->type,"driver")){		
		char *str = new_component->function;
		new_component->function = (char*) malloc(strlen(str)+25);
		strcpy(new_component->function,"kernel_module_init_");
		strcat(new_component->function,str);
		free(str);	
	}

	create_sw_task(new_component);
	return new_component;

}
/**
 * \brief Decode the arguments of a Exec_Instance clause
 *
 * \param path Clause path
 * \param name Clause name
 * \return the created struct
 */
void *xml_load_exec_instance(char *path, const char **attr){
	int i;
	char *str=NULL, *c;
	struct sw_allocation * new_component = (struct sw_allocation *) calloc(sizeof(struct sw_allocation),1);
	new_component->path = (char*) malloc(strlen(path)+1);
	strcpy(new_component->path,path);
	
	for(i = 0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"name")){
			new_component->name = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+5);
			strcpy(new_component->name,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"priority")){
			new_component->priority = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->priority,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"policy")){
			new_component->policy = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->policy,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"arguments")){
			new_component->args = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(new_component->args,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"component")){
			if(strchr(xml_string_index_convert(attr[i+1]),'@')){
				new_component->task = get_sw_task_by_path(xml_string_index_convert(attr[i+1]));
			}
			else{
				new_component->task = get_sw_task_by_name(xml_string_index_convert(attr[i+1]));
			}
			if(new_component->task == NULL){
				printf("Error: component %s not found\n",xml_string_index_convert(attr[i+1]));
				exit(-1);
			}
		}
		else if(strcasecmp_null(attr[i],"resource")){
			str = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy(str,xml_string_index_convert(attr[i+1]));
		}
		else if(strcasecmp_null(attr[i],"os")){
			if(strchr(xml_string_index_convert(attr[i+1]),'@')){
				new_component->sw_resource = get_sw_instance_by_path(xml_string_index_convert(attr[i+1]));
			}
			else{
				new_component->sw_resource = get_sw_instance_by_name(xml_string_index_convert(attr[i+1]));
			}
			if(new_component->sw_resource == NULL){
				printf("Error: os %s not found\n",xml_string_index_convert(attr[i+1]));
				exit(-1);
			}
		}
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
	}

	if(str!= NULL){
		if(strchr(str,'@')){
			new_component->hw_resource = get_hw_instance_by_path(str);
			if(new_component->hw_resource == NULL){
				new_component->hw_group = get_hw_group_by_path(str);
			}
		}
		else{
			new_component->hw_resource = get_hw_instance_by_name(str);
			if(new_component->hw_resource == NULL){
				new_component->hw_group = get_hw_group_by_name(str);
			}
		}
	}
	if(new_component->sw_resource == NULL && new_component->hw_resource == NULL && new_component->hw_group == NULL){
		printf("Error: hw resource %s not found\n",str);
	exit(-1);
	}
	new_component->offset = 0;
	create_sw_allocation(new_component);

	if(str!= NULL) free(str);

	return new_component;
}
/**
 * \brief Add an argument to the previous sw allocation
 *
 * \param path Clause path
 * \param name Clause name
 * 
 */
void xml_add_process_argument(char *path, const char **attr){
	int i;
	char *str, *old_args;
	struct sw_allocation * allocation;

	for(i = 0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"value")){
			str = (char*) malloc(strlen(xml_string_index_convert(attr[i+1]))+5);
			strcpy(str,xml_string_index_convert(attr[i+1]));
		}
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
	}

	// Add the new argument to the arguments of the last allocation added
	allocation = (struct sw_allocation *) xml_list_last_element(get_sw_allocation_header());
	old_args = allocation->args;
	if(old_args!= NULL){
		allocation->args = (char*) malloc(strlen(old_args) + strlen(str) + 5);
		sprintf(allocation->args, "%s %s", old_args, str);
		free (old_args);
	}else{
		allocation->args = (char*) malloc(strlen(str) + 5);
		sprintf(allocation->args, "%s", str);
	}
	free (str);
	
}

/**
 * \brief Return the double value from an string
 * It detect the units and applies the proper factor
 *
 * \param str The string to convert
 * \param default_unit The unit to be applied by default if there is no unit specified in the string
 * \return The converted value
 * 
 */
double xml_convert_number(const char* str, float default_unit){
	char *unit;// = (char*)malloc(200);
	double num;
	if(str == NULL || strlen(str)<=0) return 0;
	num = strtod(str,&unit);
	while (unit[0]!= '\0' && isspace(unit[0])) unit = &unit[1];
	switch(unit[0]){
		case 'f': num*=1e-15;
			break;
		case 'p': num*=1e-12;
			break;
		case 'n': num*=1e-9;
			break;
		case 'u': num*=1e-6;
			break;
		case 'm': num*=1e-3;
			break;
		case 'k': num*=1e3;
			break;
		case 'K': num*=1e3;
			break;
		case 'M': num*=1e6;
			break;
		case 'G': num*=1e9;
			break;
		case 'g': num*=1e9;
			break;
		case 'T': num*=1e12;
			break;
		case 't': num*=1e12;
			break;
		default:
			num*=default_unit;
	}
	//free(unit);
	return num;
};

unsigned int xml_convert_int_number(const char* str, float default_unit){
	return (unsigned int)xml_convert_number(str, default_unit);
}


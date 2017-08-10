//==========================================================================
//
// Author(s): Hector Posadas
// Contributors:
// Description:   Save the information readed from the XML System Description file into internal structures that will be readed to create the system model
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

#include "xml_input.h"
#include "xml_list.h"
#include "xml_if.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

struct list_elem *hw_component_header = 0;
struct list_elem *hw_instance_header = 0;
struct list_elem *hw_connection_header = 0;
struct list_elem *hw_group_header = 0;
struct list_elem *sw_component_header = 0;
struct list_elem *sw_instance_header = 0;
struct list_elem *sw_task_header = 0;
struct list_elem *sw_allocation_header = 0;


/**
 * \brief Adjust a searching path by ensuring the path does not contain a ".0"
 *
 * \param path The original path
 * \return The fixed path
 */

char *xml_convert_path(const char *path){
	char *str, *c;
	str = (char*) malloc(strlen(path)+1);
	strcpy(str,path);
	c = str;
	while((c = strstr(c,".0/"))){
		strcpy(c,&c[2]);
	}
  	if(strcmp(&str[strlen(str)-2],".0")==0){
		str[strlen(str)-2] = '\0';
	}

	return str;
}

/**
 * \brief Return the list of HW components
 *
 * \return The list pointer
 */

struct list_elem *get_hw_component_header(){
	return hw_component_header;
}
/**
 * \brief Return the list of HW connections
 *
 * \return The list pointer
 */

struct list_elem *get_hw_connection_header(){
	return hw_connection_header;
}
/**
 * \brief Return the list of SW components
 *
 * \return The list pointer
 */

struct list_elem *get_sw_component_header(){
	return sw_component_header;
}
/**
 * \brief Return the list of SW tasks
 *
 * \return The list pointer
 */

struct list_elem *get_sw_task_header(){
	return sw_task_header;
}


/**
 * \brief Return the list of HW instances
 *
 * \return The list pointer
 */

struct list_elem *get_hw_instance_header(){
	return hw_instance_header;
}

/**
 * \brief Return the list of SW instances
 *
 * \return The list pointer
 */
struct list_elem *get_sw_instance_header(){
	return sw_instance_header;
}

/**
 * \brief Return the list of SW allocations
 *
 * \return The list pointer
 */
struct list_elem *get_sw_allocation_header(){
	return sw_allocation_header;
}

/**
 * \brief Return the list of coomputing groups
 *
 * \return The list pointer
 */
struct list_elem *get_hw_group_header(){
	return hw_group_header;
}

/**
 * \brief Insert a new HW component into the corresponding list
 *
 * \param elem The new component
 * \return 
 */
void create_hw_component(struct hw_component* elem){
	xml_list_add_element(&hw_component_header,(void*)elem);
}
/**
 * \brief Return the component indicated by the input name
 *
 * \param name The component name
 * \return the component
 */
struct hw_component *get_hw_component_by_name(const char* name){
	struct list_elem   * elem = hw_component_header;
	struct hw_component* component;
	
	if(elem == NULL) return NULL;
	component = (struct hw_component* )elem->elem;
	while(strcasecmp(name,component->name) != 0) {
		elem = elem->next;
		if(elem == NULL) return NULL;
		component = (struct hw_component* )elem->elem;
	} 
	return component;
}
/**
 * \brief Return the component indicated by the input path
 *
 * \param name The component path
 * \return the component
 */
struct hw_component *get_hw_component_by_path(const char* path){
	struct list_elem   * elem = hw_component_header;
	struct hw_component* component;
	
	if(elem == NULL) return NULL;

	char *str= xml_convert_path(path);
	path = str;

	component = (struct hw_component* )elem->elem;
	while(strcasecmp(path,component->path) != 0) {
		elem = elem->next;
		if(elem == NULL) {
			free(str);
			return NULL;
		}
		component = (struct hw_component* )elem->elem;
	} 
	free(str);
	return component;
}

/**
 * \brief Insert a new HW component into the corresponding list
 *
 * \param elem The new component
 * \return 
 */
void create_hw_instance(struct hw_instance* elem){
	char *path,*c;
	struct hw_instance* parent;
	int index;
	int found;

	xml_list_add_element(&hw_instance_header,(void*)elem);

	path = (char*) malloc(strlen(elem->path)+1);
	strcpy(path,elem->path );
	c = strrchr(path, '@');
	if (c != NULL) {
		c[-1] = '\0';
	}

	do{
		found = 0;
		index = strlen(path)-strlen("/@Repeat");
		if(strcasecmp(&path[index],"/@Repeat")==0){
			path[index]='\0';
			found = 1;
		}
		else if(strncasecmp(&path[index-2],"/@Repeat",strlen("/@Repeat"))==0){
			path[index-2]='\0';
			found = 1;
		}else if(strncasecmp(&path[index-3],"/@Repeat",strlen("/@Repeat"))==0){
			path[index-3]='\0';
			found = 1;
		}

	}
	while (found);
	parent = get_hw_instance_by_path(path);
	if(parent != NULL){
		xml_list_add_element(&parent->contain,elem);
	}else{
		c = strrchr(path, '@');
		if (c == NULL || strcasecmp(c,"@HW_architecture")!=0) {
			printf("Error: Parent element %s not found when creating hw instance with %s \n",path,elem->name);
			assert(0);
		}
	}

}
/**
 * \brief Return the component indicated by the input name
 *
 * \param name The component name
 * \return the component
 */
struct hw_instance *get_hw_instance_by_name(const char* name){
	struct list_elem   * elem = hw_instance_header;
	struct hw_instance * component;
	
	if(elem == NULL) return NULL;
	component = (struct hw_instance* )elem->elem;
	while(strcasecmp(name,component->name) != 0) {
		elem = elem->next;
		if(elem == NULL) return NULL;
		component = (struct hw_instance* )elem->elem;
	} 
	return component;
} 
/**
 * \brief Return the component indicated by the input path
 *
 * \param name The component path
 * \return the component
 */
struct hw_instance *get_hw_instance_by_path(const char* path){
	struct list_elem   * elem = hw_instance_header;
	struct hw_instance * component;
	
	if(elem == NULL) return NULL;
	char *str= xml_convert_path(path);
	path = str;

	component = (struct hw_instance* )elem->elem;
	while(strcasecmp(path,component->path) != 0) {
		elem = elem->next;
		if(elem == NULL) {
			free(str);
			return NULL;
		}
		component = (struct hw_instance* )elem->elem;
	} 
	free(str);

	return component;
}

/**
 * \brief Insert a new HW component into the corresponding list
 *
 * \param elem The new component
 * \return 
 */
void create_hw_connection(struct hw_connection* elem){
	char *path,*c;
	struct hw_instance* parent;
	int index;
	int found;

	xml_list_add_element(&hw_connection_header,(void*)elem);

	path = (char*) malloc(strlen(elem->path)+1);
	strcpy(path,elem->path );
	c = strrchr(path, '@');
	if (c != NULL) {
		c[-1] = '\0';
	}

	do{
		found = 0;
		index = strlen(path)-strlen("/@Repeat");
		if(strcasecmp(&path[index],"/@Repeat")==0){
			path[index]='\0';
			found = 1;
		}
		else if(strncasecmp(&path[index-2],"/@Repeat",strlen("/@Repeat"))==0){
			path[index-2]='\0';
			found = 1;
		}else if(strncasecmp(&path[index-3],"/@Repeat",strlen("/@Repeat"))==0){
			path[index-3]='\0';
			found = 1;
		}

	}
	while (found);
	parent = get_hw_instance_by_path(path);
	if(parent != NULL){
		xml_list_add_element(&parent->connections,elem);
	}else{
		c = strrchr(path, '@');
		if (c == NULL || strcasecmp(c,"@HW_architecture")!=0) {
			printf("Error: Parent element %s not found when creating hw connection %s \n",path,elem->name);
			assert(0);
		}
	}
}

/**
 * \brief Insert a new HW component into the corresponding list
 *
 * \param elem The new component
 * \return 
 */
void create_hw_group(struct hw_group *elem){
	elem->list = NULL;
	xml_list_add_element(&hw_group_header,(void*)elem);
}

/**
 * \brief Return the component indicated by the input name
 *
 * \param name The component name
 * \return the component
 */
struct hw_group *get_hw_group_by_name(const char* name){
	struct list_elem   * elem = hw_group_header;
	struct hw_group* component;
	
	if(elem == NULL) return NULL;
	component = (struct hw_group* )elem->elem;
	while(strcasecmp(name,component->name) != 0) {
		elem = elem->next;
		if(elem == NULL) return NULL;
		component = (struct hw_group* )elem->elem;
	} 
	return component;
}
/**
 * \brief Return the component indicated by the input path
 *
 * \param name The component path
 * \return the component
 */
struct hw_group *get_hw_group_by_path(const char* path){
	struct list_elem   * elem = hw_group_header;
	struct hw_group* component;
	
	if(elem == NULL) return NULL;

	char *str= xml_convert_path(path);
	path = str;

	component = (struct hw_group* )elem->elem;
	while(strcasecmp(path,component->path) != 0) {
		elem = elem->next;
		if(elem == NULL) {
			free(str);
			return NULL;
		}
		component = (struct hw_group* )elem->elem;
	} 
	free(str);

	return component;
}
/**
 * \brief Add a HW instance to a computing group previously created
 *
 * \param group The computing group
 * \param elem The component instance to be added
 * \return
 */
void add_instance_to_hw_group(struct hw_group* group, struct hw_instance* elem){
	xml_list_add_element(&group->list,(void*)elem);
}

/**
 * \brief Insert a new HW component into the corresponding list
 *
 * \param elem The new component
 * \return 
 */
void create_sw_component(struct sw_component* elem){
	xml_list_add_element(&sw_component_header,(void*)elem);
}
/**
 * \brief Return the component indicated by the input name
 *
 * \param name The component name
 * \return the component
 */
struct sw_component *get_sw_component_by_name(const char* name){
	struct list_elem   * elem = sw_component_header;
	struct sw_component* component;
	
	if(elem == NULL) return NULL;
	component = (struct sw_component* )elem->elem;
	while(strcasecmp(name,component->name) != 0) {
		elem = elem->next;
		if(elem == NULL) return NULL;
		component = (struct sw_component* )elem->elem;
	} 
	return component;
} 
/**
 * \brief Return the component indicated by the input path
 *
 * \param name The component path
 * \return the component
 */
struct sw_component *get_sw_component_by_path(const char* path){
	struct list_elem   * elem = sw_component_header;
	struct sw_component* component;
	
	if(elem == NULL) return NULL;

	char *str= xml_convert_path(path);
	path = str;

	component = (struct sw_component* )elem->elem;
	while(strcasecmp(path,component->path) != 0) {
		elem = elem->next;
		if(elem == NULL) {
			free(str);
			return NULL;
		}
		component = (struct sw_component* )elem->elem;
	} 
	free(str);

	return component;
}

/**
 * \brief Insert a new HW component into the corresponding list
 *
 * \param elem The new component
 * \return 
 */
void create_sw_instance(struct sw_instance* elem){
	xml_list_add_element(&sw_instance_header,(void*)elem);
}
/**
 * \brief Return the component indicated by the input name
 *
 * \param name The component name
 * \return the component
 */
struct sw_instance *get_sw_instance_by_name(const char* name){
	struct list_elem   * elem = sw_instance_header;
	struct sw_instance* component;
	
	if(elem == NULL) return NULL;
	component = (struct sw_instance* )elem->elem;
	while(strcasecmp(name,component->name) != 0) {
		elem = elem->next;
		if(elem == NULL) return NULL;
		component = (struct sw_instance* )elem->elem;
	} 
	return component;
} 
/**
 * \brief Return the component indicated by the input path
 *
 * \param name The component path
 * \return the component
 */
struct sw_instance *get_sw_instance_by_path(const char* path){
	struct list_elem   * elem = sw_instance_header;
	struct sw_instance* component;
	
	if(elem == NULL) return NULL;

	char *str= xml_convert_path(path);
	path = str;

	component = (struct sw_instance* )elem->elem;
	while(strcasecmp(path,component->path) != 0) {
		elem = elem->next;
		if(elem == NULL) {
			free(str);
			return NULL;
		}
		component = (struct sw_instance* )elem->elem;
	} 
	free(str);

	return component;
}

/**
 * \brief Insert a new HW component into the corresponding list
 *
 * \param elem The new component
 * \return 
 */
void create_sw_task(struct sw_task* elem){
	xml_list_add_element(&sw_task_header,(void*)elem);
}
/**
 * \brief Return the component indicated by the input name
 *
 * \param name The component name
 * \return the component
 */
struct sw_task *get_sw_task_by_name(const char* name){
	struct list_elem   * elem = sw_task_header;
	struct sw_task* component;
	
	if(elem == NULL) return NULL;
	component = (struct sw_task* )elem->elem;
	while(strcasecmp(name,component->name) != 0) {
		elem = elem->next;
		if(elem == NULL) return NULL;
		component = (struct sw_task* )elem->elem;
	} 
	return component;
}  
/**
 * \brief Return the component indicated by the input path
 *
 * \param name The component path
 * \return the component
 */
struct sw_task *get_sw_task_by_path(const char* path){
	struct list_elem   * elem = sw_task_header;
	struct sw_task* component;
	
	if(elem == NULL) return NULL;

	char *str= xml_convert_path(path);
	path = str;

	component = (struct sw_task* )elem->elem;
	while(strcasecmp(path,component->path) != 0) {
		elem = elem->next;
		if(elem == NULL) {
			free(str);
			return NULL;
		}
		component = (struct sw_task* )elem->elem;
	} 
	free(str);

	return component;
} 

/**
 * \brief Insert a new HW component into the corresponding list
 *
 * \param elem The new component
 * \return 
 */
void create_sw_allocation(struct sw_allocation* elem){
	char * c;

	xml_list_add_element(&sw_allocation_header,(void*)elem);
}

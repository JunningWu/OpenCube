//==========================================================================
//
// Author(s): Hector Posadas
// Contributors:
// Description:   Functions to load the XML System Configuration file
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

#include "xml_configuration_file.h"
#include "xml_list.h"
#include "xml_if.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct list_elem *xml_config_parameter_header = 0;

/**
 * \brief Get the corresponding value for a configuration parameter
 *
 * \param param Name of the parameter
 * \return Parameter value
 */

const char *xml_load_configuration_parameter(const char* param){
	struct xml_parameter_info *info;
	struct list_elem *elem;

	elem = xml_config_parameter_header;
	while (elem){
		info = (struct xml_parameter_info *)elem->elem;
		if(strcmp(param,info->name) == 0){
			return info->value;
		}
		elem = elem->next;
	}
	printf("Error: parameter %s not recognized\n",param);
	exit(-1);

}
// Extension for vector management.
// When a parameter has no value it is stored in prev_xml_parameter_info
// "items" are applied to this variable.
struct xml_parameter_info *prev_xml_parameter_info = NULL;

/**
 * \brief Decode the attributes of a configuration parameter xml clause
 *
 * \param userData Pointer to the user defined buffer
 * \param name Name of the parameter xml clause
 * \param atts Attributes of the xml clause
 * \return
 */

void xml_add_parameter(char* userData, const char *name, const char **atts){
	struct xml_parameter_info *info;
	int i, stored = 0;

	if(strcasecmp(name,"parameter") == 0) {

		// When a new parameter is read the previous prev_xml_parameter_info is no longer required.
		// For each vector item a new xml_parameter_info is created. Thus the original vector info can be free.
		if(prev_xml_parameter_info != NULL) {
			free(prev_xml_parameter_info);
		}
		info = (struct xml_parameter_info*) malloc(sizeof(struct xml_parameter_info));
		prev_xml_parameter_info = info;
		stored = 0;

		for(i = 0; atts[i]; i+=2){
			if(strcasecmp(atts[i],"name") == 0){
				info->name = (char*) malloc(strlen(atts[i+1])+1);
				strcpy(info->name,atts[i+1]);
			}
			else if(strcasecmp(atts[i],"value") == 0){
				info->value = (char*) malloc(strlen(atts[i+1])+1);
				strcpy(info->value,atts[i+1]);
				xml_list_add_element(&xml_config_parameter_header,(void*)info);
				stored = 1;
			}
			else{
				printf("Error: xml attribute %s not recognized\n",atts[i]);
				exit(-1);
			}
		}
		if (stored == 1){
			prev_xml_parameter_info = NULL;
		}
	}
	else if(strcasecmp(name,"item") == 0) {
		int index;

		if(prev_xml_parameter_info == NULL) {
			printf("Error: item clause not in a vector type\n");
			exit(-1);
		}
		index = -1;

		info = (struct xml_parameter_info*) malloc(sizeof(struct xml_parameter_info));

		for(i = 0; atts[i]; i+=2){
			if(strcasecmp(atts[i],"index") == 0){
				index = atoi_null(atts[i+1]);
			}
			else if(strcasecmp(atts[i],"position") == 0){
				index = atoi_null(atts[i+1]);
			}
			else if(strcasecmp(atts[i],"value") == 0){
				info->value = (char*) malloc(strlen(atts[i+1])+1);
				strcpy(info->value,atts[i+1]);
			}
			else{
				printf("Error: xml attribute %s not recognized\n",atts[i]);
				exit(-1);
			}
		}
				
		if(index < 0){
			printf("Error: item index invalid \n");
			exit(-1);
		}	
		info->name = (char*) malloc(strlen(prev_xml_parameter_info->name)+10);
		sprintf(info->name, "%s[%d]", prev_xml_parameter_info->name, index);

		xml_list_add_element(&xml_config_parameter_header,(void*)info);
	}
	else{
		printf("Error: xml clause %s not recognized\n",name);
		exit(-1);
	}	
}

/**
 * \brief Close a configuration xml clause
 *
 * \param userData Pointer to the user defined buffer
 * \param name Name of the parameter xml clause
 * \return
 */

void xml_end_parameter(char* userData, const char *name){
}

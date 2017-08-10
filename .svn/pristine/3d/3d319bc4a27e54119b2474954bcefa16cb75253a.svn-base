//==========================================================================
//
// Author(s): Hector Posadas
// Contributors:
// Description:   Generate a hierarchy with the clause name and arguments loaded from a XML System Description File. This hierarchy will be analyzed to crate the components structus in the xml_if file
//
// Modifications:
//
//==========================================================================
//
//  The following code is derived, directly or indirectly, from XML plug-in for SCoPE,
//  released June 30, 2008. Copyright (C) 2008 Univeristy of Cantabria(UC)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  For more information about SCoPE and the XML plug-in for SCoPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  the authors or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//
//==========================================================================

#include "xml_hierarchy.h"
#include "xml_configuration_file.h"
#include "uc_create_xml_platform.h"
#include "xml_list.h"
#include "xml_if.h"
#include "uc_load_xml.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>


struct xml_basic_info *xml_hierarchy_header = 0;

struct xml_basic_info *xml_hierarchy_current = 0;

int xml_indexes[30];

char *xml_select_implementation_array[11] = {NULL};

void uc_set_check_system_status_flag(bool);

/**
 * \brief Add a new struct with the info of a clause to the correct point of the xml data hierarchy
 *
 * \param path Clause path
 * \param name Clause name
 * \param atts Clause attributes
 */
void xml_add_hierarchy(char *path, const char *name, const char **args){

	struct xml_basic_info *elem;
	int i;

	elem = (struct xml_basic_info*) malloc(sizeof(struct xml_basic_info));
	elem -> parent = xml_hierarchy_current;
	elem -> children_header = NULL;

	elem -> name = (char *) malloc (strlen(name)+1);
	strcpy(elem->name, name);

	elem -> repeat = "1";
	elem -> init_index = "0";
	elem -> index = 0;
	if(strcasecmp(name, "Repeat") == 0) {
		elem -> index = 'a'+ 29;
		for(i = 0; args[i]; i+=2){
			if(strcasecmp (args[i],"number") == 0){
				elem -> repeat = (char*) malloc(strlen(args[i+1])+5);
				strcpy(elem -> repeat,args[i+1]);
				
				// elem -> repeat = atoi_null(xml_string_index_convert(args[i+1]));
			}
			else if(strcasecmp (args[i],"index") == 0){
				elem -> index = args[i+1][0];
			}
			else if(strcasecmp (args[i],"init") == 0){
				elem -> init_index = (char*) malloc(strlen(args[i+1])+5);
				strcpy(elem -> init_index,args[i+1]);
				//elem -> init_index = atoi_null(xml_string_index_convert(args[i+1]));
			}
			else{
				printf("Error: Invalid attribute in Repeat clause.\n");
				exit (-1);
			}
		}
	}else if(strcasecmp(name, "Implementation") == 0) {
		xml_select_implementation(args);
		return;
	}else if(strcasecmp(name, "Simulation") == 0) {
		xml_simulation_parameters(args);
		return;
	}else if(strcasecmp(name, "Granularity") == 0) {
		xml_granularity_parameters(args);
		return;
	}else if(strcasecmp(name, "Variable") == 0) {
		xml_set_variable(args);
		return;
	}else if(strcasecmp(name, "Plugin") == 0) {
		xml_start_plugin(args);
		return;
	}

	for(i = 0; args[i]; i+=2);
	elem->args = (char**) malloc(sizeof(char*) * (i+1));
	for(i = 0; args[i]; i+=2){
		elem->args[i] = (char*) malloc(sizeof(char) * strlen(args[i])+1);
		strcpy(elem->args[i],args[i]);
		elem->args[i+1] = (char*) malloc(sizeof(char) * strlen(args[i+1])+1);
		strcpy(elem->args[i+1],args[i+1]);
	}
	elem->args[i] = 0;

	if(xml_hierarchy_current)
		xml_list_add_element( & xml_hierarchy_current -> children_header, elem);
	
	xml_hierarchy_current = elem;

	if(xml_hierarchy_header == NULL)
		xml_hierarchy_header = elem;

}

/**
 * \brief Close a clause moving the current hierarchy node to the upper node
 *
 * \param path Clause path
 * \param name Clause name
 */
void xml_up_hierarchy(char *path, const char *name){
	if(strcasecmp(name, "Implementation") == 0) {
		return;
	} else 	if(strcasecmp(name, "Simulation") == 0) {
		return;
	}else 	if(strcasecmp(name, "Variable") == 0) {
		return;
	}else 	if(strcasecmp(name, "Granularity") == 0) {
		return;
	}
	xml_hierarchy_current = xml_hierarchy_current->parent;
}

/**
 * \brief Increase the path when entering a new clause
 *
 * \param path Clause path
 * \param elem Clause info
 * \param index used when creating several copies in a repeat clause
 */
void xml_increase_path(char *path, struct xml_basic_info *elem, int index){
	
	char *name;

	if (path != NULL && strlen(path)> 0){

		name = xml_check_name(elem->name,elem->parent, index );
		
		strcat(path,"/");
		strcat(path,"@");
		strcat(path, name); 
		free(name);
	}
	else{
		strcpy(path, "/");
		xml_hierarchy_header = elem;
	}	

}

/**
 * \brief Decrease the path when exiting a new clause
 *
 * \param path Clause path
 * \param elem Clause info
 */
void xml_decrease_path(char *path, const char *name){
	char * ch;
	ch = strrchr(path,'@');
	if(ch == NULL) return;
		
//	if(strncasecmp(&ch[1],name,strlen(name))) {
//		printf("Warning: closing clause %s does not fit opening clause %s\n",&ch[1],name);
//	}
	ch[-1]='\0';
}

/**
 * \brief Return the information struct of the clause indicated by the input path
 *
 * \param path Clause path
 * \return The clause info
 */
struct xml_basic_info *xml_search_path(const char *path){
	const char *start, *end, *str;
	struct list_elem *elem; 
	struct xml_basic_info *info;
	struct xml_basic_info *elem_info;

	if(strncmp(path,"//",2) != 0 && strcmp(path,"/") != 0){
		printf("Error: Invalid absolute path %s\n",path);
		exit(-1);
	}
	start = strchr(path,'@');
	info = xml_hierarchy_header;
	while( start != NULL && strlen(start)>0 ){
		start++;
		elem = info->children_header;
		end = strchr(&start[1],'/');
		if(end == 0) {
			end = &start[strlen(start)];
		}
		while(elem != NULL){
			elem_info = (struct xml_basic_info *)elem->elem;
			str = strstr(start,".0");
			if( str < end && str > 0) end = str;
			if(strncmp(elem_info->name,start,end-start) == 0){
				info = elem_info;
				start = strchr(end,'@') ;
				break;
			}
			elem = elem->next;
		}
		if(elem == NULL){
			printf("Error: Invalid absolute path %s\n",path);
			exit(-1);
		}
	}
	return info;
}

/**
 * \brief Create a name, adding the corresponding suffix if more clauses with the same name hangs of the same node
 *
 * \param name Clause name
 * \param prev_elem Clause info list
 * \param index used when creating several copies in a repeat clause
 * \return The new clause name
 */
char *xml_check_name(const char *name, struct xml_basic_info * prev_elem, int index){
	struct list_elem *elem;
	int number = 0, curr = -1;
	struct xml_basic_info *info;
	char * new_name;

	elem = prev_elem->children_header;
	while(elem != NULL){
		info = (struct xml_basic_info *)elem->elem;
		if(name == info->name ){
			curr = number + index;
		}
		if( strncmp(info->name, name, strlen(name)) == 0){
			number+= atoi_null(xml_string_index_convert(info->repeat));
		}
		elem = elem->next;
	}
	new_name = (char*) malloc(strlen(name)+5);

	if(curr > 0 ){
		sprintf(new_name, "%s.%d", name, curr);
	}
	else{
		strcpy(new_name,name);
	}
	return new_name;
}

/**
 * \brief Create a node for a xml clause
 *
 * \param info Clause info list
 * \param name Clause info
 * \param index used when creating several copies in a repeat clause
 * \return The new clause name
 */
void xml_display_hierarchy_node(struct xml_basic_info *info, char *path, int index ){

	struct list_elem *children;
	struct xml_basic_info *child_info;
	char * str;
	int i, repeat, init;

	xml_increase_path(path , info, index);

	if(xml_declare_function( path, info->name, (const char **)info->args) == 0){
		xml_decrease_path(path, info->name);
		return;
	}

	children = info->children_header;

	while(children){
		child_info = (struct xml_basic_info *)children->elem;

		repeat = atoi_null(xml_string_index_convert(child_info->repeat));
		init = atoi_null(xml_string_index_convert(child_info->init_index));
		//rep = child_info->repeat + child_info->init_index;

		for(i = init; i < init + repeat; i++){
			if(repeat>1 && child_info->index!=0){
				xml_indexes[child_info->index - 'a'] = i;
			}
			xml_display_hierarchy_node(child_info, path, i);
		}
		children = children->next;
	}

	xml_close_declare(path, info->name);

	xml_decrease_path(path, info->name);
}

/**
 * \brief Create a node for a certain path
 *
 * \param path Clause path
 */
void xml_display_hierarchy(char *path){
	path[0] = '\0';
	xml_display_hierarchy_node( xml_hierarchy_header,  path, 0);
}

/**
 * \brief Check if the argument is a configuratino parameter (start with '_') or contain a "repeat" index and fix the string if required
 *
 * \param data_in Input argument
 * \return The fixed argument
 */
const char *xml_string_index_convert(const char *data_in){
	char *str, *c, *name;
	const char *old_str;

	old_str = data_in;

	// Replace indexes
	c = (char*)strchr(old_str,'%');
	while(c !=0 ){
		name = str = (char*) malloc(strlen(old_str)+10);
		strncpy(str, old_str, c-old_str);
		str += c-old_str;
		sprintf(str,"%d",xml_indexes[c[1] - 'a']);
		strcat(str,&c[2]);
		old_str = name;
		c = (char*)strchr(old_str,'%');
	}

	// Replace parameters. It must be done before replacing indexes 
	// in order to allow vector types
	if(old_str[0] == '_' && old_str[1] == '_'){
		old_str = xml_load_configuration_parameter(&old_str[2]);
	}
	
	return old_str;
}

/**
 * \brief Check if the clause has been selected in the implementation clause or must be discarded.
 *
 * \param index type of index
 * \param atts arguments of the clause
 * \return 1 if valid, 0 otherwise
 */
int xml_chech_instance(int index, const char **atts){
	int i;

	if(xml_select_implementation_array[index] == NULL){
		return 1;
	}

	for(i=0; atts[i]; i+=2){
		if(strcasecmp_null(atts[i],"name"))	{
			return strcmp(xml_select_implementation_array[index],atts[i+1]) == 0;
		}
	}
	return 1;
}

/**
 * \brief Decode the attributes of an implementation clause
 *
 * \param atts Clause attributes
 * \return 
 */
void xml_select_implementation(const char **atts){

	int i, index;

	for(i=0; atts[i]; i+=2){
		if(strcasecmp_null(atts[i],"Description"))	{
			index=0;
		}
		else if(strcasecmp_null(atts[i],"HW_Platform"))	{
			index=1;
		}
		else if(strcasecmp_null(atts[i],"HW_Components"))	{
			index=2;
		}
		else if(strcasecmp_null(atts[i],"HW_Architecture")){
			index=3;
		}
		else if(strcasecmp_null(atts[i],"Computing_Groups")){
			index=4;
		}
		else if(strcasecmp_null(atts[i],"SW_Platform"))	{
			index=5;
		}
		else if(strcasecmp_null(atts[i],"SW_Components"))	{
			index=6;
		}
		else if(strcasecmp_null(atts[i],"SW_Architecture")){
			index=7;
		}
		else if(strcasecmp_null(atts[i],"Functionality"))	{
			index=8;
		}
		else if(strcasecmp_null(atts[i],"Allocation"))	{
			index=9;
		}
		else if(strcasecmp_null(atts[i],"Application"))	{
			index=10;
		}
		else {
			printf("Error: xml attribute %s not recognized\n",atts[i]);
			exit(-1);
		}
		
		xml_select_implementation_array[index] = (char*) malloc(strlen(atts[i+1]+1));
		strcpy(xml_select_implementation_array[index],xml_string_index_convert(atts[i+1]));
	}
}

/**
 * \brief Decode the attributes of a simulation clause
 *
 * \param atts Clause attributes
 */
void xml_simulation_parameters(const char **atts){
	int i, ret, j, index;

	for(i=0; atts[i]; i+=2){
		if(strcasecmp_null(atts[i],"time")){
			char val[20]="";
//			ret = sscanf(atts[i+1], "\"%s %s\"", &xml_scope_simulation_info.m_simulation_time,unit);
			index = 0;
			for(j=0; atts[i+1][j]; j++){
				if(isspace(atts[i+1][j])){
				}else if(isdigit(atts[i+1][j])){
					val[index++] = atts[i+1][j];
				}else if(atts[i+1][j] == 's'){
					if(strlen(val) == 0) {
						printf("Error: wrong format for argument 'time': 'time = \"integer unit\"'  \n");
						exit(-1);
					}
					xml_scope_simulation_info.m_sim_time_unit = SC_SEC;
					break;
				}else if(atts[i+1][j] == 'm'){
					if(strlen(val) == 0) {
						printf("Error: wrong format for argument 'time': 'time = \"integer unit\"'  \n");
						exit(-1);
					}
					xml_scope_simulation_info.m_sim_time_unit = SC_MS;
					break;
				}else if(atts[i+1][j] == 'u'){
					if(strlen(val) == 0) {
						printf("Error: wrong format for argument 'time': 'time = \"integer unit\"'  \n");
						exit(-1);
					}
					xml_scope_simulation_info.m_sim_time_unit = SC_US;
					break;
				}else if(atts[i+1][j] == 'n'){
					if(strlen(val) == 0) {
						printf("Error: wrong format for argument 'time': 'time = \"integer unit\"'  \n");
						exit(-1);
					}
					xml_scope_simulation_info.m_sim_time_unit = SC_NS;
					break;
				}else if(atts[i+1][j] == 'p'){
					if(strlen(val) == 0) {
						printf("Error: wrong format for argument 'time': 'time = \"integer unit\"'  \n");
						exit(-1);
					}
					xml_scope_simulation_info.m_sim_time_unit = SC_PS;
					break;
				}else {
					printf("Error: wrong unit for argument 'time'. Should be s, ms, us, ns or ps.\n",atts[i]);
					exit(-1);
				}
			}
			xml_scope_simulation_info.m_simulation_time = atoi_null(val);
		}
		else if(strcasecmp_null(atts[i],"debug")){
			xml_scope_simulation_info.m_debug_level = atoi_null(atts[i+1]);
			uc_set_check_system_status_flag(xml_scope_simulation_info.m_debug_level&2);
			if(xml_scope_simulation_info.m_debug_level == 1){
				gui_connector = new UC_gui_connector ("gui_connector");
			}

		}
		else if(strcasecmp_null(atts[i],"backtrace")){
			uc_activate_backtrace( atoi_null(atts[i+1]));
		}
		else if(strcasecmp_null(atts[i],"end_as_sw")){
			uc_activate_end_with_sw_destruction( 1 );
		}
		else {
			printf("Error: xml attribute %s not recognized\n",atts[i]);
			exit(-1);
		}
	}
}

/**
 * \brief Decode the attributes  defining the granularity of the simulation
 *
 * \param atts Clause attributes
 */
void xml_granularity_parameters(const char **atts){
	int i, value;
	float f_value;

	for(i=0; atts[i]; i+=2){
		if(strcasecmp_null(atts[i],"max_time")){
			f_value = xml_convert_number(xml_string_index_convert(atts[i+1]), 1);
			uc_set_max_wait_interval((long long)(f_value*1e9));
			
		}
		else if(strcasecmp_null(atts[i],"transfer_period")){
			f_value = xml_convert_number(xml_string_index_convert(atts[i+1]), 1);
			uc_set_sw_wait_period((long long)(f_value*1e9));
		}
		else if(strcasecmp_null(atts[i],"ic_buffer")){
			f_value = xml_convert_number(xml_string_index_convert(atts[i+1]), 1);
			uc_set_ic_granularity((long long)(f_value));
		}
		else {
			printf("Error: xml attribute %s not recognized\n",atts[i]);
			exit(-1);
		}
	}
}


/**
 * \brief Assign an specified value to a global variable
 *
 * \param atts Clause attributes
 */
void xml_set_variable(const char **attr){
	int i, value;
	char *name, *file=NULL;
	void *handler = NULL;
	int *var_p;

	for(i=0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"name")){
			name = (char *)malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy (name, xml_string_index_convert(attr[i+1]));	
		}else if(strcasecmp_null(attr[i],"value")){
			value = atoi_null( xml_string_index_convert(attr[i+1]));	
			
		}else if(strcasecmp_null(attr[i],"library")){
			file = (char *)malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy (file, xml_string_index_convert(attr[i+1]));	
		}
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
	}

	if(name == NULL) {
		printf("Error: variable name not assigned \n");
		return;
	}
	if(file != NULL) {
		handler = dlopen(file, RTLD_NOW | RTLD_GLOBAL ) ;
		if(handler == NULL){
			printf("Fatal error: library %s for variable %s not found \n",file,name);
			assert(0);
			return;
		}
	}
	if(handler == NULL){
		handler = dlopen(NULL, RTLD_NOW | RTLD_GLOBAL ) ;
	}
	if(handler == NULL) {
		printf("Error: fail loading static file symbols %s \n",dlerror());
//		exit(-1);
	}
	var_p = (int*)dlsym(handler, name) ;
	if(var_p == NULL) {
		printf("Error: Variable %s not found\n",name);
		exit(-1);
	}	
	*var_p = value;
}

/**
 * \brief Assign an specified value to a global variable
 *
 * \param atts Clause attributes
 */
void xml_start_plugin(const char **attr){
	int i, value;
	char *name, *file=NULL, *func=NULL;
	void *handler = NULL;
	void (*var_p)();

	for(i=0; attr[i]; i+=2){
		if(strcasecmp_null(attr[i],"name")){
			name = (char *)malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy (name, xml_string_index_convert(attr[i+1]));	
		}else if(strcasecmp_null(attr[i],"entry")){
			func = (char *)malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy (func, xml_string_index_convert(attr[i+1]));	
		}else if(strcasecmp_null(attr[i],"library")){
			file = (char *)malloc(strlen(xml_string_index_convert(attr[i+1]))+1);
			strcpy (file, xml_string_index_convert(attr[i+1]));	
		}
		else {
			printf("Error: xml attribute %s not recognized\n",attr[i]);
			exit(-1);
		}
	}

	if(name == NULL) {
		printf("Error: variable name not assigned \n");
		return;
	}
	var_p = (void (*)())get_dynamic_function(file, func);

	if(var_p == NULL) {
		printf("Error: Function %s not found\n",func);
		exit(-1);
	}	
	var_p();
}

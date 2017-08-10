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
#ifndef _XML_HIERARCHY_H_
#define _XML_HIERARCHY_H_

// SCoPE functions
int uc_activate_end_with_sw_destruction(int val);
int uc_activate_backtrace(int val);


struct xml_basic_info{
	char *name;
	char **args;
	struct xml_basic_info *parent;
	struct list_elem *children_header;
	char index;
	char *  init_index;
	char *  repeat;
};

void xml_granularity_parameters(const char **atts);

void xml_display_hierarchy(char *path);

void xml_add_hierarchy(char *path, const char *name, const char **args);

void xml_up_hierarchy(char *path, const char *name);

void xml_decrease_path(char *path, const char *name);

void xml_increase_path(char *path, struct xml_basic_info *elem, int index);

struct xml_basic_info *xml_search_path(const char *path);

char *xml_check_name(const char *name, struct xml_basic_info * prev_elem, int index);

const char *xml_string_index_convert(const char *data_in);

int xml_chech_instance(int i, const char **atts);

void xml_select_implementation(const char **atts);

void xml_simulation_parameters(const char **args);

void xml_set_variable(const char **atts);

void xml_start_plugin(const char **attr);

#endif

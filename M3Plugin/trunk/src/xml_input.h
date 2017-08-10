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
#ifndef _XML_INPUT_H_
#define _XML_INPUT_H_

struct list_elem *get_hw_component_header();
struct list_elem *get_hw_connection_header();
struct list_elem *get_sw_component_header();
struct list_elem *get_sw_task_header();
struct list_elem *get_hw_instance_header();
struct list_elem *get_sw_instance_header();
struct list_elem *get_sw_allocation_header();
struct list_elem *get_hw_group_header();

void create_hw_component(struct hw_component* elem);
struct hw_component *get_hw_component_by_name(const char* name);
struct hw_component *get_hw_component_by_path(const char* path);

void create_hw_instance(struct hw_instance* elem);
struct hw_instance *get_hw_instance_by_name(const char* name); 
struct hw_instance *get_hw_instance_by_path(const char* path);

void create_hw_connection(struct hw_connection* elem);

void create_hw_group(struct hw_group *elem);
struct hw_group *get_hw_group_by_name(const char* name);
struct hw_group *get_hw_group_by_path(const char* path);
void add_instance_to_hw_group(struct hw_group* group, struct hw_instance* elem);

void create_sw_component(struct sw_component* elem);
struct sw_component *get_sw_component_by_name(const char* name);
struct sw_component *get_sw_component_by_path(const char* path);

void create_sw_instance(struct sw_instance* elem);
struct sw_instance *get_sw_instance_by_name(const char* name);
struct sw_instance *get_sw_instance_by_path(const char* path);

void create_sw_task(struct sw_task* elem);
struct sw_task *get_sw_task_by_name(const char* name);
struct sw_task *get_sw_task_by_path(const char* path);

void create_sw_allocation(struct sw_allocation* elem);

#endif

//==========================================================================
//
// Author(s): Hector Posadas
// Contributors:
// Description:   Functions to manage a simple list of pointers
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

#include "xml_list.h"
#include <stdlib.h>


/**
 * \brief Return the size of the list
 *
 * \param header Pointer to the list (pointer to the first element)
 * \return The list size
 */
int xml_list_size(struct list_elem** header){
	
	struct list_elem *elem;
	int num ;
	
	if( *header==NULL) return 0;
	elem = *header;
	for(num = 0; elem; num++){
		elem = elem->next;
	}
	return num;
}

/**
 * \brief Add an new element to the end of the list
 *
 * \param header Pointer to the list (pointer to the first element)
 * \param data Pointer to the new data to be stored
 * \return
 */
void xml_list_add_element(struct list_elem** header, void* data){
	struct list_elem *elem, *new_elem;
	int num ;
	
	new_elem = (struct list_elem *) malloc(sizeof(struct list_elem));
	new_elem->elem = data;
	new_elem->next = NULL;

	if( *header==NULL) {
		*header = new_elem;
		return;
	}
	elem = *header;
	while(elem->next){
		elem = elem->next;
	}
	elem->next = new_elem;
}

/**
 * \brief Peek the last element of the list
 *
 * \param header Pointer to the list (pointer to the first element)
 * \param 
 * \return pointer to the last element of the list
 */
void *xml_list_last_element(struct list_elem* header){
	struct list_elem *elem;

	if( header==NULL) return NULL;
	elem = header;
	while(elem->next){
		elem = elem->next;
	}
	return elem->elem;	
}

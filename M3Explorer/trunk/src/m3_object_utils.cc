/* @M3EXPLORER_LICENSE_START@
 *
 * This file is part of the Multicube Explorer tool.
 *
 * Authors: Vittorio Zaccaria, Gianluca Palermo, Giovanni Mariani, Fabrizio Castro
 * Copyright (c) 2008-2009, Politecnico di Milano, Universita' della Svizzera italiana
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 
 *  * Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 * Neither the name of Politecnico di Milano nor Universita' della Svizzera Italiana 
 * nor the names of its contributors may be used to endorse or promote products 
 * derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This work was supported by the EC under grant MULTICUBE FP7-216693.
 * 
 * @M3EXPLORER_LICENSE_END@ */





 
#include <iostream>
#include <m3_object.h>
#include <m3_list.h>
#include <m3_map.h>
#include <m3_vector.h>
#include <m3_point.h>
#include <string>
#include <set>

extern void prs_display_error(const string &);
extern void prs_display_message(const string &);

//#define DEBUG

/* 
 * Defines macros to deal with unallocated memory objects
 */

set<m3_object *> *allocated=NULL;

string obj_name(m3_object *x)
{
	if(is_a<m3_integer *>(x)) return "m3_integer";
	if(is_a<m3_double *>(x)) return "m3_double";
	if(is_a<m3_string *>(x)) return "m3_string";
	if(is_a<m3_list *>(x)) return "m3_list";
	if(is_a<m3_map *>(x)) return "m3_map";
	if(is_a<m3_vector *>(x)) return "m3_vector";
	if(is_a<m3_point *>(x)) return "m3_point";
	return "unknown object";
}

void create_new_object(int &N, m3_object *x)
{
	if((allocated)==NULL) allocated = new set<m3_object *>();
#if defined(DEBUG)
	cout << "allocating " << x << endl;
#endif
	N++;
	allocated->insert(x);	
#if defined(DEBUG)
	cout << "object allocated at " <<  allocated << "   " << allocated->count(x) << " size: " << allocated->size() << "\n";
	cout << "object name " << obj_name(x) << endl;
#endif
}

void destroy_and_print_name(int &N, m3_object *x)
{
#if defined(DEBUG)
	cout << "destroying " << x << endl;
#endif
	if(allocated->count(x)) 
    {
		allocated->erase(allocated->find(x));
#if defined(DEBUG)
		cout << "object allocated at " <<  allocated << "   " << allocated->count(x) << " size: " << allocated->size() << "\n";
#endif
	}

	else 
    { 
#if defined(DEBUG)
        cout << "Warning, destroying an object that was not allocated (" << x << ")" << endl; 
#endif
    }
	N--;
#if defined(DEBUG)
	if(!allocated->size()) 
        prs_display_message("Memory correctly deallocated");
#endif
}

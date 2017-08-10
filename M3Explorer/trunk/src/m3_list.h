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





 
#ifndef M3_LIM3
#define M3_LIM3

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <m3_object_utils.h>
#include <m3_object.h>

using namespace std;

/** 
 * List class derived from object.
 */ 

class m3_list: public m3_object
{
    friend vector<string> get_string_vector(m3_object *);
    friend vector<double> get_double_vector(m3_object *);

    /** The object list */
	list <m3_object *> the_list;
	public:

    /** Copies two lists. The destination list is erased.
     * @param from source
     * @param to destination.
     */
	void copy(m3_list *from, m3_list *to) ;

	m3_list(); 
	m3_list(m3_list &l);
	~m3_list(); 

    /** Inserts a copy of an object into the list.
     * @param o the object to be inserted.
     */
	void 			insert(m3_object &o);  

    /** Returns a string representation of the list
     * @return the representation 
     */
	string			print() ;

    /** Iterators associated to the list. */ 
	list<m3_object *>::iterator begin() ;
	list<m3_object *>::iterator end() ;

    /** Returns the last element of the list */
	m3_object *	back() ;

    /** Removes an object from the list */
	void			remove(m3_object *);

    /** Erase the element at the specified position */
	void			erase(list<m3_object *>::iterator&);

    /** Attachs a list to the current one */
	void 			concat(m3_list *);

    /** Returns true if the list is empty */
    bool empty();

    /** Returns the size of the list */
    size_t          size();

	protected:	
    /** Generates a copy of the list */
	m3_object *		gen_copy() ;
};

#endif

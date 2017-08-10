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





#ifndef M3_MAP
#define M3_MAP

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <m3_object_utils.h>
#include <m3_object.h>

typedef map<string,m3_object *>::const_iterator stl_map_const_iter;

/** Map iterator */
struct m3_map_const_iterator: public stl_map_const_iter
{
	m3_map_const_iterator() {};
	m3_map_const_iterator(const stl_map_const_iter &i): stl_map_const_iter(i) {}
};


/** Map derived from the object class */
class m3_map: public m3_object
{
        /** The actual data structure */
		map <string, m3_object *> the_map;

        /** Creates a copy of the map */
		void copy(m3_map *from, m3_map *to) ;

		public:
		m3_map();
		m3_map(m3_map &l);
		~m3_map();

        /** Inserts an element into the map */
		void            insert(string n, m3_object &o);
        
        /** Gets an element from the map using a string name */
		bool  			get(string,m3_object *&) ;
        
        /** Returns the size of the map */
		size_t			size();

        /** Prints the content of the map */
		string          print() ;

        /** Starting iterator for the map */
		m3_map_const_iterator   begin() { return  m3_map_const_iterator(the_map.begin());}
        
        /** Final iterator for the map */
		m3_map_const_iterator   end() { return  m3_map_const_iterator(the_map.end());}

        /** Copies the values of the map into the properties of an object */
		void			fill_properties_of(m3_object *o) ;

		protected:
        /** Generates a copy of the object */
		m3_object *             gen_copy() ;
};
#endif

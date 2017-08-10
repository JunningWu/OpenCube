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





#ifndef M3_VECTOR
#define M3_VECTOR

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <m3_object_utils.h>
#include <m3_object.h>

/** Vector class derived from object */
class m3_vector: public m3_object
{
    friend vector<string> get_string_vector(m3_object *);
    friend vector<double> get_double_vector(m3_object *);

    /** The data structure containing the vector elements */
    vector <m3_object *> the_vector;
    public:

    /** Copyes two vectors */
    void copy(m3_vector *from, m3_vector *to) ;

    /** Constructor */
    m3_vector();
    
    /** Copy constructor */
    m3_vector(m3_vector &l);
    
    /** Copy constructor */
    m3_vector(m3_object **v, int n);
    
    /** Destructor */
    ~m3_vector();

    /** Inserts a copy of the object to the specified position */
    void                    insert(int n, m3_object &o);

    /** Returns a copy of the object to the specified position */
    m3_object &	    get(int) ;

    /** Returns the size of the vector */
    size_t			        size() ;
    
    /** Returns a string representation of the vector */
    string                  print() ;

    protected:
    /** Generates a copy of the object */
    m3_object *             gen_copy() ;

    public:
    /** Helper function **/
    string                  get_string_at(int) ;
};

/** Converts the object to a vector of strings (if possible) */
vector<string> get_string_vector(m3_object *);

/** Converts the object to a vector of doubles (if possible) */
vector<double> get_double_vector(m3_object *);

#endif


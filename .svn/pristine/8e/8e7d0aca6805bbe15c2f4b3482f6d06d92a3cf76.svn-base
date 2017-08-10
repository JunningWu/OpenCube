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





#ifndef M3_POINT
#define M3_POINT

class m3_point;

#include <m3_env.h>
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <m3_object_utils.h>
#include <m3_object.h>

#define M3_POINT_NO_ERROR           0 
#define M3_POINT_FATAL_ERROR        1
#define M3_POINT_NON_FATAL_ERROR    2

/** 
 * The point derived from the object class. It is a vector of integers.
 */
class m3_point: public m3_object, public vector<int> 
{
		public:
	
        /** Constructor */    
		m3_point();
        /** Constructor */    
		m3_point(m3_point &p);
        /** Constructor */    
		m3_point(const m3_point &p);
        /** Constructor */    
		m3_point(int);
        /** Destructor */
		~m3_point();

        /** Inserts the value into the specified position 
         * @param pos the position into which the value should be put
         * @param value */
		void insert(int pos,int value);

        /** Returns a string representation of the point */
		string                  print() ;

        /** Generates a copy of the point */
		m3_object *             gen_copy() ;

        /** Returns the specified metric associated to the point */
        double                  get_metric(int) ;

        /** Returns the objective associated to the point */
        double                  get_objective(m3_env *, int);

        /** Sets an error on the point */
        void                    set_error(int type, string error);

        /** Returns the error state of the point */
        int                     get_error(string & error);

        /** Outputs the point value */
        void                    print_point_value(m3_env *);

        /** Clears the metrics associated to the point */
        void                    clear_metrics(m3_env *);

        /** Checks the consistency of the point */
	    bool                    check_consistency(m3_env *);

        /** Copies the point from another point */
        void                    copy_from(m3_point &);
        
	/** Returns true if 'this' is equal to the point passed as parameter */
        bool			is_equal(m3_point &);
};

#endif

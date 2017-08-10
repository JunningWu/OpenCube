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





#ifndef M3_UTIL
#define M3_UTIL

using namespace std;
 
#include <assert.h>

class m3_object;

/* 
 * Defines macros to deal with unallocated memory objects
 * and other functions to deal with possible exceptions during the execution
 */

#ifdef DEBUG 
# define obj_destroy(N) destroy(N);
#else 
# define obj_destroy(N)
#endif

#ifdef DEBUG
# define obj_create(N) { create_new_object(N,((m3_object *) this)); }
#else
# define obj_create(N)
#endif

#ifdef DEBUG
# define obj_destroy_n(N,x) destroy_and_print_name(N,x);
#else
# define obj_destroy_n(N,x)
#endif

extern void destroy_and_print_name(int &, m3_object *); 
extern void create_new_object(int &, m3_object *); 
extern string obj_name(m3_object *);

/* 
 * assertions and weak assertions
 */

#define m3_assert(expr) assert(expr)
#endif

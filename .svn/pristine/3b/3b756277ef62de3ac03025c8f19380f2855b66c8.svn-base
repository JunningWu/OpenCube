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





#ifndef _OPT_UTILS_H 
#define _OPT_UTILS_H 

#include <m3_shell_variables.h>
#include <m3_env.h>
#include <m3_object.h>
#include <m3_list.h>
#include <m3_optimizer.h>

/** 
 * Loads the specified optimizer plug-in within the shell.
 *
 * @param env the M3 env
 * @param opt_name the name of the optimizer.
 * @return true if everything went fine when loading the optimizer.
 */
bool opt_select_optimizer(m3_env *env, string &opt_name );

/** 
 * Compute the cost function associated to the point. The cost function
 * is actually monotonically dependent on the geometric average of the objectives.
 *
 * @param env the M3 env
 * @param p the point for which the cost function should be computed
 *
 * @return the value of the cost function.
 */
double 		     opt_compute_cost_function(m3_point & p, m3_env *env);

/** 
 * Converts the point representation and the associated metrics into a csv string
 *
 * @param env the M3 env
 * @param p the point for which the csv representation should be computed
 *
 * @return the csv representation of the point.
 */
string           opt_transform_point_in_csv_string_vector(m3_point p, m3_env *env);

m3_database *opt_execute_doe_and_optimizer(m3_env *env, string doe_name, string opt_name);

extern bool dont_look_at_objectives;

#endif 


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






#ifndef SIM_UTILS_H
#define SIM_UTILS_H

#include <m3_env.h>
#include <m3_point.h>
#include <m3_object.h>
#include <m3_opt_utils.h>
#include <math.h>

/** Returns the point at the specified iterator */
#define sim_get_point_at_iterator(i) \
    dynamic_cast<m3_point &>(**i)

/** Returns the metric of the point */
#define sim_get_metric_from_point(p,m,env) \
    sim_get_metrics_stats(&const_cast<m3_point &>(p), m,env)

/** Returns the metric or the statistic associated to the point */
double          sim_get_metrics_stats(m3_point *p, string metric_stat_name, m3_env *env);

/** True if the name is a real parameter */
bool 	        sim_is_a_parameter(m3_env *, string name);

/** Computes the optimum with respect to the cost function */
void  	        sim_show_optimum(m3_env *env);

/** Computes the pareto set of a db passed as an argument */
m3_database* 	sim_compute_pareto_on_local_db(m3_env *env, m3_database* full_db);

/** Computes the pareto set of the current db */
void            sim_compute_pareto(m3_env *);

/** Computes the pareto set given a new point */
bool            sim_recompute_pareto_with_new_point(m3_env *, m3_database *, m3_point * );

/** Computes the ADRS between source and reference */
bool            sim_compute_ADRS(m3_env *env, string reference, string source, double &ADRS);

/** Computes the dominant points in the current db with respect to point */
int		        sim_count_pareto_dominants(m3_env *env, m3_point *poin);

/** Evaluates the point in the experimental design */
int             sim_evaluate_doe(m3_env *);

/** Executes the shell_command */
bool shell_command(string command);

#define randint(min, max) ((random() % ((max)-(min)+1)) + (min))
#define randfloat() (((float) randint(0, RAND_MAX))/((float) RAND_MAX))
#define powint(x, y) ((int) (pow(x,y)))

#endif

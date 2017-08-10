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
#include <m3_map.h>
#include <m3_vector.h>
#include <m3_shell_variables.h>
#include <m3_parser.h>
#include <m3_optimizer.h>
#include <m3_opt_utils.h>
#include <m3_sim_utils.h>
#include <vector>
#include <stdio.h>
#include <math.h>

class m3_aprs: public m3_optimizer
{
		int update_current_db(m3_env *,int);
    public:
        m3_aprs() {}
        string get_information();
        void explore(m3_env *env);
};

string m3_aprs::get_information()
{
        return "Adaptive Pareto Random Search optimizatation";
}

int m3_aprs::update_current_db(m3_env *env, int window_size)
{
	int number_of_points=0;
    m3_list *database_list = env->available_dbs[env->current_db_name]->get_list_of_points();
	list<m3_object *>::iterator i_db_list=database_list->begin();
	int database_list_size =database_list->size();
    for(int i_database_list_size=0; i_database_list_size<database_list_size; i_database_list_size++)
    {   
        m3_point & p =sim_get_point_at_iterator(i_db_list);
        m3_point *tmp = env->current_design_space->get_random_point_at_distance_N(env,&p,window_size);
        m3_point my_point(*tmp);
        delete tmp;
        if(env->current_design_space->is_valid(env, & my_point))
        {
			string error;
			prs_display_message("Evaluating point: " + env->current_design_space->get_point_representation(env, my_point));
			env->current_design_space->evaluate(env, &my_point);
		
			if(!(my_point.get_error(error)==M3_POINT_FATAL_ERROR)) env->available_dbs[env->current_db_name]->insert_point(&my_point);
			else
			{
				/* missing */
				return number_of_points;
			}				
			
			number_of_points++;
		}
		else
        {   
            prs_display_message("Skipping point: " + env->current_design_space->get_point_representation(env, my_point));
        }
		i_db_list++;
	}
	return number_of_points;
}

void m3_aprs::explore(m3_env *env)
{
	
	if(!env->current_driver) {
		prs_display_error("no driver defined");
		return;
	}
	
	prs_display_message("Starting with the Adaptive Pareto Random Serach optimization process");
	
	double window_size;
	double alpha;
	int number_of_points=0;
	int simulated_points=0;

    if (!get_double_from_variables(env, "m3_aprs_initial_window", window_size)) window_size = 2.0; 
    if (!get_double_from_variables(env, "m3_aprs_alpha", alpha)) alpha = 0.9;
	if (!get_integer_from_variables(env, "m3_aprs_number_of_points", number_of_points)) number_of_points = 15;

	simulated_points += sim_evaluate_doe(env);
	sim_compute_pareto(env);
	while(number_of_points > simulated_points)
	{	
        int new_sim_point = update_current_db(env,(int)ceil(window_size));
		simulated_points += new_sim_point;
        if(new_sim_point == 0)
        {
            prs_display_error("APRS was not able to find initial feasible solutions.");
            break;
        }
		sim_compute_pareto(env);
		window_size = window_size * alpha;
	}
}

extern "C"
{
    m3_optimizer *opt_generate_optimizer()
    {
	    return new m3_aprs();
    }
}

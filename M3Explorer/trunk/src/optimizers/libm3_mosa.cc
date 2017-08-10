/* @M3EXPLORER_LICENSE_START@
 *
 * This file is part of the Multicube Explorer tool.
 *
 * Authors: Vittorio Zaccaria, Gianluca Palermo, Giovanni Mariani, Fabrizio Castro, Stefano Bolli (2009)
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


/* @additional_authors @, Stefano Bolli (2009)@ */


/*
 * Derived from Smith & al. "Dominance based Multi-objective Simulated annealing"
 *
 */

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

class m3_mosa: public m3_optimizer
{
		bool probabilistic_acceptance(double);
		string original_current_db_name;
    public:
        m3_mosa() {}
        string get_information();
        void explore(m3_env *env);
};

string m3_mosa::get_information()
{
        return "MOSA: Multi-Objective Simulated Annealing optimizatation";
}

bool m3_mosa::probabilistic_acceptance(double probability)
{
    double x = (double)(rand()%10000)/10000;
	if(x <= probability)
        return true;
    else 
        return false;
}

void m3_mosa::explore(m3_env *env)
{
	
	if(!env->current_driver) {
		prs_display_error("no driver defined");
		return;
	}
	
	prs_display_message("Starting with the MOSA optimization process");
	
	double temperature=1.0;
	double temperature_decrease_coefficient;
	int epochs;
	int perturbation_window;
	int epoch_lenght;

    if (!get_integer_from_variables(env, "m3_mosa_epochs", epochs)) epochs = 10; 
    if (!get_double_from_variables(env, "m3_mosa_temperature_decrease_coefficient", temperature_decrease_coefficient)) temperature_decrease_coefficient = 0.85;
	if (!get_integer_from_variables(env, "m3_mosa_epoch_lenght", epoch_lenght)) epoch_lenght = 100;
    if (!get_integer_from_variables(env, "m3_mosa_perturbation_window", perturbation_window)) perturbation_window = 1; 
	

        /*initial feasible solution*/
        // by Stefano Bolli
        m3_point * my_point = NULL;
        string error;
	do
	{
        if(my_point!=NULL)
            delete my_point;
	    my_point= (m3_point*) (env->current_design_space->random_point(env)).gen_copy();	
	    
	    prs_display_message("Evaluating point: " + env->current_design_space->get_point_representation(env, *my_point));	
	    env->current_design_space->evaluate(env, my_point);
            #ifdef DEBUG
	        cout<<"start point evaluation, get_error value: "<<my_point->get_error(error)<<endl;
	        cout<<"start point evaluation, check_consistency value: "<<my_point->check_consistency(env)<<endl;
	        cout<<"start point evaluation, is_valid value: "<<env->current_design_space->is_valid(env, my_point)<<endl;
            #endif
	}while(env->current_design_space->is_valid(env, my_point)==false);

	env->available_dbs[env->current_db_name]->insert_point(my_point);

	original_current_db_name = env->current_db_name;

	for(int i_epochs=0; i_epochs<epochs; i_epochs++)
	{
		for(int i_epoch_lenght=0; i_epoch_lenght<epoch_lenght; i_epoch_lenght++)
		{		
	    // my_point perturbation step (4)
            m3_point * my_new_point = (env->current_design_space->get_random_point_at_distance_N(env,my_point,perturbation_window));
	    // m3_point my_new_point(*(env->current_design_space->get_random_point_at_distance_N(env,&my_point,perturbation_window)));
            // m3_point my_new_point(*mnp);
            // delete mnp;
	   prs_display_message("Evaluating point: " + env->current_design_space->get_point_representation(env, *my_new_point));	
	   env->current_design_space->evaluate(env, my_new_point);
           
/*	   int err_check = my_new_point->get_error(error);
           int err_code=2;
           if(!(err_code = my_new_point->get_error(error)))
           if(err_check==true)            
           {
*/
	    // by Stefano Bolli use is_valid() instead of get_error that work not correcly
	    
            string error;
            int err_code = my_new_point->get_error(error);
	    if(env->current_design_space->is_valid(env, my_new_point)==true)
//          if(!(err_code = my_new_point->get_error(error)))
            {
            
			    //F_tilde Initialization step (9)
			    env->available_dbs["m3_mosa_F_tilde"]= new m3_database();
			    env->current_db_name= "m3_mosa_F_tilde";
			    env->available_dbs[env->current_db_name]->insert_point(my_point);				
			    env->available_dbs[env->current_db_name]->insert_point(my_new_point);
			
			    //Energy difference calculation on F_tilde step(11)
			    double energy_difference = (double)(sim_count_pareto_dominants(env,my_new_point) - sim_count_pareto_dominants(env, my_point))/env->available_dbs[env->current_db_name]->size();

			    //probabilistic acceptance step(12-13)
			    if(probabilistic_acceptance(min(1.0, exp(-energy_difference/temperature))))
			    {
			    	my_point->copy_from(*my_new_point);
			    	env->current_db_name=original_current_db_name;

                    if(sim_recompute_pareto_with_new_point(env, env->available_dbs[env->current_db_name], my_point))
                    {
                        env->available_dbs[env->current_db_name]->insert_point(my_point);
                    }
			    }
			    delete env->available_dbs["m3_mosa_F_tilde"];
            }
            else
            {
                if(err_code == M3_POINT_FATAL_ERROR)
                {
		            prs_display_error("FATAL ERROR");
                    return;
                }
                //else
                //    env->available_dbs[env->current_db_name]->insert_point(my_new_point);
            }
            delete my_new_point;
        }
		temperature = temperature * temperature_decrease_coefficient;
	}
	env->available_dbs.erase("m3_mosa_F_tilde");
	env->current_db_name=original_current_db_name;
	sim_compute_pareto(env);

    delete my_point;
}


extern "C"
{
    m3_optimizer *opt_generate_optimizer()
    {
	    return new m3_mosa();
    }
}

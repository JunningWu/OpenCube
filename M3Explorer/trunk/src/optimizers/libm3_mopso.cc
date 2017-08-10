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
#include <vector>
#include <m3_object.h>
#include <m3_map.h>
#include <m3_vector.h>
#include <m3_shell_variables.h>
#include <m3_parser.h>
#include <m3_optimizer.h>
#include <m3_opt_utils.h>
#include <m3_sim_utils.h>
#include <m3_database.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <math.h>

class m3_mopso: public m3_optimizer
{
    int explored_points;
    public:
    m3_mopso() 
    {
    }
    ~m3_mopso()
    {
    }
    string get_information();
    void explore(m3_env *env);
    m3_point generate_random_point(m3_env *env, int num_of_prs);
    vector<int> generate_random_speed(m3_env *env, int num_of_prs);
    void find_gbest(m3_env *env, vector<m3_point*> &, vector<int> &, m3_point *);
};

string m3_mopso::get_information()
{
        return "Multi-objective Particle Swarm Optimizer - (iterations, pareto_metrics, sub_swarm_size, sub_swarm_number, p2, max_exponent) => PSO";
}


void m3_mopso::find_gbest(m3_env *env, 
                          vector<m3_point*> & swarm_particles, 
                          vector<int> & exponents, 
                          m3_point * current_gbest)
{
    for(int j = 0; j<swarm_particles.size(); j++)
    {
        long double costf = 1;
        string error;
        if(!swarm_particles[j]->get_error(error) && swarm_particles[j]->check_consistency(env))
        {
            for(int k=0; k<env->current_design_space->objectives.size(); k++)
            {
                long double metric_value_j = (swarm_particles[j]->get_objective(env, k))/(current_gbest->get_objective(env, k));
                costf *= pow(metric_value_j, exponents[k]);
            }
            if(costf < 1) current_gbest -> copy_from(*swarm_particles[j]);
        }
    }
}

vector<int> m3_mopso::generate_random_speed(m3_env *env, int num_of_prs)
{
	vector<int> actual_speed;
    actual_speed.resize(num_of_prs);

    for(int index=0; index < num_of_prs ; index++)
    {
        int pos = randint(-1, +1);
        actual_speed[index] = pos;
    }

    return actual_speed;
}

#define delta(x) (((x)>0) ? 1 : (((x)<0) ? -1 : 0))

void m3_mopso::explore(m3_env *env)
{
	if(!env->current_driver) 
    {
		prs_display_error("No driver defined");
        return;
	}
	prs_display_message("Starting with the Particle Swarm optimization process");

	int iterations;
    int sub_swarm_size;
    double W;
    /* double C1; */
    double C2;
    double p2;
    int max_exponent;
    explored_points = 0;
    

	if (!get_integer_from_variables(env,"iterations", iterations)) 
    {
        iterations = 5; //200; /* M. Reyes suggests 50,100, 200 */
    }
	
    if (!get_integer_from_variables(env,"sub_swarm_size", sub_swarm_size)) 
    {
        sub_swarm_size = 10; //20; /* M. Reyes suggests total swarm size at 50,100, 200 */
    }

    int sub_swarm_number;
    if (!get_integer_from_variables(env,"sub_swarm_number", sub_swarm_number)) 
    {
        sub_swarm_number = 5; 
    }
    
    if (!get_integer_from_variables(env,"max_exponent", max_exponent)) 
    {
        max_exponent = 3;
    }
    int max_errors; 
    if (!get_integer_from_variables(env,"max_errors", max_errors)) 
    {
        max_errors = 100;
    }
	
    if (!get_double_from_variables(env,"p2", p2)) 
    {
        p2=0.8; 
    }

	int num_of_prs=env->current_design_space->ds_parameters.size();
	
    unsigned int objectives_size = env->current_design_space->objectives.size();

    vector<vector<int> > exponents_for_swarm;

    vector<vector<m3_point *> > x;

    vector<m3_point *> gbest;

    
    vector<vector<vector<int> > > v;

    exponents_for_swarm.resize(sub_swarm_number);
    x.resize(sub_swarm_number);
    v.resize(sub_swarm_number);

    gbest.resize(sub_swarm_number);

    //Initialize. 
    //Compute the exponents for the cost functions 
    for(int i=0; i<sub_swarm_number; i++)
    {
        exponents_for_swarm[i].resize(objectives_size);
        gbest[i] = (m3_point*)(env->current_design_space->random_point(env)).gen_copy();
            env->current_design_space->evaluate(env, gbest[i]);
            prs_display_message("Evaluating point: " + env->current_design_space->get_point_representation(env, *gbest[i]));
            env->available_dbs[env->current_db_name]->insert_point(gbest[i]);


        for(int j=0; j<objectives_size; j++)
        {
            exponents_for_swarm[i][j] = randint(1, max_exponent);
        }
        x[i].resize(sub_swarm_size);
        v[i].resize(sub_swarm_size);
        for(int j=0; j<sub_swarm_size; j++)
        {
            // Initialize position 
            x[i][j] = (m3_point*)(env->current_design_space->random_point(env)).gen_copy();
            env->current_design_space->evaluate(env, x[i][j]);
            prs_display_message("Evaluating point: " + env->current_design_space->get_point_representation(env, *x[i][j]));
            
            env->available_dbs[env->current_db_name]->insert_point(x[i][j]);

            // Initialize speed
            v[i][j] = generate_random_speed(env, num_of_prs);
        }
    }
	


    //prs_display_message("HERE NO PROBLEM");
    // Real PSO algortihm body
    for(int t=0; t<iterations;t++)
    {
        for(int i=0; i<sub_swarm_number; i++)
        {   
            find_gbest(env, x[i],exponents_for_swarm[i], gbest[i]);
            for(int j=0; j<sub_swarm_size; j++)
            {
                int errors=0;
                bool finished = false;
                while(errors<max_errors && !finished)
                {
                    double r2 = randfloat(); 
                    // for each particle recompute position and speed 
                    for(int k=0; k<num_of_prs; k++)
                    {
                        if(r2<p2)
                            v[i][j][k] = delta((*gbest[i])[k] - (*x[i][j])[k]);
                        else
                            v[i][j][k] = randint(-1, +1);
                       
                        (*x[i][j])[k] = (*x[i][j])[k] + v[i][j][k];
                    }

                    x[i][j]->clear_metrics(env);
                    *x[i][j] = env->current_design_space->consolidate(env, *x[i][j]);
                    if(env->current_design_space->is_valid(env, x[i][j]))
                    {
                        env->current_design_space->evaluate(env, x[i][j]);
                        if( env->available_dbs[env->current_db_name]->find_point( *(x[i][j]) )==NULL )
                            env->available_dbs[env->current_db_name]->insert_point(x[i][j]);

                        prs_display_message("Evaluating point: " + env->current_design_space->get_point_representation(env, *x[i][j]));

                        explored_points ++;
                        string error;
                        if(!(*x[i][j]).get_error(error) && (*x[i][j]).check_consistency(env))
                        {
                            finished = true;
                        }
                        else
                            errors ++;
                    }
                    else
                        errors ++;
                }
            }
        }
    }

    for(int i=0; i<sub_swarm_number; i++)
    {    
        for(int j=0; j<sub_swarm_size; j++)
            delete x[i][j];
        delete gbest[i];
    } 
    sim_compute_pareto(env);

}


extern "C"
{
    m3_optimizer *opt_generate_optimizer()
    {
        return new m3_mopso();
    }
}


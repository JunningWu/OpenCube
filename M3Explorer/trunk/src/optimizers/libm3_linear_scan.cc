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

class m3_linear_scan: public m3_optimizer
{
    public:
        m3_linear_scan() {}
        string get_information();
        void explore(m3_env *env);
};

string m3_linear_scan::get_information()
{
    return "Linear scan optimizer";
}

extern bool dont_look_at_objectives;


void m3_linear_scan::explore(m3_env *env)
{
    bool finished = false;
    m3_assert(env->current_driver);
    prs_display_message("Starting with the linear scan optimization process");

//    vector<m3_point> design_of_experiments = env->current_doe->generate_doe(env);

    m3_point actual_point; 

    int experiment = 0;
//    int exp_size = design_of_experiments.size();

    bool temporary_save = false;

    string source_database = "root";
    int   gran = 10;
    int points = 0;

    m3_database *result = env->available_dbs[env->current_db_name]; 

    m3_list *database_list = env->available_dbs[source_database]->get_list_of_points();
    list<m3_object *>::iterator c;
    for(c=database_list->begin();c!=database_list->end(); c++)
    { 
        m3_point & p =sim_get_point_at_iterator(c);
        if(p.check_consistency(env) && env->current_design_space->is_within_scalar_bounds(env, p))
        {
            if(env->current_design_space->is_valid(env, &p))
            {
                if(!dont_look_at_objectives)
                {   
                    if(sim_recompute_pareto_with_new_point(env, result, &p))
                    {
                        result->insert_point(&p);
                    }
                }
                else
                {
                    result->insert_point(&p);
                }
            }
        }
    }
    cout << endl;
}

extern "C"
{
    m3_optimizer *opt_generate_optimizer()
    {
        return new m3_linear_scan();
    }
}

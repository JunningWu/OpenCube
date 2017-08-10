/* @M3EXPLORER_LICENSE_START@
 *
 * This file is part of the Multicube Explorer tool.
 *
 * Authors: Vittorio Zaccaria, Gianluca Palermo, Giovanni Mariani, Fabrizio Castro,  Al-Hissi Mohammad (2009), Leandro Fiorin (2009)
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

/* @additional_authors @,  Al-Hissi Mohammad (2009), Leandro Fiorin (2009)@*/


#include <iostream>
#include <sstream>
#include <m3_object.h>
#include <m3_map.h>
#include <m3_vector.h>
#include <m3_shell_variables.h>
#include <m3_parser.h>
#include <m3_optimizer.h>
#include <m3_opt_utils.h>
#include <m3_sim_utils.h>

class m3_point_femo : public m3_point
{
	public:
	m3_point_femo();
	m3_point_femo(m3_point &p);
	int w;
};

m3_point_femo::m3_point_femo():m3_point() {}
m3_point_femo::m3_point_femo(m3_point &l):m3_point(l){w=0;}

class m3_femo: public m3_optimizer
{
	public:
	m3_femo() {}
	string get_information();
	void explore(m3_env *env);
};

string m3_femo::get_information()
{
	return "Fair Evolutionary Multi-objective Optimizer";
}

void m3_femo::explore(m3_env *env)
{
	prs_display_message("Starting with the FEMO optimization process");
	m3_assert(env->current_driver);

	bool temporary_save = false;
	string filename;
	if (get_string_from_variables(env, "temp_database", filename)) 
		temporary_save = true; 

	//doe_orig contains set of random solutions
	prs_display_message("Generating Some Random Points and pick one of them");

	m3_point firstSol;
	bool first_sol_found = false;
	//get any solution
	for(;;)
	{
		vector<m3_point> doe_orig = env->current_doe->generate_doe(env);
		int doe_orig_size = doe_orig.size();
		for(int i=0; i<doe_orig_size; i++)
		{
			if(env->current_design_space->is_valid(env, & doe_orig[i]))
			{
				firstSol = doe_orig[i];
				first_sol_found = true;
				break;
			}
		}
		if(first_sol_found == true)
			break;
	}

	env->current_design_space->evaluate(env, & firstSol);
	if(sim_recompute_pareto_with_new_point(env, env->available_dbs[env->current_db_name], &firstSol))
	{
		env->available_dbs[env->current_db_name]->insert_point(& firstSol);
	}

	//push it in P
	vector<m3_point_femo> P;
	P.push_back(firstSol);
	P[0].w = 0;

	int obj_size = env->current_design_space->objectives.size();
	
    int generations;
    if(!get_integer_from_variables(&current_environment,"generations", generations))
	{
		prs_display_message("No 'generations' defined; defaulting to 100 generations as stopping criteria");
		generations = 100;
	}
	int gen;

	//start the main loop
	prs_display_message("Starting the generation process");
	for(gen=0; gen<generations; gen++)
	{
		int pop_size = P.size();
		vector<m3_point_femo> newP;
		//generate random index between 0 and pop_size-1

		//check min_w
		int min_w = P[0].w;
		for(int i=0; i<pop_size; i++)
		{
			if(P[i].w < min_w)
				min_w = P[i].w;
		}
		//allocate memory with size of pop_size
		vector<int> min_w_points;
		//push the index of points with min_w
		for(int i=0; i<pop_size; i++)
		{
			if(P[i].w == min_w)
				min_w_points.push_back(i);
		}
		//count the number of pushes wich = numer of points with min_w
		//create random number between zero and the (numer of points with min_w-1)
		int min_w_points_size = min_w_points.size();
		double rando = (double)rand() / (double)RAND_MAX;
		int rand_index_2 = rando * min_w_points_size;
		int rand_index = min_w_points[rand_index_2];
		min_w_points.clear();
		//access the pushed indeces to get a random point with min_w
		//this point is the parent, increment w of it
		m3_point parent = P[rand_index];
		P[rand_index].w = P[rand_index].w + 1;
		m3_point child = env->current_design_space->genetic_mutation(env, &parent, 0.1);
		env->current_design_space->evaluate(env, & child);
		if(sim_recompute_pareto_with_new_point(env, env->available_dbs[env->current_db_name], & child))
		{
			env->available_dbs[env->current_db_name]->insert_point(& child);
		}

		for(int i=0; i<pop_size; i++)
		{
			bool old_is_better = false;
			for(int j=0; j<obj_size; j++)
			{
				if(P[i].get_objective(env, j) < child.get_objective(env, j))
				{
					old_is_better = true;
					break;
				}
			}
			if(old_is_better == true)
			{
				newP.push_back(P[i]);
			}
		}
		P.clear();
		for(int i=0; i < newP.size(); i++)
		{
			P.push_back(newP[i]);
		}
		newP.clear();

		pop_size = P.size();
		bool add = true;
		for(int i=0; i<pop_size; i++)
		{
			bool new_is_better = false;
			for(int j=0; j<obj_size; j++)
			{
				if(P[i].get_objective(env, j) > child.get_objective(env, j))
					new_is_better = true;
			}

			//new child didn't successeed to be better in any objective
			if(new_is_better == false)
			{
				add = false;
				break;
			}
		}
		if (add == true)
		{
			P.push_back(m3_point_femo(child));
		}
	}
}


extern "C"
{
    m3_optimizer *opt_generate_optimizer()
    {
        return new m3_femo();
    }
}


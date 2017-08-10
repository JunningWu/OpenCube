/* @M3EXPLORER_LICENSE_START@
 *
 * This file is part of the Multicube Explorer tool.
 *
 * Authors: Vittorio Zaccaria, Gianluca Palermo, Giovanni Mariani, Fabrizio Castro, Leandro Fiorin (2009)
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

/* @additional_authors @, Leandro Fiorin (2009)@ */


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
#include <limits.h>
//#include <stdlib.h>
//#include <stdio.h>

class m3_point_gemo : public m3_point
{
	public:
	m3_point_gemo();
	m3_point_gemo(m3_point &p);
	int w;
};

m3_point_gemo::m3_point_gemo():m3_point() {}
m3_point_gemo::m3_point_gemo(m3_point &l):m3_point(l){}


class m3_gemo_func: public m3_optimizer
{
	public:
	m3_gemo_func() {}
	void explore(m3_env *env);
	string get_information();
	m3_point get_initial_individual(m3_env *env);
	int select_element(vector<m3_point_gemo> P);
	m3_point create_offspring(m3_env *env, m3_point parent);
	bool is_dominant(m3_env *env, m3_point x, m3_point y);
};

void m3_gemo_func::explore(m3_env *env)
{
	char tempInt [50];

	prs_display_message("Starting with the GEMO optimization process");
	m3_assert(env->current_driver);

	bool temporary_save = false;
	string filename;
	if (get_string_from_variables(env, "temp_database", filename)) 
		temporary_save = true; 

	// Choose an initial individual x uniformly from X
	m3_point firstSol = get_initial_individual(env);
	
	// P <-- {x} 
	vector<m3_point_gemo> P;
	P.push_back(firstSol);
	// w(x) <-- 0
	P[0].w = 0;

	int obj_size = env->current_design_space->objectives.size();

	int generations;
	if(!get_integer_from_variables(&current_environment,"generations", generations))
	{
		prs_display_message("No 'generations' defined; defaulting to 100 generations as stopping criteria");
		generations = 100;
	}

	// start the main loop
	prs_display_message("Starting the generation process");
	for(int gen=0; gen<generations; gen++)
	{
		// prs_display_message("New generation");

		// Select one element x out of {y in P | w(y) =< w(z) every z in P} uniformly	
		int index = select_element(P);

		// increment offspring count w(x) <-- w(x) + 1
		P[index].w = P[index].w + 1;

		// prs_display_message("Creating offspring");
		// Create offspring x' by flipping a randomly chosen bit
		m3_point parent = P[index];
		m3_point child = create_offspring(env, parent);

		// if does not exist z in P such that (z dominates x')
		bool P_is_dominant = false;
		for(int i=0, p_size = P.size(); i<p_size; i++)
		{
		  	if(is_dominant(env, P[i], child))
			{
				P_is_dominant = true;
				break;
			}
		}

		if (P_is_dominant == false)
		{
			// if exists z in P such that (f(z) = f(x'))			
			bool g_found = false;			
			for(int i = 0, p_size = P.size(); i < p_size; i++)
			{
				bool found = true;
				for(int j=0; j < obj_size; j++)
				{
					if(P[i].get_objective(env,j) != child.get_objective(env, j))
					{
						found = false;
						break;
					}
				}

				if(found == true)
				{	
					// if w(z) = INF
					if(P[i].w == INT_MAX)
					{
						P[i].w = 0;
					}
					g_found = true;
				}
			}
			// else
			if(g_found == false)	 
			{
				vector<m3_point_gemo> PminusD;				
				bool D_not_equal_0 = false;
				for(int i=0, p_size = P.size(); i< p_size; i++)
				{
					if (is_dominant(env, child, P[i]) == false)
					{
						PminusD.push_back(P[i]);
					}
					else D_not_equal_0 = true;
				}
				if (D_not_equal_0 == true)
				{
					P.clear();
					for(int i=0, pmd_size = PminusD.size(); i < pmd_size; i++)
					{
						P.push_back(PminusD[i]);

					}
					PminusD.clear();
					// for all y in P do w(y) <-- INF
					for(int i=0, p_size = P.size(); i < p_size; i++)
					{
						P[i].w = INT_MAX;
					}					
				}
				// P <-- P U {x'}
				// w(x') <-- 0
				P.push_back(child);
				int index = P.size()-1;
				P[index].w = 0;
			}
		}
	}
}


string m3_gemo_func::get_information()
{
	return "Greedy Evolutionary Multi-objective Optimizer";
}

// Choose an initial individual x uniformly from X
m3_point m3_gemo_func::get_initial_individual(m3_env *env)
{
	m3_point firstSol;
	//doe_orig contains set of random solutions
	prs_display_message("Generating Some Random Points and pick one of them");
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
	// evaluate first solution
	env->current_design_space->evaluate(env, &firstSol);
	if(sim_recompute_pareto_with_new_point(env, env->available_dbs[env->current_db_name], &firstSol))
	{
		env->available_dbs[env->current_db_name]->insert_point(&firstSol);	
	}	
	return firstSol;
}

// Select one element x out of {y in P | w(y) =< w(z) every z in P} uniformly
int m3_gemo_func::select_element(vector<m3_point_gemo> P)
{
	int pop_size = P.size();

	//generate random index between 0 and pop_size-1
	//check min_w
	int min_w = P[0].w;

	for(int i=0; i<pop_size; i++)
	{
		if(P[i].w < min_w) min_w = P[i].w;
	}

	//allocate memory with size of pop_size
	vector<int> min_w_points;
	//push elements with w = min_w
	for(int i=0; i<pop_size; i++)
	{
		if(P[i].w == min_w) min_w_points.push_back(i);
	}
	//count the number of pushes which equals the number of points with min_w
	//create random number between zero and the number of points with min_w-1
	int min_w_points_size = min_w_points.size();

	double rando = (double)rand() / (double)RAND_MAX;
	int rand_index_2 = rando * min_w_points_size;
	int rand_index = min_w_points[rand_index_2];
	min_w_points.clear();

	return rand_index;
}

// Function to create offspring
m3_point m3_gemo_func::create_offspring(m3_env *env, m3_point parent)
{
	//prs_display_message("Creating offspring");
	m3_point child = env->current_design_space->genetic_mutation(env, &parent, 0.1);
		
	// evaluate point
	env->current_design_space->evaluate(env, &child);
	if(sim_recompute_pareto_with_new_point(env, env->available_dbs[env->current_db_name], &child))
	{
		env->available_dbs[env->current_db_name]->insert_point(&child);
	}
	
	return child;
}

// Function to determine whether "x" is dominant over the elements of vector "y"
bool m3_gemo_func::is_dominant(m3_env *env, m3_point x, m3_point y)
{
	int obj_size = env->current_design_space->objectives.size();
	bool x_is_dominant = false;
	bool x_is_better = false;
	bool y_is_better = false;
	bool x_same_y = true;
	
	for(int j=0; j<obj_size; j++)
	{
		if(y.get_objective(env, j) != x.get_objective(env, j))
			x_same_y = false;
		if(y.get_objective(env, j) < x.get_objective(env, j))
		{
			y_is_better = true;
		}
		if(y.get_objective(env, j) > x.get_objective(env, j))
		{
			x_is_better = true;
		}
	}
	if((y_is_better == false && x_is_better == true) && !x_same_y)
	{
		x_is_dominant = true;
	}
	return x_is_dominant;
}

extern "C"
{
    m3_optimizer *opt_generate_optimizer()
    {
        return new m3_gemo_func();
    }
}


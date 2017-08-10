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





 
#include <m3_opt_utils.h>
#include <m3_common_utils.h>
#include <m3_parser.h>
#include <m3_sim_utils.h>
#include <iostream>
#include <dlfcn.h>
#include <map>

typedef m3_optimizer * (*opt_fun_type)();

bool dont_look_at_objectives = false;

bool opt_select_optimizer(m3_env *env, string & opt_name)
{
    string c_opt_name; // complete driver name
    if(m3_look_for_filename_in_search_path(env, opt_name, c_opt_name))
    {
        void *h=dlopen(c_opt_name.c_str(),RTLD_NOW);
        if(h) 
        {
            opt_fun_type genopt=(opt_fun_type) dlsym(h,"opt_generate_optimizer");
            m3_assert(genopt);
            if(env->current_optimizer!=NULL) 
            {
                delete env->current_optimizer;
                env->current_optimizer=NULL;
            }
            env->current_optimizer=genopt();
            return true;
        }
    }
    return false;
}

extern bool doe_select_doe(m3_env *env, string & doe_name);
string itos(int v);

m3_database *opt_execute_doe_and_optimizer(m3_env *env, string doe_name, string opt_name)
{
    if (!current_environment.current_driver) 
    { 
        return NULL;
    }

    if(!doe_select_doe(env, doe_name))
        return NULL;

    if(!opt_select_optimizer(env, opt_name))
        return NULL;

    prs_display_message("Beginning optimization, root has "+itos(env->available_dbs["root"]->size())+" points");
    string saved_current_database="";
    string privdb= "temp_m3_explorer_database";
    if(env->available_dbs.count(privdb))
    {
       env->available_dbs[privdb]->clear(); 
    }
    else
    {
       env->available_dbs.insert(make_pair(privdb, new m3_database()));
    }
    saved_current_database = env->current_db_name;
    env->current_db_name = privdb;

    env->current_optimizer->explore(env);

    m3_database *computed = env->available_dbs[env->current_db_name];
    env->current_db_name = saved_current_database;

    return computed;
}

string opt_transform_point_in_csv_string_vector(m3_point p, m3_env *env)
{
    string ret;
	for(int i=0; i< env->current_design_space->ds_parameters.size(); i++)
	{
		ret = ret+ env->current_design_space->get_parameter_representation(env, p, env->current_design_space->ds_parameters[i].name) + ";";
	}
	return ret;
}

double opt_compute_cost_function(m3_point & p, m3_env *env)
{	
	double tot_cost_function=1;
    string error;
    if(!p.check_consistency(env) || p.get_error(error))
        return 0;
	try
	{
		for(int i=0; i<env->current_design_space->objectives.size(); i++)
		{
            double mv = p.get_objective(env, i);
			tot_cost_function *= mv;
		}
	}
	catch (...)
	{
		prs_display_error("Problems computing the cost function");
		return 0;
	}
	return tot_cost_function;
}

m3_map_const_iterator i;

void opt_start_constraints_scan(m3_env *e)
{
     i=e->shell_variables.begin();
}

bool opt_get_next_constraint_from_shell(m3_string &c, bool & max, m3_env *e)
{
	bool ret=false;
	for(;i!=e->shell_variables.end() && !ret;i++)
	{
		if(i->first.find("max_",0)==0)
		{ 
			c=i->first;
			max=true;
			ret=true; 
		}
		if(i->first.find("min_",0)==0)
		{ 
			c=i->first;
			max=false;
			ret=true; 
		}
	}
	return ret;
}



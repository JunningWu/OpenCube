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





 
#include <m3_sim_utils.h>
#include <m3_parser.h>
#include <math.h>
#include <m3_object.h>


double sim_get_metrics_stats(m3_point *p, string metric_stat_name, m3_env *env)
{
    vector<double> point_metrics = get_double_vector(p->get_properties("metrics"));
    m3_assert(env->current_design_space->metric_index.count(metric_stat_name));
	int index=env->current_design_space->metric_index[metric_stat_name];
    m3_assert(index<point_metrics.size());
    return point_metrics[index];
}

bool sim_is_a_parameter(m3_env *env, string p)
{
    return env->current_design_space->ds_parameters_index.count(p);
}

void sim_show_optimum(m3_env *env)
{
	
	prs_display_message("Computing optimum point; please note that the cost function is the product of all the metrics");
	list<m3_object *>::iterator i=env->available_dbs[env->current_db_name]->get_list_of_points()->begin();
	int n=0;
	m3_point const *opt_point;
	bool is_first=true;
	double opt_cost=0;
	for(;i!=env->available_dbs[env->current_db_name]->get_list_of_points()->end();i++)
	{
		m3_point & p =sim_get_point_at_iterator(i);
		double cost=opt_compute_cost_function(p,env); 
		if(cost < opt_cost || is_first)
		{
			opt_point = &p;
			opt_cost = cost;
			is_first=false;
		}
		n++;
	}
	if(n!=0)
	{	
		cout << "Optimum point: " ;
		cout << env->current_design_space->get_point_representation(env, const_cast<m3_point &>(*opt_point));
		cout << "   cost:" << opt_cost << "\n";	 
	}
}


inline bool check_metrics(m3_env* env,m3_list* metrics)//check if all the metrics in the list exist in the driver
{	
    vector<string> checked_metrics = get_string_vector(metrics);
    for(int i=0; i<checked_metrics.size(); i++)
    {
        if(!env->current_design_space->metric_index.count(checked_metrics[i]))
            return false;
    }
	return true;
}

bool sim_is_purely_dominated_by(m3_env *env, m3_point & e, m3_point &p)
{
    bool p_no_worse_than_e = true;
    bool p_once_better_than_e = false;
    for(int i=0; i<env->current_design_space->objectives.size(); i++)
    {	
        double e_m_value = e.get_objective(env, i);
        double p_m_value = p.get_objective(env, i);

        if (e_m_value < p_m_value)	p_no_worse_than_e = false;
        if (e_m_value > p_m_value)	p_once_better_than_e = true;
    }
    if( p_no_worse_than_e  && p_once_better_than_e )
    {
        return true;
    }
    return false;
}

bool sim_recompute_pareto_with_new_point(m3_env *env, m3_database *the_database, m3_point *p_ptr)
{
    m3_point & p = *p_ptr;
    list<m3_object *>::iterator c;
    m3_list *database_list=the_database->get_list_of_points();

    string error;
    
    if(!p_ptr->check_consistency(env) || p_ptr->get_error(error))
        return false;

    bool is_pareto=true;
    for(c=database_list->begin();c!=database_list->end();)
    { 
        bool e_dominates_p = false;
        bool p_dominates_e = false;
        m3_point & e =sim_get_point_at_iterator(c);

        if(!e.get_error(error))
        {
            //cout << "Comparing " << e.full_print() << " with " << p.full_print() << endl;
            p_dominates_e = sim_is_purely_dominated_by(env, e, p);
            e_dominates_p = sim_is_purely_dominated_by(env, p, e);
            if(p_dominates_e)
            {
                list<m3_object *>::iterator j = c;
                j++;
                database_list->erase(c);
                c = j;
            }
            else 
            {
               c++;
            }

            if( e_dominates_p || e.is_equal(p))
            {
                is_pareto= false;
                break;
            }
        }
        else
            c++;
    }
    //cout << "P:" << p_ptr->full_print() << " IS " << is_pareto << endl;
    return is_pareto;
}

m3_database* sim_compute_pareto_on_local_db(m3_env *env, m3_database* full_db)
{
    m3_database *new_database = new m3_database();
    m3_list *database_list = full_db->get_list_of_points();
    list<m3_object *>::iterator c;
    for(c=database_list->begin();c!=database_list->end();)
    {
        m3_point & p =sim_get_point_at_iterator(c);	
        if(sim_recompute_pareto_with_new_point(env, new_database, &p))
        {
            new_database->insert_point(&p);
        }
        c++;
    }
    return new_database;
}

void sim_compute_pareto(m3_env * env)//env is the environment 
{
    m3_database* filtered_db = sim_compute_pareto_on_local_db(env, env->available_dbs[env->current_db_name]);
    
    delete env->available_dbs[env->current_db_name];
    env->available_dbs[env->current_db_name] = filtered_db;
}

double sim_compute_ADRS_delta(m3_env *env, m3_point *xa, m3_point *xr)
{
   double delta = 0;
   for(int i = 0; i<env->current_design_space->objectives.size(); i++)
   {
       delta = max(delta, (xa->get_objective(env, i) - xr->get_objective(env, i))/xr->get_objective(env,i));
   } 
   return delta;
}

bool sim_compute_ADRS(m3_env *env, string reference, string source, double &ADRS)
{
    ADRS = 0;
    if(!env->available_dbs.count(reference) || !env->available_dbs.count(source))	
    {
        return false;
    }

    list<m3_object *>::iterator cr;
    list<m3_object *>::iterator ca;

	m3_list *rl =env->available_dbs[reference]->get_list_of_points();
	m3_list *ra =env->available_dbs[source]->get_list_of_points();

    int mod_pi = 0;
    double sum = 0;
    string error;
	for(cr=rl->begin();cr!=rl->end();cr++)
	{ 
	    m3_point & xr =sim_get_point_at_iterator(cr);
        if(!xr.get_error(error))
        {
            mod_pi ++;
            double min_delta = INFINITY;
            int mod_lambda =0;
            for(ca=ra->begin(); ca!=ra->end(); ca++)
            {
                m3_point & xa = sim_get_point_at_iterator(ca);
                if(!xa.get_error(error))
                {
                    min_delta = min(min_delta, sim_compute_ADRS_delta(env, &xa, &xr));         
                    //cout << "Min delta is: " << min_delta << endl;
                    mod_lambda ++;
                }
            }
            if(!mod_lambda)
                return false;
            sum = sum + min_delta;
        }
    }
    if(!mod_pi)
        return false;
    ADRS = sum / mod_pi;
    return true;
}

int sim_count_pareto_dominants(m3_env *env, m3_point * element)
{
    int score = 0;
    m3_list *database_list = env->available_dbs[env->current_db_name]->get_list_of_points();
    list<m3_object *>::iterator c;
    string error;
    for(c=database_list->begin();c!=database_list->end();)
    {
        m3_point & p =sim_get_point_at_iterator(c);	
        if(!p.get_error(error))
        { 
            if(sim_is_purely_dominated_by(env, *element, p))
                score++;
        }
        c++;
    }
    return score;
}

int sim_evaluate_doe(m3_env *env)
{
    vector<m3_point> doe_v = env->current_doe->generate_doe(env);
    int doe_v_size = doe_v.size();
    m3_point my_point(env->current_design_space->ds_parameters.size());
    int number_of_points=0;

    for(int i_doe_v=0;i_doe_v<doe_v_size;i_doe_v++)
    {
        my_point = doe_v[i_doe_v];

        if(env->current_design_space->is_valid(env, & my_point))
        {
            number_of_points++;
            prs_display_message("Evaluating point: " + env->current_design_space->get_point_representation(env, my_point));
            my_point.clear_metrics(env);
            env->current_design_space->evaluate(env, & my_point);
            string error;
            if(!(my_point.get_error(error)==M3_POINT_FATAL_ERROR)) 
                env->available_dbs[env->current_db_name]->insert_point(&my_point);
            else
            {
                /* missing */
                return number_of_points;
            }

            my_point.clear_metrics(env);
        }
        else
        {
            prs_display_message("Skipping point: " + env->current_design_space->get_point_representation(env, my_point));
        }
    }
    return number_of_points;
}

//#define DEBUG

bool shell_command(string command)
{
#if defined(DEBUG)
    string sh_com = "/bin/bash -c \"(" + command + ")\"";
#else
    string sh_com = "/bin/bash -c \"(" + command + ")\" > /dev/null";
#endif
    int rs = system(sh_com.c_str());
    if(rs==-1 || rs==127)
    {
#if defined(DEBUG)
        cout << sh_com << " : failed - " << rs << endl;
#endif
        return false;
    }
    /** The upper 8 bits contain the exit status */
    rs = rs >> 8;
#if defined(DEBUG)
    cout << sh_com << " : " << ((rs) ? "FAIL" : "OK" ) << endl;
#endif
    if(rs)
        return false;
    return true;
}


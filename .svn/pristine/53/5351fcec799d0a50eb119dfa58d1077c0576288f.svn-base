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


/*
 * Note: Significant and important parts of this source file have been contributed by Al-Hissi Mohammad while at
 * Universita' della Svizzera Italiana (2008).
 */

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
//#include <stdlib.h>
#include <stdio.h>




class m3_point_EA : public m3_point
{
    public:
    m3_point_EA();
    m3_point_EA(m3_point &p);
    m3_point_EA(const m3_point_EA &p);
    int dominated_p;
    int dominating_p;
    vector<int> dominating_v;
    int rank;
    bool ranked;
    bool evaluated;
    bool edge;
    double distance;
};

m3_point_EA::m3_point_EA():m3_point() {
    dominating_p = 0;
	evaluated = false;
	ranked = false;
	edge = false;
	distance = 0.0;
}

m3_point_EA::m3_point_EA(m3_point &l):m3_point(l){
    dominating_p = 0;
	evaluated = false;
	ranked = false;
	edge = false;
	distance = 0.0;
}

m3_point_EA::m3_point_EA(const m3_point_EA &l):m3_point(l){
    dominating_p = 0;
    rank = l.rank;
	evaluated = l.evaluated;
	ranked = false;
	edge = false;
	distance = 0.0;
}



class m3_nsga_II: public m3_optimizer
{
    public:
        m3_nsga_II() {}
        string get_information();
        void explore(m3_env *env);
    private:

        void get_valid_points(m3_env *env, vector<m3_point> &doe_orig, vector<m3_point_EA> &doe);

        void compare_points(m3_env *env, vector<m3_point_EA> &doe);
        void compare_this_point(m3_env *env, vector<m3_point_EA> & doe, m3_point_EA &current_point);

        void rank_points(m3_env *env, vector<m3_point_EA> & doe_ranked, vector<m3_point_EA> & doe);

        void diverse_points(m3_env *env, vector<m3_point_EA> &doe_ranked);
        void generate_next_pop (m3_env *env, vector<m3_point_EA> &children,vector<m3_point_EA> &doe_ranked);
        void select_best_half(m3_env *env, vector<m3_point_EA> &doe_ranked, vector<m3_point_EA> &doe_merged_ranked);
};


string m3_nsga_II::get_information()
{
    return "Nondominated Sorting Genetic Algorithm II optimizer";
}


void m3_nsga_II::explore(m3_env *env)
{
    prs_display_message("Starting with the NSGA-II optimization process");
    m3_assert(env->current_driver);


    bool temporary_save = false;
    string filename;
    if (get_string_from_variables(env, "temp_database", filename)) 
        temporary_save = true; 


    vector<m3_point> doe_orig = env->current_doe->generate_doe(env);

    int doe_orig_size = doe_orig.size();

    char tempInt [20];
    vector<m3_point_EA> doe;

    get_valid_points(env, doe_orig, doe);

    /*the main algorithm that compare the points regarding domination
    this algorithm adds the following attributes to each point,    the vector of dominated nodes, 
    the No. of dominated points and the No. of dominating points...*/
    compare_points(env, doe);

    for(int i = 0; i < doe.size(); i++)
    {
        prs_display_message("point: " + env->current_design_space->get_point_representation(env, doe[i]));
        string error;
        if(!doe[i].get_error(error))
        {
            for(int j = 0; j < env->current_design_space->objectives.size(); j++)
            {
                double my_value = doe[i].get_objective(env, j);
                sprintf(tempInt,"%.0f",my_value);
                prs_display_message(tempInt);
            }
        }
    }

    /*the main algorithm that rank the points by going through sccessive fronts
    this algorithm adds the following attributes to each point
    the rank No., it also sort the points by the rank No. in the first argument*/
    vector<m3_point_EA> doe_ranked;
    rank_points(env, doe_ranked, doe);

    //compute the crowding parameter to support the diversity feature this algorithm adds average distance to indicate the crowding
    diverse_points(env, doe_ranked);

    int generations;
    if(!get_integer_from_variables(&current_environment,"generations", generations))
    {
        prs_display_message("No 'generations' defined; defaulting to 100 generations as stopping criteria");
        generations = 100;
    }
    for(int genNo = 0; genNo<generations; genNo++)
    {
        //for each generaton the points would be already added in doe_ranked, so start by generating the new population 
        //produce the next population in children, by crossover and permutation
        vector<m3_point_EA> children;
        generate_next_pop (env, children, doe_ranked);

        //merge the two populations parents "doe_ranked" and children to have population of double size
        vector<m3_point_EA> doe_merged;
        vector<m3_point_EA> doe_merged_ranked;
        for(int i=0; i<doe_ranked.size(); i++)
            doe_merged.push_back(doe_ranked[i]);
        for(int i=0; i<children.size(); i++)
            doe_merged.push_back(children[i]);
        //should be empty for next generations
        doe_ranked.clear();
        children.clear();

        //rank the new double size population, parents+children as previous, "compare_points", rank_points and "diverse_points"
        compare_points(env, doe_merged);

        rank_points(env, doe_merged_ranked, doe_merged);
        diverse_points(env, doe_merged_ranked);

        //create the new generation by getting the best N solutions of the 2N solutions,
        //this process gurantees the eliticism
        select_best_half(env, doe_ranked, doe_merged_ranked);

        //should be empty for next generations
        doe_merged.clear();
        doe_merged_ranked.clear();
        //now doe_ranked would be already ranked "best ranks are selected" but diversity should be computed again
        //diverse_points(env, doe_ranked);
        //now doe_ranked is considered the new generation

        //cout << endl;
        sprintf(tempInt,"%d",genNo);
        prs_display_message("Generation No.: ");
        prs_display_message(tempInt);

        if (temporary_save)
        {
            prs_display_message("Writing temporary database..");
            env->available_dbs["root"]->write_to_file(filename.c_str());
        }
    }
    cout << endl;
}


void m3_nsga_II::get_valid_points(m3_env *env, vector<m3_point> &doe_orig, vector<m3_point_EA> &doe)
{
    m3_point_EA current_point;
    int doe_orig_size = doe_orig.size();
    int doe_size = doe.size();

    for (int i = 0; i < doe_orig_size ; i++)
    {
        current_point = doe_orig[i];
        //it always return true if the used driver is "m3_driver" ,!!!
        // but if the used driver is m3_xml_driver, it seems that it checks the rules
        if(env->current_design_space->is_valid(env, & doe_orig[i]))
        {
            prs_display_message("Adding point: " + env->current_design_space->get_point_representation(env, current_point));
            doe.push_back(current_point);
        }
        else
        {
            prs_display_message("Skipping point: " + env->current_design_space->get_point_representation(env,current_point));
        }
    }
}


void m3_nsga_II::compare_points(m3_env *env, vector<m3_point_EA> &doe)
{
    prs_display_message("Comparing Points");
    int exp_size = doe.size();

    for(int experiment = 0; experiment < exp_size; experiment ++)
    {
        if(!doe[experiment].evaluated)
        {
            //m3_point &current_point = doe[experiment];
            //prs_display_message("Evaluating point: " + env->current_design_space->get_point_representation(env, doe[experiment]));
            //why the clear metrics should be called
            //doe[experiment].clear_metrics(env);
            env->current_design_space->evaluate(env, & doe[experiment]);
            
            if(sim_recompute_pareto_with_new_point(env, env->available_dbs[env->current_db_name], &doe[experiment]))
            {
                if( env->available_dbs[env->current_db_name]->find_point(doe[experiment])==NULL )
                    env->available_dbs[env->current_db_name]->insert_point(& doe[experiment]);
            }
            doe[experiment].evaluated = true;
            string error;
            int err_code;
            /*it seems that == operator should be replaced by the assignment
             operator =, which returns false if the right hand expression = 0*/
            err_code = doe[experiment].get_error(error);
            if(err_code == M3_POINT_FATAL_ERROR)
               break;
        }
    }

    char tempInt [20];
    int obj_size = env->current_design_space->objectives.size();

    for(int experiment = 0; experiment < exp_size; experiment ++)
    {
        compare_this_point(env, doe, doe[experiment]);
    }
}    


void m3_nsga_II::compare_this_point(m3_env *env, vector<m3_point_EA> & doe, m3_point_EA & current_point)
{
    int dominating_p = 0;                // No. of points current_point domintates
    int    dominated_p = 0;            // No. of points domintate current_point
    int exp_size = doe.size();
    int obj_size = env->current_design_space->objectives.size();
    vector<int> dominating_v(exp_size);        // dominating vector space
    char tempInt [20];
    string error;

    if( current_point.get_error(error) )        // if the current point is an error point
    {        
        for (int i=0; i<exp_size; i++)
        {
            if(!doe[i].get_error(error))
            {
                dominated_p++;
            }
        }
        current_point.dominating_p = 0;
        current_point.dominated_p = dominated_p;
        return;
    }

    //for(int j = experiment+1; j < exp_size; j++)
    for(int j = 0; j < exp_size; j++)
    {
        m3_point_EA compared_point = doe[j];
        if (&current_point != &compared_point)         //don't compare a point to itself 
        {
            if (!compared_point.get_error(error))    // if the compared point is not error point
            {
                bool m_worse=false;
                bool c_worse=false;
                //scan for objectives
                for(int i=0; i < obj_size ;i++)
                {
                    double my_value = current_point.get_objective(env, i);
                    double comp_value = compared_point.get_objective(env, i);
    
                    if (my_value>comp_value)    m_worse=true;
                    if (comp_value>my_value)    c_worse=true;
                }
    
                if(c_worse && !m_worse)            //my current point dominates the compared
                {
                    dominating_v[dominating_p] = j;    //push the dominated point ID
                    dominating_p ++;
                }
                if(!c_worse && m_worse)            //the compared point dominates my point
                {
                    dominated_p ++;
                }
            }
            else    // error points are always dominated
            {
                dominating_v[dominating_p] = j;    //push the dominated point ID
                dominating_p ++;
                
            }
        }
    }

    current_point.dominating_p = dominating_p;
    current_point.dominated_p = dominated_p;
    current_point.dominating_v.clear();
    for (int i=0; i<dominating_p; i++){
//        cout << " d_v_i " << dominating_v[i] << endl;
        current_point.dominating_v.push_back(dominating_v[i]);
    }
}

void m3_nsga_II::rank_points(m3_env *env, vector<m3_point_EA> &doe_ranked, vector<m3_point_EA> &doe)
{
    prs_display_message("Ranking Points");
    int dominating_p;
    int rank = 1;
    char tempInt [20];
    string temp;
    string error;
    for(int i = 0; i < doe.size(); i++)
        doe[i].ranked = false;
    while (doe_ranked.size() < doe.size())
    {
        bool changed=false;//will become true when only error points will remain
        for(int i = 0; i < doe.size(); i++)
        {
            //if the point is an error point rank it now and modify it rank at the end as the maximum
            if( (doe[i].get_error(error)) )
                continue;
            if(doe[i].ranked || doe[i].dominated_p)
                continue;        //already ranked or still not pareto jump to the next point

            changed=true;
            //pareto regarding this front
            sprintf(tempInt,"%d",rank);
            temp = tempInt;
            
            ostringstream objs;
            int obj_size = env->current_design_space->objectives.size();

            for(int j=0; j < obj_size ;j++)
            {
                objs << doe[i].get_objective(env, j) << " ";
            }

            for (int j = 0; j < doe[i].dominating_p; j++)
            {
                //get points dominated by current_point, msut not be ranked yet
                int index = doe[i].dominating_v[j];
                if(doe[index].ranked)//an algorithmic error
                    prs_display_message("Algorithmic Error");
                doe[index].dominated_p --;
                if(doe[index].dominated_p < 0)//an algorithmic error
                    prs_display_message("Algorithmic Error");
            }
            doe[i].rank = rank;
            doe[i].ranked = true;
            doe_ranked.push_back(doe[i]);
        }
        rank ++;

        //missing only error points
        if(!changed){
            for(int i = 0; i < doe.size(); i++){
                if((doe[i].get_error(error))){
                    doe[i].rank = rank;
                    doe[i].ranked = true;
                    doe_ranked.push_back(doe[i]);
                }
            }
        }
    }
}

void m3_nsga_II::generate_next_pop (m3_env *env, vector<m3_point_EA> &children, vector<m3_point_EA> &doe_ranked)
{
    prs_display_message("Generating Next Pop");
    while(children.size() < doe_ranked.size())
    {
        int randNo1 = rand()%doe_ranked.size();
        int randNo2 = rand()%doe_ranked.size();
        int randNo3 = rand()%doe_ranked.size();
        int randNo4 = rand()%doe_ranked.size();
        int parentNo1;
        int parentNo2;

        if (doe_ranked[randNo1].rank < doe_ranked[randNo2].rank)
            parentNo1 = randNo1;
        else if(doe_ranked[randNo2].rank < doe_ranked[randNo1].rank)
            parentNo1 = randNo2;
        else            //the same rank
        {
            if(doe_ranked[randNo1].edge)        //point on the edge, injfinite crowding distance
                parentNo1 = randNo1;
            else if(doe_ranked[randNo2].edge)    //point on the edge, injfinite crowding distance
                parentNo1 = randNo2;
            else
            {
                if(doe_ranked[randNo1].distance > doe_ranked[randNo2].distance)
                    parentNo1 = randNo1;
                else
                    parentNo1 = randNo2;
            }
        }

        if (doe_ranked[randNo3].rank < doe_ranked[randNo4].rank)
            parentNo2 = randNo3;
        else if(doe_ranked[randNo4].rank < doe_ranked[randNo3].rank)
            parentNo2 = randNo4;
        else            //the same rank
        {
            if(doe_ranked[randNo3].edge)        //point on the edge, injfinite crowding distance
                parentNo2 = randNo3;
            else if(doe_ranked[randNo4].edge)    //point on the edge, injfinite crowding distance
                parentNo2 = randNo4;
            else
            {
                if(doe_ranked[randNo3].distance > doe_ranked[randNo4].distance)
                    parentNo2 = randNo3;
                else
                    parentNo2 = randNo4;
            }
        }


        m3_point parent1 = doe_ranked[parentNo1];
        m3_point parent2 = doe_ranked[parentNo2];
        m3_point cross_p = env->current_design_space->genetic_crossover(env, &parent1, &parent2);
        m3_point perm_p = env->current_design_space->genetic_mutation(env, &cross_p, 0.1);
        if(env->current_design_space->is_valid(env, & perm_p))
        {
            prs_display_message("Adding point: " + env->current_design_space->get_point_representation(env, perm_p));
            children.push_back(perm_p);
        }
        else
        {
            prs_display_message("Skipping point: " + env->current_design_space->get_point_representation(env, perm_p));
        }
    }

    prs_display_message("Finished Generating Next Pop");
}

void m3_nsga_II::select_best_half(m3_env *env, vector<m3_point_EA> &doe_ranked, vector<m3_point_EA> &doe_merged_ranked)
{
    prs_display_message("Selecting Best half");
    int rank = 1, count=0, lastCount=0;
    int doe_merged_size = doe_merged_ranked.size();
    int first = 0;
    for(int p=0; p<doe_merged_size; p++)
    {
        if(doe_merged_ranked[p].rank == rank && (p!=(doe_merged_size-1)))    //check the end of input vector
            continue;
        if(p<doe_merged_size/2)            //all this front should be taken, and should continue with next fronts
        {
            for(int i=first; i<p; i++)
            {
                doe_ranked.push_back(doe_merged_ranked[i]);
            }
            rank++;
            first = p;
            continue;
        }
        else if(p==doe_merged_size/2)            //all this front should be taken, and should stop taking next fronts
        {
            for(int i=first; i<p; i++)
                {
                doe_ranked.push_back(doe_merged_ranked[i]);
            }
            break;
        }
        else                                    //parts of this front should be taken, and then stop
        {
            for(int i=first; i<p; i++)                //take any point at the edge
            {
                if(doe_merged_ranked[i].edge)
                {
                doe_ranked.push_back(doe_merged_ranked[i]);
                if(doe_ranked.size() == doe_merged_size/2)
                    break;                        //stop, so break the inner loop then the outer loop
                }
            }

            while(doe_ranked.size() < doe_merged_size/2)
            {
                double highestDist = 0;
                int index = 0;
                for(int i=first; i<p; i++)    //take the points with highest distance, one by one, all the points at the edge aleady taken
                {
                	if(!doe_merged_ranked[i].edge)        //eligible point
                	{
                		if(doe_merged_ranked[i].distance > highestDist)
                		{
                			highestDist = doe_merged_ranked[i].distance;
                			index = i;
                		}
                	}

                }
                doe_ranked.push_back(doe_merged_ranked[index]);
                doe_merged_ranked[index].edge = true;            //don't consider it next time
            }
            break;
        }
    }
    char tempInt [20];
}

//asuming that the vector is ranked and sorted.
void m3_nsga_II::diverse_points(m3_env *env, vector<m3_point_EA> &doe_ranked)
{
    int first=0, rank=1;
    int obj_size = env->current_design_space->objectives.size();
    double *lowest = new double [obj_size];
    double *highest = new double [obj_size];

    for(int i = 0; i < doe_ranked.size(); i++)
        doe_ranked[i].edge = false;

    for(int p=0; p<doe_ranked.size(); p++)
    {
        string error;
        if( (doe_ranked[p].rank == rank) || (doe_ranked[p].get_error(error)) )
            continue;
        else
        {            //points belong to the same nondominating front, from first to i-1
            for(int k=0; k<obj_size; k++)
            {
                lowest[k]=doe_ranked[first].get_objective(env, k);
                highest[k]=doe_ranked[first].get_objective(env, k);
            }
            for(int j=first+1; j<p; j++)
            {
                for(int k=0; k<obj_size; k++)
                {
                    if(doe_ranked[j].get_objective(env, k) < lowest[k])
                        lowest[k]=doe_ranked[j].get_objective(env, k);
                    if(doe_ranked[j].get_objective(env, k) > highest[k])
                        highest[k]=doe_ranked[j].get_objective(env, k);
                }
            }
            for(int j=first; j<p; j++)
            {
                double distance = 0;
                for(int k=0; k<obj_size; k++)
                {
                    double dist2lower = doe_ranked[j].get_objective(env,k)-lowest[k];
                    double dist2higher = highest[k]-doe_ranked[j].get_objective(env,k);
                    if((dist2higher==0)||(dist2lower==0))    //point on the edge
                    {
                        doe_ranked[j].edge = true;    //equivelent to distance of infinity
                        doe_ranked[j].distance = 0;
                        break;
                    }
                    for(int l=first; l<p; l++)
                    {
                        if(l==j)        //don't compare a point to itself
                            continue;
                        else
                        {
                            int diff = doe_ranked[j].get_objective(env,k)-doe_ranked[l].get_objective(env,k);
                            if(diff <= 0)    //the point j is lower than l, higher value may be updated
                                {
                                if (diff < dist2higher)
                                    dist2higher = diff;
                            }
                            if(diff >= 0)    //the point j is higher than l, lower value may be updated
                            {
                                if (diff < dist2lower)
                                    dist2lower = diff;
                            }
                        }
                    }    //end of compared point scanning
                    distance = distance + (dist2lower+dist2higher)/(highest[k]-lowest[k]);
                }    //end of objectives scanning
                doe_ranked[j].distance = distance;
            }    //end of scanning this nondominating front
            first = p;    //begin of a new non dominating front
        }
    }
    delete [] lowest;
    delete [] highest;
}


extern "C"
{
    m3_optimizer *opt_generate_optimizer()
    {
        return new m3_nsga_II();
    }
}


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





 
#include <m3_point.h>
#include <cstdio>


m3_point::m3_point() { obj_create(num_obj)};

m3_point::m3_point(m3_point &l): m3_object(l), vector<int>(l) { 
obj_create(num_obj) 
}

m3_point::m3_point(const m3_point &l): m3_object(const_cast<m3_point &>(l)), vector<int>(l) { 
obj_create(num_obj) 
}

m3_point::m3_point(int n): vector<int>(n) { obj_create(num_obj) }

m3_point::~m3_point()
{
}

bool m3_point::is_equal(m3_point &p)
{
    for(int j=0; j<p.size(); j++)
    {
        if((*this)[j] != p[j])
	   return false;
    }
    return true; 
}

void m3_point::copy_from(m3_point &p)
{
    for(int j=0; j<p.size(); j++)
    {
        (*this)[j] = p[j];
    }

    map<string,m3_object *>::const_iterator i;
    for(i=p.properties.begin(); i!=p.properties.end(); i++)
    {
        if(properties.find(i->first) != properties.end() ){
            delete properties[i->first];
        }
        properties[i->first]=i->second->gen_copy();
    }
}

double m3_point::get_objective(m3_env *env, int i)
{
    assert(i<env->current_design_space->objectives.size());
    double value;
    if(!env->current_design_space->eval_expression(env, this, env->current_design_space->objectives[i], true, value))
        throw m3_exception("The objective can't be specified");

    return value;
}


string m3_point::print() 
{
    string s="< ";
    int i;
    for(i=0; i<size(); i++)
    {
        char buf[4];
        sprintf(buf,"%d",(*this)[i]);
        s=s+buf+" ";
    }
    s=s+">";
    return s;
}

void m3_point::insert(int pos, int i)
{
    if(pos>size()) resize(pos+1,0);
    (*this)[pos]=i;
}

m3_object *m3_point::gen_copy() 
{
    m3_point *v=new m3_point(*this);
    return v;
}

vector<double> get_double_vector(m3_object *);

#include <m3_exception.h>

double m3_point::get_metric(int i) 
{
    m3_object *the_metric_ve=NULL;
    
    vector<double> vect = get_double_vector(get_properties("metrics"));
    if(i>=vect.size())
        throw m3_exception("Metrics not existing for the point");


    return vect[i];
}

void m3_point::set_error(int type, string reason)
{
    m3_integer type3(type); 
    m3_string reason3(reason); 

    set_properties("error_code", type3);
    set_properties("error_reason", reason3);
}

int m3_point::get_error(string & reason)
{
    int theerror = 0;
    try
    {
        theerror = obj_get_integer(get_properties("error_code"));
    }
    catch(m3_exception e)
    {
        //cout << "No error code" << theerror << endl;
    }
    try
    {
        reason = obj_get_string(get_properties("error_reason"));
    }
    catch(m3_exception e)
    {
        //cout << "No reason " << theerror << endl;
    }
    //cout << "Error: " << theerror << endl;
    return theerror;
}



bool m3_point::check_consistency(m3_env * env){
    m3_design_space *ds = env->current_design_space;

    string reason;

    if(get_properties("metrics"))
    {
        int this_metric_size = get_double_vector(get_properties("metrics")).size();
        int DS_metric_size = ds->metric_names.size();

        if( this_metric_size != DS_metric_size )	//check number of metrics
            return false;

    }
    int error=get_error(reason);
    if((error)>M3_POINT_NON_FATAL_ERROR)
    {
        return false;
    }
    if( this->size() != ds->ds_parameters.size() )	//check number of parameters
        return false;



    //check parameters bounds consistency (permutations)
    for(map<string, m3_permutation>::iterator pr = ds->permutation_parameters.begin(); pr!=ds->permutation_parameters.end();pr++){
        int p = ds->ds_parameters_index[pr->first];
        if( (*this)[p] < ds->get_permutation_min(env, pr->first, this) )
            return false;
        if( (*this)[p] > ds->get_permutation_max(env, pr->first, this) )
            return false;
    }

    //check parameters bounds consistency (on_off_masks)
    for( map<string, m3_on_off_mask>::iterator pr = ds->on_off_mask_parameters.begin(); pr!=ds->on_off_mask_parameters.end();pr++){
        int p = ds->ds_parameters_index[pr->first];
        if( (*this)[p] < ds->get_mask_min(env, pr->first, this) )
            return false;
        if( (*this)[p] > ds->get_mask_max(env, pr->first, this) )
            return false;
    }

    //check parameters bounds consistency (scalars)
    for( map<string, m3_scalar>::iterator pr = ds->scalar_parameters.begin(); pr!=ds->scalar_parameters.end();pr++){
        int p = ds->ds_parameters_index[pr->first];
        if( (*this)[p] < ds->get_scalar_min(env, pr->first) )
            return false;
        if( (*this)[p] > ds->get_scalar_max(env, pr->first) )
            return false;
    }

    return true;
}

void m3_point::clear_metrics(m3_env *)
{
    map<string,m3_object*>::iterator it;


    it=properties.find(string("metrics"));
    if(it!=properties.end()){
        delete properties["metrics"];
        properties.erase (it);
    }
}

void m3_point::print_point_value(m3_env *env)
{
    if(get_properties("metrics"))
    {
        vector<double> metrics = get_double_vector(get_properties("metrics"));
        for(int i = 0; i<metrics.size();i++)
            cout << "  " << metrics[i];

        for(int i = 0; i<env->current_design_space->objectives.size();i++)
            cout << "  " << get_objective(env, i);

        cout << endl;
    }
    else
    {
        string reason;
        int e;
        if(e = get_error(reason))
        {
            cout << ((e ==  M3_POINT_FATAL_ERROR) ? "Fatal error: " : "Non-fatal error: ") << reason << endl;
        }
        else
        {
            cout << "N/A" << endl;
        }
    }
}



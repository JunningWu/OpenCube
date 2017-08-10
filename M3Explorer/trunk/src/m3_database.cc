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
#include <m3_database.h>
#include <m3_xdr_api.h>
#include <m3_parser.h>
#include <fstream>
#include <regex.h>
#include <math.h>

m3_preprocess_pipeline::m3_preprocess_pipeline()
{
	disabled = true;
}

m3_preprocess_pipeline::m3_preprocess_pipeline( double box_cox_transform_v, bool box_cox_is_log_v)
{
	if( box_cox_transform_v==0 )
		box_cox_transform_v = 1.0;
	box_cox_transform	= box_cox_transform_v;
	box_cox_is_log		= box_cox_is_log_v;
	disabled 		= true;
}

double m3_preprocess_pipeline::pre_process( double value)
{
	if( !disabled )
	{
		if( box_cox_is_log )
			value = log( value );
		else
			value = pow( value, box_cox_transform);

		value = (value-mean)/std;
		return value;
	}
	else
	{
		return value;
	}
}

double m3_preprocess_pipeline::post_process(double value)
{
	if( !disabled )
	{
		value = (value*std) + mean;
		if( box_cox_is_log  )
			value = exp(value);
		else
			value = pow( value, 1/box_cox_transform);
		return value;
	}
    return value;
}

m3_database::m3_database()
{
	points=new m3_list();
};

m3_database::m3_database( m3_env *env, m3_doe *doe)
{
	points = new m3_list();
	int 			doe_point	= 0;
	vector<m3_point> 	doe_points 	= doe->generate_doe(env);
	while( doe_point < doe_points.size() )
	{
		insert_point( &doe_points[doe_point] );
		doe_point++;
	}
};

m3_database::~m3_database()
{
	delete points;	
};

m3_point *m3_database::find_point(m3_point &p)
{
	list<m3_object *>::iterator i=points->begin();
    //cout << "P is " << p.full_print() << endl;
    //cout << "************" << endl;
	for(;i!=points->end();i++)
	{
        //cout << "Looking ..." << endl;
        m3_point *f = (dynamic_cast<m3_point *>(*i));
        //cout << "Comparing P with " << f->full_print() << endl;
        if(p==(*f))
        {
            //cout << "They are the same!" << endl;
            return (f);
        }
    }
    //cout << "Not found!" << endl;
    return NULL;
}

/** Does not check anymore for duplicates */
void m3_database::insert_point(m3_point *p) 
{
    /*if (find_point(*p)==NULL) 
    { */
	    points->insert(*p);
    /* } */
};

#define MAX_LINE_LENGTH

int m3_database::read_from_file(const char *filename)
{
    try
    {

        string pattern_int = "[0-9][0-9]*";
        string pattern_point = "<[ 0-9]*>";
        /** FIXME should add and test [+-] to pattern_metrics and pattern_statistics */
        string pattern_metrics = "metrics[ ]*=[ ]*\\[[ 0-9\\.,]*\\]";
        string pattern_metric = "[+-]*[0-9][0-9]*\\.[0-9]*";

        regex_t pattern_point_comp;
        regex_t pattern_int_comp;
        regex_t pattern_metrics_comp;
        regex_t pattern_metric_comp;


        if(regcomp(&pattern_point_comp, pattern_point.c_str(), 0) ||
                (regcomp(&pattern_int_comp, pattern_int.c_str(), 0)) ||
                (regcomp(&pattern_metrics_comp, pattern_metrics.c_str(), 0)) ||
                (regcomp(&pattern_metric_comp, pattern_metric.c_str(), 0))) 
        {
            cout << "Problems compiling the expression;" << endl;
            return 0;
        }

        char line[1000];

        string input;

        regmatch_t positions[1];

        FILE *file = fopen(filename,"r");
        if(!file)   
        {
            prs_display_error("File does not exist");
            return 0;
        }

#define LEN (positions[0].rm_eo-positions[0].rm_so)
#define GETSTR(s) ((s).substr(positions[0].rm_so, LEN))
#define TRIM(s) ((s).substr(positions[0].rm_eo, (s).length()-positions[0].rm_eo))

        int n=current_environment.current_design_space->ds_parameters.size();
        int p = current_environment.current_design_space->metric_names.size();
        m3_point the_point(n);
        clear();

        while(fgets(line, sizeof(line), file))
        {
            string point_part;
            string metrics_part;

            input = line;
            if(!regexec(&pattern_point_comp, input.c_str(), 1, positions, 0))
            {
                point_part = GETSTR(input);
            }

            int k = 0;
            while(!regexec(&pattern_int_comp, point_part.c_str(), 1, positions, 0))
            {
                if(k>=n)
                    throw m3_exception("Unfeasible design space");

                int i = atoi(GETSTR(point_part).c_str());
                point_part = TRIM(point_part);
                the_point[k] = i;
                k++;
            }

            string metrics;

            if(!regexec(&pattern_metrics_comp, input.c_str(), 1, positions, 0))
            {
                metrics = GETSTR(input);
            }

            m3_vector mv;
            k = 0;
            while(!regexec(&pattern_metric_comp, metrics.c_str(), 1, positions, 0))
            {
                if(k>=p)
                    throw m3_exception("Unfeasible design space");

                double value = atof(GETSTR(metrics).c_str());
                metrics = TRIM(metrics);
                m3_double obj(value);
                mv.insert(k, obj);
                k++;
            }
            the_point.set_properties("metrics", mv);
            points->insert(the_point);
        }
        regfree(&pattern_point_comp);
        regfree(&pattern_int_comp);
        regfree(&pattern_metrics_comp);
        regfree(&pattern_metric_comp);
    }
    catch(m3_exception &e)
    {
        return 0;
    }
    return 1;
}

int m3_database::write_to_file(const char* filename) 
{
	ofstream file_out(filename,ios::out);
	m3_assert(!file_out.fail());
	list<m3_object *>::iterator i=points->begin();

	for(;i!=points->end();i++)
	{
        m3_point *p = to<m3_point *>(*i);
        if(p!=NULL)
        {
            if(p->check_consistency(&current_environment))
                file_out << "db_insert_point " << p->full_print() << " \n";
        }
	}	
	return 1;
};

//#define trick(k) (((double) (k)) + 0.01 )
#define trick(k) ((k))


int m3_database::write_param_obj_to_file(m3_env* env ,const char* filename, int obj1, int par1, int par2) 
{
	ofstream file_out(filename,ios::out);
	m3_assert(!file_out.fail());
	list<m3_object *>::iterator i=points->begin();
    string error;
    vector<double> matrix;
    vector<int> matrixn;
    int max1 = env->current_design_space->get_scalar_max(env, env->current_design_space->ds_parameters[par1].name);
    int min1 = env->current_design_space->get_scalar_min(env, env->current_design_space->ds_parameters[par1].name);

    int max2 = env->current_design_space->get_scalar_max(env, env->current_design_space->ds_parameters[par2].name);
    int min2 = env->current_design_space->get_scalar_min(env, env->current_design_space->ds_parameters[par2].name);
#define SZ(mn,mx) ((mx-mn)+1)
#define EL(i,j) ((row)*(i-min1)+(j-min2))
    matrix.resize(SZ(min1,max1)*SZ(min2,max2), 0.0);
    matrixn.resize(SZ(min1,max1)*SZ(min2,max2), 0 );
    int row = SZ(min2,max2);
	for(;i!=points->end();i++)
	{
			m3_point *f = (dynamic_cast<m3_point *>(*i));
            if(f->check_consistency(env) && !f->get_error(error))
            {
                int r1 = (*f)[par1];
                int r2 = (*f)[par2];
                matrix[EL(r1,r2)] += (*f).get_objective(env, obj1);
                matrixn[EL(r1,r2)] += 1;
            }
	}
    for(int i=0; i<SZ(min1,max1); i++)
    {
        for(int j=0; j<SZ(min2,max2); j++)
        {
                if( matrixn[EL(i,j)] > 0 )
                {
                    file_out << i << " " << j << " " << matrix[EL(i,j)]/matrixn[EL(i,j)];
		            file_out << endl;
                }
        }
    }
	file_out.close();
    return 1;
};

int m3_database::write_objectives_to_file(m3_env* env ,const char* filename, int obj1, int obj2) 
{
	ofstream file_out(filename,ios::out);
	m3_assert(!file_out.fail());
	list<m3_object *>::iterator i=points->begin();
    string error;
	for(;i!=points->end();i++)
	{
			m3_point *f = (dynamic_cast<m3_point *>(*i));
            if(f->check_consistency(env) && !f->get_error(error))
            {
                file_out << (*f).get_objective(env, obj1) << " " << (*f).get_objective(env, obj2);
            }
		    file_out << endl;
	}
	file_out.close();
	return 1;
};


void m3_database::clear()
{
	if(points) delete points;
	points=new m3_list();
}

int m3_database::size()
{
    return points->size();
}

m3_list *m3_database::get_list_of_points()
{
	return points; 
}

void m3_database::copy_from(m3_list *l)
{
	if(!l) { prs_display_error("Copy from buffer failed since source buffer is empty"); return; }
	clear();
        points=new m3_list(*l);		
}

m3_list *m3_database::generate_copy_of_database_list()
{
	return new m3_list(*points);	
}

/**RSM related methods*/

#define BOX(v) ((box_cox_is_log) ? log(v) : pow((v), box_cox))

void m3_database::xdr_init( m3_env *env, double box_cox, bool box_cox_is_log)
{
	xdr_init_predictors_only( env );
	xdr_metric_pipes.resize( xdr_n_metrics );
	for( int i=0; i<xdr_n_metrics; i++ )
	{
		xdr_metric_pipes[i].box_cox_transform	= box_cox;
		xdr_metric_pipes[i].box_cox_is_log	= box_cox_is_log;
		xdr_metric_pipes[i].disabled		= true;
		xdr_metric_pipes[i].mean		= 0;
		xdr_metric_pipes[i].std			= 0;
	}
	int n = 0;
	list<m3_object *>::iterator i = points->begin();
	for( ;i!=points->end(); i++ )
	{
		m3_point *point = (m3_point*)*i;
		string error;
		if( point->check_consistency(env) && !point->get_error(error) )
		{
			for( int i=0; i<xdr_n_metrics; i++ )
			{
				xdr_metric_pipes[i].mean += BOX( point->get_metric(i) );
			}
			n++;
		}
	}
	if( n>0 )
	{
		for( int i=0; i<xdr_n_metrics; i++ )
		{
			xdr_metric_pipes[i].mean = xdr_metric_pipes[i].mean/n;
		}
		list<m3_object *>::iterator i = points->begin();

		for( ;i!=points->end(); i++ )
		{
			m3_point *point = (m3_point*)*i;
			string error;
			if( point->check_consistency(env) && !point->get_error(error) )
			{
				for( int i=0; i<xdr_n_metrics; i++ )
				{
					xdr_metric_pipes[i].std += pow( BOX(point->get_metric(i)) - xdr_metric_pipes[i].mean, 2);
				}
			}
		}
		for( int i=0; i<xdr_n_metrics; i++ )
		{
			xdr_metric_pipes[i].std = sqrt( xdr_metric_pipes[i].std/n );
			if( xdr_metric_pipes[i].std > 0.000000001 )
				xdr_metric_pipes[i].disabled = false;
		}	
	}
}

void m3_database::xdr_init_predictors_only(m3_env *env)
{
	xdr_metric_pipes.clear();
	xdr_n_predictors		= env->current_design_space->ds_parameters.size();
	xdr_n_metrics			= env->current_design_space->metric_index.size();
}

bool m3_database::xdr_write(m3_env *env, string file_name)
{
	int metrics_n	= env->current_design_space->metric_index.size();
	ofstream file_out( file_name.c_str(), ios::out);
	if( file_out.fail() )
		return false;
	list<m3_object *>::iterator i = points->begin();
	for( ; i!=points->end(); i++ )
	{
		m3_point *point	= (m3_point*)*i;
		string error;
		if( point->check_consistency(env) && !point->get_error(error) )
		{
			vector<int> 	design = *point;
			vector<double> 	observations;
			for( int i=0; i<metrics_n; i++ )
			{
				double new_metric = xdr_metric_pipes[i].pre_process(point->get_metric(i));
				observations.push_back( new_metric );
			}
			m3_xdr_write_design( file_out, design, observations);
		}
	}
	file_out.close();
	return true;
}

bool m3_database::xdr_write_predictors_only(m3_env *env, string file_name)
{
	ofstream file_out( file_name.c_str(), ios::out);
	if( file_out.fail() )
		return false;
	list<m3_object *>::iterator i = points->begin();
	for( ; i!=points->end(); i++ )
	{
		vector<double> 	observations;
		m3_point 	*point = (m3_point*)*i;
		vector<int> 	design = *point;
		m3_xdr_write_design( file_out, design, observations);
	}
	return true;
}

bool m3_database::xdr_read(m3_env *env, string file_name)
{
	clear();
	vector<double> design;
	vector<double> predictions;
	ifstream file_in( file_name.c_str() );
	while( m3_xdr_read_design( file_in, design, predictions)  )
	{
		if( design.size()!=xdr_n_predictors  )
			return false;
		if( predictions.size()!=xdr_n_metrics )
			return false;
		m3_point actual_point( xdr_n_predictors );
		for( int i=0; i<xdr_n_predictors; i++ )
		{
			actual_point[i] = (int) design[i];
		}
		m3_vector metrics;
		for( int i=0; i<xdr_n_metrics; i++ )
		{
			double 		value = xdr_metric_pipes[i].post_process( predictions[i] );
			m3_double 	obj( value );
			metrics.insert( i, obj);
		}
		actual_point.set_properties( "metrics", metrics );
		insert_point( &actual_point );
	}
	file_in.close();
	return true;
}

void m3_database::copy_xdr_data( m3_database *db)
{
	xdr_n_metrics		= db->xdr_n_metrics;
	xdr_n_predictors	= db->xdr_n_predictors;
	xdr_metric_pipes.clear();
	xdr_metric_pipes.resize( xdr_n_metrics );
	for( int i=0; i<xdr_n_metrics; i++ )
	{
		xdr_metric_pipes[i].box_cox_transform	= db->xdr_metric_pipes[i].box_cox_transform;
		xdr_metric_pipes[i].box_cox_is_log	= db->xdr_metric_pipes[i].box_cox_is_log;
		xdr_metric_pipes[i].disabled		= db->xdr_metric_pipes[i].disabled;
		xdr_metric_pipes[i].mean		= db->xdr_metric_pipes[i].mean;
		xdr_metric_pipes[i].std			= db->xdr_metric_pipes[i].std;
	}
}

m3_database *m3_database::get_random_subset( int points_number )
{
	if( points_number<1 || points_number>points->size() )
		throw m3_exception("Invalid parameter for the random database generation");

	m3_database	*subset		= new m3_database();
	list<m3_object *>::iterator	i;
	float		threshold	= ((float)points_number)/((float)points->size());

	for( i=points->begin(); i!=points->end(); i++ )
	{
		float	extraction	= randfloat();
		if( extraction<=threshold )
		{
			m3_point *p = (dynamic_cast<m3_point *>(*i));
			subset->insert_point( p );
		}		
	}
	return subset;
}

m3_database *m3_database::get_filtered_valid( m3_env *env )
{
	m3_database *valid_db 		= new m3_database();
	list<m3_object *>::iterator i 	= points->begin();
	for( ; i!=points->end(); i++ )
	{
		m3_point *p = (dynamic_cast<m3_point *>(*i));
		if( env->current_design_space->is_valid( env, p ) )
			valid_db->insert_point(p);
	}
	return valid_db;
}

m3_database *m3_database::get_filtered_consistent( m3_env *env )
{
	m3_database *consistent_db	= new m3_database();
	list<m3_object *>::iterator i	= points->begin();
	for( ; i!=points->end(); i++ )
	{
		m3_point *p = (dynamic_cast<m3_point *>(*i));
		string error;
		if( p->check_consistency(env) && !p->get_error(error) )
			consistent_db->insert_point( p );
	}
	return consistent_db;
}

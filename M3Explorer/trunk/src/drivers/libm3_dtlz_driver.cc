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
#include <sstream>
#include <m3_shell_variables.h>
#include <string>
#include <m3_parser.h>
#include <m3_sim_utils.h>
#include <m3_driver.h>
#include <math.h>


using namespace std;

class m3_dtlz_driver: public m3_driver
{
    vector<int> sample_num;
    public:
        string get_information();
        m3_design_space * get_design_space(m3_env* env);
        m3_vector * get_statistics();
        m3_point  * simulate(m3_point &, m3_env *);
        bool is_valid(m3_point &, m3_env *) { return true;};
        string get_name();
        int get_number_of_parameters();
        double f1(m3_point &);
        double f2(m3_point &);
        double f3(m3_point &);
        double g(m3_point &);
        double t1(m3_point &);
        double t2(m3_point &);
        /** Only for problem 6 */
        double h(double f1,double f2, double g);
        /** Only for problem 7 */
        double g1(m3_point &);
        double g2(m3_point &);
        double g3(m3_point &);
        m3_dtlz_driver();
        ~m3_dtlz_driver();
};

#define X_(k) (((double) x[(k)-1])/((double) sample_num[(k)-1] - 1.0))

double m3_dtlz_driver::f1(m3_point & x)
{
    string problem;

    m3_object* var;
    current_environment.shell_variables.get("problem", var);
    problem = to<m3_string *>(var)->get_string();
    if(problem == "dtlz1") return (1.0/2.0*X_(1)*X_(2)*(1.0+g(x)));
    if(problem == "dtlz2") return (cos(X_(1)*M_PI/2.0)*cos(X_(2)*M_PI/2.0)*(1.0+g(x)));
    if(problem == "dtlz3") return (cos(X_(1)*M_PI/2.0)*cos(X_(2)*M_PI/2.0)*(1.0+g(x)));
    if(problem == "dtlz4") return (cos(pow(X_(1),100)*M_PI/2)*cos(pow(X_(2),100)*M_PI/2.0)*(1.0+g(x)));
    if(problem == "dtlz5") return (cos(t1(x)*M_PI/2.0)*cos(t2(x))*(1.0+g(x)));
    if(problem == "dtlz6") return (X_(1));
    if(problem == "dtlz7") return (X_(1)+X_(2)+X_(3)+X_(4)+X_(5)+X_(6)+X_(7)+X_(8)+X_(9)+X_(10))/10.0;
    m3_assert(0);
}

double m3_dtlz_driver::f2(m3_point & x)
{
    string problem;
    
    m3_object* var;
    current_environment.shell_variables.get("problem", var);
    problem = to<m3_string *>(var)->get_string();
    if(problem == "dtlz1") return (1.0/2.0*X_(1)*(1.0-X_(2))*(1.0+g(x)));
    if(problem == "dtlz2") return (cos(X_(1)*M_PI/2.0)*sin(X_(2)*M_PI/2.0)*(1.0+g(x)));
    if(problem == "dtlz3") return (cos(X_(1)*M_PI/2.0)*sin(X_(2)*M_PI/2.0)*(1.0+g(x)));
    if(problem == "dtlz4") return (cos(pow(X_(1),100)*M_PI/2.0)*sin(pow(X_(2),100)*M_PI/2.0)*(1.0+g(x)));
    if(problem == "dtlz5") return (cos(t1(x)*M_PI/2.0)*sin(t2(x))*(1.0+g(x)));
    if(problem == "dtlz6") return (X_(2));
    if(problem == "dtlz7") return (X_(11)+X_(12)+X_(13)+X_(14)+X_(15)+X_(16)+X_(17)+X_(18)+X_(19)+X_(20))/10.0;
    m3_assert(0);
}

double m3_dtlz_driver::f3(m3_point & x)
{
    string problem;
    
    m3_object* var;
    current_environment.shell_variables.get("problem", var);
    problem = to<m3_string *>(var)->get_string();
    if(problem == "dtlz1") return (1.0/2.0*(1-X_(1))*(1.0+g(x)));
    if(problem == "dtlz2") return (sin(X_(1)*M_PI/2.0)*(1.0+g(x)));
    if(problem == "dtlz3") return (sin(X_(1)*M_PI/2.0)*(1.0+g(x)));
    if(problem == "dtlz4") return (sin(pow(X_(1),100)*M_PI/2.0)*(1.0+g(x)));
    if(problem == "dtlz5") return (sin(t1(x)*M_PI/2.0)*(1.0+g(x)));
    if(problem == "dtlz6") return (1.0+g(x))*h(f1(x),f2(x),g(x));
    if(problem == "dtlz7") return (X_(21)+X_(22)+X_(23)+X_(24)+X_(25)+X_(26)+X_(27)+X_(28)+X_(29)+X_(30))/10.0;
    m3_assert(0);
}

#define NORM(x) ((double) x.size())

double m3_dtlz_driver::g(m3_point & x)
{
    string problem;
    
    m3_object* var;
    current_environment.shell_variables.get("problem", var);
    problem = to<m3_string *>(var)->get_string();
    if(problem == "dtlz1" || problem == "dtlz3") 
    {
        double result = NORM(x)-2.0;
        double sum_res = 0.0;
        for(int i=3; i<=x.size(); i++)
        {
            sum_res += pow(X_(i)-0.5,2)-cos(20.0*M_PI*(X_(i)-0.5));
        }
        result = 100.0*(result + sum_res);
        return result;
    }
    if(problem == "dtlz2") 
    {
        double sum_res =0.0;
        for(int i=3; i<=x.size(); i++)
        {
            sum_res += pow(X_(i)-0.5,2);
        }
        return sum_res;

    }
    if(problem == "dtlz4") 
    {
        double sum_res =0.0;
        for(int i=3; i<=x.size(); i++)
        {
            sum_res += pow(X_(i)-0.5,2);
        }
        return sum_res;
    }
    if(problem == "dtlz5") 
    {
        double sum_res =0.0;
        for(int i=3; i<=x.size(); i++)
        {
            sum_res += pow(X_(i)-0.5,2);
        }
        return sum_res;
    }
    if(problem == "dtlz6") 
    {
        double sum_res =0.0;
        for(int i=3; i<=x.size(); i++)
        {
            sum_res += (X_(i));
        }
        sum_res = sum_res * 9.0 /(22.0-2.0); 
        return sum_res;
    }
}

double m3_dtlz_driver::t1(m3_point &x)
{
    return X_(1);
}

double m3_dtlz_driver::t2(m3_point &x)
{
    return M_PI/(4.0*(1.0+g(x)))*(1.0+2.0*g(x)*X_(2));
}

double m3_dtlz_driver::h(double f1,double f2, double g)
{
    return 3.0-f1/(1.0+g)*(1.0+sin(3.0*M_PI*f1))+f2/(1.0+g)*(1.0+sin(3.0*M_PI*f2));
}

double m3_dtlz_driver::g1(m3_point &x)
{
    return f3(x)+4.0*f1(x)-1.0;
}

double m3_dtlz_driver::g2(m3_point &x)
{
    return f3(x)+4.0*f2(x)-1.0;
}

double m3_dtlz_driver::g3(m3_point &x)
{
    return 2.0*f3(x)+f1(x)+f2(x)-1.0;
}

string m3_dtlz_driver::get_information()
{
    string info = "";
    info.append("DTLZ reference functions for testing optimization algorithms");
    return info;
}


m3_design_space *m3_dtlz_driver::get_design_space(m3_env* env)
{
    int discretization_factor;
    if(!get_integer_from_variables(&current_environment,"discretization_factor", discretization_factor))
    {
        prs_display_message("Please size of the discretized space with 'discretization_factor', defaulting to 10,000");
        discretization_factor = 10000;
	m3_integer value(discretization_factor);
        current_environment.shell_variables.insert("discretization_factor", value);
    }

    int num_of_parameters = get_number_of_parameters();
    sample_num.resize(num_of_parameters,1);


    m3_design_space *ds = new m3_design_space();
    for(int p=0; p<num_of_parameters; p++)
    {
	sample_num[p] = discretization_factor;
        /*m3_vector value_vector;
        for(int i = 0 ; i < sample_num[p]; i++)
        {
            ostringstream value_string;
            value_string << ((double) i)/(sample_num[p]-1);
            value_vector.insert(i, m3_string(value_string.str()));
        }*/
        ostringstream par_string;
        par_string << "x_" << p; 
        //value_vector.set_properties("name", m3_string(par_string.str()));
	
        ds->insert_scalar(&current_environment, par_string.str(), M3_SCALAR_TYPE_INTEGER, 0, discretization_factor-1, vector<string>() );
    }


    ds->insert_metric(env, "f1", "");
    ds->insert_metric(env, "f2", "");
    ds->insert_metric(env, "f3", "");
    return ds;
}

m3_vector *m3_dtlz_driver::get_statistics()
{
    return (new m3_vector());
}

m3_point *m3_dtlz_driver::simulate(m3_point &point, m3_env *env)
{
    m3_vector metrics;
    m3_vector stats;
    m3_point *fp;

    m3_point *simulated_point = new m3_point(point);
    m3_design_space *ds = current_environment.current_design_space;

    if(point.size() != ds->size())
    {
        simulated_point->set_error(M3_POINT_FATAL_ERROR,"dtlz point size error");
        return simulated_point;
    }

    double M1, M2, M3;
    
    M1 = f1(*simulated_point);
    M2 = f2(*simulated_point);
    M3 = f3(*simulated_point);

    string problem;
    get_string_from_variables(env,"problem", problem);

    if(problem == "dtlz7")
    {
        if(g1(*simulated_point) < 0 ||
           g2(*simulated_point) < 0 ||
           g3(*simulated_point) < 0)
        {
            simulated_point->set_error(M3_POINT_NON_FATAL_ERROR,"dtlz constraints broken");
            return simulated_point;
        }
    }

    m3_double M31(M1);
    m3_double M32(M2);
    m3_double M33(M3);

    metrics.insert(0, (M31));
    metrics.insert(1, (M32));
    metrics.insert(2, (M33));

    simulated_point->set_properties("metrics", metrics);
    simulated_point->set_properties("statistics", stats);

    return simulated_point;
}

string m3_dtlz_driver::get_name()
{
    string name = "dtlz";
    return name;
}

m3_dtlz_driver::m3_dtlz_driver()
{
    string problem;
    prs_display_message("Loading the dtlz driver");
    if (!get_string_from_variables(&current_environment,"problem", problem))
    {
        prs_display_message("Please specify the 'problem' variable as {'dtlz1' ... 'dtlz7'}, defaulting to dtlz1");
        problem = "dtlz1";
	m3_string value(problem);
        current_environment.shell_variables.insert("problem", value);
    }
}

int m3_dtlz_driver::get_number_of_parameters()
{
    string problem;
    
    m3_object* var;
    current_environment.shell_variables.get("problem", var);
    problem = to<m3_string *>(var)->get_string();
    if(problem == "dtlz1") return 7;
    if(problem == "dtlz2") return 12;
    if(problem == "dtlz3") return 12;
    if(problem == "dtlz4") return 12;
    if(problem == "dtlz5") return 12;
    if(problem == "dtlz6") return 22;
    if(problem == "dtlz7") return 30;
}


m3_dtlz_driver::~m3_dtlz_driver()
{
    prs_display_message("Removing dtlz driver");
}


extern "C"
{

    m3_dtlz_driver *drv_generate_driver()
    {
        prs_display_message("Creating the dtlz driver");
        return new m3_dtlz_driver();
    }

}

/* @M3EXPLORER_LICENSE_START@
 *
 * This file is part of the Multicube Explorer tool.
 *
 * Authors: Vittorio Zaccaria, Gianluca Palermo, Giovanni Mariani, Fabrizio Castro, Stefano Bolli (2009)
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



/* @additional_authors @, Stefano Bolli (2009)@ */


#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <m3_design_space.h>
#include <m3_shell_variables.h>
#include <m3_parser.h>
#include <m3_sim_utils.h>
#include <stdexcept>
#include <sstream>

// if DEBUG is defined some additional output are provided
//#define DEBUG

#define is_scalar(x) if(!(scalar_parameters.count(x)!=0)) throw m3_design_space_exception("expecting a scalar parameter")
#define is_perm(x) if(!(permutation_parameters.count(x)!=0)) throw m3_design_space_exception("expecting a permutation parameter")
#define is_mask(x) if(!(on_off_mask_parameters.count(x)!=0)) throw m3_design_space_exception("expecting a mask parameter")

int fac(int x)
{
    if(x<=1)
      return 1;
    else return x*fac(x-1); 
}

m3_design_space::m3_design_space()
{

}

/*
 * Random % d :  0 ... (d)-1
 * Random % d + min: min ... min+(d)-1
 * ->              : min ... min+(max-min+1)-1
 * ->              : min ... max
 */

         
void m3_design_space::insert_scalar(m3_env *env, string name, int type, int min, int max, vector<string> list)
{
            m3_parameter p;
            p.name = name;
            p.type = M3_DS_SCALAR;
            m3_scalar s;
            s.min = min;
            s.max = max;
            s.type = type;
            s.list_size = list.size();
            s.list = list;
            ds_parameters.push_back(p);            
            pair<string, int> index(p.name, ds_parameters.size()-1);
            ds_parameters_index.insert(index);
            pair<string, m3_scalar> my_pair(p.name, s);
            scalar_parameters.insert(my_pair);
}

void m3_design_space::insert_permutation(m3_env *, string name, bool variable_dimension, string variable_dimension_parameter, int fixed_dimension)
{
            m3_parameter p;
            p.name = name;
            p.type = M3_DS_PERMUTATION;
            m3_permutation s;
            s.variable_dimension = variable_dimension;
            s.variable_dimension_parameter = variable_dimension_parameter;
            s.fixed_dimension = fixed_dimension;

            ds_parameters.push_back(p);            

            pair<string, int> index(p.name, ds_parameters.size()-1);
            ds_parameters_index.insert(index);
            pair<string, m3_permutation> my_pair(p.name, s);
            permutation_parameters.insert(my_pair);
}

void m3_design_space::insert_on_off_mask(m3_env *, string name, bool variable_dimension, string variable_dimension_parameter, int fixed_dimension, bool no_on_set_size,
                                bool variable_on_set_size, string variable_on_set_size_parameter, int fixed_on_set_size)
{
            m3_parameter p;
            p.name = name;
            p.type = M3_DS_ON_OFF_MASK;

            m3_on_off_mask s;
            s.variable_dimension = variable_dimension;
            s.variable_dimension_parameter = variable_dimension_parameter;
            s.fixed_dimension = fixed_dimension;
            s.no_on_set_size = no_on_set_size;
            
            s.variable_on_set_size = variable_on_set_size;
            s.variable_on_set_size_parameter = variable_on_set_size_parameter;
            s.fixed_on_set_size = fixed_on_set_size;

            ds_parameters.push_back(p);            

            pair<string, int> index(p.name, ds_parameters.size()-1);
            ds_parameters_index.insert(index);
            pair<string, m3_on_off_mask> my_pair(p.name, s);
            on_off_mask_parameters.insert(my_pair);
}

int count_on(int k)
{
    //cout << "Counting: " << k << endl;
    unsigned int l = (unsigned int) k;
    int n=0;
    while(l!=0)
    {
       if(l & 1) 
       {
          //cout << "n: " << n << endl;
          n++;
       } 

       //cout << "l: " << l << endl;
       l = l >> 1; 
    }
    //cout << "Counted: " << n << endl;
    return n;
}

int exp2(int y)
{
   int j = 1;
   while(y>0)
   {
      j = j << 1; 
      y--;
   }
   return j;
}

bool look_for_next_on_set(int & current, int dimension, int osize)
{
   if(osize==0)
       return false;

   current++;
   int max = exp2(dimension);
   while(current<max)
   {
       if(count_on(current)==osize)
       {
           return true;
       }
       current++;
   } 
   return false;
}

bool look_for_previous_on_set(int & current, int dimension, int osize)
{
   if(current==0)
       return false;
   current--;
   while(current>0)
   {
       if(count_on(current)==osize)
           return true;
       current--;
   }
   return false;
}

void print_vector(vector <int> x)
{
   cout<<"[";
   for(int i=0; i<x.size();i++)
         cout<< x[i];
         
   cout<<" ]";


}

vector <int> factoradic_to_permutation(int fact, int dimension)
{


    int i, j;
    vector<int> data(dimension);
    vector<int> fc(dimension);
    vector<int> temp(dimension);

    // k-th factoradic
    for(j=1; j<=dimension; ++j)
    {
		fc[dimension-j]=fact%j;
		fact/=j;
    }
    // factoradic to permutation
    for(i=0; i<dimension; ++i)
    {
    	temp[i]=++fc[i];
    }
    data[dimension-1]=1;
    for(i=dimension-2; i>=0; --i)
    {
		data[i]=temp[i];
		for(j=i+1; j<dimension; ++j)
		{
		    if(data[j]>=data[i])
		    {
		    	++data[j];
		    }
		}
    }
    return data;
}

int permutation_to_factoradic(vector <int> perm)
{
    int dim = perm.size();
    int factoradic_rep = 0;
    
    vector <int> inv_vector(dim);
    
    for(int i=0; i<dim;i++)
    {
        inv_vector[i]=0;
        for(int j=0; j<dim; j++)
        {
            if(i<j && perm[i]>perm[j])
                inv_vector[i] = inv_vector[i]+1;
        }
    }
    
    for(int i=0; i<dim; i++)
    {
        factoradic_rep += inv_vector[i]*fac(dim-1-i);
    }
    
    return factoradic_rep;
}

vector<int> int_to_bit_vector(int j, int dimension)
{
    vector<int> data(dimension);
    while(dimension>0)
    {
        data[dimension-1] = ((j&1) ? 1:0);
        dimension--;
        j = j >> 1;
    }
    return data;
}

int bit_vector_to_int(vector<int> v)
{
    int temp=0;
    for(int i=v.size()-1;i>=0;i--)
    {
        temp+=v[i]*pow(2, (v.size()-i-1));
    }
    
    return temp;
}


m3_point m3_design_space::random_point_unsafe(m3_env *env)
{
    m3_point x(ds_parameters.size());
    map<string, m3_scalar>::iterator i;
    for(i=scalar_parameters.begin();i!=scalar_parameters.end();i++)
    {
        x[ds_parameters_index[i->first]] = randint(get_scalar_min(env, i->first), get_scalar_max(env, i->first));
    }
    
    map<string, m3_permutation>::iterator k;
    for(k=permutation_parameters.begin();k!=permutation_parameters.end();k++)
    {
        x[ds_parameters_index[k->first]]=randint(get_permutation_min(env, k->first, &x), get_permutation_max(env, k->first, &x)); 
    }
    
    map<string, m3_on_off_mask>::iterator j;
    for(j=on_off_mask_parameters.begin();j!=on_off_mask_parameters.end();j++)
    {
        x[ds_parameters_index[j->first]] = randint(get_mask_min(env, j->first, &x), get_mask_max(env,j->first, &x));
    }
    return x;
}

#define for_each_permutation(pr)  map<string, m3_permutation>::iterator pr; for(pr=permutation_parameters.begin(); pr!=permutation_parameters.end();pr++)

#define for_each_mask(pr)  map<string, m3_on_off_mask>::iterator pr; for(pr=on_off_mask_parameters.begin(); pr!=on_off_mask_parameters.end();pr++)

#define for_each_scalar(pr)  map<string, m3_scalar>::iterator pr; for(pr=scalar_parameters.begin(); pr!=scalar_parameters.end();pr++)

m3_point m3_design_space::random_point(m3_env *env)
{
    m3_point current = random_point_unsafe(env);
    for_each_mask(k)
    {
       current[ds_parameters_index[k->first]] = look_for_next_valid_mask(env,&current,k->first);
    } 
    return current;
}
void m3_design_space::set_mask(m3_env *env, m3_point *point, string parname, vector<int> v)
{
    is_mask(parname);
    int dimension = get_mask_dimension(env, parname, point);
    if (dimension == v.size())
    {

        (*point)[ds_parameters_index[parname]]=bit_vector_to_int(v);
    
    }
    else throw m3_design_space_exception("the mask vector has not the right size");

    return;
}


vector<int> m3_design_space::get_mask(m3_env *, m3_point *point, string parname)
{
    m3_on_off_mask & mask = on_off_mask_parameters[parname];
    int dimension;
    if(mask.variable_dimension)
        dimension = (*point)[ds_parameters_index[mask.variable_dimension_parameter]];
    else
        dimension = mask.fixed_dimension;
    int j = (*point)[ds_parameters_index[parname]];
    vector<int> data(dimension);
    while(dimension>0)
    {
        data[dimension-1] = ((j&1) ? 1:0);
        dimension--;
        j = j >> 1;
    }
    return data;
}

int m3_design_space::get_mask_dimension(m3_env *env, string parname, m3_point *point)
{
    if(!on_off_mask_parameters[parname].variable_dimension)
    {
        return (on_off_mask_parameters[parname].fixed_dimension);
    }
    else
    {
        return (*point)[ds_parameters_index[on_off_mask_parameters[parname].variable_dimension_parameter]];
    }
}

int m3_design_space::get_mask_on_set_size(m3_env *env, string parname, m3_point *point)
{

    if(on_off_mask_parameters[parname].variable_on_set_size && on_off_mask_parameters[parname].no_on_set_size==false)
  	return (*point)[ds_parameters_index[on_off_mask_parameters[parname].variable_on_set_size_parameter]];
    else
	if(on_off_mask_parameters[parname].no_on_set_size==true)
	    return on_set_size(get_mask(env, point, parname));
	else
            return (on_off_mask_parameters[parname].fixed_on_set_size);
}

#include <stdlib.h>
#include <limits.h>



string   m3_design_space::get_vector_representation(m3_env *env, vector<int> v)
{
    string s="";
    for(int n=0; n<v.size(); n++)
    {
        ostringstream str;
        str << v[n];
        s = s + ((n==0) ? "":"-") + str.str();
    }
    return s;
}

void m3_design_space::set_permutation(m3_env *env, m3_point *point, string parname, vector<int> v)
{
    is_perm(parname);
    int dimension = get_permutation_dimension(env, parname, point);
    if (dimension == v.size())
    {
        int temp=0;
        for(int i=0;i<dimension;i++)
        {
            int fact_n=1;
            for (int ii=1; ii<dimension-i; ii++) fact_n*=ii;
            
            int mod_j=0;
            for (int j=dimension-1; j>i; j--)
            {
                if (v[i]>v[j]) mod_j++;
            }

            temp+=mod_j*(fact_n);

        }
        
        (*point)[ds_parameters_index[parname]]=temp;
    
    }
    else throw m3_design_space_exception("the permutation vector has not the right size");

    return;
}


vector<int> m3_design_space::get_permutation(m3_env *env, m3_point *point, string parname)
{
    is_perm(parname);

    int index = (*point)[ds_parameters_index[parname]];
    int dimension = get_permutation_dimension(env, parname, point);
    int i, j;
    vector<int> data(dimension);
    vector<int> fc(dimension);
    vector<int> temp(dimension);

    // k-th factoradic
    for(j=1; j<=dimension; ++j)
    {
		fc[dimension-j]=index%j;
		index/=j;
    }
    // factoradic to permutation
    for(i=0; i<dimension; ++i)
    {
    	temp[i]=++fc[i];
    }
    data[dimension-1]=1;
    for(i=dimension-2; i>=0; --i)
    {
		data[i]=temp[i];
		for(j=i+1; j<dimension; ++j)
		{
		    if(data[j]>=data[i])
		    {
		    	++data[j];
		    }
		}
    }
    /*
    for(i=0; i<dimension; ++i)
    {
    	--data[i];
    }*/
    return data;
}

int m3_design_space::get_scalar_level(m3_env *env, string par, string symbol)
{
    is_scalar(par);

    if(scalar_parameters[par].type == M3_SCALAR_TYPE_INTEGER)
    {
       int level = (int)strtol(symbol.c_str(), (char **)NULL, 10); 
       return level;
    }
    else
    {
       for(int i=0;i<scalar_parameters[par].list_size;i++)
       {
            if(scalar_parameters[par].list[i]==symbol)
                return i;
       } 
    }
    throw m3_design_space_exception("the requested parameter is not a scalar");
}

string m3_design_space::get_scalar_min_symbol(m3_env *env, string parname)
{
    is_scalar(parname);

    if(scalar_parameters[parname].type != M3_SCALAR_TYPE_INTEGER)
    {
        return scalar_parameters[parname].list[0];
    }
    else
    {
        ostringstream str;
        str << scalar_parameters[parname].min;
        return str.str();
    }
}

string m3_design_space::get_scalar_max_symbol(m3_env *env, string parname)
{
    is_scalar(parname);

    if(scalar_parameters[parname].type != M3_SCALAR_TYPE_INTEGER)
    {
        return scalar_parameters[parname].list[scalar_parameters[parname].list.size()-1];
    }
    else
    {
        ostringstream str;
        str << scalar_parameters[parname].max;
        return str.str();
    }
}

int m3_design_space::get_scalar_max(m3_env *env, string parname)
{
    is_scalar(parname);

    m3_vector *bounds;
    if(!get_vector_from_variables(env, parname, bounds))
    {
        if(scalar_parameters[parname].type != M3_SCALAR_TYPE_INTEGER)
        {
            return scalar_parameters[parname].list_size-1;
        }
        else
        {
            return scalar_parameters[parname].max;
        }
    }
    string lower_bound = bounds->get_string_at(1);
    return get_scalar_level(env, parname, lower_bound);
}


int m3_design_space::get_scalar_min(m3_env *env, string parname)
{
    is_scalar(parname);

    m3_vector *bounds;
    if(!get_vector_from_variables(env, parname, bounds))
    {
        if(scalar_parameters[parname].type != M3_SCALAR_TYPE_INTEGER)
        {
            return 0;
        }
        else
        {
            return scalar_parameters[parname].min;
        }
    }
    string lower_bound = bounds->get_string_at(0);
    return get_scalar_level(env, parname, lower_bound);
}

vector<string> m3_design_space::get_scalar_range(m3_env *env, string parname)
{
    vector<string> v;
    m3_scalar sc;
    if(ds_parameters[ds_parameters_index[parname]].type == M3_DS_SCALAR)
    {
        m3_scalar sc;
        sc = scalar_parameters[parname];
        if(sc.type == M3_SCALAR_TYPE_INTEGER)
        {
            for(int i=sc.min; i<=sc.max;i++)
            {
                ostringstream str;
                str << i;
                v.push_back(str.str());
            }
        }
        else
        {
            return sc.list;
        }
    }
    return v;
}

string  m3_design_space::get_parameter_representation(m3_env *env, m3_point &current, string parname)
{
    if(ds_parameters[ds_parameters_index[parname]].type == M3_DS_SCALAR)
    {
        m3_scalar sc;
        sc = scalar_parameters[parname];
        if(sc.type == M3_SCALAR_TYPE_INTEGER)
        {
            ostringstream str;
            str << current[ds_parameters_index[parname]];
            return str.str();
        }
        else
        {
            return sc.list[current[ds_parameters_index[parname]]];
        }
    }
    else
    {
        if(ds_parameters[ds_parameters_index[parname]].type == M3_DS_PERMUTATION)
        {
            ostringstream str;
            str << current[ds_parameters_index[parname]];
            return "@" + get_vector_representation(env, get_permutation(env, &current, parname)) + "@";
        }
        else
        {
            m3_on_off_mask & mask = on_off_mask_parameters[parname];
            int dimension;
            if(mask.variable_dimension)
                dimension = current[ds_parameters_index[mask.variable_dimension_parameter]];
            else
                dimension = mask.fixed_dimension;
            return "(" + get_vector_representation(env, get_mask(env, &current, parname)) + ")";
        }
    }
    throw m3_design_space_exception("Inconsistency in the design space definition");
}

string m3_design_space::get_point_representation(m3_env *env, m3_point &current)
{
   vector<m3_parameter>::iterator k;
   if(!current.check_consistency(env))
       return "[Consistency violation, invalid point]";
   //cout << "representation for " ;
   //cout << current.print() << endl;
   string s = "[ ";
   for(k=ds_parameters.begin(); k!=ds_parameters.end(); k++)
   {
        //cout << "Creating representation for " << k->name << endl;
        s = s + k->name + "=" + get_parameter_representation(env, current, k->name) + " ";
   }
   s = s + "]";
   return s;
}
    

bool m3_design_space::next(m3_env *env, m3_point &current)
{
    bool maximum_reached=true;
    /** Check if the variable vectors have reached the maximum value */
    {
        for_each_permutation(k)
        {
            int max = get_permutation_max(env, k->first, & current);
            if(current[ds_parameters_index[k->first]] != max)
            {
                maximum_reached = false;
                break;
            }
        } 
        if(maximum_reached)
        {
            for_each_mask(k)
            {
                int max = get_mask_max(env, k->first, & current);
                if(current[ds_parameters_index[k->first]] != max)
                {
                    maximum_reached = false;
                    break;
                }
            } 
        }
    } 
    if(maximum_reached)
    {
        /** Adjust scalar values */
        bool scalar_maximum_reached;
        map<string, m3_scalar>::iterator k; 
        for(k=scalar_parameters.begin(); k!=scalar_parameters.end();k++)
        {
            int max = get_scalar_max(env, k->first);
            int min = get_scalar_min(env, k->first);
            if(current[ds_parameters_index[k->first]] != max)
            {
                current[ds_parameters_index[k->first]] ++;
                break;
            }
            else
            {
                current[ds_parameters_index[k->first]] = min; 
            }
        } 
        if(k==scalar_parameters.end()) /* maximum_reached */
        {
            return false;
        }
        
        /** Reset variable vectors */
        {
            for_each_permutation(k)
            {
                int min = get_permutation_min(env, k->first, & current);
                current[ds_parameters_index[k->first]] = min;
            } 
        }
        {
            for_each_mask(k)
            {
                int min = get_mask_min(env, k->first, & current);
                current[ds_parameters_index[k->first]] = min;
            } 
        }
        return true;
    }
    else
    {
        maximum_reached = true;
        /** Increment variable vectors */
        for_each_permutation(k)
        {
            int max = get_permutation_max(env, k->first, & current);
            int min = get_permutation_min(env, k->first, & current);
            if(current[ds_parameters_index[k->first]] != max)
            {
                current[ds_parameters_index[k->first]] ++;
                maximum_reached = false;
                break;
            }
            else
            {
                current[ds_parameters_index[k->first]] = min;
            }
        } 
        if(maximum_reached)
        {
            for_each_mask(k)
            {
               m3_on_off_mask & mask = on_off_mask_parameters[k->first];

               int osize;
               if(mask.variable_on_set_size)
                   osize = current[ds_parameters_index[mask.variable_on_set_size_parameter]];
               else
                   osize = mask.fixed_on_set_size;

               int dimension;
               if(mask.variable_dimension)
                   dimension = current[ds_parameters_index[mask.variable_dimension_parameter]];
               else
                   dimension = mask.fixed_dimension;


                int max = get_mask_max(env, k->first, & current);
                if(current[ds_parameters_index[k->first]] != max)
                {
                    int current_index = current[ds_parameters_index[k->first]];

                    if(!mask.no_on_set_size)
                    {
                        if(!look_for_next_on_set(current_index, dimension, osize))
                        {
                            cout << k->first << " has max " << max << " the value is " << current[ds_parameters_index[k->first]] << endl;
                            throw m3_design_space_exception("parameter is different from max but no next element does exist");
                        }
                        current[ds_parameters_index[k->first]] = current_index;
                        maximum_reached = false;
                        break;
                    }
                    else
                    {
                        current[ds_parameters_index[k->first]] ++;
                        maximum_reached = false;
                        break;
                    }
                }
                else
                {
                    int min = get_mask_min(env, k->first, & current);
                    current[ds_parameters_index[k->first]] = min;
                }
            }
            if(maximum_reached)
               throw m3_design_space_exception("inconsistency error, variable vectors seem to have reached an unforessen max");

            return true;
        }
        return true;
    }
}

m3_point m3_design_space::begin(m3_env *env)
{
    m3_point x(ds_parameters.size());
    map<string, m3_scalar>::iterator i;
    for(i=scalar_parameters.begin();i!=scalar_parameters.end();i++)
    {
        //cout << "Initializing parameter " << i->first << " index " << ds_parameters_index[i->first] << endl;
        x[ds_parameters_index[i->first]] = get_scalar_min(env, i->first);
    }
    
    map<string, m3_permutation>::iterator k;
    for(k=permutation_parameters.begin();k!=permutation_parameters.end();k++)
    {
        //cout << "Initializing parameter " << k->first << " index " << ds_parameters_index[k->first] << endl;
        x[ds_parameters_index[k->first]]=get_permutation_min(env, k->first, &x); 
    }
    
    map<string, m3_on_off_mask>::iterator j;
    for(j=on_off_mask_parameters.begin();j!=on_off_mask_parameters.end();j++)
    {
        x[ds_parameters_index[j->first]] = get_mask_min(env, j->first, &x);
        //cout << "Initialized parameter " << j->first << " with: " << x[ds_parameters_index[j->first]] << endl;
    }
    return x;
}
        
int m3_design_space::get_permutation_min(m3_env *env, string parname, m3_point *p)
{
    /** At the moment, permutation start at 0 up to n!-1 elements */
    return 0;
}

int m3_design_space::get_permutation_dimension(m3_env *env, string parname, m3_point *p)
{
    if(!permutation_parameters[parname].variable_dimension)
    {
        return (permutation_parameters[parname].fixed_dimension);
    }
    else
    {
        return (*p)[ds_parameters_index[permutation_parameters[parname].variable_dimension_parameter]];
    }
}

int m3_design_space::get_permutation_max(m3_env *env, string parname, m3_point *p)
{
   is_perm(parname);
   return fac(get_permutation_dimension(env, parname, p))-1;
}

int m3_design_space::get_mask_min(m3_env *env, string parname, m3_point *p)
{
   is_mask(parname);
   m3_on_off_mask & mask = on_off_mask_parameters[parname];
   if(mask.no_on_set_size)
   {
       return 0;
   }
   else
   {
       int osize;
       if(mask.variable_on_set_size)
           osize = (*p)[ds_parameters_index[mask.variable_on_set_size_parameter]];
       else
           osize = mask.fixed_on_set_size;

       int dimension;
       if(mask.variable_dimension)
           dimension = (*p)[ds_parameters_index[mask.variable_dimension_parameter]];
       else
           dimension = mask.fixed_dimension;

       
       int current = 0;

       if(osize == 0) 
           return 0;

       //if(osize == dimension)
         //  return current;
       
       if(!look_for_next_on_set(current, dimension, osize))
       {
           cout << "Dimension is " << dimension << endl;
           cout << "Onset size is " << osize << endl;
           throw m3_design_space_exception("cannot find a suitable minimal on_off mask");
       }

       return current; 
   }

}

int m3_design_space::get_mask_max(m3_env *env, string parname, m3_point *p)
{
   is_mask(parname);

   m3_on_off_mask & mask = on_off_mask_parameters[parname];
   if(mask.no_on_set_size)
   {
       int dimension;
       if(mask.variable_dimension)
           dimension = (*p)[ds_parameters_index[mask.variable_dimension_parameter]];
       else
           dimension = mask.fixed_dimension;

       return exp2(dimension)-1;
   }
   else
   {
       int osize;
       if(mask.variable_on_set_size)
           osize = (*p)[ds_parameters_index[mask.variable_on_set_size_parameter]];
       else
           osize = mask.fixed_on_set_size;

       int dimension;
       if(mask.variable_dimension)
           dimension = (*p)[ds_parameters_index[mask.variable_dimension_parameter]];
       else
           dimension = mask.fixed_dimension;

       int current = exp2(dimension)-1;

       if(osize == 0) 
           return 0;

       if(osize == dimension)
           return current;

       if(!look_for_previous_on_set(current, dimension, osize))
       {
           cout << "Dimension is " << dimension << endl;
           cout << "Onset size is " << osize << endl;
           throw m3_design_space_exception("cannot find a suitable maximal on_off mask");
       }

       return current; 
   }

}

bool m3_design_space::is_valid(m3_env *env, m3_point *p)
{
    // call the simulator validity checker (if simulator is not invoked return true)
    if(env->current_driver->is_valid(*p, env)==false)
	return false;

    // check #metrics, #parameters
    // bounds for scalars, permutations, masks
    if(p->check_consistency(env)==false)
	return false;

    string error_str;
    if(p->get_error(error_str)!=M3_POINT_NO_ERROR)
	return false;

    return true;
}


void m3_design_space::print(m3_env *env)
{
    cout << "Design space parameters: " << endl;
    {
        map<string, m3_scalar>::iterator i;
        for(i=scalar_parameters.begin();i!=scalar_parameters.end();i++)
        {
            cout << i->first << ": ";
            if(i->second.type == M3_SCALAR_TYPE_INTEGER)
            {
                cout << "[" ;
                for(int k=i->second.min; k<=i->second.max; k++)
                {
                    cout << "\"" << k << "\"";
                    if(k!=i->second.max)
                        cout << ", ";
                }
                cout << "]" << endl;
            }
            else
            {
                cout << "[" ;
                for(int k=0; k<i->second.list.size(); k++)
                {
                    cout << "\"" << i->second.list[k] << "\"";
                    if(k<(i->second.list.size()-1))
                        cout << ", ";
                }
                cout << "]" << endl;
            }
        }
    }
    {
        map<string, m3_permutation>::iterator i;
        for(i=permutation_parameters.begin();i!=permutation_parameters.end();i++)
        {
            cout << i->first << ": ";
            if(i->second.variable_dimension)
            {
                cout << "Variable vector permutation dependent on parameter " << i->second.variable_dimension_parameter;
            }
            else
            {
                cout << "Fixed vector permutation with size " << i->second.fixed_dimension;
            }
            cout << endl;
        }
    }
    {
        map<string, m3_on_off_mask>::iterator i;
        for(i=on_off_mask_parameters.begin();i!=on_off_mask_parameters.end();i++)
        {
            cout << i->first << ": ";
            if(i->second.variable_dimension)
            {
                cout << "Variable on_off_mask dependent on parameter " << i->second.variable_dimension_parameter ;
            }
            else
            {
                cout << "Fixed on_off_mask with size " << i->second.fixed_dimension ;
            }
            if(i->second.no_on_set_size)
            {
            	cout << ", all possible mask combinations";
            }
            else
            {
            	if(i->second.variable_on_set_size)
	            {
	                cout << ", variable on_set dependent on parameter " << i->second.variable_on_set_size_parameter;
	            }
                else
                {
                    cout << ", on_set size fixed at " << i->second.fixed_on_set_size;
                }
            }
            cout << endl;
        }
    }
    cout << "Metrics: " << endl;
    {
        for(int i=0; i<metric_names.size(); i++)
        {
            cout << metric_names[i] << endl;
        }
    }
}


m3_design_space_exception::m3_design_space_exception(string msg): m3_exception("DESIGN SPACE inconsistency exception, " + msg)
{
	
}

m3_design_space_exception::~m3_design_space_exception() throw()
{
	
}

bool m3_design_space::set_architecture_info(m3_env *env, m3_object *cf)
{
    vector<string> o;
    try
    { 
         o = get_string_vector(cf);
    }
    catch (m3_exception & x)
    {
        prs_display_error("Architecture info should be a list composed of the name of the architecture and the owner's name.");
        return false;
    }
    if(o.size()>=2)
    {
        architecture_name = o[0];
        architecture_owner = o[1];
    }
    else
    {
        if(o.size()==1)
        {
            architecture_name = o[0];
        }
        else
        {
            prs_display_error("Architecture info should be a list composed of the name of the architecture and the owner's name.");
            return false;
        }
    }
    return true;
}

bool m3_design_space::set_objectives_units(m3_env *env, m3_object *cf)
{
    vector<string> o;
    try
    { 
         o = get_string_vector(cf);
    }
    catch (m3_exception & x)
    {
        prs_display_error("The specified objectives are inconsistent");
        return false;
    }
    objectives_units.clear();
    for(int i=0; i<o.size(); i++)
    {
         objectives_units.push_back(o[i]);
    }
    return true;
}

bool m3_design_space::set_objectives_names(m3_env *env, m3_object *cf)
{
    vector<string> o;
    try
    { 
         o = get_string_vector(cf);
    }
    catch (m3_exception & x)
    {
        prs_display_error("The specified objectives are inconsistent");
        return false;
    }
    objectives_names.clear();
    for(int i=0; i<o.size(); i++)
    {
         objectives_names.push_back(o[i]);
    }
    return true;
}

bool m3_design_space::set_objectives(m3_env *env, m3_object *cf)
{
    vector<string> o;
    try
    { 
         o = get_string_vector(cf);
    }
    catch (m3_exception & x)
    {
        prs_display_error("The specified objectives are inconsistent");
        return false;
    }
    objectives.clear();
    objectives_units.clear();
    objectives_names.clear();
    for(int i=0; i<o.size(); i++)
    {
         objectives.push_back(o[i]);
         objectives_names.push_back(o[i]);
         objectives_units.push_back("");
    }
    return true;
}

void increment_evaluation_number(m3_env *env, int n)
{
    int num;
    if(!get_integer_from_variables(env, "evaluations", num))
    {
        num=0;
    }
    num += n;
    m3_integer m3num(num);
    env->shell_variables.insert("evaluations", m3num);
}

#include <sstream>
#include <fstream>


ifstream bc_out;
ofstream bc_in;

#define OUT 1
#define IN 0 

void m3_shutdown_expression_eval()
{
    remove("bc_input");
    remove("bc_output");
}

#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

pthread_mutex_t bc_lock;

#define MAXBUF 1000 

char buf[MAXBUF];

void m3_init_expression_eval()
{
    pid_t pid;

    int res = mkfifo("bc_input", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
    if((res == -1) && (errno!= EEXIST))
    {
        prs_display_error("Cannot init expression evaluation.");
        exit(1);
    }
    res = mkfifo("bc_output", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
    if((res == -1) && (errno!= EEXIST))
    {
        prs_display_error("Cannot init expression evaluation.");
        exit(1);
    }


    pid=fork();

    if(pid == -1)
    {
        prs_display_error("Cannot fork. Exit.");
        exit(1); 
    }
    else 
    {
        if(!pid)
        { 
#if !defined(__MAC_OSX__)
            if(!shell_command("bc < bc_input > bc_output"))
            {
                 cout << "Problems with the normal program\n"; 
                 exit(0);
            }
            exit(0);
#else
            if(!shell_command("bc < bc_input > bc_output"))
            {
                 cout << "Problems with the normal program\n"; 
                 exit(0);
            }
            exit(0);
#endif
        }
        else
        {
            bc_in.open("bc_input");
            bc_out.open("bc_output");
        }
    }
    pthread_mutex_init (&bc_lock, NULL);
}

/** 
 * The following evaluates an expression based on configurations x and y where
 *
 * x_par is parameter 'par' associated to x.
 * f_x   is the metric (or objective) associated to x.
 *
 * same for y.
 *
 * Example:
 * 
 * x_pn < y_pn && energy_x > energy_y
 *
 * where pn is a parameter.
 * and energy is a metric or an objective.
 *
 */

bool m3_design_space::eval_expression_2(  m3_env *env, 
                                        m3_point *x, 
                                        m3_point *y, 
                                        string expression, 
                                        double & value)
{
    value = 0;
    string error; 
    if(!x->get_error(error) && !y->get_error(error))
    {
        ostringstream bc_script;

        bc_script.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed
        bc_script.precision(6);

        for(int i=0; i<metric_names.size(); i++)
        {
            bc_script << metric_names[i] << "_" << x << " = " << x->get_metric(metric_index[metric_names[i]]) << endl;
            bc_script << metric_names[i] << "_" << y << " = " << y->get_metric(metric_index[metric_names[i]]) << endl;
        }
        
        string scr;
        for(int i=0; i<ds_parameters.size(); i++)
        {
            scr = scr + "x_" + ds_parameters[i].name + " = " + get_parameter_representation(env, *x, ds_parameters[i].name) + "\n";
            scr = scr + "y_" + ds_parameters[i].name + " = " + get_parameter_representation(env, *y, ds_parameters[i].name) + "\n";
        }
        bc_script << scr;
        
        for(int i=0; i<objectives.size(); i++)
        {
            double value = 0;
            bool res = eval_expression(env, x, objectives[i], true, value);
            if(!res)
                return false;
            bc_script << objectives_names[i] << "_x =" << value << endl;
            res = eval_expression(env, y, objectives[i], true, value);
            if(!res)
                return false;
            bc_script << objectives_names[i] << "_y =" << value << endl;
        }

        double res = 0;
        bc_script << "scale = 6" << endl;
        bc_script << expression << endl;

        scr = bc_script.str();

        pthread_mutex_lock(&bc_lock);

        bc_in << scr;
        bc_in.flush();
        bc_out >> res;

        pthread_mutex_unlock(&bc_lock);
        value = res;
        return true;
    }
    return false;
}

bool m3_design_space::eval_expression(  m3_env *env, 
                                        m3_point *p, 
                                        string expression, 
                                        bool is_objective_or_constraint, 
                                        double & value)
{
    value = 0;
    string error; 
    if(!p->get_error(error))
    {
        ostringstream bc_script;

        bc_script.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed
        bc_script.precision(6);

        for(int i=0; i<metric_names.size(); i++)
        {
            if(expression==metric_names[i])
            {
                value = p->get_metric(metric_index[metric_names[i]]);
                return true;
            }
            bc_script << metric_names[i] << " = " << p->get_metric(metric_index[metric_names[i]]) << endl;
        }
        
        string scr;
        for(int i=0; i<ds_parameters.size(); i++)
        {
            scr = scr + ds_parameters[i].name + " = " + get_parameter_representation(env, *p, ds_parameters[i].name) + "\n";
        }
        bc_script << scr;
        //cout << "Script: " << bc_script.str() << endl;
        
        if(!is_objective_or_constraint)
        {
            for(int i=0; i<objectives.size(); i++)
            {
                double value = 0;
                bool res = eval_expression(env, p, objectives[i], true, value);
                if(!res)
                    return false;
                bc_script << "obj_" << i << "=" << value << endl;
            }
        }
        double res = 0;
        bc_script << "scale = 6" << endl;
        bc_script << expression << endl;

        scr = bc_script.str();

        pthread_mutex_lock(&bc_lock);

        bc_in << scr;
        bc_in.flush();
        bc_out >> res;

        pthread_mutex_unlock(&bc_lock);
        value = res;
    }
    //exit(0);
    return true;
}

bool m3_design_space::evaluate(m3_env *env, m3_point *p)
{
    m3_point *ps;
    try
    {
        if((!(*env).available_dbs.count("root")) || ((ps=(*env).available_dbs["root"]->find_point(*p))==NULL))
        {
            //cout << "Looking for " << p->full_print() << endl;
            //cout << "Root is " << (*env).available_dbs.count("root") << endl;
            ps = env->current_driver->simulate(*p, env);

            if((*env).available_dbs.count("root"))
                (*env).available_dbs["root"]->insert_point(ps);

            p->copy_from(*ps);
            delete ps;
        }
        else
        {
            p->copy_from(*ps);
        }
        increment_evaluation_number(env,1);
    }
    catch(m3_exception &x)
    {
        return false;
    }
    return true;
}


typedef struct permutation_distances
{
    //double hamming;
    double hamming_normalized;
    double m3_dice;
    //double levenshtein;
    double levenshtein_normalized;
    //double shift_factor;
    double shift_factor_normalized;
    double avg_all_normalized;
}permutation_distances;


void get_closest_permutations(vector<int> start_permutation, vector<int> &close_permutations)
{
    int p_dim = start_permutation.size();
    vector <int> work_permutation;
    // the following is the facrtoradic representation
    vector <int> generated_permutation;
    vector<permutation_distances> distance_generated_permutation;
    
    permutation_distances min;

    // initialize min distances (note that normlized means that at max are 1.00)
    min.hamming_normalized = 2.00;
    min.m3_dice = 2.00;
    min.levenshtein_normalized = 2.00;
    min.shift_factor_normalized = 2.00;
    min.avg_all_normalized = 2.00;

    // cout<<"- - - 2 elements swap - - -"<<endl;   
      
    for(int i=0; i<p_dim; i++)
    {
        for(int j=i+1;j<p_dim;j++)
        {
            work_permutation = start_permutation;
       
            int temp = work_permutation[i];
            work_permutation[i] = work_permutation[j];
            work_permutation[j] = temp;
           
	    permutation_distances tmp_dist;

            tmp_dist.hamming_normalized = normalized_hamming_distance(start_permutation, work_permutation);
            tmp_dist.m3_dice = m3_dice_distance(start_permutation, work_permutation);
            tmp_dist.levenshtein_normalized = normalized_levenshtein_distance(start_permutation, work_permutation);
            tmp_dist.shift_factor_normalized=normalized_shift_factor(start_permutation, work_permutation);
            tmp_dist.avg_all_normalized = normalized_average_permutation_distance(start_permutation, work_permutation);
        
            // add permutation and its distance measure 
	    generated_permutation.push_back(permutation_to_factoradic(work_permutation));
	    distance_generated_permutation.push_back(tmp_dist);
            
	    if(tmp_dist.hamming_normalized < min.hamming_normalized && tmp_dist.avg_all_normalized > 0)
	        min.hamming_normalized = tmp_dist.hamming_normalized;

            if(tmp_dist.m3_dice < min.m3_dice && tmp_dist.avg_all_normalized > 0)
	        min.m3_dice = tmp_dist.m3_dice;

            if(tmp_dist.shift_factor_normalized < min.shift_factor_normalized && tmp_dist.avg_all_normalized > 0)
	        min.shift_factor_normalized = tmp_dist.shift_factor_normalized;


            #ifdef DEBUG
  	        cout<<"start_permutation: ";
                print_vector(start_permutation);
                cout<<" work_permutation: ";
                print_vector(work_permutation);
                printf(" %1.3f %1.3f %1.3f %1.3f %1.3f\n", tmp_dist.hamming_normalized, tmp_dist.m3_dice, tmp_dist.levenshtein_normalized, tmp_dist.shift_factor_normalized, tmp_dist.avg_all_normalized);   
            #endif


        }
    }
   
    // cout<<"- - - rotation - - -"<<endl;   
    for(int k=0; k<p_dim;k++)
    {
        int j=k;
        work_permutation = start_permutation;   
       
        for(int i=0; i<p_dim; i++)
        {
            if(j==p_dim)
                j=0;
          
            work_permutation[j] = start_permutation[i];
            j++;
        }   

	permutation_distances tmp_dist;

	tmp_dist.hamming_normalized = normalized_hamming_distance(start_permutation, work_permutation);
	tmp_dist.m3_dice = m3_dice_distance(start_permutation, work_permutation);
	tmp_dist.levenshtein_normalized = normalized_levenshtein_distance(start_permutation, work_permutation);
	tmp_dist.shift_factor_normalized=normalized_shift_factor(start_permutation, work_permutation);
	tmp_dist.avg_all_normalized = normalized_average_permutation_distance(start_permutation, work_permutation);
    
	// add permutation and its distance measure 
	generated_permutation.push_back(permutation_to_factoradic(work_permutation));
	distance_generated_permutation.push_back(tmp_dist);
	
	//if(tmp_dist.hamming_normalized < min.hamming_normalized && tmp_dist.avg_all_normalized > 0)
	//    min.hamming_normalized = tmp_dist.hamming_normalized;

	if(tmp_dist.m3_dice < min.m3_dice && tmp_dist.avg_all_normalized > 0)
	    min.m3_dice = tmp_dist.m3_dice;

	if(tmp_dist.shift_factor_normalized < min.shift_factor_normalized && tmp_dist.avg_all_normalized > 0)
	    min.shift_factor_normalized = tmp_dist.shift_factor_normalized;


	#ifdef DEBUG
	    cout<<"start_permutation: ";
	    print_vector(start_permutation);
	    cout<<" work_permutation: ";
	    print_vector(work_permutation);
            printf(" %1.3f %1.3f %1.3f %1.3f %1.3f\n", tmp_dist.hamming_normalized, tmp_dist.m3_dice, tmp_dist.levenshtein_normalized, tmp_dist.shift_factor_normalized, tmp_dist.avg_all_normalized);   
	#endif

    }

    #ifdef DEBUG
	cout<<" MIN: ";
        printf(" %1.3f %1.3f %1.3f\n", min.m3_dice, min.shift_factor_normalized, min.avg_all_normalized);   
    #endif


	 
    #ifdef DEBUG
	cout<<" SELECT CLOSEST: "<<endl;
    #endif

    for(int i=0; i<generated_permutation.size();i++)
    {
        if( distance_generated_permutation[i].avg_all_normalized >0 && 
	    (distance_generated_permutation[i].m3_dice == min.m3_dice || distance_generated_permutation[i].shift_factor_normalized == min.shift_factor_normalized) ) 
	{
	    close_permutations.push_back(generated_permutation[i]);
    
    	    #ifdef DEBUG
	        cout<<"start_permutation: ";
	        print_vector(start_permutation);
	        cout<<" work_permutation: ";
	        print_vector(factoradic_to_permutation(generated_permutation[i], p_dim));
                printf(" %1.3f %1.3f %1.3f %1.3f %1.3f\n", distance_generated_permutation[i].hamming_normalized, distance_generated_permutation[i].m3_dice, distance_generated_permutation[i].levenshtein_normalized, distance_generated_permutation[i].shift_factor_normalized, distance_generated_permutation[i].avg_all_normalized);   
	    #endif
        }
    }
}


typedef struct mask_distances
{
    //double hamming;
    double hamming_normalized;
    double m3_dice;
    //double levenshtein;
    double levenshtein_normalized;
    double avg_all_normalized;
}mask_distances;


void get_closest_masks(vector<int> start_mask, bool no_on_set_size,  vector<int> &close_masks)
{
    int m_dim = start_mask.size();
    vector <int> work_mask;
    // the following is the facrtoradic representation
    vector <int> generated_mask;
    vector<mask_distances> distance_generated_mask;
    
    mask_distances min;

    // initialize min distances (note that normlized means that at max are 1.00)
    min.hamming_normalized = 2.00;
    min.m3_dice = 2.00;
    min.levenshtein_normalized = 2.00;
    min.avg_all_normalized = 2.00;


    if(no_on_set_size == true)
    {
        for(int i=0, ut=0;i<m_dim;i++, ut++)
	{
	    work_mask=start_mask;
	    if(i==ut)
		if(work_mask[i]==0) 
		    work_mask[i]=1;
		else
		    work_mask[i]=0;
	    
	    mask_distances tmp_dist;
	    
	    tmp_dist.hamming_normalized = normalized_hamming_distance(start_mask, work_mask);
	    tmp_dist.m3_dice = m3_dice_distance(start_mask, work_mask);
	    tmp_dist.levenshtein_normalized = normalized_levenshtein_distance(start_mask, work_mask);
	    tmp_dist.avg_all_normalized = normalized_average_mask_distance(start_mask, work_mask);
	    
	    // add mask and its distance measure 
	    generated_mask.push_back(bit_vector_to_int(work_mask));
	    distance_generated_mask.push_back(tmp_dist);
		
	    if(tmp_dist.hamming_normalized < min.hamming_normalized && tmp_dist.avg_all_normalized > 0)
		min.hamming_normalized = tmp_dist.hamming_normalized;

	    if(tmp_dist.m3_dice < min.m3_dice && tmp_dist.avg_all_normalized > 0)
		min.m3_dice = tmp_dist.m3_dice;

	    #ifdef DEBUG
	        cout<<"start_mask: ";
	        print_vector(start_mask);
	        cout<<" work_mask: ";
	        print_vector(work_mask);
                printf(" %1.3f %1.3f %1.3f %1.3f\n", tmp_dist.hamming_normalized, tmp_dist.m3_dice, tmp_dist.levenshtein_normalized, tmp_dist.avg_all_normalized);   
            #endif
		     
	}
    }


    //  cout<<"- - - the same osize - - -"<<endl;   
    work_mask = start_mask;
    sort(work_mask.begin(), work_mask.end());
   
    // queste le considero in ogni caso
    do{

          mask_distances tmp_dist;
	
	  tmp_dist.hamming_normalized = normalized_hamming_distance(start_mask, work_mask);
	  tmp_dist.m3_dice = m3_dice_distance(start_mask, work_mask);
	  tmp_dist.levenshtein_normalized = normalized_levenshtein_distance(start_mask, work_mask);
	  tmp_dist.avg_all_normalized = normalized_average_mask_distance(start_mask, work_mask);
	
	   // add mask and its distance measure 
	   generated_mask.push_back(bit_vector_to_int(work_mask));
	   distance_generated_mask.push_back(tmp_dist);
	    
	   if(tmp_dist.hamming_normalized < min.hamming_normalized && tmp_dist.avg_all_normalized > 0)
	       min.hamming_normalized = tmp_dist.hamming_normalized;

	   if(tmp_dist.m3_dice < min.m3_dice && tmp_dist.avg_all_normalized > 0)
	       min.m3_dice = tmp_dist.m3_dice;

	    #ifdef DEBUG
	        cout<<"start_mask: ";
	        print_vector(start_mask);
	        cout<<" work_mask: ";
	        print_vector(work_mask);
                printf(" %1.3f %1.3f %1.3f %1.3f\n", tmp_dist.hamming_normalized, tmp_dist.m3_dice, tmp_dist.levenshtein_normalized, tmp_dist.avg_all_normalized);   
            #endif
   }while(next_permutation(work_mask.begin(), work_mask.end()));


    #ifdef DEBUG
	cout<<" MIN: ";
        printf(" %1.3f %1.3f %1.3f\n", min.hamming_normalized, min.m3_dice, min.avg_all_normalized);   
    #endif



    // select closest
    
    #ifdef DEBUG
	cout<<" SELECT CLOSEST: "<<endl;
    #endif

    for(int i=0; i<generated_mask.size();i++)
    {
        if( distance_generated_mask[i].avg_all_normalized >0 && 
	    (distance_generated_mask[i].m3_dice == min.m3_dice || distance_generated_mask[i].hamming_normalized == min.hamming_normalized) )
	{
	    close_masks.push_back(generated_mask[i]);
     
    	    #ifdef DEBUG
	        cout<<"start_mask: ";
    	        print_vector(start_mask);
	        cout<<" work_mask: ";
	        print_vector(int_to_bit_vector(generated_mask[i], m_dim));
                printf(" %1.3f %1.3f %1.3f %1.3f\n", distance_generated_mask[i].hamming_normalized, distance_generated_mask[i].m3_dice, distance_generated_mask[i].levenshtein_normalized, distance_generated_mask[i].avg_all_normalized);   
	    #endif
	}
    }
}


void m3_design_space::get_closest_points(m3_env *env, m3_point *central_p, vector<m3_point> &closest_points_v)
{
	m3_point *my_point;
	map<string, m3_scalar>::iterator i;
    for(i=scalar_parameters.begin();i!=scalar_parameters.end();i++)
    {
		if (!((*central_p)[ds_parameters_index[i->first]]==get_scalar_min(env, i->first)))
		{ 
			my_point = (m3_point*) central_p->gen_copy();
			(*my_point)[ds_parameters_index[i->first]] = (*central_p)[ds_parameters_index[i->first]]-1;
			if((env->current_design_space->is_valid(env, my_point)) && (closest_points_v.end()==find(closest_points_v.begin(),closest_points_v.end(), *my_point))) closest_points_v.push_back(*my_point);
            delete my_point;
		}
		if (!((*central_p)[ds_parameters_index[i->first]]==get_scalar_max(env, i->first)))
		{ 
			my_point = (m3_point*) central_p->gen_copy();
			(*my_point)[ds_parameters_index[i->first]] = (*central_p)[ds_parameters_index[i->first]]+1;
			if((env->current_design_space->is_valid(env, my_point)) && (closest_points_v.end()==find(closest_points_v.begin(),closest_points_v.end(), *my_point))) closest_points_v.push_back(*my_point);
            delete my_point;
		}
    }
	
    map<string, m3_permutation>::iterator k;
    for(k=permutation_parameters.begin();k!=permutation_parameters.end();k++)
    {
	int p_dimension = get_permutation_dimension(env, k->first, central_p);
	vector <int> close_permutations;
	vector <int> start_permutation = factoradic_to_permutation( (*central_p)[ds_parameters_index[k->first]], p_dimension);

	get_closest_permutations(start_permutation, close_permutations);

	for(int gg=0; gg<close_permutations.size();gg++)
	{
	    my_point = (m3_point*) central_p->gen_copy();
	    (*my_point)[ds_parameters_index[k->first]] = close_permutations[gg];

	    if((env->current_design_space->is_valid(env, my_point)) && (closest_points_v.end()==find (closest_points_v.begin(),closest_points_v.end(), *my_point))) 
	        closest_points_v.push_back(*my_point);

            delete my_point;
	}
   }
	
    map<string, m3_on_off_mask>::iterator j;
    for(j=on_off_mask_parameters.begin();j!=on_off_mask_parameters.end();j++)
    {
	int m_dimension = get_mask_dimension(env, j->first, central_p);
	int m_osize = get_mask_on_set_size(env, j->first, central_p);
	m3_on_off_mask & mask = on_off_mask_parameters[j->first];
	bool no_on_set_size = mask.no_on_set_size;

	vector <int> close_masks;
	vector <int> start_mask = int_to_bit_vector( (*central_p)[ds_parameters_index[j->first]], m_dimension);

	get_closest_masks(start_mask, no_on_set_size, close_masks);

	for(int gg=0; gg<close_masks.size();gg++)
	{  
	    my_point = (m3_point*) central_p->gen_copy();
	    (*my_point)[ds_parameters_index[j->first]] = close_masks[gg];

	    if((env->current_design_space->is_valid(env, my_point)) && (closest_points_v.end()==find (closest_points_v.begin(),closest_points_v.end(), *my_point))) 
	        closest_points_v.push_back(*my_point);

            delete my_point;
	}
    }

}

void m3_design_space::get_points_at_distance_N(m3_env *env, m3_point *central_p, int n,vector<m3_point> &point_v)
{	
//	m3_point my_point(ds_parameters.size());
//	my_point.copy_from(*central_p);
	if (n<1) point_v.push_back(*central_p);
	else
	{ 
		get_closest_points(env, central_p, point_v);
		int prev_point_v_size = 0;
		for(int ii=1; ii<n; ii++)
		{
			int point_v_size = point_v.size();
			for(int i_point_v=prev_point_v_size; i_point_v<point_v_size;i_point_v++)
			{
				//m3_point my_central_point(ds_parameters.size());
				//my_central_point.copy_from(point_v[i_point_v]);
				//get_closest_points(env, &(my_central_point), point_v);
				m3_point* my_central_point= (m3_point *) (point_v[i_point_v]).gen_copy();
				get_closest_points(env, my_central_point, point_v);
                delete my_central_point;
			}	
			prev_point_v_size=point_v_size;
		}
	}
}

m3_point* m3_design_space::get_random_point_at_distance_N(m3_env *env, m3_point *central_p, int n)
{
		vector<m3_point> my_vector;
		//m3_point* my_point= new m3_point(ds_parameters.size());
		get_points_at_distance_N(env, central_p, n, my_vector);
		int my_vector_size=my_vector.size();
                
                #ifdef DEBUG
                    cout << "size of the closest points: "<<my_vector_size<<endl;
    		    for(int i=0;i<my_vector_size;i++)
		    {
		        cout << "p: "<<get_point_representation(env, my_vector[i])<<endl;
		    }
		#endif


                m3_point *my_point = (m3_point*) (my_vector[randint(0, my_vector_size-1)]).gen_copy();
		return my_point;
}

/*
 * Position independent genetic crossover with two parents.
 * Each DS scalar parameter has equal chances to be inherited from parent_0 or parent_1.
 * Permutations with same size on both parents are crossedover by crossover_permutation
 * function. Permutations with different parents' size are inherited from the
 * parent which size has been inherited.
 *
 * For every on_off_mask OOM:
 * 1) if only one parent is compatible with the child (has same dimension and same
 * on_set_size) -> OOM is inherited from this parent
 * 2) if both parents are compatible -> OOM is crossedover by crossover_mask
 * 3) if no parent is compatible -> OOM it is randomly assigned
 */
m3_point m3_design_space::genetic_crossover(m3_env *env, m3_point *parent_0, m3_point *parent_1)
{
	m3_point child(ds_parameters.size());
	child.copy_from(*parent_0);

	for_each_scalar(i)//all scalars are taken from one of the two parent with same probability (default parent_0)
	{
		if(randfloat()<0.5)
			child[ds_parameters_index[i->first]] = (*parent_1)[ds_parameters_index[i->first]];
	}


	for_each_permutation(pr)//all permutations are crossed over
	{
		bool pr_compatible_0 = true;
		bool pr_compatible_1 = true;
		if( pr->second.variable_dimension )
		{
			int size_param_pos = ds_parameters_index[pr->second.variable_dimension_parameter];
			
			if( (*parent_0)[size_param_pos] != child[size_param_pos] )
				pr_compatible_0 = false;

			if( (*parent_1)[size_param_pos] != child[size_param_pos] )
				pr_compatible_1 = false;
		}

		if(pr_compatible_0 && pr_compatible_1)//complete crossover if both parent are compatible
			crossover_permutation(env,pr->first, parent_0, parent_1, child);
		else
		{
			if(pr_compatible_1)//inherited from parent_1 if this is the only compatible parent
				child[ds_parameters_index[pr->first]] = (*parent_1)[ds_parameters_index[pr->first]];

			if(pr_compatible_0)//inherited from parent_0 if this is the only compatible parent
				child[ds_parameters_index[pr->first]] = (*parent_0)[ds_parameters_index[pr->first]];

			if(!(pr_compatible_0 || pr_compatible_1))
			{
				cout << "Error in genetic_crossover" << endl;
				cout << "Child is not child of its parent" << endl;
				throw m3_design_space_exception("cannot identify any parent for this child");
			}
		}
	}


	for_each_mask(m)//all masks are crossed over
	{
		bool m_compatible_0 = true;
		bool m_compatible_1 = true;


		if( m->second.variable_dimension )//check parent compatability for dimensions
		{
			int size_param_pos = ds_parameters_index[m->second.variable_dimension_parameter];
			
			if( (*parent_0)[size_param_pos] != child[size_param_pos] )
				m_compatible_0 = false;

			if( (*parent_1)[size_param_pos] != child[size_param_pos] )
				m_compatible_1 = false;
		}

		if(!m->second.no_on_set_size && m->second.variable_on_set_size)//check parent compatability for on_set_size
		{
			int on_size_param_pos = ds_parameters_index[m->second.variable_on_set_size_parameter];

			if( (*parent_0)[on_size_param_pos] != child[on_size_param_pos] )
				m_compatible_0 = false;

			if( (*parent_1)[on_size_param_pos] != child[on_size_param_pos] )
				m_compatible_1 = false;
		}


		

		if(m_compatible_0 && m_compatible_1)//complete crossover if both parent are compatible
			crossover_mask(env,m->first, parent_0, parent_1, child);
		else
		{
			if(m_compatible_1)//inherited from parent_1 if this is the only compatible parent
				child[ds_parameters_index[m->first]] = (*parent_1)[ds_parameters_index[m->first]];

			if(m_compatible_0)//inherited from parent_0 if this is the only compatible parent
				child[ds_parameters_index[m->first]] = (*parent_0)[ds_parameters_index[m->first]];

			if(!m_compatible_0 && !m_compatible_1)//generate random if no parent are compatible
			{
						mutate_mask(env,&child,m->first);
			}
		}
	}
	if(!env->current_design_space->is_valid(env, &child))
		child = random_point(env);
	return child;
}


void m3_design_space::crossover_permutation(m3_env *env, string perm_name, m3_point *parent_0, m3_point *parent_1, m3_point &child)
{	// at the moment a whole permutation is considered as an atomic gene (inherited from one of the parents)
	if(randfloat()<0.5)
		child[ds_parameters_index[perm_name]] = (*parent_1)[ds_parameters_index[perm_name]];
	else
		child[ds_parameters_index[perm_name]] = (*parent_0)[ds_parameters_index[perm_name]];
}

void m3_design_space::crossover_mask(m3_env *env, string mask_name, m3_point *parent_0, m3_point *parent_1, m3_point &child)
{	// at the moment a whole mask is considered as an atomic gene (inherited from one of the parents)
	if(randfloat()<0.5)
		child[ds_parameters_index[mask_name]] = (*parent_1)[ds_parameters_index[mask_name]];
	else
		child[ds_parameters_index[mask_name]] = (*parent_0)[ds_parameters_index[mask_name]];
}


m3_point m3_design_space::genetic_mutation(m3_env *env, m3_point *mutant,double p_gen_mutate)
{
	m3_point mutated(ds_parameters.size());
	mutated.copy_from(*mutant);

	for_each_scalar(i)
	{
		if((double)rand() / (double)RAND_MAX < p_gen_mutate)
		{
			mutated[ds_parameters_index[i->first]] = randint(get_scalar_min(env, i->first), get_scalar_max(env, i->first));
		}
	}


	for_each_permutation(pr)
	{
		string size_param = pr->second.variable_dimension_parameter;
		if( pr->second.variable_dimension && 
		((*mutant)[ds_parameters_index[size_param]] != mutated[ds_parameters_index[size_param]]) )
		// if permutation size has been mutated -> generate a random permutation
		{
			mutate_permutation(env,&mutated,pr->first);
		}
		else
		{

			if((double)rand() / (double)RAND_MAX < p_gen_mutate)// probability of mutate a permutation is the same of mutate a scalar
			{
				mutate_permutation(env,&mutated,pr->first);
			}
		}
	}

	for_each_mask(m)
	{
		bool on_size_compatible = false;
		if(m->second.no_on_set_size)
			on_size_compatible = true;
		else
		{
			if(!m->second.variable_on_set_size)
				on_size_compatible = true;
			else
			{
				string size_param = m->second.variable_on_set_size_parameter;
				if( (*mutant)[ds_parameters_index[size_param]] == mutated[ds_parameters_index[size_param]] )
					on_size_compatible = true;
			}
		}

		bool dimension_compatible = false;
		if( !m->second.variable_dimension )
			dimension_compatible = true;
		else
		{
			string size_param = m->second.variable_dimension_parameter;
			if( (*mutant)[ds_parameters_index[size_param]] == mutated[ds_parameters_index[size_param]] )
				dimension_compatible = true;
		}

		if( dimension_compatible && on_size_compatible)// if mutant and mutated mask are fully compatible
		{
			if((double)rand() / (double)RAND_MAX < p_gen_mutate)// probability of mutate a mask is the same of mutate a scalar
			{
				mutate_mask(env,&mutated,m->first);
			}
		}
		else// if mutant and mutated mask are not fully compatible, generate a random acceptable mask.
		{
			mutate_mask(env,&mutated,m->first);
		}
	}
	if(!env->current_design_space->is_valid(env, &mutated))
		mutated = random_point(env);
	return mutated;
}


/** The following assumes that the mask is at least within the computed min and max */
int m3_design_space::look_for_next_valid_mask(m3_env *env, m3_point *p, string mask_name)
{
    int res = (*p)[ds_parameters_index[mask_name]];
	m3_on_off_mask & mask = on_off_mask_parameters[mask_name];

	int osize;
	if(mask.variable_on_set_size)
		osize = (*p)[ds_parameters_index[mask.variable_on_set_size_parameter]];
	else
		osize = mask.fixed_on_set_size;

	int dimension;
	if(mask.variable_dimension)
		dimension = (*p)[ds_parameters_index[mask.variable_dimension_parameter]];
	else
		dimension = mask.fixed_dimension;

	if(!mask.no_on_set_size)
	{
		if(count_on(res) != osize)
		{

			if(!look_for_next_on_set(res, dimension, osize))
			{
				res = get_mask_min(env, mask_name, p);
			}
		}
	}
	return res;
}

void m3_design_space::mutate_permutation(m3_env *env, m3_point *p, string perm_name)
{
	(*p)[ds_parameters_index[perm_name]]=randint(get_permutation_min(env, perm_name, p), get_permutation_max(env, perm_name, p));
}

void m3_design_space::mutate_mask(m3_env *env, m3_point *p, string mask_name)
{
    (*p)[ds_parameters_index[mask_name]] = randint(get_mask_min(env, mask_name, p), get_mask_max(env,mask_name, p));
    (*p)[ds_parameters_index[mask_name]] = look_for_next_valid_mask(env, p, mask_name);
}

#define center(a,b) ((a)+(((b)-(a)+1)/2))

void m3_design_space::convert_two_level_factorial_representation_from_int(m3_env *env, m3_point &point, int i, string &result, int mode)
{
   vector<string> the_string;
   the_string.resize(ds_parameters.size());
   i = i & ((1 << (ds_parameters.size()))-1);
   ostringstream str;
   str << " [ " ;
   for(int j=0; j<ds_parameters.size(); j++)
   {
       str << ((i & 1) ? "+":"-");
       the_string[ds_parameters.size()-1-j] = ((i & 1) ? "+":"-");
       i = i >> 1;
   }
   str << " ] ";
   result = str.str();
   convert_factorial_representation(env, point, the_string, mode);
} 

bool m3_design_space::convert_factorial_representation(m3_env *env, m3_point &point, vector<string> the_string, int mode)
{
    if(the_string.size() != ds_parameters.size())
        return false;  
   
    if(mode != TWO_LEVEL_FF_MODE_CLASSIC && mode != TWO_LEVEL_FF_MODE_SCRAMBLED)
        throw m3_design_space_exception("convert_factorial_representation: mode unknown");

    m3_point x(ds_parameters.size());
    map<string, m3_scalar>::iterator i;
    for(i=scalar_parameters.begin();i!=scalar_parameters.end();i++)
    {
        bool valid = false;
        if(the_string[ds_parameters_index[i->first]]=="+")
        {
            valid = true;
            x[ds_parameters_index[i->first]] = get_scalar_max(env, i->first);
        }

        if(the_string[ds_parameters_index[i->first]]=="-")
        {
            valid = true;
            x[ds_parameters_index[i->first]] = get_scalar_min(env, i->first);
        }
        
        if(the_string[ds_parameters_index[i->first]]=="0")
        {
            valid = true;
            x[ds_parameters_index[i->first]] = center(get_scalar_min(env, i->first), get_scalar_max(env, i->first));
        }
        if(!valid)
           return false; 
    }
    
    // work on permutation
    map<string, m3_permutation>::iterator k;
    for(k=permutation_parameters.begin();k!=permutation_parameters.end();k++)
    {
        bool valid = false;
        int idx;

	if(mode == TWO_LEVEL_FF_MODE_SCRAMBLED)
	{
            // get the index and if not available generate the sscramble pair according to the parameter in point
	    idx = get_permutation_opposite_level_index(env, &x, k->first);
	}
        if(the_string[ds_parameters_index[k->first]]=="+")
        {
            valid = true;
	    if(mode == TWO_LEVEL_FF_MODE_CLASSIC) //classic
                x[ds_parameters_index[k->first]] = get_permutation_max(env, k->first, &x);
	    else if(mode == TWO_LEVEL_FF_MODE_SCRAMBLED)  //extended
                x[ds_parameters_index[k->first]] = permutation_opposite_level[idx].high_level;       
        }

        if(the_string[ds_parameters_index[k->first]]=="-")
        {
            valid = true;
	    if(mode == TWO_LEVEL_FF_MODE_CLASSIC) //classic
                x[ds_parameters_index[k->first]] = get_permutation_min(env, k->first, &x);
	    else if(mode == TWO_LEVEL_FF_MODE_SCRAMBLED)  //extended
                x[ds_parameters_index[k->first]] = permutation_opposite_level[idx].low_level;
        }
        // not considered for permutation the third level
        if(the_string[ds_parameters_index[k->first]]=="0")
        {
            valid = true;
            x[ds_parameters_index[k->first]] = center(get_permutation_min(env, k->first, &x), get_permutation_max(env, k->first, &x));
        }
        if(!valid)
           return false; 
    }
  
    // work on mask 
    map<string, m3_on_off_mask>::iterator j;
    for(j=on_off_mask_parameters.begin();j!=on_off_mask_parameters.end();j++)
    {
        bool valid = false;
        int idx;

	if(mode == TWO_LEVEL_FF_MODE_SCRAMBLED)
	{
 	    // get the index and if not available generate the sscramble pair according to the parameter in point
            idx = get_mask_opposite_level_index(env, &x, j->first);
	}
        if(the_string[ds_parameters_index[j->first]]=="+")
        {
            valid = true;
	    if(mode == TWO_LEVEL_FF_MODE_CLASSIC)
                x[ds_parameters_index[j->first]] = get_mask_max(env, j->first, &x);
            else if (mode == TWO_LEVEL_FF_MODE_SCRAMBLED) //extended
                x[ds_parameters_index[j->first]] = mask_opposite_level[idx].high_level;
        }

        if(the_string[ds_parameters_index[j->first]]=="-")
        {
            valid = true;
	    if(mode == TWO_LEVEL_FF_MODE_CLASSIC)
                x[ds_parameters_index[j->first]] = get_mask_min(env, j->first, &x);
            else if(mode == TWO_LEVEL_FF_MODE_SCRAMBLED)
		x[ds_parameters_index[j->first]] = mask_opposite_level[idx].low_level;
        }
        // not considered for mask the third level
        if(the_string[ds_parameters_index[j->first]]=="0")
        {
            valid = true;
            x[ds_parameters_index[j->first]] = center(get_mask_min(env, j->first, &x), get_mask_max(env, j->first, &x));
            x[ds_parameters_index[j->first]] = look_for_next_valid_mask(env,&x,j->first);
        }
        if(!valid)
           return false; 
    }
    point = x;
    return true;
}

//-----------------------------------------------------------------------
//                 used for scrambled DOE
//----------------------------------------------------------------------- 

void m3_design_space::convert_two_level_factorial_representation_from_int_multi(m3_env *env, vector<m3_point> &points, int i, string &result)
{
   vector<string> the_string;
   the_string.resize(ds_parameters.size());
   i = i & ((1 << (ds_parameters.size()))-1);
   ostringstream str;
   str << " [ " ;
   for(int j=0; j<ds_parameters.size(); j++)
   {
       str << ((i & 1) ? "+":"-");
       the_string[ds_parameters.size()-1-j] = ((i & 1) ? "+":"-");
       i = i >> 1;
   }
   str << " ] ";
   result = str.str();
   convert_factorial_representation_multi(env, points, the_string);
} 

bool m3_design_space::convert_factorial_representation_multi(m3_env *env, vector<m3_point> &points, vector<string> the_string)
{
    if(the_string.size() != ds_parameters.size())
        return false;  
   
    m3_point x(ds_parameters.size());
    vector <m3_point> xx;


	map<string, m3_scalar>::iterator i;
	for(i=scalar_parameters.begin();i!=scalar_parameters.end();i++)
	{
	    bool valid = false;
	    if(the_string[ds_parameters_index[i->first]]=="+")
	    {
		valid = true;
		x[ds_parameters_index[i->first]] = get_scalar_max(env, i->first);
	    }

	    if(the_string[ds_parameters_index[i->first]]=="-")
	    {
		valid = true;
		x[ds_parameters_index[i->first]] = get_scalar_min(env, i->first);
	    }
	    
	    if(the_string[ds_parameters_index[i->first]]=="0")
	    {
		valid = true;
		x[ds_parameters_index[i->first]] = center(get_scalar_min(env, i->first), get_scalar_max(env, i->first));
	    }
	    if(!valid)
	       return false; 
	}
	
        xx.push_back(x);

	// work on permutation
	map<string, m3_permutation>::iterator k;
	for(k=permutation_parameters.begin();k!=permutation_parameters.end();k++)
	{
	    bool valid = false;
	    int idx;

	    idx = get_permutation_multi_opposite_level_index(env, &x, k->first);
	    if(the_string[ds_parameters_index[k->first]]=="+")
	    {
		valid = true;
	        int xx_init_size = xx.size();
	        for(int tt=0; tt<xx_init_size;tt++)
		{
		    m3_point temp = xx[tt];			
		    // from 1 because 0 is the low_generator level
		    for(int gg=1; gg<permutation_scrambled_level[idx].size(); gg++)
		    {
		       temp[ds_parameters_index[k->first]] = permutation_scrambled_level[idx][gg];
		       if(gg>1)
			   xx.push_back(temp);
		       else
			   xx[tt] = temp;
		    }
		}


	    }

	    if(the_string[ds_parameters_index[k->first]]=="-")
	    {
		valid = true;
                #ifdef DEBUG
		    cout<<"idx: "<<idx<<" size at idx "<< permutation_scrambled_level[idx].size()<<endl;
	        #endif
		int xx_init_size = xx.size();
		for(int tt=0; tt<xx_init_size;tt++)
		{
		    m3_point temp = xx[tt];			
		    temp[ds_parameters_index[k->first]] = permutation_scrambled_level[idx][0];
		    xx[tt] = temp;
		}
		//x[ds_parameters_index[k->first]] = permutation_scrambled_level[idx][0];
		
	       }
	    if(!valid)
	       return false; 
	}
      
	// work on mask 
	map<string, m3_on_off_mask>::iterator j;
	for(j=on_off_mask_parameters.begin();j!=on_off_mask_parameters.end();j++)
	{
	    bool valid = false;
	    int idx;

            idx = get_mask_multi_opposite_level_index(env, &x, j->first);
	    
	    if(the_string[ds_parameters_index[j->first]]=="+")
	    {
		valid = true;
		int xx_init_size = xx.size();
		for(int tt=0; tt<xx_init_size;tt++)
		{
		    m3_point temp = xx[tt];			
		    // from 1 because 0 is the low_generator level
		    for(int gg=1; gg<mask_scrambled_level[idx].size(); gg++)
		    {
		       temp[ds_parameters_index[j->first]] = mask_scrambled_level[idx][gg];
		       if(gg>1)
			   xx.push_back(temp);
		       else
			   xx[tt] = temp;
		    }
		}
	    }

	    if(the_string[ds_parameters_index[j->first]]=="-")
	    {
		valid = true;
                #ifdef DEBUG
    		    cout<<"idx: "<<idx<<" size at idx "<< mask_scrambled_level[idx].size()<<endl;
                #endif

		int xx_init_size = xx.size();
		for(int tt=0; tt<xx_init_size;tt++)
		{
		    m3_point temp = xx[tt];			
		    temp[ds_parameters_index[j->first]] = mask_scrambled_level[idx][0];
		    xx[tt] = temp;
		}
	       }
	    if(!valid)
	       return false; 
	}
   // }

    points= xx;
    return true;
}
//-----------------------------------------------------------------------
//               END  used for scrambled DOE
//-----------------------------------------------------------------------




bool m3_design_space::is_within_scalar_bounds(m3_env *env, m3_point point)
{
    m3_point x(ds_parameters.size());
    map<string, m3_scalar>::iterator i;
    for(i=scalar_parameters.begin();i!=scalar_parameters.end();i++)
    {
        //cout << "Initializing parameter " << i->first << " index " << ds_parameters_index[i->first] << endl;
        if(point[ds_parameters_index[i->first]] < get_scalar_min(env, i->first) ||
           point[ds_parameters_index[i->first]] > get_scalar_max(env, i->first))
            return false;
    }
    return true;
}

m3_point m3_design_space::consolidate(m3_env *env, m3_point point)
{
    m3_point x(ds_parameters.size());
    map<string, m3_scalar>::iterator i;
    for(i=scalar_parameters.begin();i!=scalar_parameters.end();i++)
    {
        //cout << "Initializing parameter " << i->first << " index " << ds_parameters_index[i->first] << endl;
        if(point[ds_parameters_index[i->first]] < get_scalar_min(env, i->first))
        {
            x[ds_parameters_index[i->first]] = get_scalar_min(env, i->first);
        }
        else
        {
            if(point[ds_parameters_index[i->first]] > get_scalar_max(env, i->first))
            {
                x[ds_parameters_index[i->first]] = get_scalar_max(env, i->first);
            }
            else
            {
                x[ds_parameters_index[i->first]] = point[ds_parameters_index[i->first]];
            }
        }
    }
    
    map<string, m3_permutation>::iterator k;
    for(k=permutation_parameters.begin();k!=permutation_parameters.end();k++)
    {
        if(point[ds_parameters_index[k->first]] <= get_permutation_max(env, k->first, &x))
            x[ds_parameters_index[k->first]] = point[ds_parameters_index[k->first]];
        else
            x[ds_parameters_index[k->first]] = get_permutation_max(env, k->first, &x); 
    }
    
    map<string, m3_on_off_mask>::iterator j;
    for(j=on_off_mask_parameters.begin();j!=on_off_mask_parameters.end();j++)
    {
        x[ds_parameters_index[j->first]] = look_for_next_valid_mask(env, &point, j->first);
    }
    return x;
}

//-----------------------------------------------------------------------------
//                 START scrambling functions
//-----------------------------------------------------------------------------


#define not_on_int(a) ((a&1) ? 0:1)

// given a 0 1 int vector returns the number of 1 elements
int on_set_size(vector <int> vect)
{
    int num_one=0;
    
    for(int i=0; i<vect.size(); i++)
        num_one+=vect[i];
        
    return num_one;
}

// return a vector containing the index of the vect_under_test whose corresponding value is equal to condition value parameter
vector <int> index_equal_to(vector <int> vect_under_test, int condition_value)
{
    vector <int> index;
    
    for(int i=0; i<vect_under_test.size(); i++)
        if(vect_under_test[i]==condition_value)
            index.push_back(i);

    return index;
}

// return the min distance (absolute difference) between the position value
// and the elements contained in vect_under_test
int min_abs_index_distance(vector <int> vect_under_test, int position)
{
    int min=abs(vect_under_test[0]-position);
    
    for(int i=1; i<vect_under_test.size(); i++)
        if(abs(vect_under_test[i]-position)<min)
            min=abs(vect_under_test[i]-position);

    return min;
}

// save the index of the biggest group (means consecutive elements) of 1 elements in
// the reference parameter saved_index, the number of elements  in the gruop in the
// refernce parameter saved_count, 
// while a 0 is returned if all is ok, a 1 if something go wrong.
int identify_group_of_1(vector <int> m, int *saved_index, int *saved_count)
{
    *saved_index = 0;
    *saved_count = 0;
    
    int error = 0;
    int count = 0;
    int start_index = 0;
    
    for(int i=0; i<m.size(); i++)
    {
        if(m[i] == 1)
        {
            // the following IF is probably unuseful
            if(i==0)
            {
                start_index=i;
                count=1;
            }
            else if(i>0 && m[i-1]==0)
            {
                start_index=i;
                count=1;                
            }
        }
        if(i>0 && m[i]==1 && m[i-1]==1)
            count++;
        if(i>0 && m[i]==0 && m[i-1]==1)
        {
            if(count>(*saved_count))
            {
                *saved_count=count;
                *saved_index=start_index;
            }
        }
        #ifdef DEBUG            
             cout<<"i: "<<i<<" m.size(): "<<m.size()<<endl;
        #endif
        if(i==(m.size()-1))
            if(count>(*saved_count))
            {
                *saved_count=count;
                *saved_index=start_index;
            }              
    }  // for
    if ((*saved_index)==0 && (*saved_count)==0)
        error=1;
        
    return error;
}

// return the scrambled of the start mask when 
// mask_on_set_size are FIXED
vector <int> scramble_mask_fix(vector <int> start_mask)
{
    // FIX FIX: mask_length and on_set_size are fixed
    
    int mask_length = start_mask.size();
    vector<int> not_mask(mask_length);
    vector<int> index_0;
    int k_num_1;

    #ifdef DEBUG
         cout<<"START: ";
         print_vector(start_mask);
         cout<<endl;    
    #endif
    
    for(int i=0;i<mask_length;i++)
        not_mask[i] = not_on_int(start_mask[i]);
        
    #ifdef DEBUG   
       cout<<"NOT: ";
       print_vector(not_mask);
       cout<<endl;    
       cout<<"on_set_size_START: "<<on_set_size(start_mask)<<" on_set_size_NOT: "<<on_set_size(not_mask)<<endl;
    #endif
            
    // The following implementation is the 'not_dist_group' described in the documentation
    if(on_set_size(start_mask) == on_set_size(not_mask))
        return not_mask;
    else if (on_set_size(start_mask)==0 || on_set_size(not_mask)==0)
        return start_mask;
    else
    {
        k_num_1 = on_set_size(start_mask);
        index_0 = index_equal_to(not_mask, 0);
        
        #ifdef DEBUG   
            cout<<"index_0: ";
            print_vector(index_0);
            cout<<endl;    
        #endif
        
        // Compute the distance
        vector <int> dist;
        vector <int> dist_elab;
        dist.assign(mask_length, 0);    // a repetition mask_length times of value 1
        
        for(int i=0; i<start_mask.size(); i++)
        {
            if(not_mask[i] != 0)
                dist[i] = min_abs_index_distance(index_0, i);
            else
                dist[i] = 0;
        }
        
        #ifdef DEBUG   
            cout<<"DISTANCE: ";
            print_vector(dist);
            cout<<endl;    
        #endif
        
        vector <int> temp_mask;
        dist_elab=dist;
        temp_mask.assign(mask_length,0);
        
        for(int j=0; j<k_num_1; j++)
        {
            
            // find maximum value and its corresponding index
            vector<int>::iterator position;
            position = max_element (dist_elab.begin(), dist_elab.end());
            int value_max = *position;
            int index_max = distance(dist_elab.begin(), position);
            
            if(value_max != 0)
            {
                temp_mask[index_max] = 1;
                dist_elab[index_max] = -1;
            }
            else
            {
                break;
            }
        }
        #ifdef  DEBUG           
            cout<<"TEMP_MASK: ";
            print_vector(temp_mask);
            cout<<endl;    
        #endif
       
        int num_1_to_place = k_num_1 - on_set_size(temp_mask);
        
        #ifdef DEBUG   
            cout<<"num_1_to_place: "<<num_1_to_place;
            cout<<endl;    
        #endif

        while(num_1_to_place > 0)
        {
            int group_index=0;
            int group_dim=0;
            int error=identify_group_of_1(temp_mask, &group_index, &group_dim);
            
            #ifdef DEBUG   
                cout<<"group_index: "<<group_index<<" group_dim: "<<group_dim<<" error: "<<error;
                cout<<endl;     
            #endif            
            
            if(error != 0)
            {
                cout<<"not_dist_group:error: no 1 present"<<endl;
            }
            
            // identify the group
            if(group_index>0)
            {
                temp_mask[group_index-1] = 1;
                num_1_to_place = num_1_to_place-1;
            }
            else if ((group_index+group_dim) < temp_mask.size())
            {
                temp_mask[group_index+group_dim] = 1;
                num_1_to_place = num_1_to_place-1;
            }
        }
        #ifdef DEBUG    
            cout<<"END: ";
            print_vector(temp_mask);
            cout<<endl;    
        #endif
        return temp_mask;
    }
}

// return the scrambled of the start mask when 
// mask_on_set_size is NOT DECLEARED
vector <int> scramble_mask_nd(vector <int> start_mask)
{    
    // FIX ND: mask_length fixed and on_set_size not decleared
    // correspond to a not of the mask
    
    int mask_length = start_mask.size();
    vector<int> not_mask(mask_length);
    
    for(int i=0;i<mask_length;i++)
        not_mask[i] = not_on_int(start_mask[i]);
        
    return not_mask;
}

#define PERC_LEVEL 0.3
// return the scrambled of the start permutation
vector <int> scramble_permutation(vector <int> start_permutation)
{
    int permutation_length=start_permutation.size();
    int g;
    
    #ifdef DEBUG       
        cout<<"START"<<endl;
        print_vector(start_permutation);
        cout<<endl;    
    #endif
   
    // The following implementation is the 'g_derangement' described in the documentation    
    g = round(permutation_length*PERC_LEVEL);
    for(int i=1; i<permutation_length; i++)
    {
        int m = (g%i);
        g = floor(g/i);
        
        int temp = start_permutation[i];
        start_permutation[i] = start_permutation[m];
        start_permutation[m] = temp;
    }

    #ifdef DEBUG             
        cout<<"SCRAMBLED"<<endl;
        print_vector(start_permutation);
        cout<<endl;    
    #endif
        
    return start_permutation;
}

//-----------------------------------------------------------------------------
//                 END scrambling functions
//-----------------------------------------------------------------------------

// now is linear in complexity THETA(n)
int m3_design_space::get_permutation_opposite_level_index(m3_permutation_low_high_parameter t)
{

    for(int i=0; i<permutation_opposite_level_index.size(); i++)
        if(t.permutation_name == permutation_opposite_level_index[i].permutation_name && t.variable_dimension==permutation_opposite_level_index[i].variable_dimension && t.dimension==permutation_opposite_level_index[i].dimension)
           return permutation_opposite_level_index[i].index;
          
   
    return -1;

}

// now is linear in complexity THETA(n)
int m3_design_space::get_mask_opposite_level_index(m3_mask_low_high_parameter t)
{

    for(int i=0; i<mask_opposite_level_index.size(); i++)
        if(t.mask_name == mask_opposite_level_index[i].mask_name && t.no_on_set_size==mask_opposite_level_index[i].no_on_set_size && t.variable_dimension==mask_opposite_level_index[i].variable_dimension && t.variable_on_set_size==mask_opposite_level_index[i].variable_on_set_size && t.on_set_size==mask_opposite_level_index[i].on_set_size && t.dimension==mask_opposite_level_index[i].dimension)
           return mask_opposite_level_index[i].index;
          
   
    return -1;

}

int m3_design_space::get_permutation_opposite_level_index(m3_env *env, m3_point *p, string parname)
{
    is_perm(parname);
      
    m3_permutation & permutation = permutation_parameters[parname];
    
    int dimension;
    if(permutation.variable_dimension)
        dimension = (*p)[ds_parameters_index[permutation.variable_dimension_parameter]];
    else
        dimension = permutation.fixed_dimension;

    m3_permutation_low_high_parameter x;
    x.permutation_name = parname;
    x.variable_dimension = permutation.variable_dimension;
    x.dimension = dimension; 
    x.index=-1;
    
    int index = get_permutation_opposite_level_index(x);

    if (index==-1)
    {
        // not exist an opposite level layer for that parameter          
        // INSERT
        x.index = scramble_permutation_generator(env, parname, p, x);      
        permutation_opposite_level_index.push_back(x);
        index = x.index;
    }
    return index;
}

// used for scramble doe
int m3_design_space::get_permutation_multi_opposite_level_index(m3_env *env, m3_point *p, string parname)
{
    is_perm(parname);
      
    m3_permutation & permutation = permutation_parameters[parname];
    
    int dimension;
    if(permutation.variable_dimension)
        dimension = (*p)[ds_parameters_index[permutation.variable_dimension_parameter]];
    else
        dimension = permutation.fixed_dimension;

    m3_permutation_low_high_parameter x;
    x.permutation_name = parname;
    x.variable_dimension = permutation.variable_dimension;
    x.dimension = dimension; 
    x.index=-1;
    
    int index = get_permutation_opposite_level_index(x);

    #ifdef DEBUG 
        cout<<"in MULTI: index: "<<index<<endl;
    #endif

    if (index==-1)
    {
        // not exist an opposite level layer for that parameter          
        // INSERT
        x.index = scramble_multi_permutation_generator(env, parname, p, x);      
        permutation_opposite_level_index.push_back(x);
        index = x.index;
    }
    return index;
}



int m3_design_space::get_mask_opposite_level_index(m3_env *env, m3_point *p, string parname)
{
    is_mask(parname);
      
    m3_on_off_mask & mask = on_off_mask_parameters[parname];
    
    int osize;  	     
    if(mask.variable_on_set_size && mask.no_on_set_size==false)
        osize = (*p)[ds_parameters_index[mask.variable_on_set_size_parameter]];
    else
        osize = mask.fixed_on_set_size;
		  
    int dimension;
    if(mask.variable_dimension)
        dimension = (*p)[ds_parameters_index[mask.variable_dimension_parameter]];
    else
	dimension = mask.fixed_dimension;

    m3_mask_low_high_parameter x;
    x.mask_name = parname;
    x.no_on_set_size = mask.no_on_set_size;
    x.variable_dimension = mask.variable_dimension;
    x.variable_on_set_size = mask.variable_on_set_size;
    x.on_set_size = osize;
    x.dimension = dimension; 
    x.index=-1;
    
    int index = get_mask_opposite_level_index(x);

    if (index==-1)
    {
        // not exist an opposite level layer for that parameter          
        // INSERT
        x.index = scramble_mask_generator(env, parname, p, x);      
        mask_opposite_level_index.push_back(x);
        index = x.index;
    }
    return index;
}

// used for scramble doe
int m3_design_space::get_mask_multi_opposite_level_index(m3_env *env, m3_point *p, string parname)
{
    is_mask(parname);
      
    m3_on_off_mask & mask = on_off_mask_parameters[parname];
    
    int osize;  	     
    if((mask.variable_on_set_size && mask.no_on_set_size==false))// || mask.no_on_set_size==true)
        osize = (*p)[ds_parameters_index[mask.variable_on_set_size_parameter]];
    else
        osize = mask.fixed_on_set_size;
		  
    int dimension;
    if(mask.variable_dimension)
        dimension = (*p)[ds_parameters_index[mask.variable_dimension_parameter]];
    else
	dimension = mask.fixed_dimension;

    m3_mask_low_high_parameter x;
    x.mask_name = parname;
    x.no_on_set_size = mask.no_on_set_size;
    x.variable_dimension = mask.variable_dimension;
    x.variable_on_set_size = mask.variable_on_set_size;
    x.on_set_size = osize;
    x.dimension = dimension; 
    x.index=-1;
    
    int index = get_mask_opposite_level_index(x);

    if (index==-1)
    {
        // not exist an opposite level layer for that parameter          
        // INSERT
        x.index = scramble_multi_mask_generator(env, parname, p, x);      
        mask_opposite_level_index.push_back(x);
        index = x.index;
    }
    return index;
}


vector <int> m3_design_space::permutation_scrambler(vector <int> start_permutation, m3_permutation_low_high_parameter cur_permutation)
{
   // there isn't any distinction because scrambling is defined for a fixed dimension
   // and permutation haven't any other parameter that can change
   return scramble_permutation(start_permutation);
}


vector <int> m3_design_space::mask_scrambler(vector <int> start_mask, m3_mask_low_high_parameter cur_mask)
{
   // there is distinction because scrambling is defined for a fixed dimension
   // but mask can have a fixed on_set_size (from a variable or a number in XML)
   // or a not decleared on_set_size, so it can be from 0 to mask dimension.
   // The following code discriminates the possible cases.
  
   vector <int> final_mask;
   //int dimension = cur_mask.dimension;
   int new_osize = cur_mask.on_set_size;
   
   // dimension: FIX on_set_size: NOT DECLARED
   if( /*cur_mask.variable_dimension==false && */cur_mask.no_on_set_size==true )
   {
       //cout<<"FIX ND"<<endl;
       final_mask = scramble_mask_nd(start_mask);       
   }
   // dimension: FIX on_set_size: FIX
   else if( /*cur_mask.variable_dimension==false &&*/ cur_mask.no_on_set_size==false && cur_mask.variable_on_set_size==false )
   {
       //cout<<"FIX FIX"<<endl;
       final_mask =  scramble_mask_fix(start_mask);	 
   }
   // dimension: FIX on_set_size: PARAMETER
   else if( /*cur_mask.variable_dimension==false &&*/ cur_mask.no_on_set_size==false && cur_mask.variable_on_set_size==true )
   {
       //cout<<"FIX PAR(fix)"<<endl;
       // is the same because scrambling is defined only for mask of the same dimension 
       //final_mask = scramble_mask_fix_par(start_mask, new_osize);
       final_mask = scramble_mask_fix(start_mask);
   }  
/*
   // dimension: PARAMETER on_set_size: NOT DECLARED
   else if( cur_mask.variable_dimension==true && cur_mask.no_on_set_size==true)
   {
       cout<<"PAR ND"<<endl;	
       // To be completed
       final_mask = scramble_mask_fix_nd(start_mask);       
   }
   // dimension: PARAMETER on_set_size: FIX
   else if( cur_mask.variable_dimension==true && cur_mask.no_on_set_size==false && cur_mask.variable_on_set_size==false )
   {
        cout<<"PAR FIX"<<endl;	
        // To be completed
        final_mask = scramble_mask_fix_fix(start_mask);
   }
   // dimension: PARAMETER on_set_size: PARAMETER
   else if( cur_mask.variable_dimension==true && cur_mask.no_on_set_size==false && cur_mask.variable_on_set_size==true )
   {
        cout<<"PAR PAR(fix)"<<endl;	
        // To be completed
        // final_mask = scramble_mask_fix_par(start_mask, new_osize);
    	final_mask = scramble_mask_fix_fix(start_mask);
   }*/
   else
   {
        cout<<"Should never arrive here, else there is an error->"<<endl;
   }
   return final_mask;
}


int m3_design_space::scramble_permutation_generator(m3_env *env, string parname, m3_point *p, m3_permutation_low_high_parameter cur_permutation)
{
   // FIRST generate low at random respect the contraints
   int random_permutation = randint(get_permutation_min(env, parname, p), get_permutation_max(env, parname, p));         
   m3_permutation & permutation = permutation_parameters[parname];
   int dimension = cur_permutation.dimension;
   m3_low_high_representation lh;
   lh.low_level = random_permutation;
   
   // THEN generate high according to value    	       
   vector <int> start_permutation = factoradic_to_permutation(random_permutation, dimension);
   vector <int> final_permutation;
  	
   final_permutation = permutation_scrambler(start_permutation, cur_permutation);	lh.high_level = permutation_to_factoradic(final_permutation);
   permutation_opposite_level.push_back(lh);

   //cout<<"permutation low: "<<lh.low_level<<" permutation high: "<<lh.high_level<<endl;
   return permutation_opposite_level.size()-1;
}

// used for scramble doe
int m3_design_space::scramble_multi_permutation_generator(m3_env *env, string parname, m3_point *p, m3_permutation_low_high_parameter cur_permutation)
{

   vector <int> scrambled_subset;
   // FIRST generate low at random respect the contraints
   int random_permutation = randint(get_permutation_min(env, parname, p), get_permutation_max(env, parname, p));         
   m3_permutation & permutation = permutation_parameters[parname];
   int dimension = cur_permutation.dimension;

   #ifdef DEBUG
       cout<<"scramble_multi_permutation_generator"<<endl;
   #endif

   // THEN generate high according to value    	       
   vector <int> low_level_start_permutation = factoradic_to_permutation(random_permutation, dimension);
   vector <int> cycle_start_permutation = low_level_start_permutation;
   vector <int> temp_permutation;
   int num_ex = 0;

   // this is a brute force code...
   while( permutation_to_factoradic(temp_permutation) != permutation_to_factoradic(low_level_start_permutation) || num_ex==0)
   {
       #ifdef DEBUG
           cout<<"PUSH_BACK NOW: "<<get_vector_representation(env, cycle_start_permutation)<<endl;
       #endif

       scrambled_subset.push_back(permutation_to_factoradic(cycle_start_permutation));
       temp_permutation = permutation_scrambler(cycle_start_permutation, cur_permutation);
       cycle_start_permutation = temp_permutation;
       num_ex++;
   }

   permutation_scrambled_level.push_back(scrambled_subset);
   #ifdef DEBUG
       cout << "permutation_scrambled_level.size(): "<<permutation_scrambled_level.size()<<endl;
   #endif
   return permutation_scrambled_level.size()-1;
}


int m3_design_space::scramble_mask_generator(m3_env *env, string parname, m3_point *p, m3_mask_low_high_parameter cur_mask)
{
 
   // FIRST generate low at random respect the contraints
   int random_mask = randint(get_mask_min(env, parname, p), get_mask_max(env, parname, p));         
   m3_on_off_mask & mask = on_off_mask_parameters[parname];
   int dimension = cur_mask.dimension;
   int osize = cur_mask.on_set_size;
	   
   if(!cur_mask.no_on_set_size)
       if(count_on(random_mask) != osize)
       {
           if(!look_for_next_on_set(random_mask, dimension, osize))
	   {
	       random_mask = get_mask_min(env, parname, p);
	   }
	}
   m3_low_high_representation lh;
   lh.low_level = random_mask;
   
   // THEN generate high according to value    	       
   vector <int> start_mask = int_to_bit_vector(random_mask, dimension);
   vector <int> final_mask;
   
   final_mask = mask_scrambler(start_mask, cur_mask);
   lh.high_level = bit_vector_to_int(final_mask);
   mask_opposite_level.push_back(lh);

   //cout<<"low: "<<lh.low_level<<" high: "<<lh.high_level<<endl; 
   //cout<<"mask_size: "<<mask_opposite_level.size()<<endl;
   return mask_opposite_level.size()-1;
}


// used for scramble doe 
int m3_design_space::scramble_multi_mask_generator(m3_env *env, string parname, m3_point *p, m3_mask_low_high_parameter cur_mask)
{
 
   vector <int> scrambled_subset;

   // FIRST generate low at random respect the contraints
   int random_mask = randint(get_mask_min(env, parname, p), get_mask_max(env, parname, p));         
   m3_on_off_mask & mask = on_off_mask_parameters[parname];
   int dimension = cur_mask.dimension;
   int osize = cur_mask.on_set_size;
	   
   if(!cur_mask.no_on_set_size)
       if(count_on(random_mask) != osize)
       {
           if(!look_for_next_on_set(random_mask, dimension, osize))
	   {
	       random_mask = get_mask_min(env, parname, p);
	   }
	}
   m3_low_high_representation lh;
   lh.low_level = random_mask;
   
   
   
   vector <int> start_mask = int_to_bit_vector(random_mask, dimension);
   
   #ifdef DEBUG
       cout<<"ADDING: "<<get_vector_representation(env, start_mask)<<" - "<<bit_vector_to_int(start_mask)<<endl;
   #endif

   scrambled_subset.push_back(bit_vector_to_int(start_mask));

   if(cur_mask.no_on_set_size==true)
   {
      
       // is only becasuse the scrambling is biunivocal
       vector <int> temp_mask;

       temp_mask = mask_scrambler(start_mask, cur_mask);

       #ifdef DEBUG
           cout<<"cur_mask_on: "<<cur_mask.on_set_size<<" temp_mask: "<<get_vector_representation(env, temp_mask)<<" - "<<bit_vector_to_int(temp_mask)<<endl;
           cout<<"cur_mask_on: "<<cur_mask.on_set_size<<" start_mask: "<<get_vector_representation(env, start_mask)<<" - "<<bit_vector_to_int(start_mask)<<endl;
       #endif
   
       scrambled_subset.push_back(bit_vector_to_int(temp_mask));
   }
   else
   {
       vector <int> temp_mask;
       vector <int> cycle_start_mask = start_mask;
       bool stop = false;
       int iter = 0;

       // at max a number of iteration like the mask dimension (is a sort of safe)
       while(stop==false && iter<dimension)
       {
	   temp_mask = mask_scrambler(cycle_start_mask, cur_mask);
	   
	   // if is not yet in the subset add
	   bool present = false;
           for(int ii = 0; ii<scrambled_subset.size() && present == false;ii++)
	   {
               if(scrambled_subset[ii] == bit_vector_to_int(temp_mask))
	       {
	           present = true;
	       }
	   }

	   if(present==false)
	   {
              #ifdef DEBUG
	          cout<<"ADDING: "<<get_vector_representation(env, temp_mask)<<" - "<<bit_vector_to_int(temp_mask)<<endl;
	      #endif

      	      scrambled_subset.push_back(bit_vector_to_int(temp_mask));
	      cycle_start_mask = temp_mask;
	   }
	   else
              stop=true;

	   iter++;
       }
   }

   mask_scrambled_level.push_back(scrambled_subset);

   return mask_scrambled_level.size()-1;
}



//-----------------------------------------------------------------------------
//                 START distance functions
//-----------------------------------------------------------------------------

double m3_dice_distance(vector<int> x, vector<int> y)
{
    vector<string> x_bigrams;
    vector<string> y_bigrams;
    stringstream tmp;
    
    for(int i = 0; i < (x.size() - 1); i++) 
    {   // extract character bigrams from x
        tmp.str("");
        tmp << x[i] << x[i+1];
        x_bigrams.push_back(tmp.str());
    }
    for(int i = 0; i < (y.size() - 1); i++) 
    {  // extract character bigrams from y
        tmp.str("");
        tmp << y[i] << y[i+1];
        y_bigrams.push_back(tmp.str());
    }
    
    int num_bigrams_overlap = 0;
    
    for(int i = 0; i<x_bigrams.size(); i++)
    {
        for(int j = 0; j<y_bigrams.size(); j++)
        {
            if(x_bigrams[i].compare("--")!=0 && y_bigrams[j].compare("--")!=0 && x_bigrams[i].compare(y_bigrams[j])==0)
            {
                num_bigrams_overlap++;
                x_bigrams[i]="--";
                y_bigrams[j]="--";
                break;
            }
        }
    }

    double dice_coefficient = (2 * (float)num_bigrams_overlap) / ((float)x_bigrams.size() + (float)y_bigrams.size());    
    double dice_max_coefficient = (2 * min((float)x_bigrams.size(), (float)y_bigrams.size())) / ((float)x_bigrams.size() + (float)y_bigrams.size());
        
    dice_coefficient = dice_coefficient/dice_max_coefficient;    
    return (1-dice_coefficient); // which is Dice distance
}


// adapted from wikipedia 
int levenshtein_distance(vector<int> x, vector<int> y)
{

   string s1;
   string s2;
   stringstream tmp;
    
   for(int i=0; i<x.size(); i++){
       tmp << s1 << x[i];       
   }
   s1=tmp.str();
   
   tmp.str("");
   for(int i=0; i<y.size(); i++){
       tmp << s2 << y[i];       
   }
   s2=tmp.str();
	
   int len1 = s1.size();
   int len2 = s2.size();
	
   vector<vector<int> > d(len1 + 1, vector<int>(len2 + 1));
 
   for(int i = 1; i <= len1; ++i)
       d[i][0] = i;
   for(int i = 1; i <= len2; ++i)
       d[0][i] = i;
 
   for(int i = 1; i <= len1; ++i)
       for(int j = 1; j <= len2; ++j)
           d[i][j] = std::min( std::min(d[i - 1][j] + 1,d[i][j - 1] + 1),
                               d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) );
   return d[len1][len2];
}


double normalized_levenshtein_distance(vector<int> x, vector<int> y)
{
   int lower = abs( (int)(x.size()-y.size()) );
   int upper = max(x.size(), y.size());
   
   return ((double)levenshtein_distance(x, y)-(double)lower)/(double)upper;
}


int hamming_distance(vector<int> x, vector<int> y)
{
    int dist = 0;
    
    if(x.size() != y.size())
    {
        cout<<"error: hamming_distance: vector of different size !"<<endl;
        return -1;
    }
    
    for(int i=0; i<x.size(); i++)
    {
        if(x[i] != y[i])
            dist++;
    }
    return dist;
}


double normalized_hamming_distance(vector<int> x, vector<int> y)
{
    if(x.size() != y.size())
    {
        cout<<"error: normalized_hamming_distance: vector of different size !"<<endl;
        return -1;
    }
    return (double)hamming_distance(x, y)/(double)x.size();
}


double shift_factor(vector<int> x, vector<int> y)
{
    if(x.size() != y.size())
    {
        cout<<"error: shift_distance: vector of different size !"<<endl;
        return -1;
    }
    
    int sf = 0;
    
    for(int i=0; i<x.size(); i++)
    {
        for(int j=0; j<y.size(); j++)
        {
            if(y[j]==x[i])
            {
                sf += abs(i-j);
            }
        }
    }
    return (double)sf/(double)x.size();
}


double normalized_shift_factor(vector<int> x, vector<int> y)
{
    if(x.size() != y.size())
    {
        cout<<"error: normalized_shift_factor: vector of different size !"<<endl;
        return -1;
    }
    
    int n = x.size();
    double upper;

    if ( n%2 == 0 )
        upper = ((double)n/2.00);
    else
        upper = ((double)n/2.00-(1.00/(2.00*(double)n)));

    return shift_factor(x, y)/upper;
}


double normalized_average_permutation_distance(vector<int> x, vector<int> y)
{
    // if dimension is different not consider hamming but a penalty
    if(x.size()!=y.size())
    {
        int max_size, min_size;
        // the 1.00 is the weigth of the length difference
        cout<<"not usign hamming because of different length"<<endl;
        if( x.size()>y.size() )
        {
            max_size=x.size();
            min_size=y.size();
        }
        else
        {
            max_size=y.size();
            min_size=x.size();
        }
       
        double penalty = (double)(max_size-min_size)/(double)max_size;
        return ( penalty + (double) m3_dice_distance(x, y) + normalized_levenshtein_distance(x, y) + normalized_shift_factor(x, y) )/4.00;
    }
    else
        return ( normalized_hamming_distance(x, y) + (double) m3_dice_distance(x, y) + normalized_levenshtein_distance(x, y) + normalized_shift_factor(x, y) )/4.00;
}


double normalized_average_mask_distance(vector<int> x, vector<int> y)
{
    // if dimension is different not consider hamming but a penalty
    if(x.size()!=y.size())
    {   
        int max_size, min_size;
        // the 1.00 is the weigth of the length difference
        cout<<"not usign hamming because of different length"<<endl;
        if( x.size()>y.size() )
        {
            max_size=x.size();
            min_size=y.size();
        }
        else
        {
            max_size=y.size();
            min_size=x.size();
        } 
       
        double penalty = (double)(max_size-min_size)/(double)max_size;
        return ( penalty + (double) m3_dice_distance(x, y) + normalized_levenshtein_distance(x, y) )/3.00;
    }
    else
        return ( normalized_hamming_distance(x, y) + (double) m3_dice_distance(x, y) + normalized_levenshtein_distance(x, y) )/3.00;
}

//-----------------------------------------------------------------------------
//                 END distance functions
//-----------------------------------------------------------------------------


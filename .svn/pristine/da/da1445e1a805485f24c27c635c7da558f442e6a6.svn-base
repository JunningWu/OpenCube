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




/* @additional_authors @, Stefano Bolli (2009)@*/


#ifndef M3_DESIGN_SPACE
#define M3_DESIGN_SPACE

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <m3_vector.h>

class m3_design_space;

#include <m3_env.h>
#include <m3_exception.h>

#define M3_SCALAR_TYPE_INTEGER  0
#define M3_SCALAR_TYPE_LIST     1

#define TWO_LEVEL_FF_MODE_CLASSIC 0
#define TWO_LEVEL_FF_MODE_SCRAMBLED 1

/** This class represents the scalar parameters within the design space.  */
struct m3_scalar
{
    /** The type can be either M3_SCALAR_TYPE_INTEGER or M3_SCALAR_TYPE_LIST */
    int type;
    
    /** Minimum used for M3_SCALAR_TYPE_INTEGER */
    int min;
    
    /** Maximum used for M3_SCALAR_TYPE_INTEGER */
    int max;
    
    /** Size of the symbol list associated to M3_SCALAR_TYPE_LIST */
    int list_size;
    
    /** Symbol list associated to M3_SCALAR_TYPE_LIST */
    vector<string> list;
};

/** This class represents the permutation parameters within the design space.  */
struct m3_permutation
{
    /** True if the permutation has variable dimension */
    bool variable_dimension;
    
    /** Parameter the dimension depends on */
    string variable_dimension_parameter;
    
    /** Contains the fixed dimension of the permutation */
    int fixed_dimension;
};


/** This class represents the on off masks within the design space.  */
struct m3_on_off_mask
{
    /** True if the mask has variable dimension */
    bool variable_dimension;
    
    /** True if the on set has variable dimension */
    bool variable_on_set_size;
    
    /** True if the mask does not have a fixed on set size */
    bool no_on_set_size;

    /** Contains the fixed dimension of the mask */
    int fixed_dimension;
    
    /** Contains the fixed on set size of the mask */
    int fixed_on_set_size;
    
    /** Parameter the dimension depends on */
    string variable_dimension_parameter;
    
    /** Parameter the on set size depends on */
    string variable_on_set_size_parameter;
};


/** This class represents a pair of scrambled configuration. 
 * This data structure is used both for mask and permutation. */
struct m3_low_high_representation
{
    /** Contains the low level for the representation */
    int low_level;
    
    /** Contains the high level for the representation */
    int high_level;
};


/** This class represents the parameters need to identify a low high mask pair.  */
struct m3_mask_low_high_parameter
{
    /** The mask name */    
    string mask_name;
    
    /** True if the mask has variable dimension */
    bool variable_dimension;
    
    /** True if the on set has variable dimension */
    bool variable_on_set_size;
    
    /** True if the mask does not have a fixed on set size */
    bool no_on_set_size;

    /** Contains the fixed dimension of the mask in a specific configuration */
    int on_set_size;
    
    /** Contains the fixed on set size of the mask in a specific configuration */
    int dimension;

    /** Contains the index where are available the low and high
        representation for mask parametrized as specified by above parameters */
    int index;
};

/** This class represents the parameters need to identify a low high permutation pair. */
struct m3_permutation_low_high_parameter
{
    /** The permutation name */    
    string permutation_name;
    
    /** True if the permutation has variable dimension */
    bool variable_dimension;
    
    /** Contains the fixed on set size of the permutation in a specific
        configuration */
    int dimension;

    /** Contains the index where are available the low and high representation for 
        permutation parametrized as specified by above parameters */
    int index;
};


#define M3_DS_SCALAR        0 
#define M3_DS_PERMUTATION   1 
#define M3_DS_ON_OFF_MASK   2  

/** This class represent a generic parameter */
struct m3_parameter
{
    /** Name of the parameter */
    string name;

    /** Type of the parameter. It can be:
     *
     * M3_DS_SCALAR 
     * M3_DS_PERMUTATION
     * M3_DS_ON_OFF_MASK
     */
    int type;
};

//------------------------------------------------------------------------
//  function prototypes for internal/external representation conversion
//------------------------------------------------------------------------

vector <int> factoradic_to_permutation(int fact, int dimension);

int permutation_to_factoradic(vector <int> perm);

vector<int> int_to_bit_vector(int j, int dimension);

int bit_vector_to_int(vector<int> v);

//------------------------------------------------------------------------
//                  function prototypes for distance    
//------------------------------------------------------------------------

double m3_dice_distance(vector<int> x, vector<int> y);

int levenshtein_distance(vector<int> x, vector<int> y);

double normalized_levenshtein_distance(vector<int> x, vector<int> y);

int hamming_distance(vector<int> x, vector<int> y);

double normalized_hamming_distance(vector<int> x, vector<int> y);

double shift_factor(vector<int> x, vector<int> y);

double normalized_shift_factor(vector<int> x, vector<int> y);

double normalized_average_mask_distance(vector<int> x, vector<int> y);

double normalized_average_permutation_distance(vector<int> x, vector<int> y);

//------------------------------------------------------------------------
//                   others function prototypes    
//------------------------------------------------------------------------
int on_set_size(vector <int> vect);

void print_vector(vector <int> x);

//------------------------------------------------------------------------


/** This class represents the design space associated to the current use case. 
 * It contains the API for generating and navigating points
 * witin the design space. It also contains a set of API functions for evolutionary algorithms.  
 * */
class m3_design_space
{
        public:

        string architecture_name;

        string architecture_owner;

        /** Vector of the parameters of the design space */
        vector<m3_parameter> ds_parameters;

        /** Scalar parameters the parameters of the design space */
        map<string, m3_scalar> scalar_parameters;

        /** Permutation parameters of the design space */
        map<string, m3_permutation> permutation_parameters;

        /** Mask parameters of the design space */
        map<string, m3_on_off_mask> on_off_mask_parameters;

        /** Map from parameter names to their position in the design space */
        map<string, int> ds_parameters_index;

        /** Map from permutation parameter to the index at which the opposite 
            configuration are available in permutation_opposite_level */  
        vector<m3_permutation_low_high_parameter> permutation_opposite_level_index;      
        /** Contain the opposite 'low' and 'high' levels for the permutation */
        vector<m3_low_high_representation> permutation_opposite_level;

	/** Contain the multiple scrambled value for scramble doe construction */
	vector< vector<int> > permutation_scrambled_level;

        /** Map from mask parameter to the index at which the opposite
            configuration are available in mask_opposite_level */        
        vector<m3_mask_low_high_parameter> mask_opposite_level_index;      

        /** Contain the opposite 'low' and 'high' levels for the mask */ 
        vector<m3_low_high_representation> mask_opposite_level;

	/** Contain the multiple scrambled value for scramble doe construction */
	vector< vector<int> > mask_scrambled_level;

        /** Current metrics available */
        vector<string> metric_names;
        
        /** Current metric units */
        vector<string> unit_names;

        /** Map from metrics names to their index */
        map<string, int> metric_index;

        /** Objective */
        vector<string> objectives;
        
        /** Objective */
        vector<string> objectives_names;

        /** Current objectives units */
        vector<string> objectives_units;


        /** Constructor */ 
        m3_design_space();
   
        /** Return the number of parameters in the design space
          * @return the number of parameters in the design space
          */ 
        int size() { return ds_parameters.size(); };

        /** Insert a metric in the design space 
         *
         * @param env the multicube environment
         * @param name the name of the metric 
	       * @param unit the unit of the metric
         */
        void insert_metric(m3_env *env, string name, string unit) 
        { 
            metric_names.push_back(name); 
            unit_names.push_back(unit); 
            metric_index.insert(pair<string, int>(name, metric_names.size()-1)); 
        }

        /** Insert a scalar into the design space. 
         *
         * Scalars can be either integers or lists of symbols (also called 'string').
         *
         * @param env the multicube environment
         * @param name the parameter name 
         * @param type the type of the parameter (either integer or list). It has the same definition of 'type' field in m3_scalar
         * @param min the minimum if the parameter is not a list.
         * @param max the maximum if the parameter is not a list.
         * @param list_of_values the list of values associated to the parameter.
         */ 

        void insert_scalar(m3_env *env, string name, int type, int min, int max, vector<string> list_of_values);

        /** Insert a permutation into the design space. 
         *
         * @param env the multicube environment
         * @param name the parameter name 
         * @param variable_dimension true if the dimension is variable 
         * @param variable_dimension_parameter the name of the scalar parameter that determines the size of the object.
         * At the moment the parameter should be M3_SCALAR_TYPE_INTEGER.
         * @param fixed_dimension the dimension of the fixed size permutation
         */ 
        void insert_permutation(m3_env *env, 
                                string name, 
                                bool variable_dimension, 
                                string variable_dimension_parameter, 
                                int fixed_dimension);

        /** Insert an on_off mask into the design space. 
         *
         * @param env the multicube environment
         * @param name the parameter name 
         * @param variable_dimension true if the dimension is variable 
         * @param variable_dimension_parameter the name of the scalar parameter that determines the size of the object.
         * At the moment the parameter should be M3_SCALAR_TYPE_INTEGER.
         * @param fixed_dimension the dimension of the fixed size permutation
         * @param no_on_set_size true if the mask does not have an on set size.
         * @param variable_on_set_size true if the parameter has a variable on set size.
         * @param variable_on_set_size_parameter  the name of the scalar parameter that determines the on set size. 
         * At the moment the parameter should be M3_SCALAR_TYPE_INTEGER.
         * @param fixed_on_set_size the fixed size of the on set. 
         */ 
        void insert_on_off_mask(m3_env *env, 
                                string name, 
                                bool variable_dimension, 
                                string variable_dimension_parameter, 
                                int fixed_dimension, 
                                bool no_on_set_size,
                                bool variable_on_set_size, 
                                string variable_on_set_size_parameter, 
                                int fixed_on_set_size);

        /** Computes the internal coding of the symbol associated to a parameter.
         * Throws an exception if the symbol can't be cast to an internal parameter value.
         *
         * @param env the multicube environment
         * @param par the name of the parameter
         * @param symbol the symbol for which the internal coding should be returned
         * @return the internal coding of the parameter. iin the case of M3_SCALAR_TYPE_LIST, this is the
         * index of the symbol within the list.
         * */
        int get_scalar_level(m3_env *env, string par, string symbol);

        /** Returns the symbol associated to the minimum value of a parameter.
         *
         * @param env the multicube environment
         * @param parname the name of the parameter
         * @return the string representation of the minimum value
         */
        string get_scalar_min_symbol(m3_env *env, string parname);
        
        /** Returns the symbol associated to the maximum value of a parameter.
         *
         * @param env the multicube environment
         * @param parname the name of the parameter
         * @return the string representation of the maximum value
         */
        string get_scalar_max_symbol(m3_env *env, string parname);

        /* SUPPORT TO FULL SEARCH */

        /** Initializes the iterator to the full search.
         *
         * @param env the multicube environment.
         * @return the first point of the design space.
         */
        m3_point begin(m3_env *env);

        /** Computes the next point in the design space.
         *
         * @param env the multicube environment.
         * @param point the point for which the next point should be computed.
         * @return true if the next point has been succesfully found. 
         */
        bool next(m3_env *env, m3_point &point);

        /* SUPPORT FOR RANDOM SEARCH */

        /** Computes a very random point in the design space. 
         * It can be not feasible due to internal parameter representation.
         *
         * @param env the multicube environment
         * @return the random point
         */
        m3_point random_point_unsafe(m3_env *env);
        
        /** Computes a random point in the design space. 
         * The point is always feasible due to internal parameter representation.
         *
         * @param env the multicube environment
         * @return the random point
         */
        m3_point random_point(m3_env *env);

        /** Computes the next valid mask value associated to a parameter in a point.
         * If the mask is already feasible, it is returned as it is.
         *
         * @param env the multcube environment
         * @param point the point in the design space
         * @param mask_name the mask name
         * @return the current mask value
         */
        int look_for_next_valid_mask(m3_env *env, m3_point *point, string mask_name);
       
        /* NEIGHBORHOOD SEARCH */ 

        /** Returns a set of points closest to a point.
         *
         * @param env the M3 env
         * @param point the point
         * @param nearest vector of nearest points
         */
        void get_closest_points(m3_env *env, m3_point *point, vector<m3_point> &nearest);

        /** Returns a set of points closest to a point within distance N.
         *
         * @param env the M3 env
         * @param point the point
         * @param nearest vector of nearest points
         * @param N the distance to be considered
         */
        void get_points_at_distance_N(m3_env *env, m3_point *point, int N,vector<m3_point> &nearest);

        /** Returns a random point within distance N.
         *
         * @param env the M3 env
         * @param point the point
         * @param N the distance to be considered
         * @return a random point within distance N
         */
        m3_point* get_random_point_at_distance_N(m3_env *env, m3_point *point, int N);

        /* EVOLUTIONARY ALGORITHMS */
	/** Uniform random genetic crossover operator. Return a point child with position inherited
	 * from the two parents. Each Design Space parameter has equal probability to be inherited
	 * from one of the parents.
         *
         * @param env the M3 env
         * @param parent_0 the first parent of the child
         * @param parent_1 the second parent of the child
         * @return the inherited child point
         */
        m3_point genetic_crossover(m3_env *env, m3_point *parent_0, m3_point *parent_1);

	/** Uniform random mutation operator. Return a point with position mutated from the given one.
         *
         * @param env the M3 env
         * @param mutant the mutating point
         * @param p_gen_mutate the probability of mutation, default 10%
         * @return the mutated point
         */
        m3_point genetic_mutation(m3_env *env, m3_point *mutant,double p_gen_mutate=0.2);

        private:

	/**  Performs the crossover operation inheriting a permutation parameter from given parents.
         *
         * @param env the M3 env
         * @param perm_name the name of the permutation target
         * @param parent_0 the first parent of the child
         * @param parent_1 the second parent of the child
	 * @param child contains the inherited child
         */
        void crossover_permutation(m3_env *env, string perm_name, m3_point *parent_0, m3_point *parent_1, m3_point &child);

	/**  Performs the crossover operation inheriting a mask parameters from given parents.
         *
         * @param env the M3 env
         * @param mask_name the name of the target mask
         * @param parent_0 the first parent of the child
         * @param parent_1 the second parent of the child
	 * @param child contains the inherited child
         */
        void crossover_mask(m3_env *env, string mask_name, m3_point *parent_0, m3_point *parent_1, m3_point &child);

	/**  Performs the mutation operation for a given permutation over a given ponit.
         *
         * @param env the M3 env
         * @param point the point
         * @param perm_name the name of the target permutation
         */
        void mutate_permutation(m3_env *env, m3_point *point, string perm_name);

	/**  Performs the mutation operation for a given mask over a given ponit.
         *
         * @param env the M3 env
         * @param point the point
         * @param mask_name the name of the target mask
         */
        void mutate_mask(m3_env *env, m3_point *point, string mask_name);

        public:
        /* INTERNAL TO EXTERNAL REPRESENTATIONS */
       
        /** Converts a two level, boolean factorial representation into a feasible design space point.
         *
         * @param env the multicube environment
         * @param point the computed point in the design space
         * @param i the value of the two level representation (it is the natural integer representation of the boolean vector)
         * @param res contains the '+''-' representation of the computed vector.
	 * @param mode the mode in which the ff has to be used (TWO_LEVEL_FF_MODE_CLASSIC or TWO_LEVEL_FF_MODE_SCRAMBLED)
         *  */ 
        void     convert_two_level_factorial_representation_from_int(m3_env *env, m3_point &point, int i, string & res, int mode);

        /** Converts a two level, boolean factorial representation into a feasible design space point, this is specific for scramble doe contempling more tha one point generation from the starting string representation.
         *
         * @param env the multicube environment
         * @param points the computed point in the design space
         * @param i the value of the two level representation (it is the natural integer representation of the boolean vector)
         * @param res contains the '+''-' representation of the computed vector.
         *  */ 
        void     convert_two_level_factorial_representation_from_int_multi(m3_env *env, vector<m3_point> &points, int i, string & res);


        /** Converts a three level, factorial representation into a feasible design space point.
         *
         * @param env the multicube environment
         * @param point the computed point
         * @param fact the string vector representing the factorial representation
	 * @param mode the mode in which the ff has to be used (TWO_LEVEL_FF_MODE_CLASSIC or TWO_LEVEL_FF_MODE_SCRAMBLED)
         * @return true the representation was feasible
         */
        bool     convert_factorial_representation(m3_env *env, m3_point &point, vector<string> fact, int mode);

        /** Converts a three level, factorial representation into a feasible design space point, this is specific for scramble doe contempling more than one point generation from the starting string representation.
         *
         * @param env the multicube environment
         * @param points the computed vector of points
         * @param fact the string vector representing the factorial representation
         * @return true the representation was feasible
         */
        bool     convert_factorial_representation_multi(m3_env *env, vector<m3_point> &points, vector<string> fact);

        /** Returns the point representation in string format
         *
         * @param env the multicube environment
         * @param point the point to be represented 
         * @return the point representation
         */
        string   get_point_representation(m3_env *env, m3_point &point); 

        /** Returns the string representation of a single parameter. 
         *
         * @param env the multicube environment
         * @param point the point to be represented 
         * @param parname the parameter to be represented
         * @return the parameter representation
         */
        string   get_parameter_representation(m3_env *env, m3_point &point, string parname);
        
        /** Returns the string representation of an integer vector. 
         *
         * @param env the multicube environment
         * @param vector the vector to be represented 
         * @return the vector representation
         */
        string   get_vector_representation(m3_env *env, vector<int> vector);

        /** Returns the integer vectors associated to a permutation in a point.
         *
         * @param env the M3 env
         * @param point the point to be represented
         * @param parname the parameter name
         * @return the integer vector
         */
        vector<int> get_permutation(m3_env *env, m3_point *point, string parname);

         /** Sets the integer vectors associated to a permutation in a point.
         *
         * @param env the M3 env
         * @param point the point to be represented
         * @param parname the parameter name
         * @param v the integer vector
         */
        void set_permutation(m3_env *env, m3_point *point, string parname, vector<int> v);
        
        /** Returns the integer vectors associated to a mask in a point.
         *
         * @param env the M3 env
         * @param point the point to be represented
         * @param parname the parameter name
         * @return the integer vector
         */
        vector<int> get_mask(m3_env *env, m3_point *point, string parname);
 
        /** Sets the integer vector associated to a mask in a point.
         *
         * @param env the M3 env
         * @param point the point to be represented
         * @param parname the parameter name
         * @param v the integer vector (have to be composed only by 0 and 1 elements)
         */
        void set_mask(m3_env *env, m3_point *point, string parname, vector<int> v);
 
        /** Checks for feasibility rules.
         *
         * @param env the environment 
         * @param point the point 
         * @return true if the point passes the feasibility test. 
         */
        bool is_valid(m3_env *env, m3_point *point);

        /** Checks all the parameters of the point and fixes inconsistencies on masks and permutations.
         *
         * @param env the M3 environment
         * @param point the input point
         * @return a consolidated point
         */
        m3_point consolidate(m3_env *env, m3_point point);

        /**
        vector<string> get_upper_bound();
        vector<string> get_lower_bound();
        vector<string> get_center();
        */

        /** Converts an m3_vector of strings into a point.
         *
         * @param env the M3 env
         * @param vector the string vector
         * @return the point associated to the string vector
         */
        m3_point string_vector_to_point(m3_env *env, m3_vector *vector);

        /** Returns the level associated to the minimum value of a parameter.
         *
         * @param env the multicube environment
         * @param parname the name of the parameter
         * @return the level associated to the scalar minimum.
         */
        int get_scalar_min(m3_env *env, string parname);
        
        /** Returns the level associated to the maximum value of a parameter.
         *
         * @param env the multicube environment
         * @param parname the name of the parameter
         * @return the level associated to the scalar minimum.
         */
        int get_scalar_max(m3_env *env, string parname);

        /** Returns the dimension of a permutation associated to a point.
         * Attention, point scalar coordinates should have already been set.
         *
         * @param env the multicube environment
         * @param parname the name of the parameter
         * @param point the point for which the permutation size should be computed.
         * @return the dimension of the permutation
         */
        int get_permutation_dimension(m3_env *env, string parname, m3_point *point);
        
        /** Returns the permutation minimum value associated to a point.
         * Attention, point scalar coordinates should have already been set.
         *
         * @param env the multicube environment
         * @param parname the name of the parameter
         * @param point the point for which the permutation minimum should be computed.
         * @return the minimum of the permutation
         */
        int get_permutation_min(m3_env *env, string parname, m3_point *point);
        
        /** Returns the permutation maximum value associated to a point.
         * Attention, point scalar coordinates should have already been set.
         * 
         * @param env the multicube environment
         * @param parname the name of the parameter
         * @param point the point for which the max value should be computed.
         * @return the max of the permutation
         */
        int get_permutation_max(m3_env *env, string parname, m3_point *point);

        /** Returns the dimension of a mask associated to a point.
         * Attention, point scalar coordinates should have already been set.
         *
         * @param env the multicube environment
         * @param parname the name of the parameter
         * @param point the point for which the mask size should be computed.
         * @return the dimension of the mask
         */
        int get_mask_dimension(m3_env *env, string parname, m3_point *point);
        
        /** Returns the on set size of a mask associated to a point.
         *
         * @param env the multicube environment
         * @param parname the name of the parameter
         * @param p the point for which the mask on set size should be computed.
         * @return the on set size of the mask
         */
        int get_mask_on_set_size(m3_env *env, string parname, m3_point *p);
      
        /** Returns the mask minimum value associated to a point.
         * Attention, point scalar coordinates should have already been set.
         *
         * @param env the multicube environment
         * @param parname the name of the parameter
         * @param point the point for which the mask minimum should be computed.
         * @return the minimum of the mask  
         */
        int get_mask_min(m3_env *env, string parname, m3_point *point);
        
        /** Returns the mask maximum value associated to a point.
         * Attention, point scalar coordinates should have already been set.
         *
         * @param env the multicube environment
         * @param parname the name of the parameter
         * @param point the point for which the mask maximum should be computed.
         * @return the maximum of the mask  
         */
        int get_mask_max(m3_env *env, string parname, m3_point *point);

        /** Sets the objectives of the optimization problem.
         * The should be a subset of the available metrics.
         *
         * @param env the M3 environment
         * @param list it is an m3_list of strings containing the metrics which are objectives of
         * the optimization problem.
         * @return true if the objectives are correct.
         */
        bool set_objectives(m3_env *env, m3_object *list);

        /** Sets the objective units of the optimization problem.
         * The should be a subset of the available metrics.
         *
         * @param env the M3 environment
         * @param list it is an m3_list of strings containing the metrics which are objectives of
         * the optimization problem.
         * @return true if the objectives are correct.
         */
        bool set_objectives_units(m3_env *env, m3_object *list);

        /** Sets the objective names of the optimization problem.
         * The should be a subset of the available metrics.
         *
         * @param env the M3 environment
         * @param list it is an m3_list of strings containing the metrics which are objectives of
         * the optimization problem.
         * @return true if the objectives are correct.
         */

        bool set_objectives_names(m3_env *env, m3_object *list);

        /** Evaluates a point in the design space computing the associated statistics and metrics
         * by using the problem driver. The metrics and statistics are attached to the point as properties.
         *
         * @param env the M3 env
         * @param point the point to be evaluated by using the driver.
         * @return true if everithing went fine during the simulation.
         */
        bool evaluate(m3_env *env, m3_point *point);

        /** Prints the current content of the design space.
         *
         * @param env the M3 env
         */
        void print(m3_env *env);


        /**
         * Evaluates an expression composed.
         *
         * The expression can be composed of elements x_par or f_x where
         *
         * x_par is parameter 'par' associated to x.
         * f_x   is the metric (or objective) associated to x.
         *
         *
         * Example:
         * 
         * x_pn < 4 && energy_x > power_x * 8.32
         *
         * where pn is a parameter and energy is a metric or an objective.
         *
         * @param env the M3 env
         * @param x the x point to be evaluated.
         * @param expr the expression to be evaluated
         * @param is_objective_or_constraint a flag reporting if the expression is an objective or a costraint
         * @param value the value of the expression
         * @return true if everything went fine during the simulation.
         * 
         */
        bool eval_expression(m3_env *env, m3_point *x, string expr, bool is_objective_or_constraint, double & value);

        /**
         * Evaluates an expression composed of 2 points (x and y). 
         *
         * The expression can be composed of elements x_par (y_par) or f_x (f_y) where
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
         * where pn is a parameter and energy is a metric or an objective.
         *
         * @param env the M3 env
         * @param x the x point to be evaluated.
         * @param y the y point to be evaluated.
         * @param expr the expression to be evaluated
         * @param value the value of the expression
         * @return true if everything went fine during the simulation.
         * 
         */

        bool eval_expression_2(  m3_env *env, m3_point *x, m3_point *y, string expr, double & value);

        bool set_architecture_info(m3_env *env, m3_object *cf);

        /** Returns true if all design parameters of the point are within scalar bounds
         * @param env the M3 env
         * @param point
         * @return the true if all design parameters of the point are within scalar bounds
         */
        bool is_within_scalar_bounds(m3_env *env, m3_point point);

        /** Returns the boundaries for the scalar parameter
         * @param env the M3 env
         * @param parname the parameter name
         * @return the boundaries for the scalar parameter
         */
        vector<string> get_scalar_range(m3_env *env, string parname);

        /* SCRAMBLE FOR MASK AND PERMUTATION */

        /** Returns the index in permutation_opposite_level of a couple of scrambled assignment for the permutation whose parameters are specified in t structure. If no matching permutation is found -1 is returned.
         *
         * @param t 
         * @return the index in permutation_opposite_level, or -1 if no matching permutation is found
         */
        int get_permutation_opposite_level_index(m3_permutation_low_high_parameter t);
        
        /** Returns the index in permutation_opposite_level of a couple of scrambled assignment for the permutation whose parameters are specified in t structure. If no matching permutation is found a new couple for the given permutation is generated in permutation_opposite_level vector.
         *
         * @param env the M3 env
         * @param p the point to consider
         * @param parname the parameter name (which have to be a permutation)
         * @return the index in permutation_opposite_level, or -1 if no matching permutation is found
         */
        // OVERLOAD: if the index aren't available generate a new couple of configuration for the given permutation
        int get_permutation_opposite_level_index(m3_env *env, m3_point *p, string parname);

         /** Returns the index in permutation_opposite_level of a couple of scrambled assignment for the permutation whose parameters are specified in t structure. If no matching permutation is found a set of close to scramble operator permutation for the given permutation.
         *
         * @param env the M3 env
         * @param p the point to consider
         * @param parname the parameter name (which have to be a mask)
         * @return the index in mask_opposite_level, or -1 if no matching mask is found
         */
         int get_permutation_multi_opposite_level_index(m3_env *env, m3_point *p, string parname);

        /** Returns the index in mask_opposite_level of a couple of scrambled assignment for the mask whose parameters are specified in t structure. If no matching mask is found -1 is returned.
         *
         * @param t 
         * @return the index in mask_opposite_level, or -1 if no matching mask is found
         */
        int get_mask_opposite_level_index(m3_mask_low_high_parameter t);
   
        /** Returns the index in mask_opposite_level of a couple of scrambled assignment for the mask whose parameters are specified in t structure. If no matching mask is found a new couple for the given mask is generated in mask_opposite_level vector.
         *
         * @param env the M3 env
         * @param p the point to consider
         * @param parname the parameter name (which have to be a mask)
         * @return the index in mask_opposite_level, or -1 if no matching mask is found
         */
        // OVERLOAD: if the index aren't available generate a new couple of configuration for the given mask
        int get_mask_opposite_level_index(m3_env *env, m3_point *p, string parname);

    
        /** Returns the index in mask_opposite_level of a couple of scrambled assignment for the mask whose parameters are specified in t structure. If no matching mask is found a set of close to scramble operator masks for the given mask.
         *
         * @param env the M3 env
         * @param p the point to consider
         * @param parname the parameter name (which have to be a mask)
         * @return the index in mask_opposite_level, or -1 if no matching mask is found
         */
        int get_mask_multi_opposite_level_index(m3_env *env, m3_point *p, string parname);
        /** Generate a couple of low high permutation according to the parameter in cur_permutation and save the generated couple to permutation_opposite_level vector.
         *
         * @param env the M3 env
         * @param parname the parameter name (which have to be a permutation)
         * @param p the point to consider
         * @param cur_permutation the set of actual specification for permutation
         * @return the index in permutation_opposite_level where is saved the scrambled pair
         */
        int scramble_permutation_generator(m3_env *env, string parname, m3_point *p, m3_permutation_low_high_parameter cur_permutation);
        
	 /** Generate a set of permutation such that it is close respect the scramble operator and the permutation respect the specification parameter in cur_permutation and save the generated couple to permutation_scramble vector.
         *
         * @param env the M3 env
         * @param parname the parameter name (which have to be a mask)
         * @param p the point to consider
         * @param cur_permutation the set of actual specification for the permutation 
         * @return the index in mask_opposite_level where is saved the scrambled pair
         */
	int scramble_multi_permutation_generator(m3_env *env, string parname, m3_point *p, m3_permutation_low_high_parameter cur_permutation);
        

        /** Given start_permutation and its parametrization throught cur_permutation returns the corresponding scrambled permutation.
         *
         * @param start_permutation starting permutation in vector representation
         * @param cur_permutation the set of actual specification for permutation
         * @return correponding scrambled permutation
         */
        vector <int> permutation_scrambler(vector <int> start_permutation, m3_permutation_low_high_parameter cur_permutation);
          
        /** Generate a couple of low high mask according to the parameter in cur_mask and save the generated couple to mask_opposite_level vector.
         *
         * @param env the M3 env
         * @param parname the parameter name (which have to be a mask)
         * @param p the point to consider
         * @param cur_mask the set of actual specification for mask
         * @return the index in mask_opposite_level where is saved the scrambled pair
         */
        int scramble_mask_generator(m3_env *env, string parname, m3_point *p, m3_mask_low_high_parameter cur_mask);
        
        /** Generate a set of mask such that it is close respect the scramble operatorand the mask respect the specification parameter in cur_mask and save the generated couple to mask_scramble vector.
         *
         * @param env the M3 env
         * @param parname the parameter name (which have to be a mask)
         * @param p the point to consider
         * @param cur_mask the set of actual specification for mask
         * @return the index in mask_opposite_level where is saved the scrambled pair
         */
        int scramble_multi_mask_generator(m3_env *env, string parname, m3_point *p, m3_mask_low_high_parameter cur_mask);

       /** Given start_mask and its parametrization throught cur_mask returns the corresponding scrambled mask.
         *
         * @param start_mask starting permutation in vector representation
         * @param cur_mask the set of actual specification for mask
         * @return correponding scrambled mask
         */
        vector <int> mask_scrambler(vector <int> start_mask, m3_mask_low_high_parameter cur_mask);
               
};

/** Design space exception class. */
class m3_design_space_exception: public m3_exception
{
	public:
        /** Constructor. 
         * @param description of the exception */
		m3_design_space_exception(string description);

        /** Destructur */
		~m3_design_space_exception() throw();
};
        
void m3_shutdown_expression_eval();
void m3_init_expression_eval();

#endif

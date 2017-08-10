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






#ifndef M3_DATABASE
#define M3_DATABASE

class m3_database;

#include <m3_object.h>
#include <m3_point.h>
#include <m3_list.h>
#include <map>


/** Pipeline for processing m3_database contents and improving metric predictability */
class m3_preprocess_pipeline
{
  public:
  bool   disabled;
  double   box_cox_transform;
  double   box_cox_is_log;
  double   mean;
  double   std;

  /** Constructor */
  m3_preprocess_pipeline();
  /** Constructor with box-cox transformation initialization */
  m3_preprocess_pipeline(double box_cox_transform_v, bool box_cox_is_log_v);
  
  /** Preprocess the value
    * @param value the measured value to be preprocessed (the metric)
    * @return the box-cox transformed value
    */
  double pre_process(double value);
  /** Postprocess the value
    * @param value the value to be postprocessed
    * @return the box-cox invers-transformed value (the prediction)
    */
  double post_process(double value);
};



/** This class represents a collection of design points */
class m3_database
{
    public:
  
  /** The collection of design points */
  m3_list   *points;      //list of points simulated
    
  /** Constructor */
  m3_database();
        /** Constructor. 
         * @param env the M3 env
         * @param doe a design of experiments
         */
  m3_database( m3_env *env, m3_doe *doe);
        /** Destructor. */ 
  ~m3_database();
  
  
  
  /** Add a design point to the collection.
    * @param p the design point
    */
  void  insert_point(m3_point *p);
  
  /** Save the design points in a file.
    * @param filename the name of the target file
    * @return 1 on success
    */
  int   write_to_file(const char* filename); 
   
  /** Load the design points from a file.
    * @param filename the name of the target file
    * @return 1 on success
    */
  int   read_from_file(const char *filename);
  
  
  
  /** Write 2 design point objectives to a file.
    * @param env the M3 env
    * @param filename the name of the target file
    * @param obj1 the index of the first objective
    * @param obj2 the index of the second objective
    * @return 1 on success
    */
  int   write_objectives_to_file(m3_env* env,const char* filename, int obj1=0, int obj2=1);  
  
  /** Write on a file a matrix which two dimensions represent two parameters and each cell content the average value of a given objective for the specific parameter combination.
    * @param env the M3 env
    * @param filename the name of the target file
    * @param obj1 the index of the objective
    * @param par1 the index of the first parameter
    * @param par2 the index of the second parameter
    * @return 1 on success
    */
  int   write_param_obj_to_file(m3_env* env ,const char* filename, int obj1, int par1, int par2);
  
  
  
  /** Search in the database for a specific design point.
    * @param p the design point to search
    * @return the design point found with the metric values stored in the db. NULL if point not found
    */
  m3_point *find_point(m3_point &p);
  
  
  
  /** Clear the database content */
  void  clear();
  /** Return the amount of stored points
    * @return the amount of stored points
    */
  int   size();
  /** Return the stored points
    * @return the stored points
    */
  m3_list *get_list_of_points();
  /** Return a copy of the stored points
    * @return a copy of the stored points
    */
  m3_list *generate_copy_of_database_list();
  /** Copy the content of the target list of points
    * @param points the list of points
    */
  void   copy_from(m3_list *points);



  /** Generate a new m3_database by random sampling this one
    * @param points_number the amount of points to sample
    * @return the randomly sampled database
    */
  m3_database  *get_random_subset( int points_number );
  
  /** Generate a new m3_database containing only the points which are valid for the design rules and consistent with the design space
    * @param env the M3 environment
    * @return the filtered database
    */
  m3_database  *get_filtered_valid( m3_env * env);
  
  /** Generate a new m3_database containing only the points which are consistent with the design space
    * @param env the M3 environment
    * @return the filtered database
    */
  m3_database  *get_filtered_consistent( m3_env * env);



  /* RSM related methods*/
  /** Initialize the xdr interface for the communication with the RSM
    * @param env the M3 environment
    * @param box_cox the box_cox order
    * @param box_cox_is_log the box_cox flag for the log transformation
    */
  void     xdr_init(m3_env *env, double box_cox, bool box_cox_is_log);
  /** Initialize only the predictors in the xdr interface for the communication with the RSM
    * @param env the M3 environment
    */
  void     xdr_init_predictors_only(m3_env *env);
  /** Write to a file the database in the format for the RSM communication
    * @param env the M3 environment
    * @param file_name the file name
    * @return 1 on success
    */
  bool     xdr_write(m3_env *env, string file_name);
  /** Write to a file the predictors of the database in the format for the RSM communication
    * @param env the M3 environment
    * @param file_name the file name
    * @return 1 on success
    */
  bool     xdr_write_predictors_only(m3_env *env, string file_name);
  /** Read from a file the RSM predictions to be stored in the database
    * @param env the M3 environment
    * @param file_name the file name
    * @return 1 on success
    */
  bool     xdr_read(m3_env *env, string file_name);
  /** Copy from a target database the xdr parameters
    * @param db the target database
    */
  void    copy_xdr_data( m3_database *db);
  /** Amount of metrics visible to the RSM */
  int     xdr_n_metrics;
  /** Amount of parameters visible to the RSM */
  int     xdr_n_predictors;
  /** Preprocessing pipelines */
  vector<m3_preprocess_pipeline> xdr_metric_pipes;
};
#endif

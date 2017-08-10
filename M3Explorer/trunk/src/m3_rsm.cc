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







#include <m3_shell_variables.h>
#include <m3_design_space.h>
#include <m3_common_utils.h>
#include <m3_sim_utils.h>
#include <m3_opt_utils.h>
#include <m3_gen_html.h>
#include <m3_commands.h>
#include <m3_xdr_api.h>
#include <m3_parser.h>
#include <m3_object.h>
#include <m3_rsm.h>
#include <sstream>
#include <fstream>
#include <stdio.h>

//#define DEBUG

void		prepare_shell_db( m3_env *, string db_name );
double 		get_avg( vector<double> sequence );
vector<double> 	compute_average_normalized_error( m3_env *env, m3_database *reference_db, 
						  m3_database *current_db);
double 		get_avg_error_from_rsm( m3_env *, string rsm_name, m3_database *ref_db, m3_database *training_db );
bool		get_preprocess_parameter( m3_env *, bool &use_log, double &preprocess);

bool	get_preprocess_parameter( m3_env *env, bool &use_log, double &preprocess)
{
	m3_object	*obj;
	if( env->shell_variables.get( "preprocess", obj ) )
	{
		int 	int_value 	= 0;
		string	string_value 	= "";
		if( get_integer_from_variables( env, "preprocess", int_value) )
		{
			use_log		= false;
			preprocess	= (double) int_value;
			return true;
		}
		else if( get_double_from_variables( env, "preprocess", preprocess ) )
		{
			use_log		= false;			
			return true;
		}
		else if( get_string_from_variables( env, "preprocess", string_value ) )
		{
			if( string_value == "log" )
			{
				use_log		= true;
				preprocess	= 0.0;
				return true;
			}
			else
			{
				use_log		= false;
				preprocess	= 0.0;
				return false;
			}
		}
		else
		{
			use_log		= false;
			preprocess	= 0.0;
			return false;
		}
	}
	else
	{
		use_log 	= false;
		preprocess 	= 0.0;
		return false;
	}
}

void prepare_shell_db( m3_env *env, string db_name )
{
	if( !env->available_dbs.count( db_name ) )
		env->available_dbs[ db_name ] = new m3_database();
	else
		env->available_dbs[ db_name ]->clear();
}

//------------------------------------------------------------------------------//
//					SHEPARD					//
//------------------------------------------------------------------------------//

class m3_rsm_sh_i: public m3_rsm
{
	public:

		m3_object *train_rsm( m3_env *env, string predictions_db )
		{
			if( !env->current_doe )
			{
				prs_display_error( "Please define a DoE before training this RSM" );
				return new m3_integer( 0 );
			}
			string training_db = env->current_db_name;
			if( training_db!=predictions_db )
				prepare_shell_db( env, predictions_db );

			//managing preprocess parameter
			bool 		use_log;
			double 		preprocess;
			
			if( !get_preprocess_parameter( env, use_log, preprocess ) )
				get_default_preprocess_value( use_log, preprocess );

			//managing power parameter
			int power;
			if( !get_integer_from_variables( env, "power", power ) )
				get_default_power_value( power );
			
			//creating a temporary directory
			string temp_dir_path	= m3_get_unique_temp_dir_name( "/m3explorer_tmp_rsm" );
			if( temp_dir_path == "" )
				return new m3_integer( 0 );
			if( !m3_mkdir( temp_dir_path ) )
			{
				prs_display_error( "Unable to create directory: " + temp_dir_path );
				return new m3_integer( 0 );
			}

			//managing training db
			m3_database *training = env->available_dbs[ training_db ];
			training->xdr_init( env, preprocess, use_log);
			training->xdr_write( env, temp_dir_path + "/training.db" );

			//managing predictors db
			m3_database predictors( env, env->current_doe );
			predictors.xdr_init_predictors_only( env );
			predictors.xdr_write_predictors_only( env, temp_dir_path + "/predictors.db" );

			//Calling the Shepard interpolator
			string command 	= m3_get_image_path(env) + "/rsm/sh_i";
			command		= command + " " + temp_dir_path + "/training.db";
			command		= command + " " + temp_dir_path + "/predictors.db";
			command		= command + " " + temp_dir_path + "/predictions.db";
			command		= command + " " + m3_itos( power );
			if( !shell_command( command ) )
			{
				prs_display_error( "The Shepard interpolator returned an error" );
				return new m3_integer( 0 );
			}

			//managing predictions db
			env->available_dbs[ predictions_db ]->clear();
			if( training_db!=predictions_db )
			{			
				env->available_dbs[ predictions_db ]->copy_xdr_data( training );
			}
			if( !env->available_dbs[ predictions_db ]->xdr_read( env, temp_dir_path + "/predictions.db" ) )
			{
				prs_display_error( "The Shepard interpolator has generated an unreadable set of predictions" );
				return new m3_integer( 0 );
			}

			//cleaning the temporary directory
			#ifndef DEBUG
				if( !m3_rmdir( temp_dir_path ) )
				{
					prs_display_error( "Unable to delete temporary directory: " + temp_dir_path );
				}
			#endif

			return new m3_integer( 1 );
		};

		string print_informations()
		{
			return "parameters: power=INT; preprocess={FLOAT,\"log\"}";
		};

		bool set_validation_parameters( m3_env *env, int current_validation_configuration )
		{

			if( current_validation_configuration>=get_validation_configurations( env ) )
				return false;
			m3_vector *power;
			m3_vector *preprocess;

			if( get_vector_from_variables( env, "power_list", power ) )
			{
				if( current_validation_configuration < power->size() )
					prs_command_set( new m3_string( "power" ), 
					&power->get( current_validation_configuration ) );
				else
					set_default_power_variable();
			}
			else
				set_default_power_variable();

			if( get_vector_from_variables( env, "preprocess_list", preprocess ) )
			{
				if( current_validation_configuration < preprocess->size() )
					prs_command_set( new m3_string( "preprocess" ), 
					&preprocess->get( current_validation_configuration ) );
				else
					set_default_preprocess_variable();
			}
			else
				set_default_preprocess_variable();
			return true;
		};

		int get_validation_configurations( m3_env *env )
		{
			int preprocess_configurations	= 0;
			int power_configurations 	= 0;
			m3_vector *power;
			m3_vector *preprocess;
			if( get_vector_from_variables( env, "power_list", power ) )
				power_configurations		= power->size();
			if( get_vector_from_variables( env, "preprocess_list", preprocess ) )
				preprocess_configurations	= preprocess->size();					
			if( preprocess_configurations > power_configurations )
				return preprocess_configurations;
			else
				return power_configurations;
		};		

		void set_default_validation_configuration( m3_env *env )
		{
			m3_vector *preprocess_obj = new m3_vector();

			int power; 
			get_default_power_value( power );
			m3_integer m3_power( power );
			m3_vector *power_obj = new m3_vector();
			power_obj->insert( 0, m3_power );
			prs_command_set( new m3_string( "power_list" ), power_obj );

			bool use_log;
			double preprocess;
			get_default_preprocess_value( use_log, preprocess );
			if( use_log )
			{
				m3_string preprocess_value("log");
				preprocess_obj->insert( 0, preprocess_value );
			}
			else
			{
				m3_double preprocess_value( preprocess );
				preprocess_obj->insert( 0, preprocess_value );
			}
			prs_command_set( new m3_string( "preprocess_list" ), preprocess_obj );
		};

		string get_validation_graphic_title()
		{
			return "Shepard on DoE validation";
		};

		string get_validation_curve_name( m3_env *env )
		{		
			string 	power, preprocess;
			int 	i_data;
			double	d_data;
			bool 	use_log;

			if( !get_integer_from_variables( env, "power", i_data ) )
				power	= get_power_default_string();
			else
				power	= m3_itos( i_data );

			
			if( !get_preprocess_parameter( env, use_log, d_data ) )
				preprocess = get_preprocess_default_string();
			else
			{
				ostringstream message;
				if( use_log )
					message << "log";
				else
					message << d_data;
				preprocess = message.str();
			}
			return "power=" + power + "-box=" + preprocess;
		};

	private:

		void get_default_preprocess_value( bool &use_log, double &preprocess)
		{
			use_log		= false;
			preprocess	= 1.0;
		};

		void get_default_power_value( int &power )
		{
			power		= 0;
		};

		string get_power_default_string()
		{
			int power;
			get_default_power_value( power );		
			return m3_itos( power );
		};

		string get_preprocess_default_string()
		{
			bool 		use_log;
			double		preprocess;
			ostringstream	message;
			get_default_preprocess_value( use_log, preprocess );
			if( use_log == true )
				message << "log";
			else
				message << preprocess; 
			return message.str();
		};
		
		void set_default_power_variable()
		{
			int power;
			get_default_power_value( power );
			prs_command_set( new m3_string( "power" ), new m3_integer( power ) );	
		};

		void set_default_preprocess_variable()
		{
			bool	use_log;
			double	preprocess;
			get_default_preprocess_value( use_log, preprocess );
			if( use_log )
				prs_command_set( new m3_string( "preprocess" ), new m3_string( "log" ) );
			else
				prs_command_set( new m3_string( "preprocess" ), new m3_double( preprocess ) );
		};

};

//------------------------------------------------------------------------------//
//				NEURAL NETWORK					//
//------------------------------------------------------------------------------//

class m3_rsm_nn: public m3_rsm
{
	public:
	
		m3_object *train_rsm( m3_env *env, string predictions_db )
		{
			if( !env->current_doe )
			{
				prs_display_error( "Please define a DoE before training this RSM" );
				return new m3_integer( 0 );
			}
			string training_db = env->current_db_name;
			if( training_db!=predictions_db )
				prepare_shell_db( env, predictions_db );

			//managing preprocess parameter
			bool 		use_log;
			double 		preprocess;
			if( !get_preprocess_parameter( env, use_log, preprocess ) )
				get_default_preprocess_value( use_log, preprocess );

			//managing effort parameter
			string effort;
			get_string_from_variables( env, "effort", effort );
			if( effort!="fast" && effort!="low" && effort!="medium" && effort!="high" )
				get_default_effort_value( effort );
			
			//creating a temporary directory
			string temp_dir_path	= m3_get_unique_temp_dir_name( "/m3explorer_tmp_rsm" );
			if( temp_dir_path == "" )
				return new m3_integer( 0 );
			if( !m3_mkdir( temp_dir_path ) )
			{
				prs_display_error("Unable to create directory: " + temp_dir_path );
				return new m3_integer( 0 );
			}

			//managing training db
			m3_database *training = env->available_dbs[ training_db ];
			training->xdr_init( env, preprocess, use_log);
			training->xdr_write( env, temp_dir_path + "/training.db" );
            //consolidate...
			if(training->points->size() < 2 )
			{
				prs_display_error("Too few points in the training DB" );
				return new m3_integer( 0 );
			}

			//managing predictors db
			m3_database predictors( env, env->current_doe );
			predictors.xdr_init_predictors_only( env );
			predictors.xdr_write_predictors_only( env, temp_dir_path + "/predictors.db" );

			//Calling the Neural Network interpolator
			string command 	= m3_get_image_path(env) + "/rsm/nn";
			command		= command + " --all";
			command		= command + " " + temp_dir_path + "/training.db";
			command		= command + " " + temp_dir_path + "/predictors.db";
			command		= command + " " + temp_dir_path + "/nn_topology";
			command		= command + " " + temp_dir_path + "/predictions.db";
			command		= command + " " + effort;
			if( !shell_command( command ) )
			{
				prs_display_error("The Neural Network interpolator returned an error");
				return new m3_integer( 0 );
			}

			//managing predictions db
			env->available_dbs[ predictions_db ]->clear();
			if( training_db!=predictions_db )
			{			
				env->available_dbs[ predictions_db ]->copy_xdr_data( training );
			}
			if( !env->available_dbs[ predictions_db ]->xdr_read( env, temp_dir_path + "/predictions.db" ) )
			{
				prs_display_error( "The Neural Network interpolator has generated an unreadable set of predictions" );
				return new m3_integer( 0 );
			}
			
			//cleaning the temporary directory
			#ifndef DEBUG
				if( !m3_rmdir( temp_dir_path ) )
				{
					prs_display_error( "Unable to delete temporary directory: " + temp_dir_path );
				}
			#endif

			return new m3_integer( 1 );
		};

		string print_informations()
		{
			return "parameters: effort={\"fast\",\"low\",\"medium\",\"high\"}; preprocess={FLOAT,\"log\"}";
		};

		bool set_validation_parameters( m3_env *env, int current_validation_configuration )
		{
			if( current_validation_configuration >= get_validation_configurations( env ) )
				return false;
			m3_vector *effort;
			m3_vector *preprocess;

			if( get_vector_from_variables( env, "effort_list", effort ) )
			{
				if( current_validation_configuration<effort->size() )
					prs_command_set( new m3_string("effort"), 
					&effort->get( current_validation_configuration ) );
				else
					set_default_effort_variable();
			}
			else
				set_default_effort_variable();

			if( get_vector_from_variables( env, "preprocess_list", preprocess ) )
			{
				if( current_validation_configuration<preprocess->size() )
					prs_command_set( new m3_string( "preprocess" ), 
					&preprocess->get( current_validation_configuration ) );
				else
					set_default_preprocess_variable();
			}
			else
				set_default_preprocess_variable();

			return true;
		};

		int get_validation_configurations( m3_env *env )
		{
			int preprocess_configurations	= 0;
			int effort_configurations 	= 0;		
			m3_vector *effort;
			m3_vector *preprocess;
			if( get_vector_from_variables( env, "effort_list", effort ) )
				effort_configurations		= effort->size();
			if( get_vector_from_variables( env, "preprocess_list", preprocess) )
				preprocess_configurations	= preprocess->size();					
			if( preprocess_configurations > effort_configurations)
				return preprocess_configurations;
			else
				return effort_configurations;
		};

		void set_default_validation_configuration( m3_env *env )
		{

			string effort;
			get_default_effort_value( effort );
			m3_string m3_effort( effort );
			m3_vector *effort_obj = new m3_vector();
			effort_obj->insert( 0, m3_effort );
			prs_command_set( new m3_string( "effort_list" ), effort_obj );

			bool use_log;
			double preprocess;
			m3_vector *preprocess_obj = new m3_vector();
			get_default_preprocess_value( use_log, preprocess );
			if( use_log )
			{
				m3_string preprocess_value("log");
				preprocess_obj->insert( 0, preprocess_value );
			}
			else
			{
				m3_double preprocess_value( preprocess );
				preprocess_obj->insert( 0, preprocess_value );
			}
			prs_command_set( new m3_string( "preprocess_list" ), preprocess_obj );
		};

		string get_validation_graphic_title()
		{
			return "Neural Network on DoE validation";
		};

		string get_validation_curve_name( m3_env *env )
		{
			bool	use_log;
			double	d_data;
			string 	effort, preprocess;

			if( !get_string_from_variables( env, "effort", effort ) )
				effort 		= get_effort_default_string();

			if( !get_preprocess_parameter( env, use_log, d_data ) )
				preprocess = get_preprocess_default_string();
			else
			{
				ostringstream message;
				if( use_log )
					message << "log";
				else
					message << d_data;
				preprocess = message.str();
			}
			return "effort=" + effort +"-box=" + preprocess;
		};

	private:

		void get_default_preprocess_value( bool &use_log, double &preprocess )
		{
			use_log		= false;
			preprocess	= 1.0;
		};

		void get_default_effort_value( string &effort )
		{
			effort 		= "fast";
		};

		string get_preprocess_default_string()
		{
			bool 		use_log;
			double		preprocess;
			ostringstream	message;
			get_default_preprocess_value( use_log, preprocess );
			if( use_log == true )
				message << "log";
			else
				message << preprocess; 
			return message.str();
		};

		string get_effort_default_string()
		{
			string effort;
			get_default_effort_value( effort );
			return effort;
		};
		
		void set_default_effort_variable()
		{
			string effort;
			get_default_effort_value( effort );
			prs_command_set( new m3_string("effort"), new m3_string( effort ) );	
		};

		void set_default_preprocess_variable()
		{
			bool	use_log;
			double	preprocess;
			get_default_preprocess_value( use_log, preprocess );
			if( use_log )
				prs_command_set( new m3_string( "preprocess" ), new m3_string( "log" ) );
			else
				prs_command_set( new m3_string( "preprocess" ), new m3_double( preprocess ) );
		};
};

//------------------------------------------------------------------------------//
//					SPLINE					//
//------------------------------------------------------------------------------//

class m3_rsm_spline: public m3_rsm
{
    public:

		m3_object *train_rsm( m3_env *env, string predictions_db )
		{
			if( !env->current_doe )
			{
				prs_display_error( "Please define a DoE before training this RSM" );
				return new m3_integer( 0 );
			}
			string training_db = env->current_db_name;
			if( training_db!=predictions_db )
				prepare_shell_db( env, predictions_db );

			//managing preprocess parameter
			bool 		use_log;
			double 		preprocess;
			if( !get_preprocess_parameter( env, use_log, preprocess ) )
				get_default_preprocess_value( use_log, preprocess );

			//creating a temporary directory
			string temp_dir_path	= m3_get_unique_temp_dir_name( "/m3explorer_tmp_rsm" );
			if( temp_dir_path == "" )
				return new m3_integer( 0 );
			if( !m3_mkdir( temp_dir_path ) )
			{
				prs_display_error( "Unable to create directory: " + temp_dir_path );
				return new m3_integer( 0 );
			}

			//managing training db
			m3_database *training = env->available_dbs[ training_db ];
			training->xdr_init( env, preprocess, use_log );
			training->xdr_write( env, temp_dir_path + "/training.db" );

			//managing predictors db
			m3_database predictors( env, env->current_doe );
			predictors.xdr_init_predictors_only( env );
			predictors.xdr_write_predictors_only( env, temp_dir_path + "/predictors.db" );

			//Calling the Spline interpolator
			string command 	= m3_get_image_path(env) + "/rsm/spline";
			command		= command + " --trai";
			command		= command + " " + temp_dir_path + "/training.db";
			command		= command + " --pred";
			command		= command + " " + temp_dir_path + "/predictors.db";
			command		= command + " --output";
			command		= command + " " + temp_dir_path + "/predictions.db";
			if( !shell_command( command ) )
			{
				prs_display_error( "The Spline interpolator returned an error" );
				return new m3_integer( 0 );
			}

			//managing predictions db
			env->available_dbs[ predictions_db ]->clear();
			if( training_db!=predictions_db )
			{			
				env->available_dbs[ predictions_db ]->copy_xdr_data( training );
			}
			if( !env->available_dbs[ predictions_db ]->xdr_read( env, temp_dir_path + "/predictions.db" ) )
			{
				prs_display_error( "The Spline interpolator has generated an unreadable set of predictions" );
				return new m3_integer( 0 );
			}
			
			//cleaning the temporary directory
			#ifndef DEBUG
				if( !m3_rmdir( temp_dir_path ) )
				{
					prs_display_error( "Unable to delete temporary directory: " + temp_dir_path );
				}
			#endif

			return new m3_integer( 1 );
		};

		string print_informations()
		{
			return "parameters: preprocess={FLOAT,\"log\"}";
		};

		bool set_validation_parameters( m3_env *env, int current_validation_configuration )
		{
			if( current_validation_configuration >= get_validation_configurations( env ) )
				return false;
			m3_vector *preprocess;
			get_vector_from_variables( env, "preprocess_list", preprocess ); 
			prs_command_set( new m3_string( "preprocess" ), 
					&preprocess->get( current_validation_configuration ) );
			return true;
		};

		int get_validation_configurations( m3_env *env )
		{
			int preprocess_configurations	= 0;
			m3_vector *preprocess;
			if( get_vector_from_variables( env, "preprocess_list", preprocess ) )
				preprocess_configurations	= preprocess->size();
			return preprocess_configurations;
		};

		void set_default_validation_configuration( m3_env *env )
		{
			bool use_log;
			double preprocess;
			m3_vector *preprocess_obj = new m3_vector();
			get_default_preprocess_value( use_log, preprocess );
			if( use_log )
			{
				m3_string preprocess_value("log");
				preprocess_obj->insert( 0, preprocess_value );
			}
			else
			{
				m3_double preprocess_value( preprocess );
				preprocess_obj->insert( 0, preprocess_value );
			}
			prs_command_set( new m3_string( "preprocess_list" ), preprocess_obj );
		};

		string get_validation_graphic_title()
		{
			return "Spline on DoE validation";
		};

		string get_validation_curve_name( m3_env *env )
		{
			bool	use_log;
			double	d_data;
			string 	preprocess;

			if( !get_preprocess_parameter( env, use_log, d_data ) )
				preprocess = get_preprocess_default_string();
			else
			{
				ostringstream message;
				if( use_log )
					message << "log";
				else
					message << d_data;
				preprocess = message.str();
			}
			return "box=" + preprocess;
		};

	private:

		void get_default_preprocess_value( bool &use_log, double &preprocess )
		{
			use_log		= false;
			preprocess	= 1.0;
		};

		string get_preprocess_default_string()
		{
			bool 		use_log;
			double		preprocess;
			ostringstream	message;
			get_default_preprocess_value( use_log, preprocess );
			if( use_log == true )
				message << "log";
			else
				message << preprocess; 
			return message.str();
		};

		void set_default_preprocess_variable()
		{
			bool	use_log;
			double	preprocess;
			get_default_preprocess_value( use_log, preprocess );
			if( use_log )
				prs_command_set( new m3_string( "preprocess" ), new m3_string( "log" ) );
			else
				prs_command_set( new m3_string( "preprocess" ), new m3_double( preprocess ) );
		};
};

//------------------------------------------------------------------------------//
//					RBF					//
//------------------------------------------------------------------------------//

class m3_rsm_rbf: public m3_rsm
{
	public:

		m3_object *train_rsm( m3_env *env, string predictions_db )
		{
			if( !env->current_doe )
			{
				prs_display_error( "Please define a DoE before training this RSM" );
				return new m3_integer( 0 );
			}
			string training_db = env->current_db_name;
			if( training_db!=predictions_db )
				prepare_shell_db( env, predictions_db );

			//managing preprocess parameter
			bool 		use_log;
			double 		preprocess;
			if( !get_preprocess_parameter( env, use_log, preprocess ) )
				get_default_preprocess_value( use_log, preprocess );

			//managing type parameter
			string	type;
			int 	the_rbf_type = 1;
			if( get_string_from_variables( env, "type", type ) )
			{
				if(type=="power")
					the_rbf_type=1;
				else if(type=="power_log")
					the_rbf_type=2;
				else if(type=="sqrt")
					the_rbf_type=3;
				else if(type=="inv_sqrt")
					the_rbf_type=4;
				else if(type=="exp")
					the_rbf_type=5;
				else
					get_default_type_value( type );
			}
			else
			{
				get_default_type_value( type );
			}

			//managing parameter parameter
			int parameter;
			if( !get_integer_from_variables( env, "parameter", parameter ) )
				get_default_parameter_value( parameter );

			//creating a temporary directory
			string temp_dir_path	= m3_get_unique_temp_dir_name( "/m3explorer_tmp_rsm" );
			if( temp_dir_path == "" )
				return new m3_integer( 0 );
			if( !m3_mkdir( temp_dir_path ) )
			{
				prs_display_error( "Unable to create directory: " + temp_dir_path );
				return new m3_integer( 0 );
			}

			//managing training db
			m3_database *training = env->available_dbs[ training_db ];
			training->xdr_init( env, preprocess, use_log );
			training->xdr_write( env, temp_dir_path + "/training.db" );

			//managing predictors db
			m3_database predictors( env, env->current_doe );
			predictors.xdr_init_predictors_only( env );
			predictors.xdr_write_predictors_only( env, temp_dir_path + "/predictors.db" );

			//Calling the Rbf interpolator
			int num_estimates = predictors.xdr_n_metrics;
			
			string command 	= m3_get_image_path( env ) + "/rsm/rbf_interpolator";
			command		= command + " " + m3_itos( num_estimates );
			for(int i=0; i< num_estimates; i++)
				command = command + " " + m3_itos( the_rbf_type );
			for(int i=0; i< num_estimates; i++)
				command = command + " " + m3_itos( parameter );
			command		= command + " " + temp_dir_path + "/training.db";
			command		= command + " " + temp_dir_path + "/predictors.db";
			command		= command + " " + temp_dir_path + "/predictions.db";
			if( !shell_command( command ) )
			{
				prs_display_error( "The Rbf interpolator returned an error" );
				return new m3_integer( 0 );
			}

			//managing predictions db
			env->available_dbs[ predictions_db ]->clear();
			if( training_db!=predictions_db )
			{			
				env->available_dbs[ predictions_db ]->copy_xdr_data( training );
			}
			if( !env->available_dbs[ predictions_db ]->xdr_read( env, temp_dir_path + "/predictions.db" ) )
			{
				prs_display_error( "The Rbf interpolator has generated an unreadable set of predictions" );
				return new m3_integer( 0 );
			}
			
			//cleaning the temporary directory
			#ifndef DEBUG
				if( !m3_rmdir( temp_dir_path ) )
				{
					prs_display_error( "Unable to delete temporary directory: " + temp_dir_path );
				}
			#endif

			return new m3_integer( 1 );
		};

		string print_informations()
		{
			return "parameters: type={\"power\", \"power_log\", \"sqrt\", \"inv_sqrt\", \"exp\" }; parameter=INT; preprocess={FLOAT,\"log\"}";
		};

		bool set_validation_parameters( m3_env *env, int current_validation_configuration )
		{
			if( current_validation_configuration>=get_validation_configurations( env ) )
				return false;
			m3_vector *type;
			m3_vector *parameter;
			m3_vector *preprocess;

			if( get_vector_from_variables( env, "type_list", type ) )
			{
				if( current_validation_configuration<type->size() )
					prs_command_set( new m3_string( "type" ), &type->get( current_validation_configuration ) );
				else
					set_default_type_variable();
			}
			else
				set_default_type_variable();

			if( get_vector_from_variables( env, "parameter_list", parameter ) )
			{
				if( current_validation_configuration<parameter->size() )
					prs_command_set( new m3_string( "parameter" ), &parameter->get( current_validation_configuration ) );
				else
					set_default_parameter_variable();		
			}
			else
				set_default_parameter_variable();

			if( get_vector_from_variables( env, "preprocess_list", preprocess ) )
			{
				if( current_validation_configuration<preprocess->size() )
					prs_command_set( new m3_string( "preprocess" ), &preprocess->get( current_validation_configuration ) );
				else
					set_default_preprocess_variable();
			}
			else
				set_default_preprocess_variable();
			return true;
		};

		void set_default_validation_configuration( m3_env *env )
		{
			bool use_log;
			double preprocess;
			m3_vector *preprocess_obj = new m3_vector();
			get_default_preprocess_value( use_log, preprocess );
			if( use_log )
			{
				m3_string preprocess_value("log");
				preprocess_obj->insert( 0, preprocess_value );
			}
			else
			{
				m3_double preprocess_value( preprocess );
				preprocess_obj->insert( 0, preprocess_value );
			}
			prs_command_set( new m3_string( "preprocess_list" ), preprocess_obj );

			int parameter;
			m3_vector *parameter_obj = new m3_vector();
			get_default_parameter_value( parameter );
			m3_integer parameter_value( parameter );
			parameter_obj->insert( 0, parameter_value );
			prs_command_set( new m3_string( "parameter_list" ), parameter_obj );

			string type;
			m3_vector *type_obj = new m3_vector();
			get_default_type_value( type );
			m3_string type_value( type );
			type_obj->insert( 0, type_value );
			prs_command_set( new m3_string( "type_list" ), type_obj );
		};

		int get_validation_configurations( m3_env *env )
		{
			int preprocess_configurations	= 0;
			int parameter_configurations	= 0;
			int type_configurations 	= 0;
			m3_vector *type;
			m3_vector *parameter;
			m3_vector *preprocess;
			if( get_vector_from_variables( env, "type_list", type ) )
				type_configurations		= type->size();
			if( get_vector_from_variables( env, "parameter_list", parameter ) )
				parameter_configurations	= parameter->size();
			if( get_vector_from_variables( env, "preprocess_list", preprocess ) )
				preprocess_configurations	= preprocess->size();
			if( preprocess_configurations > type_configurations)
				if( preprocess_configurations > parameter_configurations )
					return preprocess_configurations;
				else
					return parameter_configurations;
			else
				if( type_configurations > parameter_configurations )
					return type_configurations;
				else
					return parameter_configurations;
		};

		string get_validation_graphic_title()
		{
			return "Rbf on DoE validation";
		};

		string get_validation_curve_name( m3_env *env )
		{
			string	type, parameter, preprocess;
			bool	use_log;
			double 	d_data;
			int	i_data;

			if( !get_integer_from_variables( env, "parameter", i_data ) )
				parameter	= get_parameter_default_string();
			else
				parameter	= m3_itos( i_data );

			if( !get_string_from_variables( env, "type", type ) )
				type	= get_type_default_string();
			else
				if( type!="power" && type!="power_log" && type!="sqrt" && type!="inv_sqrt" && type!="exp")
					type	= get_type_default_string();

			if( !get_preprocess_parameter( env, use_log, d_data ) )
				preprocess = get_preprocess_default_string();
			else
			{
				ostringstream message;
				if( use_log )
					message << "log";
				else
					message << d_data;
				preprocess = message.str();
			}

			return "type=" + type + "-par=" + parameter +"-box=" + preprocess;
		};

	private:

		void get_default_preprocess_value( bool &use_log, double &preprocess)
		{
			use_log		= false;
			preprocess	= 1.0;
		};

		void get_default_parameter_value( int &parameter )
		{
			parameter	= 2;
		};

		void get_default_type_value( string &type )
		{
			type		= "power_log";
		};

		string get_type_default_string()
		{
			string type;
			get_default_type_value( type );
			return type;
		};

		string get_parameter_default_string()
		{
			int parameter;
			get_default_parameter_value( parameter );
			return m3_itos( parameter );
		};

		string get_preprocess_default_string()
		{
			bool 		use_log;
			double		preprocess;
			ostringstream	message;
			get_default_preprocess_value( use_log, preprocess );
			if( use_log == true )
				message << "log";
			else
				message << preprocess; 
			return message.str();
		};
		
		void set_default_type_variable()
		{
			string type;
			get_default_type_value( type );
			prs_command_set( new m3_string("type"), new m3_string( type ) );	
		};

		void set_default_parameter_variable()
		{		
			int parameter;
			get_default_parameter_value( parameter );
			prs_command_set( new m3_string( "parameter" ), new m3_integer( parameter )  );
		};

		void set_default_preprocess_variable()
		{
			bool	use_log;
			double	preprocess;
			get_default_preprocess_value( use_log, preprocess );
			if( use_log )
				prs_command_set( new m3_string( "preprocess" ), new m3_string( "log" ) );
			else
				prs_command_set( new m3_string( "preprocess" ), new m3_double( preprocess ) );
		};
};

//------------------------------------------------------------------------------//
//				LINEAR REGRESSION				//
//------------------------------------------------------------------------------//
//#define DEBUG

typedef struct regress_data 
{
	vector<double>      coeff;
	double              Radj;
	double              F;
	double              PF;
	vector< double >    T;
	vector< double >    PT;
	double              mean;
	double              variance;
	double              stdev;
	double              max;
	vector<double>      residuals;
	vector<double>      zj;
};

typedef struct regress_pars
{
	int                 number_of_metrics;
	int                 number_of_parameters;
	bool                normax;
	bool                square;
	vector< bool >	    exclusion_list;
	bool                interaction;
	bool                use_log;
	double              preprocess;
};

#define NUM_INTERCEPT 	1
#define NUM_CHISQ     	0
#define NUM_PREDICTORS	(num_predictors)
#define NUM_PARS	(NUM_PREDICTORS+NUM_INTERCEPT)
#define NUM_F_P_RAdj  	3
#define NUM_T_PT	((NUM_PARS)*2) 
#define TOT_NUM		(NUM_CHISQ+NUM_PARS+NUM_F_P_RAdj+NUM_T_PT)

bool m3_rsm_read_from_raw_data( vector<double> & raw_data, int num_predictors, vector<double> & coeff, double & Radj, 
				double & F, double & PF,vector<double> & T, vector<double> & PT )
{
	if( raw_data.size() < TOT_NUM )
	{
		return false;
	}
	coeff.clear();
	int start_pos	= NUM_CHISQ;
	for( int ind=0; ind<NUM_PARS; ind++ )
	{
		coeff.push_back( raw_data[ start_pos + ind ] );
	}
	F		=  ( raw_data[ start_pos + NUM_PARS ] );
	PF		=  ( raw_data[ start_pos + NUM_PARS+1 ] );
	Radj		=  ( raw_data[ start_pos + NUM_PARS+2 ] );
	start_pos 	+= NUM_PARS + NUM_F_P_RAdj;
	T.clear();
	PT.clear();
	for(int ind=0; ind<NUM_PARS; ind++)
	{
		T.push_back( raw_data[ start_pos + 2 * ind ] );
		PT.push_back( raw_data[ start_pos + 2 * ind + 1 ] );
	}
	return true;
}

double m3_rsm_compute_metric( 	vector<int> & design, vector<double> &coeff, double & mean, 
				double & stdev, double &maxm, bool normax, double preprocess, bool use_log )
{
	double j = coeff[ 0 ];
	for( int k=0; k<design.size(); k++ )
		j = j + coeff[ 1 + k ] * design[ k ];		

	if( !normax )
		j = j * stdev + mean;
	else
		j = j * maxm;

	if( !use_log )
        	j = pow( j, 1 / preprocess );
	else
		j = exp( j );

	return j;
}

void m3_rsm_compute_coded( m3_point &p, vector<int> &out, bool interaction, bool square, vector<bool> &exclude )
{	
	out.clear();
	for( int i=0; i < p.size(); i++ )
	{
		if( !exclude[ i ] )
			out.push_back( p[i] );
	}
	if( square )
	{
		int sz = out.size();
		for( int i=0; i<sz; i++ )
			out.push_back( out[i] * out[i] );
	}
	if( interaction )
	{
		int sz = out.size();
		for( int i=0; i<sz-1; i++ )
			for( int k=i+1; k<sz; k++ )
				out.push_back( out[i] * out[k] );
	}
}

bool m3_rsm_linear_calculate_regress_data( 	m3_env *env, m3_database *training_db, int metric_index, 
					regress_data &data, regress_pars &pars, string temp_dir_path )
{
	string 		file_in_name	= temp_dir_path + "/m3_rsm_regressor_out_"
					+ m3_itos( metric_index ) + ".dat";
	string 		file_out_name	= temp_dir_path + "/m3_rsm_regressor_in_"	
					+ m3_itos( metric_index ) + ".dat";

	#ifdef DEBUG
		cout << "DEBUG:   Calculating Max & Mean..." << endl;
	#endif
	int num		= 0;
	data.max	= 0;
	data.mean	= 0;
	list<m3_object *>::iterator i;
	list<m3_object *>::iterator end		= training_db->points->end();
	list<m3_object *>::iterator begin	= training_db->points->begin();
	for( i=begin; i!=end; i++ )
	{
		m3_point	*p	= (dynamic_cast<m3_point *>(*i));
		double 		value	= p->get_metric( metric_index );
		if( pars.use_log )
			value = log( value );
		else
			value = pow( value, pars.preprocess );
		if( isnan( value ) || isinf( value ) )
		{
			prs_display_message( "Data can't be processed/normalized" );
			return false;
		}
		data.mean	+= value;
		data.max	= max( data.max, fabs( value ) );
		num++;
	}
	if( num < 2 )
	{
		prs_display_error( "Cannot normalize" );
		return false;
	}
	data.mean	= data.mean/num;

	#ifdef DEBUG
		cout << "DEBUG:   Calculating Variance & Standard deviation..." << endl;
	#endif
	data.variance	= 0;
	for( i=begin; i!=end; i++ )
	{
		m3_point	*p	= (dynamic_cast<m3_point *>(*i));
		double		value	= p->get_metric( metric_index );
		if( pars.use_log )
			value = log( value );
		else
			value = pow( value, pars.preprocess );	
		data.variance		+= pow( value - data.mean, 2 );
	}
	data.variance	= data.variance / ( num-1 );
	data.stdev	= sqrt( data.variance );

	#ifdef DEBUG
		cout << "DEBUG:   Calculating & writing observations..." << endl;
	#endif
	ofstream fx( file_out_name.c_str() );
	if( !fx.good() )
	{
		prs_display_message( "Failed to write the regressor input file" );
		return false;
	}
	int num_predictors	= 0;
	for( i=begin; i!=end; i++ )
	{
		vector<int>	design;
		double		observation;
		vector<double>	observations;
		m3_point	*p		= (dynamic_cast<m3_point *>(*i));
		double 		value		= p->get_metric( metric_index );
		if( pars.use_log )
			value = log( value );
		else
			value = pow( value, pars.preprocess );
		pars.normax ? 	observation 	= value / data.max : observation = ( value - data.mean )/data.stdev ;
		m3_rsm_compute_coded( 	*p, design, pars.interaction, pars.square, pars.exclusion_list );
		observations.push_back( observation );
		m3_xdr_write_design( fx, design, observations );
		num_predictors	= design.size();
	}
	fx.close();

	#ifdef DEBUG
		cout << "DEBUG:   Calling the regressor..." << endl;
	#endif
	string	command;
	command	= m3_get_image_path( env ) + "/rsm/regressor";
	command	= command + " " + file_out_name + " " + file_in_name;
	if( !shell_command( command ) )
	{
		prs_display_error( "Failed to run the regressor" );
		return false;
	}

	#ifdef DEBUG
		cout << "DEBUG:   Reading regressor output..." << endl;
	#endif
	vector<double> raw_data;
	ifstream ifx( file_in_name.c_str() );
	if( !m3_xdr_read_vector( ifx, raw_data ))
	{
		prs_display_error( "Unable to read raw data from the regressor" );
		return false;
	}
	m3_xdr_read_vector( ifx, data.residuals );
	m3_xdr_read_vector( ifx, data.zj );
	ifx.close();
	if( !m3_rsm_read_from_raw_data(	raw_data, num_predictors, 
					data.coeff, data.Radj, data.F, data.PF, data.T, data.PT ) )
	{
		prs_display_error( "Unable to parse raw data from the regressor" );
		return false;
	}

	#ifndef DEBUG
		if( !shell_command( "rm -f " + file_in_name ) )
			prs_display_error( "Impossible to delete: " + file_in_name );

		if( !shell_command( "rm -f " + file_out_name ) )
			prs_display_error( "Impossible to delete: " + file_out_name );
	#else
		cout << "DEBUG:   ** Statistical data for metric " << metric_index << " **" << endl;
		cout << "DEBUG:      Max            = " << data.max << endl;
		cout << "DEBUG:      Mean           = " << data.mean << endl;
		cout << "DEBUG:      Variance       = " << data.variance << endl;
		cout << "DEBUG:      Stdev          = " << data.stdev << endl;
		cout << "DEBUG:   ** Computed data for metric " << metric_index << " **" << endl;
		cout << "DEBUG:      num_predictors = " << num_predictors << endl;
		cout << "DEBUG:      data.Radj      = " << data.Radj << endl;
		cout << "DEBUG:      data.F         = " << data.F << endl;
		cout << "DEBUG:      data.PF        = " << data.PF << endl;
		for( int i=0; i<data.coeff.size(); i++ )
			cout << "DEBUG:      data.coeff[" << i << "] = " << data.coeff[i] << endl;
		for( int i=0; i<data.T.size(); i++ )
			cout << "DEBUG:      data.T[" << i << "] = " << data.T[i] << endl;
		for( int i=0; i<data.PT.size(); i++ )
			cout << "DEBUG:      data.PT[" << i << "] = " << data.PT[i] << endl;
	#endif
	
	return true;
}

bool m3_rsm_linear_regression_main( 	m3_env *env, m3_database *training_db, m3_database *predictors_db, 
					m3_database *predictions_db, regress_pars &pars, string temp_dir_path )
{
    m3_database * training_consistent_db = training_db->get_filtered_consistent(env);
	int 				metrics_number	= env->current_design_space->metric_index.size();
	list<m3_object *>::iterator 	tr_begin	= training_consistent_db->points->begin();
	list<m3_object *>::iterator 	tr_end		= training_consistent_db->points->end();
	list<m3_object *>::iterator 	pr_begin	= predictors_db->points->begin();
	list<m3_object *>::iterator 	pr_end		= predictors_db->points->end();
	list<m3_object *>::iterator 	it;
	#ifdef DEBUG
		cout << "DEBUG: Regression parameters initialization " << endl;
		cout << "DEBUG:   interaction = " << pars.interaction << endl;
		cout << "DEBUG:   square      = " << pars.square << endl;
		cout << "DEBUG:   normax      = " << pars.normax << endl;
		cout << "DEBUG:   use_log     = " << pars.use_log << endl;
		cout << "DEBUG:   preprocess  = " << pars.preprocess << endl;
		cout << "DEBUG:   exclusion_list: " << endl;
		for( int i=0; i< pars.exclusion_list.size(); i++ )		
			cout << "DEBUG:      exclusion_list[" << i << "] = " << pars.exclusion_list[i] << endl;
	#endif

	#ifdef DEBUG
		cout << "DEBUG: Calculating regression data" << endl;
	#endif
	vector<regress_data>	tot_data;
	for( int i=0; i<metrics_number; i++ )
	{
		regress_data 	data;
		bool res = m3_rsm_linear_calculate_regress_data( env, training_consistent_db, i, data, pars, temp_dir_path );
		if( !res )
		{
			prs_display_error( "Impossible to run the Linear Regression predictor" );
            delete training_consistent_db;
			return false;
		}
		tot_data.push_back( data );
	}
	#ifdef DEBUG
		cout << "DEBUG: Making predictions" << endl;
	#endif
	predictions_db->clear();
	for( it=pr_begin; it!=pr_end; it++ )
	{
		vector<double>	metrics;
		m3_point 	*p = (dynamic_cast<m3_point *>(*it));
		for( int i=0; i<pars.number_of_metrics; i++ )
		{
			vector<int>	design;
			m3_rsm_compute_coded( *p, design, pars.interaction, pars.square, pars.exclusion_list );
			double value =	m3_rsm_compute_metric( design, tot_data[i].coeff, 
					tot_data[i].mean, tot_data[i].stdev, tot_data[i].max, 
					pars.normax, pars.preprocess, pars.use_log );
			metrics.push_back( value );
		}
		m3_vector 	*m3_metrics 	= new m3_vector();
		vector<int>	point_coord	= *p;
		m3_point	*actual		= new m3_point( pars.number_of_parameters );
		for( int i=0; i<point_coord.size(); i++ )
		{
			(*actual)[i] = point_coord[i];
		}
		for( int i=0; i<metrics_number; i++ )
		{
            m3_double m(metrics[ i ]);
			m3_metrics->insert( i, m );
		}
		actual->set_properties( "metrics", *m3_metrics );
		predictions_db->insert_point( actual );
		delete m3_metrics;
		delete actual;
	}
    delete training_consistent_db;
	return true;
}

class m3_rsm_linear_regression: public m3_rsm
{
	public:
		m3_object *train_rsm( m3_env *env, string predictions_db )
		{
			if( !env->current_doe )
			{
				prs_display_error( "Please define a DoE before training this RSM" );
				return new m3_integer( 0 );
			}
			string training_db = env->current_db_name;
			if( training_db!=predictions_db )
				prepare_shell_db( env, predictions_db );

			regress_pars pars;
			pars.number_of_metrics		= env->current_design_space->metric_index.size();
			pars.number_of_parameters	= env->current_design_space->ds_parameters.size();

			//managing preprocess parameter
			if( !get_preprocess_parameter( env, pars.use_log, pars.preprocess ) )
				get_default_preprocess_value( pars.use_log, pars.preprocess );

			//managing order parameter
			int order;
			if( !get_integer_from_variables( env, "order", order ) )
				get_default_square_value( pars.square );
			else
			{
				if( order == 1 )
					pars.square = false;
				else if( order == 2 )
					pars.square = true;
				else
					get_default_square_value( pars.square );
			}

			//managing normax parameter
			string normax;
			if( !get_string_from_variables( env, "normax", normax ) )
				get_default_normax_value( pars.normax );
			else
			{
				if( normax == "true" )
					pars.normax = true;
				else if( normax == "false" )
					pars.normax = false;
				else
					get_default_normax_value( pars.normax );
			}

			//managing interaction parameter
			string interaction;
			if( !get_string_from_variables( env, "interaction", interaction ) )
				get_default_interaction_value( pars.interaction );
			else
			{
				if( interaction == "true" )
					pars.interaction = true;
				else if( interaction == "false" )
					pars.interaction = false;
				else
					get_default_interaction_value( pars.interaction );
			}

			//managing exclude parameter
			m3_vector *exclude;
			if( !get_vector_from_variables( env, "exclude", exclude ) )
				get_default_exclusion_list_value( pars.exclusion_list, pars.number_of_parameters );
			else
			{
				pars.exclusion_list.clear();
				pars.exclusion_list.resize( pars.number_of_parameters, false );
				for( int i=0; i<exclude->size(); i++ )
				{
					m3_object *obj = &exclude->get( i ); 
					if( is_a<m3_string *>(obj) )
					{
						string to_exclude = obj_get_string( obj );
						if( env->current_design_space->ds_parameters_index.count( to_exclude ) )
						{
							int to_exclude_index	= env->current_design_space->ds_parameters_index[ to_exclude ];
							pars.exclusion_list[ to_exclude_index ] = true;
						}
						else
							prs_display_error( "Impossible to exclude \"" + to_exclude + "\", it doesn't exists.");
					}
				}
			}
			
			//creating a temporary directory
			string temp_dir_path	= m3_get_unique_temp_dir_name( "/m3explorer_tmp_rsm" );
			if( temp_dir_path == "" )
				return new m3_integer( 0 );
			if( !m3_mkdir( temp_dir_path ) )
			{
				prs_display_error( "Unable to create directory: " + temp_dir_path );
				return new m3_integer( 0 );
			}

			//managing training db
			m3_database *training = env->available_dbs[ training_db ];

			//managing predictors db
			m3_database predictors( env, env->current_doe );

			//Calling the linear regression interpolator
			m3_database *predictions = env->available_dbs[ predictions_db ];
			m3_rsm_linear_regression_main( env, training, &predictors, predictions, pars, temp_dir_path );

			//cleaning the temporary directory
			#ifndef DEBUG
				if( !m3_rmdir( temp_dir_path ) )
				{
					prs_display_error("Unable to delete temporary directory: " + temp_dir_path );
				}
			#endif

			return new m3_integer( 1 );
		};

		string print_informations()
		{
			return 	"parameters: order={1,2}; interaction={\"true\",\"false\"}; normax={\"true\",\"false\"}; preprocess={FLOAT,\"log\"}";
		};

		bool set_validation_parameters( m3_env *env, int current_validation_configuration )
		{
			if( current_validation_configuration>=get_validation_configurations( env ) )
				return false;
			m3_vector *parameters_list;

			if( get_vector_from_variables( env, "preprocess_list", parameters_list ) )
			{
				if( current_validation_configuration < parameters_list->size() )
					prs_command_set( new m3_string( "preprocess" ), 
					&parameters_list->get( current_validation_configuration ) );
				else
					set_default_preprocess_variable();
			}
			else
				set_default_preprocess_variable();

			if( get_vector_from_variables( env, "order_list", parameters_list ) )
			{
				if( current_validation_configuration < parameters_list->size() )
					prs_command_set( new m3_string( "order" ),
					&parameters_list->get( current_validation_configuration ) );
				else
					set_default_order_variable();
			}
			else
				set_default_order_variable();

			if( get_vector_from_variables( env, "interaction_list", parameters_list ) )
			{
				if( current_validation_configuration < parameters_list->size() )
					prs_command_set( new m3_string( "interaction" ), 
					&parameters_list->get( current_validation_configuration ) );
				else
					set_default_interaction_variable();
			}
			else
				set_default_interaction_variable();

			if( get_vector_from_variables( env, "normax_list", parameters_list ) )
			{
				if( current_validation_configuration < parameters_list->size() )
					prs_command_set( new m3_string( "normax" ), 
					&parameters_list->get( current_validation_configuration ) );
				else
					set_default_normax_variable();
			}
			else
				set_default_normax_variable();

			if( get_vector_from_variables( env, "exclude_list", parameters_list ) )
			{
				if( current_validation_configuration < parameters_list->size() )
					prs_command_set( new m3_string( "exclude" ), 
					&parameters_list->get( current_validation_configuration ) );
				else
					set_default_exclude_variable( env );
			}
			else
				set_default_exclude_variable( env );
			return true;	
		};

		void set_default_validation_configuration( m3_env *env )
		{
			bool use_log;
			double preprocess;
			m3_vector *preprocess_obj = new m3_vector();
			get_default_preprocess_value( use_log, preprocess );
			if( use_log )
			{
				m3_string preprocess_value("log");
				preprocess_obj->insert( 0, preprocess_value );
			}
			else
			{
				m3_double preprocess_value( preprocess );
				preprocess_obj->insert( 0, preprocess_value );
			}
			prs_command_set( new m3_string( "preprocess_list" ), preprocess_obj );

			bool square;
			int order = 1;
			m3_vector *order_obj = new m3_vector();
			get_default_square_value( square );
			if( square )
				order = 2;
			m3_integer order_value( order );
			order_obj->insert( 0, order_value );
			prs_command_set( new m3_string( "order_list" ), order_obj );

			bool normax;
			m3_vector *normax_obj = new m3_vector();
			get_default_normax_value( normax );
			if( normax )
			{
				m3_string normax_value("true");
				normax_obj->insert( 0, normax_value );
			}
			else
			{
				m3_string normax_value("false");
				normax_obj->insert( 0, normax_value );
			}
			prs_command_set( new m3_string( "normax_list" ), normax_obj );

			bool interaction;
			m3_vector *interaction_obj = new m3_vector();
			get_default_interaction_value( interaction );
			if( interaction )
			{
				m3_string interaction_value("true");
				interaction_obj->insert( 0, interaction_value );
			}
			else
			{
				m3_string interaction_value("false");
				interaction_obj->insert( 0, interaction_value );
			}
			prs_command_set( new m3_string( "interaction_list") , interaction_obj );

			m3_vector exclude_element;
			m3_vector *exclude_obj = new m3_vector();
			exclude_obj->insert( 0, exclude_element );
			prs_command_set( new m3_string("exclude_list"), exclude_obj );

		};

		int get_validation_configurations( m3_env * env )
		{
			int preprocess_configurations	= 0;
			int order_configurations 	= 0;
			int interaction_configurations	= 0;
			int normax_configurations	= 0;
			int exclude_configurations	= 0;
			int max_confs;
			m3_vector *parameters_list;
			if( get_vector_from_variables( env, "preprocess_list", parameters_list ) )
				preprocess_configurations	= parameters_list->size();
			max_confs = preprocess_configurations;
			if( get_vector_from_variables( env, "order_list", parameters_list ) )
				order_configurations		= parameters_list->size();
			max_confs = max( max_confs, order_configurations );
			if( get_vector_from_variables( env, "interaction_list", parameters_list ) )
				interaction_configurations	= parameters_list->size();
			max_confs = max( max_confs, interaction_configurations );
			if( get_vector_from_variables( env, "normax_list", parameters_list ) )
				normax_configurations		= parameters_list->size();
			max_confs = max( max_confs, normax_configurations );
			if( get_vector_from_variables( env, "exclude_list", parameters_list ) )			
				exclude_configurations		= parameters_list->size();
			max_confs = max( max_confs, exclude_configurations );
			return max_confs;
		};

		string get_validation_graphic_title()
		{
			return "Linear Regression on DoE validation";
		};

		string get_validation_curve_name( m3_env *env )
		{
			string	preprocess, order, interaction, normax, exclude;
			double 	d_data;
			int	i_data;
			bool	b_data;
			string	s_data;

			if( !get_preprocess_parameter( env, b_data, d_data ) )
				preprocess = get_preprocess_default_string();
			else
			{
				ostringstream message;
				if( b_data )
					message << "log";
				else
					message << d_data;
				preprocess = message.str();
			}

			if( !get_integer_from_variables( env, "order", i_data ) )
				order	= get_order_default_string();
			else
			{
				if( i_data == 1 || i_data == 2 )
					order = m3_itos( i_data );
				else					
					order	= get_order_default_string();
			}

			if( !get_string_from_variables( env, "interaction", s_data ) )
				interaction	= get_interaction_default_string();
			else
			{
				if( s_data == "true" || s_data == "false" )
					interaction	= s_data;
				else
					interaction	= get_interaction_default_string();
			}
			
			if( !get_string_from_variables( env, "normax", s_data ) )
				normax	= get_normax_default_string();
			else
			{
				if( s_data == "true" || s_data == "false" )
					normax	= s_data;
				else
					normax	= get_interaction_default_string();
			}
				
			int 		num_of_parameters	= env->current_design_space->ds_parameters.size();
			m3_vector 	*v_data;
			if( !get_vector_from_variables( env, "exclude", v_data ) )
				exclude	= get_exclude_default_string( env );
			else
			{
				exclude = "{";
				for( int i=0; i<v_data->size(); i++ )
				{
					m3_object *obj = &v_data->get( i ); 
					if( is_a<m3_string *>(obj) )
					{
						string to_exclude = obj_get_string( obj );
						if( env->current_design_space->ds_parameters_index.count( to_exclude ) )
						{
							if( exclude != "{" )
								exclude += ",";
							exclude += to_exclude;
						}
					}		
				}
				exclude += "}";
			}
			return 	"box=" + preprocess + "-order=" + order + "-int=" + interaction + 
				"-norm=" + normax + "-excl=" + exclude;
		};

	private:

		string get_preprocess_default_string()
		{
			bool 		use_log;
			double		preprocess;
			get_default_preprocess_value( use_log, preprocess );
			if( use_log )
				return "log";
			return m3_dtos( preprocess );
		};

		string get_order_default_string()
		{
			bool	square;
			get_default_square_value( square );
			if( square )
				return "2";
			return "1";
		};

		string get_interaction_default_string()
		{
			bool	interaction;
			get_default_interaction_value( interaction );
			if( interaction )
				return "true";
			return "false";
		};		

		string get_normax_default_string()
		{
			bool 	normax;
			get_default_normax_value( normax );
			if( normax )
				return "true";
			return "false";
		};

		string get_exclude_default_string( m3_env *env )
		{
			vector<bool> exclusion_list;
			int num_of_parameters = env->current_design_space->ds_parameters.size();
			get_default_exclusion_list_value( exclusion_list, num_of_parameters );
			string exclude = "{";
			for( int i=0; i<exclusion_list.size(); i++ )
			{
				if( exclusion_list[i] )
				{
					if( exclude != "{" )
						exclude += ",";
					exclude += env->current_design_space->ds_parameters[ i ].name;
				}
			}
			exclude += "}";
			return exclude;
		};

		void set_default_preprocess_variable()
		{
			bool	use_log;
			double	preprocess;
			get_default_preprocess_value( use_log, preprocess );
			if( use_log )
				prs_command_set( new m3_string( "preprocess" ), new m3_string( "log" ) );
			else
				prs_command_set( new m3_string( "preprocess" ), new m3_double( preprocess ) );	
		};

		void set_default_order_variable()
		{
			bool square;
			get_default_square_value( square );
			if( square )
				prs_command_set( new m3_string( "order" ), new m3_integer( 2 ) );
			else
				prs_command_set( new m3_string( "order" ), new m3_integer( 1 ) );
		};

		void set_default_interaction_variable()
		{
			bool interaction;
			get_default_interaction_value( interaction );
			if( interaction )
				prs_command_set( new m3_string( "interaction" ), new m3_string( "true" ) );
			else
				prs_command_set( new m3_string( "interaction" ), new m3_string( "false" ) );
		};

		void set_default_normax_variable()		
		{
			bool	normax;
			get_default_normax_value( normax );
			if( normax )
				prs_command_set( new m3_string( "normax" ), new m3_string( "true" ) );
			else
				prs_command_set( new m3_string( "normax" ), new m3_string( "false" ) );
		};

		void set_default_exclude_variable( m3_env *env )
		{
			vector<bool>	exclusion_list;
			m3_vector	*exclude 		= new m3_vector();
			int		index 			= 0;
			int		num_of_parameters	= env->current_design_space->ds_parameters.size();
			get_default_exclusion_list_value( exclusion_list, num_of_parameters );
			for( int i=0; i<exclusion_list.size(); i++ )
			{
				if( exclusion_list[i] )
				{
					string to_exclude = env->current_design_space->ds_parameters[ i ].name;
					exclude->insert( index, *( new m3_string( to_exclude ) ) );
					index++;
				}
			}					
			prs_command_set( new m3_string( "exclude" ), exclude );
		};

		void get_default_preprocess_value( bool &use_log, double &preprocess )
		{
			use_log		= false;
			preprocess	= 1.0;
		};

		void get_default_square_value( bool &square )
		{
			square		= false;
		};

		void get_default_normax_value( bool &normax )
		{
			normax		= false;
		};

		void get_default_interaction_value( bool &interaction )
		{
			interaction	= false;
		};

		void get_default_exclusion_list_value( vector<bool> &exclusion_list, int num_of_parameters )
		{
			exclusion_list.clear();
			exclusion_list.resize( num_of_parameters, false );
		};
};

//------------------------------------------------------------------------------//
//			      Environment support				//
//------------------------------------------------------------------------------//

m3_rsm_nn			nn;
m3_rsm_rbf			rbf;
m3_rsm_sh_i			sh_i;
m3_rsm_spline			spline_i;
m3_rsm_linear_regression	linear_regression;

void rsm_init_rsms(m3_env *env)
{
	env->current_rsms["NN"]		= &nn;
	env->current_rsms["RBF"]	= &rbf;
	env->current_rsms["SHEPARD"] 	= &sh_i;
	env->current_rsms["SPLINE"]	= &spline_i;
	env->current_rsms["LINEAR"]	= &linear_regression;
}

//------------------------------------------------------------------------------//
//				   VALIDATION					//
//------------------------------------------------------------------------------//

double get_avg( vector<double> sequence )
{
	double average_value = 0.0;
	for( int i=0; i<sequence.size(); i++ )
		average_value += sequence[i];
	return ( average_value / ((double)sequence.size()) );
}

vector<double> compute_average_normalized_error( m3_env *env, m3_database *reference_db, m3_database *current_db )
{
	list<m3_object *>::iterator i;
	int		n		= 0;
	int 		metrics_number 	= env->current_design_space->metric_index.size();
	vector<double>	avg_norm_err( metrics_number );
	for(int metric_index=0; metric_index<metrics_number; metric_index++ )
		avg_norm_err[ metric_index ] = 0;
#ifdef DEBUG
	cout << "DEBUG: Running average normalized error computing...";
#endif
	for( i=current_db->get_list_of_points()->begin(); i!=current_db->get_list_of_points()->end(); i++ )
	{
		m3_point *p = (dynamic_cast<m3_point *>(*i));
		if( env->current_design_space->is_valid( env, p ) )
		{
			m3_point *ref_p	= reference_db->find_point( *p );
			if( ref_p!=NULL )
			{
				n++;
				for( int metric_index=0; metric_index<metrics_number; metric_index++ )
				{
					double sx 	= p->get_metric(metric_index) - ref_p->get_metric(metric_index);
					double absolute	= fabs( sx/ref_p->get_metric(metric_index) );
					avg_norm_err[ metric_index ] = avg_norm_err[ metric_index ] + absolute;
				}
			}
		}
	}
	for(int metric_index=0; metric_index<metrics_number; metric_index++ )
		avg_norm_err[ metric_index ] = avg_norm_err[ metric_index ]/((double)n);
#ifdef DEBUG
	cout << " error computed on "  << n << " points.\n";
#endif
	return avg_norm_err;
}

bool get_avg_error_from_rsm( m3_env *env, string rsm_name, m3_database *ref_db, m3_database *training_db, double &error )
{
	string		current_db_name		= env->current_db_name;
	string		training_db_name	= "rsm_validation_tmp_training_" 	+ m3_itos( getpid() );
	string		predictions_db_name	= "rsm_validation_tmp_predictions_" 	+ m3_itos( getpid() );
	double		average_error;
	if( env->available_dbs.count( training_db_name ) )
		delete env->available_dbs[ training_db_name ];
	env->available_dbs[ training_db_name ] 	= training_db;
	env->current_db_name			= training_db_name;	
	#ifdef DEBUG
		cout << "DEBUG: Training " << rsm_name << " on " << training_db->size() << " points...\n";
	#endif
	m3_object 	*obj 	= env->current_rsms[ rsm_name ]->train_rsm( env, predictions_db_name );
	if( obj_get_integer( obj ) != 1 )
	{		
		env->current_db_name 	= current_db_name;
		error 			= 0;
		if( env->current_db_name != training_db_name )
			env->available_dbs.erase( training_db_name );
		if( env->current_db_name != predictions_db_name )
		{
			delete env->available_dbs[ predictions_db_name ];
			env->available_dbs.erase( predictions_db_name );
		}	
		return false;
	}
	env->current_db_name	= current_db_name;	
	error			= get_avg( compute_average_normalized_error( env, ref_db, env->available_dbs[ predictions_db_name ] ) );
	if( env->current_db_name!=training_db_name )
		env->available_dbs.erase( training_db_name );
	if( env->current_db_name!=predictions_db_name )
	{
		delete env->available_dbs[ predictions_db_name ];
		env->available_dbs.erase( predictions_db_name );
	}
	return true;
}

#define DEFAULT_NUM_SAMPLES 5

int get_num_samples( m3_env *env )
{
	int num_samples;
	if( get_integer_from_variables( env, "num_samples", num_samples ) )
		if( num_samples<=0 )
			return DEFAULT_NUM_SAMPLES;
		else
			return num_samples;
	else
		return DEFAULT_NUM_SAMPLES;
}

void rsm_validate_rsm( m3_env *env, string rsm_name, m3_database *ref_db, bool reuse_trainers )
{
	srand( time( 0 ) );
	if( !env->current_rsms.count( rsm_name ) )
	{
		prs_display_error( "The RSM model '" + rsm_name  + "' doesn't exists" );
		return;
	}
	//managing trainers list
	m3_vector *trainers;
	if( !get_vector_from_variables( env, "trainers", trainers ) )
	{
		prs_display_error( "Please define the shell array \"trainers\" before launching this command ");
		return;
	}
	vector<int> 	launch_trainers( trainers->size() );
	for( int i=0; i<trainers->size(); i++ )
	{
		m3_object &obj = trainers->get(i);
		if( !is_a<m3_integer *>(&obj) )
		{
			prs_display_error( "Use only non-negatives integers with \"trainers\" shell array" );
			return;
		}
		int number = obj_get_integer( &obj );
		if( number<0 )
		{
			prs_display_error( "Use only non-negatives integers with \"trainers\" shell array" );
			return;
		}
		launch_trainers[i] = number;
	}
	//discarding unconsistents points
	prs_display_message( "Filtering the training db from unconsistents design space points..." );
	sort( launch_trainers.begin(), launch_trainers.end() );
	m3_database *consistent_db	= ref_db->get_filtered_consistent( env );
	if( consistent_db->size() < launch_trainers[ launch_trainers.size() - 1 ] )
	{
		prs_display_error( "The effective reference db is smaller than the bigger training set of points" );
		return;
	}
	
	int 				num_samples 		= get_num_samples( env );
	int 				num_configurations	= env->current_rsms[ rsm_name ]->get_validation_configurations( env );
	if( num_configurations == 0)
	{
		env->current_rsms[ rsm_name ]->set_default_validation_configuration( env );
		num_configurations = 1;
	}
	//3D arrays creation
	int 				discarded_trainers 	= 0;
	double				***validations_results;
	bool				***overall_discarded_trainers;
	validations_results		= (double***)malloc( num_configurations * sizeof*validations_results );
	overall_discarded_trainers	= (bool***)malloc( num_configurations * sizeof*overall_discarded_trainers );
	for( int conf = 0; conf < num_configurations; conf++ )
	{
		validations_results[ conf ] 		= (double**)malloc( num_samples * sizeof**validations_results );
		overall_discarded_trainers[ conf ] 	= (bool**)malloc( num_samples * sizeof**overall_discarded_trainers );
		for( int sample = 0; sample < num_samples; sample++ )
		{
			validations_results[ conf ][ sample ] 		= (double*)malloc( launch_trainers.size() * sizeof***validations_results );
			overall_discarded_trainers[ conf ][ sample ]	= (bool*)malloc( launch_trainers.size() * sizeof***overall_discarded_trainers );
		}
	}
	#ifdef DEBUG
		ofstream incomplete_results( "results.log", ios::out );
		if( incomplete_results.fail() )
			return;
		incomplete_results << "RSM name: " << rsm_name << endl;
		incomplete_results << "Consistent db size: " << consistent_db->size() << endl;
		incomplete_results << "Number of samples: " << num_samples << endl;
		incomplete_results << "Number of configurations: " << num_configurations << endl << endl;
		incomplete_results << left;
	#endif
	map< int, string >	conf_curve_name_map;
	int			longest_name		= 0;
	set< string >		names;
	//names choice
	for( int conf=0; conf < num_configurations; conf++ )
	{
		env->current_rsms[ rsm_name ]->set_validation_parameters( env, conf );
		string		curve_base_name	= env->current_rsms[ rsm_name ]->get_validation_curve_name( env );
		string 		curve_name	= curve_base_name;
		int		index		= 0;	
		m3_vector 	points;
		m3_object 	*obj;
		while( names.find( curve_name ) != names.end() )
		{
			index++;
			curve_name	= curve_base_name + "_" + m3_itos( index );
		}
		names.insert( curve_name );
		longest_name 			= max( longest_name, (int) curve_name.length() );
		conf_curve_name_map[ conf ] 	= curve_name;		
		prs_display_message( "Choosen the name \"" + curve_name+ "\" for training results generated with the parameters at index " + m3_itos( conf ) );
		#ifdef DEBUG
			incomplete_results << "Configurations at " << conf << ": \"" << curve_name << "\"" << endl;
		#endif
	}
	#ifdef DEBUG
		int incomplete_name_column_width	= longest_name + 3;
		int incomplete_column_width		= 20;		
		incomplete_results << endl; incomplete_results.width( incomplete_name_column_width ); incomplete_results << "Curve name";
		incomplete_results.width( incomplete_column_width ); incomplete_results << "Sample";
		incomplete_results.width( incomplete_column_width ); incomplete_results << "Nominal trainers";
		incomplete_results.width( incomplete_column_width ); incomplete_results << "Real trainers";
		incomplete_results.width( incomplete_column_width ); incomplete_results << "Error" << endl;
	#endif
	//validation loop 
	for( int sample = 0; sample < num_samples; sample++ )
	{
		vector<m3_database *> dbs( launch_trainers.size() );
		if( reuse_trainers )
		{
			dbs[ launch_trainers.size() - 1 ] = consistent_db->get_random_subset( launch_trainers[ launch_trainers.size() - 1 ] );
			for( int i=launch_trainers.size()-2; i>=0; i-- )
			{
				dbs[ i ] = dbs[ i + 1 ]->get_random_subset( launch_trainers[ i ] );
			}
		}
		prs_display_message( "Sample " + m3_itos( sample ) );
		for( int conf=0; conf < num_configurations; conf++ )
		{
			env->current_rsms[ rsm_name ]->set_validation_parameters( env, conf );
			string curve_name 	= conf_curve_name_map[ conf ];
			prs_display_message( "Collecting validation results for \"" + curve_name +  "\"..." );
			for( int i=0; i<launch_trainers.size(); i++ )
			{
				int		real_trainers_number;
				double		avg_error;
				m3_database	*training_db;
				if( reuse_trainers )
					training_db	= dbs[ i ];
				else
					training_db	= consistent_db->get_random_subset( launch_trainers[i] );
				bool accept_point	= get_avg_error_from_rsm( env, rsm_name, consistent_db, training_db, avg_error );
				//bool accept_point = randfloat()<0.6 ? false : true;avg_error = randfloat();
				if( !accept_point )
				{
					overall_discarded_trainers[ conf ][ sample ][ i ] 	= true;
					discarded_trainers++;
				}
				else
				{
					overall_discarded_trainers[ conf ][ sample ][ i ] 	= false;
					validations_results[ conf ][ sample ][ i ] 		= avg_error;					
				}
				#ifdef DEBUG
					incomplete_results.width( incomplete_name_column_width ); incomplete_results << curve_name;
					incomplete_results.width( incomplete_column_width ); incomplete_results << sample;
					incomplete_results.width( incomplete_column_width ); incomplete_results << launch_trainers[ i ];
					incomplete_results.width( incomplete_column_width ); incomplete_results << training_db->size();
					incomplete_results.width( incomplete_column_width );
					if( accept_point )
						incomplete_results << avg_error << endl;
					else
						incomplete_results << "X" << endl;
					incomplete_results.flush();
				#endif
				if( !reuse_trainers )
					delete training_db;
			}
		}
		if( reuse_trainers )
		{
			for( int i=0; i<launch_trainers.size(); i++ )
				delete dbs[ i ];	
			dbs.clear();
		}
	}
	#ifdef DEBUG
		incomplete_results.close();
	#endif
	//composing vector to plot
	map< string, vector< pair< double, double > > >	to_plot;
	for( int conf = 0; conf < num_configurations; conf++ )
	{
		string curve_name	= conf_curve_name_map[ conf ];
		for( int i = 0; i < launch_trainers.size(); i++ )
		{
			vector< double > current_trainers_results;
			for( int sample = 0; sample < num_samples; sample++ )
				if( !overall_discarded_trainers[ conf ][ sample ][ i ] )
					current_trainers_results.push_back( validations_results[ conf ][ sample ][ i ] );
			if( current_trainers_results.size() > 0 )
			{
				double samples_avg_error = get_avg( current_trainers_results );
				to_plot[ curve_name ].push_back( make_pair( launch_trainers[ i ], samples_avg_error ) );
			}
		}
	}
	//validation summary log file generation
	FILE 	*validation_summary;
	string 	validation_summary_filename 	= rsm_name + "_validation_results_summary.log";
	validation_summary 			= fopen( validation_summary_filename.c_str(), "w");
	if( validation_summary == NULL )
	{
		prs_display_error("Impossible to create the validation results summary file.");
	}
	else
	{
		int	column_width	= 15;
		int	left_margin	= longest_name + 2;
		int	line_width	= column_width * ( num_samples + 2 ) + 1;
		char	*line;
		line			= (char*) malloc( line_width );
		memset( line, '_', line_width -1 );
		line[ line_width - 1 ] = '\0';
		for( int conf = 0; conf < num_configurations; conf++ )
		{
			string 	curve_name 	= conf_curve_name_map[ conf ];
			fprintf( validation_summary, "\n %-*s  ", left_margin - 2, curve_name.c_str() );
			fprintf( validation_summary, "%s\n", line);
			fprintf( validation_summary, "%*s|%*s|\n", left_margin, "", line_width - 1, "" );
			fprintf( validation_summary, "%*s|%-*s", left_margin, "", column_width, "Trainers" );
			for( int sample = 0; sample < num_samples; sample++ )
			{
				string column_title = "Sample " + m3_itos( sample );
				fprintf(validation_summary, "%-*s", column_width, column_title.c_str());
			}
			fprintf( validation_summary, "%-*s|\n", column_width,"Average");		
			fprintf( validation_summary, "%*s|%s|\n", left_margin, "", line );
			fprintf( validation_summary, "%*s|%*s|\n", left_margin, "", line_width - 1, "" );
			for( int i = 0; i < launch_trainers.size(); i++ )
			{
				vector< double > current_trainers_results;
				fprintf( validation_summary, "%*s|%-*i", left_margin, "", column_width, launch_trainers[ i ] );
				for( int sample = 0; sample < num_samples; sample++ )
				{
					if( overall_discarded_trainers[ conf ][ sample ][ i ] )
						fprintf( validation_summary, "%-*s", column_width, "X" );
					else
					{
						fprintf( validation_summary, "%-*f", column_width, validations_results[ conf ][ sample ][ i ] );
						current_trainers_results.push_back( validations_results[ conf ][ sample ][ i ] );
					}
				}
				if( current_trainers_results.size() > 0 )
				{
					double samples_avg_error = get_avg( current_trainers_results );
					fprintf( validation_summary, "%-*f|\n", column_width, samples_avg_error );
				}	
				else
				{
					fprintf( validation_summary, "%-*s|\n", column_width, "X" );
				}
			}
			fprintf( validation_summary, "%*s|%s|\n", left_margin, "", line );
		}
		free( line );
		fprintf( validation_summary, "\n" );
		fclose( validation_summary );
		prs_display_message( "Generated \"" + validation_summary_filename + "\"" );
	}
	//freeing memory
	for( int conf = num_configurations - 1; conf >= 0; conf-- )
	{
		for( int sample = num_samples - 1; sample >= 0; sample-- )
		{
			free( validations_results[ conf ][ sample ] );
			free( overall_discarded_trainers[ conf ][ sample ] );
		}
		free( validations_results[ conf ] );
		free( overall_discarded_trainers[ conf ] );
	}
	free( validations_results );
	free( overall_discarded_trainers );
	delete consistent_db;
	
	m3_plot_vectors( &to_plot, rsm_name + "_validation", env->current_rsms[ rsm_name ]->get_validation_graphic_title(), 
			"Number of simulations", false, 1.0, "Average normalized error", false, 1.0, true );
	prs_display_message( "Generated \"" + rsm_name + "_validation.ps\"" );
	prs_display_message( "Validation done." );
}

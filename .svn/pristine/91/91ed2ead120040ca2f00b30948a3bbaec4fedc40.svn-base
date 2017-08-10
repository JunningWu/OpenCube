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








#ifndef M3_RSM
#define M3_RSM

class m3_rsm;

#include <m3_database.h>
#include <m3_env.h>


/** Abstract RSM definition 
 * */
class m3_rsm
{
	public:

	/** 
	 * Executes the RSM training and produces predictions.
	 * The training set is the current shell database. The predictions are written into the specified shell database.
	 * @param env the current environment.
	 * @param predictions_db the shell database name into which write the predictions.
	 * @return the exit status.
	 * */
	virtual m3_object 	*train_rsm( m3_env *env, string predictions_db )=0;

	/** 
	 * Gets RSM informations.
	 * @return the informations.
	 * */
	virtual string 		print_informations()=0;

	/** 
	 * Sets the shell variables used from the RSM as parameters with values stored in shell vectors.
	 * The name of each vector is composed of the same name of the corresponding parameter followed by "_list".
	 * The setting operation is done using the values stored in the shell vectors at the specified location.
	 * An admissible location is included between 0 and ( get_validation_configurations( m3_env * ) - 1 ).
	 * If the shell vectors aren't of the same length and the setting request is out of bounds for some vectors,
	 * the default values are assumed for the parameters corresponding to such vectors.
	 * If a specified value is wrong, the default value is assumed for the corresponding parameter.
	 * @param env the current environment.
	 * @param current_validation_configuration is the location of the shell vectors.
	 * @return false if current_validation_configuration is out of bounds.
	 * */
	virtual bool		set_validation_parameters( m3_env *env, int current_validation_configuration )=0;

	virtual void 		set_default_validation_configuration( m3_env *env )=0;

	/** 
	 * Gets the configurations number of the parameters to use during the validation process.
	 * Each parameter has a corresponding shell vector with a name composed of the corresponding parameter name followed by "_list".
	 * The number of configurations corresponds to the maximum shell vector length among those required.
	 * @param env the current environment.
	 * @return the configurations number.
	 * */
	virtual int 		get_validation_configurations( m3_env *env )=0;

	/** 
	 * Gets the title for the validation graph of the RSM.
	 * @return the validation graph title.
	 * */
	virtual string		get_validation_graphic_title()=0;

	/** 
	 * Gets the curve name for the validation graph.
	 * The curve name is based on the current parameters settings.
	 * @return the curve name.
	 * */
	virtual string		get_validation_curve_name( m3_env *)=0;
};

/** 
 * Prepares the environment to support the existings RSMs.
 * @param env the current environment.
 * */
void rsm_init_rsms( m3_env *env );

/** 
 * Executes the RSM validation.
 * This function takes subsets of a reference database to train an RSM and to produce a graph 
 * of the resulting average normalized error versus the number of simulations.
 * The number of simulations corresponds to the number of points used for training the RSM, hence to the subset size.
 * The points of each subset are selected randomly from the reference database.
 * The number of points of each subset is specified by the user into the shell vector "trainers".
 * The RSM parameters are specified into shell vectors.
 * For each RSM parameter the user have to specify a corresponding shell vector.
 * The name of each shell vector is composed of the name of the corresponding parameter followed by "_list".
 * It is possible to validate the RSM in respect to various parameters values specifying more than one value 
 * for the parameters into the corresponding vectors.
 * The first validation is done in respect to the parameters values contained into the corresponding shell vectors
 * at index 0, the second is done in respect to the values at index 1 and so on.
 * The number of validations corresponds to the maximum vector size between those specified by the user.
 * If for a validation a parameter value is missing or wrong will be used the defaul one.
 * It is possible to reuse the same points to generate the subsets in an incremental way.
 * It is also possible to reuse the same subsets for the various validations.
 * @param env the current environment.
 * @param rsm_name the name of the RSM.
 * @param ref_db the database containing the simulated points to take as reference.
 * @param reuse_trainers specify the reuse of points and subsets.
 * */
void rsm_validate_rsm( m3_env *env, string rsm_name, m3_database *ref_db, bool reuse_trainers );
#endif

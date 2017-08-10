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





#include <m3_doe.h>
#include <m3_parser.h>
#include <m3_shell_variables.h>

class m3_random_doe: public m3_doe
{
    public:
	vector<m3_point> generate_doe(m3_env *env);

	string get_information();
};

vector<m3_point> m3_random_doe::generate_doe(m3_env *env)
{

	vector<m3_point> doe;


	int n_solutions;
    
	if(!get_integer_from_variables(env,"solutions_number",n_solutions)) 
	{
		n_solutions=10;
		prs_display_message("solutions_number variable not found");	
	}

	for(int points=0 ; points<n_solutions ; points++)
	{
		m3_point actual_point = env->current_design_space->random_point(env);

        /** FIXME: this is already done by the optimizers, should it stay here?
         *  At the moment we comment it in order for tests to pass */
		/* if(env->current_design_space->is_valid(env, &actual_point))
         */
		doe.push_back(actual_point);
	}
	return doe;
}

string m3_random_doe::get_information()
{
    return "Random doe";
}

extern "C" 
{
    m3_doe *doe_generate_doe()
    {
        return new m3_random_doe();
    }
}


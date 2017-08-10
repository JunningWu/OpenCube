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






#ifndef M3_ENV
#define M3_ENV

class m3_env;

#include <m3_doe.h>
#include <m3_rsm.h>
#include <m3_driver.h>
#include <m3_database.h>
#include <m3_optimizer.h>
#include <m3_design_space.h>

using namespace std;

/** This class represent the environment of m3explorer */
class m3_env
{
    private:
    public:
        /** Map containing the shared variables */
        m3_map      shell_variables;

        /** Available databases in memory */
        map<string, m3_database *> available_dbs;

        /** Current doe linked to m3explorer */
        m3_doe   *current_doe;

        /** Current driver linked to m3explorer */
        m3_driver   *current_driver;

        /** Current optimizer algorithm linked to m3explorer */
        m3_optimizer *current_optimizer;

	/** Available RSMs */
	map<string, m3_rsm *> current_rsms;

        /** Design space under analysis */
        m3_design_space *current_design_space;
        
        /** Current database name  */
        string       current_db_name;

        /** Constructor */
        m3_env();

        /** Destructor */
        ~m3_env();

        /** Reports on current database, called directly from the parser */ 
        void db_report();
};

/** The current environment of m3explorer */
extern m3_env current_environment;

#endif

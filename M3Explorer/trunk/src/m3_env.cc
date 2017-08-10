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






#include <m3_env.h>
#include <m3_opt_utils.h>

using namespace std;

m3_env current_environment;

m3_env::m3_env()
{
    current_doe = NULL;
    current_driver = NULL;
    current_optimizer = NULL;

    current_db_name = "root";
    available_dbs[current_db_name]= new m3_database();

    rsm_init_rsms( &current_environment );
}

m3_env::~m3_env()
{
    //delete current_database; DELETE ALL THE DBs
    for(map<string, m3_database *>::iterator db_i=available_dbs.begin();db_i!=available_dbs.end();db_i++)
        delete db_i->second;
    
    delete current_design_space;
}

void m3_env::db_report()
{
    int n = 0;
    int wrong_formatted_points = 0;
    cout << current_db_name<< ": Current database contents\n";
    m3_list *database_list = available_dbs[current_db_name]->get_list_of_points();
    for(list<m3_object *>::iterator i=database_list->begin();i!=database_list->end();i++)
    {
        m3_assert(is_a<m3_point *>(*i));
        m3_point *p = to<m3_point *>(*i);
        n++;
        if( p->check_consistency( this ) )
        {
            cout << current_design_space->get_point_representation(const_cast<m3_env *>(this), const_cast<m3_point &>(*p))<< " : ";
            p->print_point_value(this);
	    }
        else wrong_formatted_points++;
    }
    cout << "Number of points in the DB: " << n << "\n";
    if (wrong_formatted_points>0)
    cout << wrong_formatted_points << " were not consistent with design space format. They have been discarded" << endl;
}



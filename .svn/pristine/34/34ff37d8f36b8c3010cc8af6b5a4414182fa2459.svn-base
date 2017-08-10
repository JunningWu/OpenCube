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
#include <m3_database.h>
#include <m3_parser.h>

bool get_integer_from_variables(m3_env *env, string name, int &number)
{
    m3_object *obj;
    if (!env->shell_variables.get(name.c_str(), obj))
    {
        number = 0;
        return false;
    }
    if (!is_a<m3_integer *>(obj)) return false;
    number = to<m3_integer *>(obj)->get_integer();
    return true;
}

bool get_string_from_variables(m3_env *env, string name, string &str)
{

    m3_object *obj;
    if (!env->shell_variables.get(name.c_str(), obj))
    {
        str = "";
        return false;
    }
    if (!is_a<m3_string *>(obj)) return false;
    str = to<m3_string *>(obj)->get_string();
    return true;
}

bool get_double_from_variables(m3_env *env, string name, double & d)
{

    m3_object *obj;
    if (!env->shell_variables.get(name.c_str(), obj))
    {
        d = 0;
        return false;
    }
    if (!is_a<m3_double *>(obj)) return false;
    d = to<m3_double *>(obj)->get_double();
    return true;
}

bool get_vector_from_variables(m3_env *env, string name, m3_vector * & d)
{

    m3_object *obj;
    if (!env->shell_variables.get(name.c_str(), obj))
    {
        d = NULL;
        return false;
    }
    if (!is_a<m3_vector *>(obj)) 
        return false;
    d = (to<m3_vector *>(obj));
    return true;
}


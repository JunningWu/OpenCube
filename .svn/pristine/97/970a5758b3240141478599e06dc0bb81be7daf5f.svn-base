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
#include <m3_parser.h>
#include <m3_map.h>
#include <m3_list.h>
#include <m3_point.h>
#include <m3_vector.h>

bool dontshowprompt = false;

void prs_display_prompt()
{
    if (!including && !dontshowprompt)
        cout << "m3_shell> ";
}

void prs_dont_show_prompt()
{
    dontshowprompt = true;
}

void prs_display_error(const string & s)
{
    cout << "Error: " << s << endl;
}

void prs_display_message(const string &s)
{   
    //    if (!verbose)
    cout << "Information: " << s << endl;
}

m3_object *prs_add_coord_to_point(m3_object *point, m3_object *c)
{
    m3_assert(is_a<m3_point *>(point));
    m3_assert(is_a<m3_integer *>(c));

    m3_point *p = to<m3_point *>(point);
    m3_integer *i = to<m3_integer *>(c);
    p->resize(p->size() + 1);
    (*p)[p->size()-1] = i->get_integer();
    return p;
}

m3_object *prs_list_concat(m3_object *l1, m3_object *l2)
{
    m3_assert(is_a<m3_list *>(l1));
    m3_assert(is_a<m3_list *>(l2));
    m3_list *list1 = to<m3_list *>(l1);
    m3_list *list2 = to<m3_list *>(l2);
    list1->concat(list2);
    return list1;
}

m3_object *prs_insert_in_list(m3_object *l1, m3_object *o)
{
    m3_assert(is_a<m3_list *>(l1));
    to<m3_list *>(l1)->insert(*o);
    return l1;
}

m3_object *prs_insert_in_vector(m3_object *l1, m3_object *o)
{
    m3_assert(is_a<m3_vector *>(l1));
    int size = to<m3_vector *>(l1)->size();
    if(o != NULL)
    {
                to<m3_vector *>(l1)->insert(size, *o);
                return l1;
    }
    return NULL;
}

m3_object *prs_add_element_to_map(m3_object *m, m3_object *s, m3_object *o)
{
    m3_assert(is_a<m3_map *>(m));
    m3_assert(is_a<m3_string *>(s));
    to<m3_map *>(m)->insert(to<m3_string *>(s)->get_string(), *o);
    return m;
}

m3_object *prs_insert_map_as_property(m3_object *o, m3_object *m)
{
    m3_assert(is_a<m3_map *>(m));
    to<m3_map *>(m)->fill_properties_of(o);
    return o;
}



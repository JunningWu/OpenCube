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





 
#include <m3_list.h>

void m3_list::copy(m3_list *from, m3_list *to) 
{
    list<m3_object *>::const_iterator i;
    // ADD cancellare gli elementi precedenti nella lista...
    for(i=from->the_list.begin(); i!=from->the_list.end(); i++)
    {
        to->the_list.push_back((*i)->gen_copy());
    }
}

m3_list::m3_list() { obj_create(num_obj)}

m3_list::m3_list(m3_list &l): m3_object(l)
{       
    obj_create(num_obj);    
    copy(&const_cast<m3_list&>(l),this);    
}

m3_object *m3_list::gen_copy() 
{
    m3_list *nl=new m3_list(*this);
    return nl;  
}

m3_list::~m3_list() 
{  
    list<m3_object *>::const_iterator i;
    for(i=the_list.begin(); i!=the_list.end(); i++)
    delete (*i);
}

void m3_list::remove(m3_object *p)
{
    delete p;
    the_list.remove(p);
}

void m3_list::erase(list<m3_object *>::iterator& i)
{
    delete (*i);
    the_list.erase(i);
}

void m3_list::insert(m3_object &o) { 
    the_list.push_back(const_cast<m3_object&>(o).gen_copy());
}

string m3_list::print() {   list<m3_object *>::const_iterator i;
    string s="{ ";
    for(i=the_list.begin(); i!=the_list.end(); i++) 
    {
        s=s+(*i)->print()+" ";
    }
    s=s+"}";
    return s;
}

list<m3_object *>::iterator m3_list::begin() 
{
    return the_list.begin();
}

list<m3_object *>::iterator m3_list::end() 
{
    return the_list.end();
}

m3_object *m3_list::back()
{
    return the_list.back();
}

bool m3_list::empty() 
{
    return the_list.empty();
}

size_t m3_list::size()
{
    return the_list.size();
}

void m3_list::concat(m3_list *l)
{
    list<m3_object *>::const_iterator it;
    for(it=l->the_list.begin();it!=l->the_list.end();it++)
    {
        the_list.insert(the_list.end(),(*it)->gen_copy());          
    }
}

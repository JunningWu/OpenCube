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






#include <m3_vector.h>
#include <stdexcept>

void m3_vector::copy(m3_vector *from, m3_vector *to) 
{
	int i;
	
	for(i=0; i<to->the_vector.size(); i++)
	{
		if(to->the_vector[i]) { delete (to->the_vector[i]); to->the_vector[i]=NULL;}
	}
	to->the_vector.resize(from->the_vector.size(),NULL);
	for(i=0; i<from->the_vector.size(); i++)
	{
		if(from->the_vector[i]) to->the_vector[i]=from->the_vector[i]->gen_copy();
	}
}

m3_vector::m3_vector() { obj_create(num_obj)};

m3_vector::m3_vector(m3_vector &l): m3_object(l) { obj_create(num_obj) copy(&l, this); }

m3_vector::m3_vector(m3_object **v, int n) 
{
	obj_create(num_obj);
	for(int i=0; i<n; i++)
	{
		insert(i,*(v[i]));
	}	
}

m3_vector::~m3_vector()
{
	vector<m3_object *>::iterator i;
	for(i=the_vector.begin(); i!=the_vector.end(); i++)
	{
		if(*i) delete (*i);	
	}
}

size_t m3_vector::size() 
{
	return the_vector.size();
}

void   m3_vector::insert(int n, m3_object &o) {
	if(n>=the_vector.size()) the_vector.resize(n+1,NULL);
	the_vector[n]=o.gen_copy();	
}

#include <m3_exception.h>
#include <m3_list.h>

vector<string> get_string_vector(m3_object *v)
{
    if(is_a<m3_vector *>(v))
    {
        vector<m3_object *> & the_vector = to<m3_vector *>(v)->the_vector;
        vector<string> ret;
        vector<m3_object *>::const_iterator i;
        for(i=the_vector.begin(); i!=the_vector.end(); i++)
        {
            if(!is_a<m3_string *>(*i))
                throw m3_exception("cannot convert into a string vector");
            ret.push_back(to<m3_string *>(*i)->get_string());
        }
        return ret;
    }
    else if(is_a<m3_list *>(v))
    {
        list<m3_object *> & the_vector = to<m3_list *>(v)->the_list;
        vector<string> ret;
        list<m3_object *>::const_iterator i;
        for(i=the_vector.begin(); i!=the_vector.end(); i++)
        {
            if(!is_a<m3_string *>(*i))
                throw m3_exception("cannot convert into a string vector");
            ret.push_back(to<m3_string *>(*i)->get_string());
        }
        return ret;
    }
    else
    {
        throw m3_exception("cannot convert into a string vector");
    }
}

vector<double> get_double_vector(m3_object *v)
{
    m3_assert(is_a<m3_vector *>(v));
    vector<m3_object *> & the_vector = to<m3_vector *>(v)->the_vector;

    vector<double> ret;
	vector<m3_object *>::const_iterator i;
	for(i=the_vector.begin(); i!=the_vector.end(); i++)
	{
        if(!is_a<m3_double *>(*i))
            throw m3_exception("cannot convert into a double vector");
        ret.push_back(to<m3_double *>(*i)->get_double());
	}
	return ret;

}

string m3_vector::print() 
{
	vector<m3_object *>::const_iterator i;
	string s="[ ";
	for(i=the_vector.begin(); i!=the_vector.end(); i++)
	{
		if(*i) s=s+(*i)->print()+" ";
		else s=s+" / ";
	}
	s=s+"]";
	return s;
}


m3_object & m3_vector::get(int n) 
{
	return (*the_vector[n]);
}

m3_object *m3_vector::gen_copy() 
{
	m3_vector *v=new m3_vector(*this);
	return v;
}

string m3_vector::get_string_at(int index) 
{
    if(the_vector.size()<index)
        throw std::logic_error("Index is out of bounds");

    if(!is_a<m3_string *>(the_vector[index]))
        throw std::logic_error("Get string failed");

    return to<m3_string *>(the_vector[index])->get_string();
}



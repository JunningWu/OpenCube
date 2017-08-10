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





 
#include <m3_map.h>

void m3_map::copy(m3_map *from, m3_map *to) 
{
	map<string, m3_object *>::iterator i;
	
	for(i=to->the_map.begin(); i!=to->the_map.end(); i++)
	{
		if((*i).second) { delete ((*i).second);}
	}
	to->the_map.erase(to->the_map.begin(),to->the_map.end());
	map<string, m3_object *>::const_iterator j;	
	for(j=from->the_map.begin(); j!=from->the_map.end(); j++)
	{
		pair<string, m3_object *> p(j->first, j->second->gen_copy());
		to->the_map.insert(p); 	
	}
}

m3_map::m3_map() { obj_create(num_obj)};

m3_map::m3_map(m3_map &l): m3_object(l) { obj_create(num_obj) copy(&l, this); }

m3_map::~m3_map()
{
	map<string, m3_object *>::iterator i;
	for(i=the_map.begin(); i!=the_map.end(); i++)
	{
		if(i->second) delete (i->second);	
	}
}

void   m3_map::insert(string n, m3_object &o) {
	
	map<string, m3_object *>::iterator i;

	if((i=the_map.find(n))!=the_map.end()) {
		delete i->second;
	}
	the_map.erase(n);
	the_map.insert(map<string, m3_object*>::value_type (n, o.gen_copy()));
}

string m3_map::print() 
{
	map<string, m3_object *>::const_iterator i;
	string s="( ";
	for(i=the_map.begin(); i!=the_map.end(); i++)
	{
		s=s+i->first+"="+i->second->print()+" ";
	}
	s=s+")";
	return s;
}

void m3_map::fill_properties_of(m3_object *o) 
{
	map<string, m3_object *>::const_iterator i;
	for(i=the_map.begin(); i!=the_map.end(); i++)
	{
		o->set_properties(i->first,*i->second);
	}
}

bool m3_map::get(string n, m3_object * &p) 
{
	map<string, m3_object *>::const_iterator j;
	if(!the_map.count(n)) return false;
	j=the_map.find(n);
	p=(j->second);
	return true;
}

m3_object *m3_map::gen_copy() 
{
	m3_map *v=new m3_map(*this);
	return v;
}



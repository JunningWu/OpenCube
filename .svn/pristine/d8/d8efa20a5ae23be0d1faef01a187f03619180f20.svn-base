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






#include <m3_object.h>
#include <m3_map.h>
#include <m3_parser.h>
#include <m3_object_utils.h>
#include <cstdio>

int num_obj;


m3_object::m3_object()
{
//  cout << "debug: base object created, " << this << endl;
}

m3_object::m3_object(m3_object &o)
{
    map<string,m3_object *>::const_iterator i;
    for(i=o.properties.begin(); i!=o.properties.end(); i++)
    {
        properties[i->first]=i->second->gen_copy();
    }
}

/*bool  m3_object::write_properties(bin_db *db) 
{
    bool error=false;
    map<string,m3_object *>::const_iterator i;
    db->write_int(properties.size());
    for(i=properties.begin(); i!=properties.end(); i++)
    {
        db->write_string(i->first);
        error = error | obj_write_object_on_disk(i->second,db); 
    }
    return error;
}*/

/*bool   m3_object::read_properties(bin_db *db)
{
    bool error=false;
    int size=db->read_int();
    for(int i=0; i<size; i++)
    {
        m3_object *r;
        string key=db->read_string();
        error = error | obj_read_object_on_disk(r,db);
        set_properties(key,*r);
        delete r;
    }
    return error;
}*/

m3_object *m3_object::get_properties(string s) 
{ 
    if(!properties.count(s)) 
    {
        return NULL;
    } 
    else 
    { 
        return properties[s];
    }
}

void  m3_object::set_properties(string s, m3_object &o) 
{ 
    if(properties.count(s)) delete properties[s]; 
    properties[s]=o.gen_copy();
}

m3_object::~m3_object()
{
    obj_destroy_n(num_obj,this) 
    map<string,m3_object *>::iterator i;
    for(i=properties.begin(); i!=properties.end(); i++) delete ((*i).second); 
}

string m3_object::full_print() 
{
    string s="_obj_repr_#";
    s=s+print()+"#";
    m3_map *m=new m3_map();
    map<string,m3_object *>::const_iterator i;
    for(i=properties.begin(); i!=properties.end(); i++)
    {
        m->insert(i->first,*(i->second));
    }
    s=s+m->print()+"#";
    delete m;
    return s;
}

m3_string::m3_string(const char *s): the_string(s)
{
    obj_create(num_obj);
}

m3_string::m3_string(const string & s): the_string(s) 
{
    obj_create(num_obj);
}

m3_string::m3_string(m3_string &ss): the_string(ss.the_string), m3_object(ss) 
{
    obj_create(num_obj);    
}   
m3_string::m3_string(const m3_string &ss): the_string(ss.the_string), m3_object(const_cast<m3_string &>(ss)) 
{
    obj_create(num_obj);    
}   


m3_object *m3_string::gen_copy() 
{ 
    return (new m3_string(*this));
}

string m3_string::print() 
{ 
    return "\""+the_string+"\"";
}

string & m3_string::get_string() { return the_string;}

m3_string::~m3_string() {
//  cout << "Deleting string '" << the_string << "'\n";
}

m3_integer::m3_integer(int i): the_int(i) 
{
    obj_create(num_obj);
}

m3_integer::m3_integer(m3_integer &i): m3_object(i), the_int(i.the_int) {obj_create(num_obj)}

int & m3_integer::get_integer() { return the_int;}

m3_integer::~m3_integer() 
{
}

string m3_integer::print() 
{
    char buf[32];
    sprintf(buf,"%d",the_int);
    return string(buf);
}

m3_object *m3_integer::gen_copy() { return new m3_integer(*this); }


m3_double::m3_double(double i): the_double(i) 
{
    obj_create(num_obj);
}

m3_double::m3_double(m3_double &d): m3_object(d), the_double(d.the_double) {obj_create(num_obj)}

double & m3_double::get_double() { return the_double;}

m3_double::~m3_double() {}

string m3_double::print() 
{
            char buf[32];
            sprintf(buf,"%f",the_double);
            return string(buf);
}

m3_object *m3_double::gen_copy() { return new m3_double(*this); }

#include <m3_exception.h>

int obj_get_integer(m3_object *o)
{
    if(!is_a<m3_integer *>(o))
        throw m3_exception("Cant convert to integer");
    else
        return to<m3_integer *>(o)->get_integer();
}

double obj_get_double(m3_object *o)
{
    if(!is_a<m3_double *>(o))
        throw m3_exception("Cant convert to double");
    else
        return to<m3_double *>(o)->get_double();
}

string obj_get_string(m3_object *o)
{
    if(!is_a<m3_string *>(o))
        throw m3_exception("Cant convert to string");
    return to<m3_string *>(o)->get_string();
}

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





#ifndef M3_OBJECTS
#define M3_OBJECTS

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <m3_object_utils.h>
#include <typeinfo>

using namespace std;

/** This variable keeps track of the number of objects created in the shell */
extern int num_obj;

/** 
* This class is the root of all the objects created within the shell.
* All objects support polymorphic trees through gen_copy and ~ methods
* and normal containers (in the future) through default (& copy) constructors and ~ methods
*/
class m3_object
{

    protected:
    /** List of properties associated to the object. 
     * Each property has a name and a value (which is another object). */
	map<string,m3_object *> properties;
	
	public:
    /** Constructor */
    m3_object();

    /** Copy constructor */
	m3_object(m3_object &);	

    /** Destructor */
	virtual 		~m3_object();  

    /** Property 'get' accessor.
     * @param s property name
     * @return the value associated to the property
     */    
	m3_object       *get_properties(string s);  
    
    /** Property 'set' accessor.
     * @param s property name
     * @param o property value to be set. 
     */    
	void    		set_properties(string s, m3_object &o); 

    /** 
     * Print the content of the object (included properties)
     * @return the string representation of the object
     */
	string			full_print() ;


    /** 
     * Print the content of the object 
     * @return the string representation of the object
     */
	virtual string 		print() =0; 
   
    /** 
     * Generates a copy of the object 
     * @return a copy of the object
     */
	virtual m3_object *	gen_copy() =0; 

};

/** The string class derived from object */
class m3_string: public m3_object
{
	string the_string;
	public:
	m3_string(const char *);
	m3_string(const string & s);
	m3_string(m3_string &ss);	
	m3_string(const m3_string &ss);	
	~m3_string(); 
	string print() ; 
	string & get_string() ;
	protected:	
	m3_object *gen_copy() ;
};

/** The integer class derived from object */
class m3_integer: public m3_object
{
	int the_int;
	public:
    /** Constructor */
	m3_integer(int );
    /** Copy constructor */
	m3_integer(m3_integer &);
    /** Destructor */
	~m3_integer();
	string			print() ;
	int & get_integer() ;
	protected:	
	m3_object *		gen_copy() ;
};

/** The double class derived from object */
class m3_double: public m3_object
{
	double the_double;
	public:
  /** Constructor */
	m3_double(double );
  /** Copy constructor */
	m3_double(m3_double &);
  /** Destructor */
	~m3_double();
	string print() ;
  /** 
   * Return the content of the variable
   * @return the content of the variable
   */
	double & get_double() ;
	protected:
	m3_object *             gen_copy() ;
};

/** Casts an object to an integer. Generates an exception if the cast is not possible.
 * @param o the object 
 * @return the casted object
 */
extern int obj_get_integer(m3_object *o);

/** Casts an object to a double. Generates an exception if the cast is not possible.
 * @param o the object 
 * @return the casted object
 */
extern double obj_get_double(m3_object *o);

/** Casts an object to a string. Generates an exception if the cast is not possible.
 * @param o the object 
 * @return the casted object
 */
extern string obj_get_string(m3_object *o);

/**
 * Templated function. True if the object is of the specified type
 * @param p the object to be tested
 * @return true if the object is of a particular kind.
 */
template<class T> inline bool is_a(m3_object *p)
{
	if(!dynamic_cast<T>(p)) return false;
	return true;
}

/**
 * Templated function. Converts the object to the specified type
 * @param p the object to be tested
 * @return true if the object is of a particular kind.
 */
template<class T> inline T to(m3_object *p)
{
	return dynamic_cast<T>(p);
}

#endif

/* @M3EXPLORER_LICENSE_START@
 *
 * This file is part of the Multicube Explorer tool.
 *
 * Authors: Vittorio Zaccaria, Gianluca Palermo, Giovanni Mariani, Fabrizio Castro, Alessandro Sivieri (2008)
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

/* @additional_authors @, Alessandro Sivieri (2008)@ */

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <map>
#include <m3_shell_variables.h>
#include <m3_parser.h>
#include <m3_sim_utils.h>
#include <m3_driver.h>
#include <m3_vector.h>
#include <m3_map.h>
#include <m3_env.h>
#include "libm3_xml_driver.h"
/*
 * libxml is a C library, so we have to do this little hack
 * to make it work here, under C++
*/
extern "C"
{
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xmlschemas.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>
}

using namespace std;

/// class m3_xml_io ///

xmlXPathObjectPtr m3_xml_io::get_nodeset(xmlDocPtr doc, xmlChar *xpath)
{
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);
	if (context == NULL)
	{
		return NULL;
	}
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL)
	{
		return NULL;
	}
	if(xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
	    xmlXPathFreeObject(result);
		return NULL;
	}
	return result;
}

bool m3_xml_version_is_1_3()
{
    int is_1_3 = 0;
    get_integer_from_variables(&current_environment, "backward_compatible_1_3", is_1_3);
    if(is_1_3)
        return true;
    else
        return false;
}

string m3_xml_get_version()
{
    if(m3_xml_version_is_1_3())
        return "1.3";
    else
        return "1.4";
}

bool m3_xml_check_version(string doc_version)
{
    bool is_1_3 = (doc_version.compare("1.3") == 0);
    bool is_1_4 = (doc_version.compare("1.4") == 0);
    if(is_1_3 || (is_1_4 && !m3_xml_version_is_1_3()))
        return true;
    else
        return false;
}

/// class m3_xml_rule ///

m3_xml_rule::m3_xml_rule(xmlNodePtr rule): rule(rule)
{
	
}

m3_xml_rule::~m3_xml_rule()
{
	xmlFreeNodeList(this->rule);
}

xmlNodePtr m3_xml_rule::get_rule()
{
	return this->rule;
}

void m3_xml_rule::set_rule(xmlNodePtr rule)
{
	xmlFreeNodeList(this->rule);
	this->rule=rule;
}

bool m3_xml_rule::validate(m3_point& point) throw(m3_xml_exception)
{
	xmlNode *cur_node;
	m3_object *result;
	
	// this little routine jumps all text nodes (that we have to ignore)
	// and finds out the only element node contained in a <rule/> tag
	for (cur_node = (this->rule)->children; cur_node; cur_node = cur_node->next)
	{
		if(cur_node->type==XML_ELEMENT_NODE)
		{
			result=this->parse(cur_node, point);
			break;
		}
	}
	if(result==NULL) return false; // never happens, exception thrown
	if(!is_a<m3_integer *>(result) || !to<m3_integer *>(result)->get_integer())
	{
		delete result;
		return false;
	}
	
	delete result;
	return true;
}

/*
 * Please notice:
 * a) xmlChar is a simple char, so casts between the two are correct.
 * b) we DO NOT check every combination of objects, for example for 'equal'
 * tag we see only int-int and string-string, for 'expr' tag we check only
 * int-int, and that is because of a pre-checking of correctness of tags into
 * the XSD validation, and because in the last lines if result==NULL we throw
 * an exception; this produces two consequences: it works as a big ELSE condition
 * for everything not checked before, and we do not need to look for thing!=NULL,
 * because this function does not return NULL, it throws the exception.
 * c) libxml, like most XML libraries, considers even the \n, \r, \t as valid
 * nodes (of type text), but our driver completely ignores this kind of tag
 * because our specification does not contain any of them: please check this as
 * a possible source of bugs (even if now it should be correctly handled, when
 * we look for XML_ELEMENT_NODE), and pay attention if some valid text nodes
 * will be added to the specification for our input/output files.
 */
m3_object *m3_xml_rule::parse(xmlNodePtr& node, m3_point& point) throw(m3_xml_exception)
{
	string name=string((char *) node->name);
	xmlNodePtr *children;
	xmlNode *current=NULL, *if_current=NULL;
	xmlChar *attr;
	m3_object *obj1=NULL, *obj2=NULL, *result=NULL;
	ostringstream temp;
	string *pname;
	int i, j, zero;
	
	// getting children count & elements
	for(i=0, current=node->children; current; current=current->next)
	{
		if(current->type==XML_ELEMENT_NODE)
		{
			++i;
		}	
	}
	children=new xmlNodePtr[i];
	for(i=0, current=node->children; current; current=current->next)
	{
		if(current->type==XML_ELEMENT_NODE)
		{
			children[i]=current;
			++i;
		}	
	}
	// switch
	if(name.compare("and")==0)
	{
		zero=1; // true: neutral element for boolean 'and'
		for(j=0; j<i; ++j)
		{
			obj1=parse(children[j], point);
			if(is_a<m3_integer *>(obj1))
			{
				zero=zero && to<m3_integer *>(obj1)->get_integer();
			}
			else
			{
				/* pay attention: normally we do not do this 'else' part,
				 * because if the is_a operator fails, result remains NULL,
				 * thus the exception is thrown.
				 * Here things are different: if is_a fails, this invocation
				 * returns correctly because result is set anyway; so we
				 * are forcing things to return false.
				 */
				prs_display_error("You have written a parameter or a constant with a string value in a boolean tag condition. Rule returns false.");
				zero=0;
				break;
			}
			delete obj1;
			obj1=NULL;
			// if we have a logic 'and' with one of the terms false,
			// we can safely return it without further processing
			if(!zero)
			{
				break;
			}
		}
		result=new m3_integer(zero);
	}
	else if(name.compare("or")==0)
	{
		zero=0; // false: neutral element for boolean 'or'
		for(j=0; j<i; ++j)
		{
			obj1=parse(children[j], point);
			if(is_a<m3_integer *>(obj1))
			{
				zero=zero || to<m3_integer *>(obj1)->get_integer();
			}
			else
			{
				/* pay attention: normally we do not do this 'else' part,
				 * because if the is_a operator fails, result remains NULL,
				 * thus the exception is thrown.
				 * Here things are different: if is_a fails, this invocation
				 * returns correctly because result is set anyway; so we
				 * are forcing things to return false.
				 */
				prs_display_error("You have written a parameter or a constant with a string value in a boolean tag condition. Rule returns false.");
				zero=0;
				break;
			}
			delete obj1;
			obj1=NULL;
			// if we have a logic 'or' with one of the terms true,
			// we can safely return it without further processing
			if(zero)
			{
				break;
			}
		}
		result=new m3_integer(zero);
	}
	else if(name.compare("not")==0)
	{
		obj1=parse(children[0], point); // only one child
		if(is_a<m3_integer *>(obj1))
		{
			result=new m3_integer(!to<m3_integer *>(obj1)->get_integer());
		}
	}
	else if(name.compare("equal")==0)
	{
		obj1=parse(children[0], point);
		obj2=parse(children[1], point);
		if(is_a<m3_integer *>(obj1) && is_a<m3_integer *>(obj2))
		{
			result=new m3_integer(to<m3_integer *>(obj1)->get_integer() == to<m3_integer *>(obj2)->get_integer());
		}
		else if(is_a<m3_string *>(obj1) && is_a<m3_string *>(obj2))
		{
			result=new m3_integer(to<m3_string *>(obj1)->get_string().compare(to<m3_string *>(obj2)->get_string())==0);
		} // else ditto
	}
	else if(name.compare("not-equal")==0)
	{
		obj1=parse(children[0], point);
		obj2=parse(children[1], point);
		if(is_a<m3_integer *>(obj1) && is_a<m3_integer *>(obj2))
		{
			result=new m3_integer(to<m3_integer *>(obj1)->get_integer() != to<m3_integer *>(obj2)->get_integer());
		}
		else if(is_a<m3_string *>(obj1) && is_a<m3_string *>(obj2))
		{
			result=new m3_integer(to<m3_string *>(obj1)->get_string().compare(to<m3_string *>(obj2)->get_string())!=0);
		}
	}
	else if(name.compare("less")==0)
	{
		obj1=parse(children[0], point);
		obj2=parse(children[1], point);
		if(is_a<m3_integer *>(obj1) && is_a<m3_integer *>(obj2))
		{
			result=new m3_integer(to<m3_integer *>(obj1)->get_integer() < to<m3_integer *>(obj2)->get_integer());
		}
	}
	else if(name.compare("less-equal")==0)
	{
		obj1=parse(children[0], point);
		obj2=parse(children[1], point);
		if(is_a<m3_integer *>(obj1) && is_a<m3_integer *>(obj2))
		{
			result=new m3_integer(to<m3_integer *>(obj1)->get_integer() <= to<m3_integer *>(obj2)->get_integer());
		}
	}
	else if(name.compare("greater")==0)
	{
		obj1=parse(children[0], point);
		obj2=parse(children[1], point);
		if(is_a<m3_integer *>(obj1) && is_a<m3_integer *>(obj2))
		{
			result=new m3_integer(to<m3_integer *>(obj1)->get_integer() > to<m3_integer *>(obj2)->get_integer());
		}
	}
	else if(name.compare("greater-equal")==0)
	{
		obj1=parse(children[0], point);
		obj2=parse(children[1], point);
		if(is_a<m3_integer *>(obj1) && is_a<m3_integer *>(obj2))
		{
			result=new m3_integer(to<m3_integer *>(obj1)->get_integer() >= to<m3_integer *>(obj2)->get_integer());
		}
	}
	else if(name.compare("expr")==0)
	{
		obj1=parse(children[0], point);
		obj2=parse(children[1], point);
		// getting operator and executing it
		xmlChar *tmp = xmlGetProp(node, (xmlChar *) "operator");
        string op = string((char *) tmp);
		xmlFree(tmp);
		if(is_a<m3_integer *>(obj1) && is_a<m3_integer *>(obj2))
		{
			if(op.compare("+")==0)
			{
				result=new m3_integer(to<m3_integer *>(obj1)->get_integer() + to<m3_integer *>(obj2)->get_integer());
			}
			else if(op.compare("-")==0)
			{
				result=new m3_integer(to<m3_integer *>(obj1)->get_integer() - to<m3_integer *>(obj2)->get_integer());
			}
			else if(op.compare("*")==0)
			{
				result=new m3_integer(to<m3_integer *>(obj1)->get_integer() * to<m3_integer *>(obj2)->get_integer());
			}
			else if(op.compare("/")==0)
			{
				result=new m3_integer(to<m3_integer *>(obj1)->get_integer() / to<m3_integer *>(obj2)->get_integer());
			}
		}
	}
	else if(name.compare("parameter")==0)
	{
		attr=xmlGetProp(node, (xmlChar *) "name");
		pname=new string((char *) attr);
		if(!sim_is_a_parameter(&current_environment, *pname))
		{
			temp << "Parameter " << pname << " in rule has not been declared.";
			throw m3_xml_exception(temp.str());
		}
		string str_value;
        str_value = current_environment.current_design_space->get_parameter_representation(&current_environment, (point), *pname);
		if(is_int(str_value))
		{
			result=new m3_integer(atoi(str_value.c_str()));
		}
		else
		{
			result=new m3_string(str_value);
		}
		delete pname;
		xmlFree(attr);
	}
	else if(name.compare("constant")==0)
	{
		attr=xmlGetProp(node, (xmlChar *) "value");
		pname=new string((char *) attr);
		if(is_int(*pname))
		{
			result=new m3_integer(atoi(pname->c_str()));
		}
		else // conversion error, constant should be a string
		{
			result=new m3_string(*pname);
		}
		delete pname;
		xmlFree(attr);
	}
	else if(name.compare("if")==0)
	{
		obj1=parse(children[0], point);
		if(is_a<m3_integer *>(obj1))
		{
			if(to<m3_integer *>(obj1)->get_integer()) // 'then' part
			{
				for(if_current=children[1]->children; if_current; if_current=if_current->next)
				{
					if(if_current->type==XML_ELEMENT_NODE)
					{
						// always executed: 'then' has just one tag by spec
						obj2=parse(if_current, point);
						break;
					}	
				}
			}
			else if(i>2) // if there is a third child, it must be 'else' (XSD)
			{
				for(if_current=children[2]->children; if_current; if_current=if_current->next)
				{
					if(if_current->type==XML_ELEMENT_NODE)
					{
						// always executed: 'else' has just one tag by spec
						obj2=parse(if_current, point);
						break;
					}	
				}
			}
			else // there is not a third child, we are lacking 'else'...
			{
				result=new m3_integer(1);
			}
			if(is_a<m3_integer *>(obj2))
			{
				result=new m3_integer(to<m3_integer *>(obj2)->get_integer());
			}
		}
	}
	
	// destroy any temporary parameter and return
	if(obj1!=NULL) delete obj1;
	if(obj2!=NULL) delete obj2;
	delete[] children;
	// this should not be NULL...
	if(result==NULL)
	{
		temp << "For tag " << name << " the result value is NULL!";
		throw m3_xml_exception(temp.str());
	}
	return result;
}

/// class m3_xml_validator ///

const string m3_xml_validator::design_space_schema="/schemas/multicube_design_space_def.xsd";
const string m3_xml_validator::simulator_schema="/schemas/multicube_simulator_interface.xsd";
m3_xml_validator *m3_xml_validator::design_space_validator=NULL;
m3_xml_validator *m3_xml_validator::simulator_validator=NULL;

bool m3_xml_validator::validate(xmlDocPtr doc)
{
	if(xmlSchemaValidateDoc(schema_valid_run, doc)==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

m3_xml_validator *m3_xml_validator::get_design_space_validator() throw(m3_xml_exception)
{
	if(design_space_validator==NULL)
	{
		design_space_validator=new m3_xml_validator(m3_xml_validator::design_space_schema);
	}
	
	return design_space_validator;
}

m3_xml_validator *m3_xml_validator::get_simulator_validator() throw(m3_xml_exception)
{
	if(simulator_validator==NULL)
	{
		simulator_validator=new m3_xml_validator(m3_xml_validator::simulator_schema);
	}
	
	return simulator_validator;
}

void m3_xml_validator::shutdown()
{
	if(simulator_validator!=NULL)
	{
		delete simulator_validator;
		simulator_validator=NULL;
	}
	if(design_space_validator!=NULL)
	{
		delete design_space_validator;
		design_space_validator=NULL;
	}
}

m3_xml_validator::~m3_xml_validator()
{
	xmlSchemaFreeValidCtxt(schema_valid_run);
	xmlSchemaFree(schema_run);
	xmlSchemaFreeParserCtxt(schema_context_run);
}

m3_xml_validator::m3_xml_validator(string filename) throw(m3_xml_exception): current_schema(filename)
{
	ostringstream temp;
	m3_object *obj;
	
	// here we create everything, only if this is our first invocation
	current_environment.shell_variables.get("current_build_path", obj);
	temp << to<m3_string *>(obj)->get_string() << current_schema;
    //cout << "Using the following build path " << to<m3_string *>(obj)->get_string() << endl;
	schema_context_run=xmlSchemaNewParserCtxt(temp.str().c_str());
	if(schema_context_run==NULL)
	{
		throw m3_xml_exception("Unable to load schema parser context for validator");
	}
	xmlSchemaSetParserErrors(schema_context_run, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema_run=xmlSchemaParse(schema_context_run);
	if(schema_run==NULL)
	{
		xmlSchemaFreeParserCtxt(schema_context_run);
		throw m3_xml_exception("Unable to load schema for validator");
	}
	schema_valid_run=xmlSchemaNewValidCtxt(schema_run);
	if(schema_valid_run==NULL)
	{
		xmlSchemaFree(schema_run);
		xmlSchemaFreeParserCtxt(schema_context_run);
		throw m3_xml_exception("Unable to load schema valid context for validator");
	}
	xmlSchemaSetValidErrors(schema_valid_run, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
}

/// class m3_xml_design_space ///

m3_xml_design_space::m3_xml_design_space()
{
	
}

m3_xml_design_space::~m3_xml_design_space()
{
	
}

void m3_xml_design_space::write(string filename) throw(m3_xml_exception)
{
	throw m3_xml_exception("Unsupported operation.");
}

xmlDocPtr m3_xml_design_space::read(string filename) throw(m3_xml_exception)
{
	int i;
	xmlDocPtr doc;
	xmlChar *xpath;
	xmlXPathObjectPtr xpath_set;
	xmlNodeSetPtr nodeset;
	string input, schema, bpath;
	ostringstream temp;
	m3_object *obj;
    
    // loading XML input and validating against the specified schema
    doc=xmlParseFile(filename.c_str());
    if(doc==NULL)
    {
    	throw m3_xml_exception("Unable to open the given input file.");
    }
    if(!m3_xml_validator::get_design_space_validator()->validate(doc))
    {
    	throw m3_xml_exception("Error during validation.");
    }
    
    // version check
	xmlChar *value = xmlGetProp(doc->children, (xmlChar *) "version");
    string docversion=string((char *) value);
	xmlFree(value);

    if(!m3_xml_check_version(docversion))
    {
    	temp << "Document version " << docversion << "does not match the actual one: " << m3_xml_get_version();
    	throw m3_xml_exception(temp.str());
    }
	
	return doc;
}

void m3_xml_design_space::fill(m3_design_space *design_space) throw(m3_xml_exception)
{
	int i;
	xmlChar* xpath;
	xmlXPathObjectPtr xpath_set;
	xmlNodeSetPtr nodeset;
	string input, schema, bpath;
	ostringstream temp;
	m3_object *obj;
	
	xpath=(xmlChar *) "/*[local-name()='design_space']/*[local-name()='parameters']/*[local-name()='parameter']";
    xpath_set=this->get_nodeset(doc, xpath);
    if(xpath_set==NULL)
    {
    	throw m3_xml_exception("Cannot load parameters.");
    }
    nodeset=xpath_set->nodesetval;
    for(i=0; i<nodeset->nodeNr; ++i)
    {
    	this->add_parameter(design_space, &current_environment, doc, nodeset->nodeTab[i]);
    }
    xmlXPathFreeObject(xpath_set);
    
    // loading metrics
	xpath=(xmlChar *) "/*[local-name()='design_space']/*[local-name()='system_metrics']/*[local-name()='system_metric']";
    xpath_set=this->get_nodeset(doc, xpath);
    if(xpath_set==NULL)
    {
    	throw m3_xml_exception("Cannot load metrics.");
    }
    nodeset=xpath_set->nodesetval;
    for(i=0; i<nodeset->nodeNr; ++i)
    {
		xmlChar *tmp = xmlGetProp(nodeset->nodeTab[i], (xmlChar *) "name");
        string tmp_str1 = string((char *) tmp);
		xmlFree(tmp);
		tmp = xmlGetProp(nodeset->nodeTab[i], (xmlChar *) "unit");
        string tmp_str2 = string((char *) tmp);
		xmlFree(tmp);
    	design_space->insert_metric(&current_environment, 
                tmp_str1,
                tmp_str2
//              ,string((char *) xmlGetProp(nodeset->nodeTab[i], (xmlChar *) "desired"))
		);
    }
    xmlXPathFreeObject(xpath_set);
	
    // loading future simulator launch configurations
	xpath=(xmlChar *) "/*[local-name()='design_space']/*[local-name()='simulator']/*[local-name()='simulator_executable']";
    xpath_set=this->get_nodeset(doc, xpath);
    if(xpath_set==NULL)
    {
    	throw m3_xml_exception("Cannot load simulation configurations.");
    }
    nodeset=xpath_set->nodesetval;
    for(i=0; i<nodeset->nodeNr; ++i)
    {
		if(i==0)
		{
			xmlChar *tmp = xmlGetProp(nodeset->nodeTab[i], (xmlChar *) "path");
    	    this->simulator = string((char *) tmp);
			xmlFree(tmp);
		}
		else
		{
			prs_display_message("WARNING: Design space description contains more than one simulator; the first one will be used.");
		}
    }
    xmlXPathFreeObject(xpath_set);
	
    // loading rules
	xpath=(xmlChar *) "/*[local-name()='design_space']/*[local-name()='rules']/*[local-name()='rule']";
    xpath_set=this->get_nodeset(doc, xpath);
    if(xpath_set!=NULL)
    {
    	// rules may be absent (at least for debug purposes)
    	nodeset=xpath_set->nodesetval;
	    for(i=0; i<nodeset->nodeNr; ++i)
	    {
			// we save a copy of each <rule /> subtree
	    	this->rules.push_back(new m3_xml_rule(xmlCopyNodeList(nodeset->nodeTab[i])));
	    }
	    xmlXPathFreeObject(xpath_set);
    }
	
    // closing XML input and leave
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

vector<m3_xml_rule *> m3_xml_design_space::get_rules()
{
	return this->rules;
}

string m3_xml_design_space::get_simulator()
{
	return this->simulator;
}

void m3_xml_design_space::add_parameter(m3_design_space *design_space, m3_env *env, xmlDocPtr doc, xmlNode *node)
{
	xmlChar *value;
	ostringstream temp;
	vector<string> el_list;
	int max, min, step=1, i, j;
	xmlXPathObjectPtr xpath_set;
	xmlNodeSetPtr nodeset;
	
	value=xmlGetProp(node, (xmlChar *) "type");

	if(xmlStrEqual(value, (xmlChar *) "integer"))
	{

        bool has_multiple_instances=false;
        int instances=1;
        // optional 'instances' parameter
		xmlFree(value);
		value=xmlGetProp(node, (xmlChar *) "instances");
		if(value!=NULL)
		{
            if(m3_xml_version_is_1_3())
            {
                prs_display_error("Cannot use multiple instances because backward compatibility is set");
            }
            else
            {
                has_multiple_instances=true;
                instances=atoi((char *) value);
                if(instances <= 1)
                {
                    temp << "Parameter " << node->name << " instances invalid. Assuming 1 instance.";
                    prs_display_error(temp.str());
                    has_multiple_instances=false;
                }
            }
		}

		// parameter 'min' (mandatory)
		xmlFree(value);
		value=xmlGetProp(node, (xmlChar *) "min");
		if(value==NULL)
		{
			temp << "Parameter " << node->name << " has no attribute 'min'.";
			prs_display_error(temp.str());
			return;
		}
		min=atoi((char *) value);
		// parameter 'max' (mandatory)
		xmlFree(value);
		value=xmlGetProp(node, (xmlChar *) "max");
		if(value==NULL)
		{
			temp << "Parameter " << node->name << " has no attribute 'max'.";
			prs_display_error(temp.str());
			
			return;
		}
		max=atoi((char *) value);
		// parameter 'step' (if present)
		xmlFree(value);
		value=xmlGetProp(node, (xmlChar *) "step");
		if(value!=NULL)
		{
			step=atoi((char *) value);
		}
		xmlFree(value);

		xmlChar *tmp = xmlGetProp(node, (xmlChar *) "name");
        string par_name = string((char *) tmp);
		xmlFree(tmp);
		if(step!=1)
		{
			for(i=min; i<=max; i=i+step)
			{
				ostringstream int2string;
				int2string << i;
				el_list.push_back(int2string.str());
			}
            if(has_multiple_instances)
            {
                for(int mi=1; mi<=instances; mi++)
                {
                    ostringstream int2string;
                    int2string << mi;
                    string suffix = int2string.str();
			        design_space->insert_scalar(env, par_name+"_"+suffix, M3_SCALAR_TYPE_LIST, min, max, el_list);
                }
            }
            else
			    design_space->insert_scalar(env, par_name, M3_SCALAR_TYPE_LIST, min, max, el_list);
		}
		else
		{
            if(has_multiple_instances)
            {
                for(int mi=1; mi<=instances; mi++)
                {
                    ostringstream int2string;
                    int2string << mi;
                    string suffix = int2string.str();
			        design_space->insert_scalar(env, par_name+"_"+suffix, M3_SCALAR_TYPE_INTEGER, min, max, vector<string>());
                }
            }
            else
			    design_space->insert_scalar(env, par_name, M3_SCALAR_TYPE_INTEGER, min, max, vector<string>());
			// don't need to load everything, for integer everything is easier
		}

	}
	else if(xmlStrEqual(value, (xmlChar *) "string"))
	{
		xmlFree(value);
		ostringstream query;
		xmlChar *tmp = xmlGetProp(node, (xmlChar *) "name");
        string tmp_str = string((char *) tmp);
		xmlFree(tmp);
		query << "/*[local-name()='design_space']/*[local-name()='parameters']/*[local-name()='parameter'][@name='" << tmp_str << "']/*[local-name()='item']";
		xpath_set=this->get_nodeset(doc, (xmlChar *) query.str().c_str());
	    if(xpath_set==NULL)
	    {
	    	temp << "Parameter " << node->name << " has no list of possibile string values.";
	    	prs_display_error(temp.str());
	    	return;
	    }
	    nodeset=xpath_set->nodesetval;
	    for(i=0; i<nodeset->nodeNr; ++i)
	    {
			xmlChar *tmp = xmlGetProp(nodeset->nodeTab[i], (xmlChar *) "value");
    	    string par_val = string((char *) tmp);
			xmlFree(tmp);
	    	el_list.push_back(par_val);
	    }
		tmp = xmlGetProp(node, (xmlChar *) "name");
        tmp_str = string((char *) tmp);
		xmlFree(tmp);
	    design_space->insert_scalar(env, tmp_str, M3_SCALAR_TYPE_LIST, 0, 0, el_list);
	}
	else if(xmlStrEqual(value, (xmlChar *) "boolean"))
	{
		xmlFree(value);
		el_list.push_back("0");
		el_list.push_back("1");
		xmlChar *tmp = xmlGetProp(node, (xmlChar *) "name");
        string tmp_str = string((char *) tmp);
		xmlFree(tmp);
		design_space->insert_scalar(env, tmp_str, M3_SCALAR_TYPE_LIST, 0, 0, el_list);
	}
	else if(xmlStrEqual(value, (xmlChar *) "exp2"))
	{
		// parameter 'min' (mandatory)
		xmlFree(value);
		value=xmlGetProp(node, (xmlChar *) "min");
		if(value==NULL)
		{
			temp << "Parameter " << node->name << " has no attribute 'min'.";
			prs_display_error(temp.str());
			return;
		}
		min=atoi((char *) value);
		// parameter 'max' (mandatory)
		xmlFree(value);
		value=xmlGetProp(node, (xmlChar *) "max");
		if(value==NULL)
		{
			temp << "Parameter " << node->name << " has no attribute 'max'.";
			prs_display_error(temp.str());
			return;
		}
		max=atoi((char *) value);
		xmlFree(value);
		if(!is_exp2(min) || !is_exp2(max))
		{
			temp << "Parameter " << node->name << " has min or max not a power of 2.";
			prs_display_error(temp.str());
			return;
		}
		// calculating and appending everything
		for(i=min; i<=max; i=i*2)
		{
			ostringstream int2string;
			int2string << i;
			el_list.push_back(int2string.str());
		}
		xmlChar *tmp = xmlGetProp(node, (xmlChar *) "name");
   	    string par_name = string((char *) tmp);
		xmlFree(tmp);
		design_space->insert_scalar(env, par_name, M3_SCALAR_TYPE_LIST, 0, 0, el_list);
	}
	else if(xmlStrEqual(value, (xmlChar *) "permutation"))
	{
		xmlFree(value);
		value=xmlGetProp(node, (xmlChar *) "dimension");
		if(value==NULL)
		{
			temp << "Parameter " << node->name << " does not have the property 'dimension'.";
			prs_display_error(temp.str());
			return;
		}
		int int_dimension;
		string str_dimension;
		if(is_int(string((char *) value)))
		{
			str_dimension="";
			int_dimension=atoi((char *) value);
		}
		else
		{
			str_dimension=string((char *) xmlStrsub(value, 1, xmlStrlen(value)-1));
			int_dimension=0;
		}
		xmlFree(value);
		xmlChar *tmp = xmlGetProp(node, (xmlChar *) "name");
        string tmp_str = string((char *) tmp);
		xmlFree(tmp);
		design_space->insert_permutation(env, tmp_str, int_dimension==0, str_dimension, int_dimension);
	}
	else // on_off_mask
	{
		xmlFree(value);
		value=xmlGetProp(node, (xmlChar *) "dimension");
		if(value==NULL)
		{
			temp << "Parameter " << node->name << " does not have the property 'dimension'.";
			prs_display_error(temp.str());
			return;
		}
		int int_dimension;
		string str_dimension;
		if(is_int(string((char *) value)))
		{
			str_dimension="";
			int_dimension=atoi((char *) value);
		}
		else
		{
			str_dimension=string((char *) xmlStrsub(value, 1, xmlStrlen(value)-1));
			int_dimension=0;
		}
		xmlFree(value);
		xmlChar *value2=xmlGetProp(node, (xmlChar *) "on_set_size");
		int fixed_on_set_size;
		string on_set_dependent_parameter;
		if(value2==NULL)
		{
			fixed_on_set_size=0;
			on_set_dependent_parameter="";
		}
		else
		{
			if(is_int(string((char *) value2)))
			{
				on_set_dependent_parameter="";
				fixed_on_set_size=atoi((char *) value2);
			}
			else
			{
				on_set_dependent_parameter=string((char *) xmlStrsub(value2, 1, xmlStrlen(value2)-1));
				fixed_on_set_size=0;
			}
		}
		xmlChar *tmp = xmlGetProp(node, (xmlChar *) "name");
        string tmp_str = string((char *) tmp);
		xmlFree(tmp);
		design_space->insert_on_off_mask(env, tmp_str, int_dimension==0, str_dimension, int_dimension, value2==NULL, fixed_on_set_size==0, on_set_dependent_parameter, fixed_on_set_size);
		xmlFree(value2);
	}
}

xmlDocPtr m3_xml_design_space::get_doc()
{
	return this->doc;
}

void m3_xml_design_space::set_doc(xmlDocPtr doc)
{
	this->doc=doc;
}

/// class m3_xml_simulator ///

string m3_xml_simulator::directory="";
unsigned int m3_xml_simulator::counter=1;

m3_xml_simulator::m3_xml_simulator(string simulator): simulator(simulator)
{
	if(m3_xml_simulator::directory.compare("")==0)
	{
		m3_xml_simulator::init();
	}
}

m3_xml_simulator::~m3_xml_simulator()
{
	
}

xmlDocPtr m3_xml_simulator::read(string filename) throw(m3_xml_exception)
{
	xmlDocPtr output;
	ostringstream temp;
	
	output=xmlParseFile(filename.c_str());
    if(output==NULL)
    {
    	temp << "Unable to open the given input file: " << filename;
    	throw m3_xml_exception(temp.str());
    }
    if(!m3_xml_validator::get_simulator_validator()->validate(output))
    {
    	throw m3_xml_exception("Error during validation.");
    }
    // version check
	xmlChar *tmp = xmlGetProp(output->children, (xmlChar *) "version");
    string docversion = string((char *) tmp);
	xmlFree(tmp);
    if(!m3_xml_check_version(docversion))
    {
    	temp << "Document version " << docversion << "does not match the actual one: " << m3_xml_get_version();
    	throw m3_xml_exception(temp.str());
    }
	
	return output;
}

void m3_xml_simulator::write(string filename) throw(m3_xml_exception)
{
	xmlDocPtr input;
	xmlNodePtr root, node, item;
	map<string, string>::iterator it;
	int i=0, j;
	m3_design_space *design_space=current_environment.current_design_space;
	
	// input preparation
	input=xmlNewDoc((xmlChar *) "1.0");
    root=xmlNewNode(NULL, (xmlChar *) "simulator_input_interface");
    xmlNewProp(root, (xmlChar *) "xmlns", (xmlChar *) "http://www.multicube.eu/");
    xmlNewProp(root, (xmlChar *) "version", (xmlChar *) m3_xml_get_version().c_str());
    xmlDocSetRootElement(input, root);
	for(it=parameters.begin(); it!=parameters.end(); it++)
	{
		node=xmlNewChild(root, NULL, (xmlChar *) "parameter", NULL);
		xmlNewProp(node, (xmlChar *) "name", (xmlChar *) it->first.c_str());
		int type=get_parameter_type(design_space->ds_parameters, it->first);
		if(type==M3_DS_PERMUTATION)
		{
			vector<int> permutation=get_elements_as_vector(it->second);
			for(j=0; j<permutation.size(); ++j)
			{
				item=xmlNewChild(node, NULL, (xmlChar *) "item", NULL);
                string jj = to_string(j+1);
                string vj = to_string(permutation[j]);
				xmlNewProp(item, (xmlChar *) "position", (xmlChar *) jj.c_str());
				xmlNewProp(item, (xmlChar *) "value", (xmlChar *) vj.c_str());
			}
		}
		else if(type==M3_DS_ON_OFF_MASK)
		{
			vector<int> mask=get_elements_as_vector(it->second);
			for(j=0; j<mask.size(); ++j)
			{
				item=xmlNewChild(node, NULL, (xmlChar *) "item", NULL);
                string jj = to_string(j+1);
                string vj = to_string(mask[j]);
				xmlNewProp(item, (xmlChar *) "index", (xmlChar *) jj.c_str());
				xmlNewProp(item, (xmlChar *) "value", (xmlChar *) vj.c_str());
			}
		}
		else // scalar value, we can insert it as is
		{
			xmlNewProp(node, (xmlChar *) "value", (xmlChar *) it->second.c_str());
		}
		++i;
	}
	xmlSaveFileEnc(filename.c_str(), input, "UTF-8");
}

string get_current_dir()
{
    char sz[400];
    string s = getcwd(sz, 400);
    return s;
}

static string invocation_dir;

/*
 * We are using a system() call because I had some problems in using
 * the standard fork()/exec() paradigm, obtaining obscure errors.
 * We do a fork, even if not strictly required by that call, because
 * we need a unique pid for each call to create unique input/output XML
 * pairs. So for each simulate() call we create two children.
 */
void m3_xml_simulator::run(map<string, string> parameters) throw()
{
	int status=0;
	ostringstream directory, filename, filename_out, temp;
	xmlDocPtr output=NULL;
	xmlChar *xpath, *attr, *attr2;
	time_t t;
	struct tm *tmp;
	char tms[200];
	bool error=false;
	xmlXPathObjectPtr xpath_set=NULL;
	xmlNodeSetPtr nodeset;
	int i;
	string *pname, *pname2;
	m3_object *obj;
	
	try
	{
		this->parameters=parameters;
		this->metrics.clear();
		t=time(NULL);
		tmp=localtime(&t);
		strftime(tms, sizeof(tms), ".%d-%m-%Y.%H-%M-%S.xml", tmp);
		// writing input file
		directory << m3_xml_simulator::directory << "/" << this->counter << "/";
		filename << "input" << tms;
		filename_out << "output" << tms;
		// we need to ignore errno 17: it happens without an explicit cause
        //cout << "Creating " << directory.str() << endl;
		if(mkdir(directory.str().c_str(), S_IRUSR | S_IWUSR | S_IXUSR)!=0 && errno!=17)
		{
			temp <<"Unable to create temporary directory: " << errno;
			throw m3_xml_exception(temp.str()); 
		}
        //cout << "Moving from " << get_dir() << " to " << directory.str() << endl;
		if(chdir(directory.str().c_str())!=0)
		{
			temp << "Unable to chdir to the temporary directory: " << errno;
			throw m3_xml_exception(temp.str());
		}
		this->write(filename.str());
		// executing the simulator
		current_environment.shell_variables.get("current_build_path", obj);
		temp << simulator << " --xml_system_configuration=" << filename.str() << " --xml_system_metrics=" << filename_out.str() << " --reference_xsd=" << to<m3_string *>(obj)->get_string() << m3_xml_validator::simulator_schema;
        string cmd = string("/bin/bash -c \"") +temp.str() + "\"";
		status=system(cmd.c_str());
//#define DEBUG
#if defined(DEBUG)
        cout << "Simulator running into working directory: " << get_current_dir() << endl;
        cout << "Invoking with: " << temp.str() << endl;
#endif
		this->counter++;
		if(WEXITSTATUS(status)!=0)
		{
			temp << "The simulator returned an error code: " << WEXITSTATUS(status);
			throw m3_xml_exception(temp.str());
		}
        //cout << "Trying to read " << get_dir() << endl;
		output=this->read(filename_out.str());
		if(chdir(invocation_dir.c_str())!=0)
		{
			temp << "Unable to chdir back to the original directory: " << errno;
			throw m3_xml_exception(temp.str());
		}
		// try normal output results
		xpath=(xmlChar *) "/*[local-name()='simulator_output_interface']/*[local-name()='system_metric']";
		xpath_set=this->get_nodeset(output, xpath);
		if(xpath_set==NULL)
		{
			// ok, maybe we have an error...
			xpath=(xmlChar *) "/*[local-name()='simulator_output_interface']/*[local-name()='error']";
			xpath_set=this->get_nodeset(output, xpath);
			if(xpath_set==NULL)
			{
				// no, something wrong happened somewhere... anyway, we
				// should not be here because of a correct validation
				throw m3_xml_exception("Cannot load parameters.");
			}
			error=true;
		}
		nodeset=xpath_set->nodesetval;
		for(i=0; i<nodeset->nodeNr; ++i)
		{
			if(error)
			{
				xmlChar *tmp = xmlGetProp(nodeset->nodeTab[i], (xmlChar *) "reason");
		        string tmp_str = string((char *) tmp);
				xmlFree(tmp);
				if(xmlStrcmp(xmlGetProp(nodeset->nodeTab[i], (xmlChar *) "kind"), (xmlChar *) "fatal")==0)
				{
					this->error_code=M3_POINT_FATAL_ERROR;
					this->error_description=tmp_str;
				}
				else
				{
					this->error_code=M3_POINT_NON_FATAL_ERROR;
					this->error_description=tmp_str;
				}
			}
			else
			{
				attr=xmlGetProp(nodeset->nodeTab[i], (xmlChar *) "name");
				pname=new string((char *) attr);
				attr2=xmlGetProp(nodeset->nodeTab[i], (xmlChar *) "value");
				pname2=new string((char *) attr2);
				double value=atof(pname2->c_str());
				metrics.insert(pair<string, double>(string(*pname), value));
				delete pname;
				delete pname2;
				xmlFree(attr);
				xmlFree(attr2);
				this->error_code=M3_POINT_NO_ERROR;
			}
		}
		xmlXPathFreeObject(xpath_set);
		xmlFreeDoc(output);
	}
	catch(m3_xml_exception e)
	{
		if(xpath_set!=NULL)
		{
			xmlXPathFreeObject(xpath_set);
		}
		if(output!=NULL)
		{
			xmlFreeDoc(output);
		}
        /** We should make it non-fatal, in the case that the simulator is not available */
		this->error_code=M3_POINT_NON_FATAL_ERROR;
		this->error_description=string(e.what());
	}
    chdir(invocation_dir.c_str());
}

map<string, double> m3_xml_simulator::get_metrics()
{
	return metrics;
}

int m3_xml_simulator::get_error_code()
{
	return error_code;
}

string m3_xml_simulator::get_error_description()
{
	return error_description;
}

map<string, string> m3_xml_simulator::get_parameters()
{
	return parameters;
}


void m3_xml_simulator::init() throw(m3_xml_exception)
{
	ostringstream dir, temp;

    invocation_dir = get_current_dir();    
	dir << invocation_dir << "/m3explorer" << getpid();
	m3_xml_simulator::directory=dir.str();
	// we need to ignore errno 17: it happens without an explicit cause
	if(mkdir(m3_xml_simulator::directory.c_str(), S_IRUSR | S_IWUSR | S_IXUSR)!=0 && errno!=17)
	{
		temp <<"Unable to create temporary directory: " << errno;
		throw m3_xml_exception(temp.str()); 
	}
}

bool m3_xml_simulator::del_element(const char *name)
{
	DIR *tdir;
	struct dirent *act_file;
	
	tdir=opendir(name);
	if(tdir!=NULL)
	{
		act_file=readdir(tdir);
		while(act_file!=NULL)
		{
			if(strcmp(act_file->d_name, ".")!=0 && strcmp(act_file->d_name, "..")!=0)
			{
				// deleting each file
				if(act_file->d_type!=DT_DIR)
				{
					ostringstream file;
					file << name << "/" << act_file->d_name;
					remove(file.str().c_str());
				}
				else // recursion
				{
					ostringstream temp;
					temp << name << "/" << act_file->d_name;
					if(!m3_xml_simulator::del_element(temp.str().c_str()))
					{
						return false;
					}
				}
			}
			act_file=readdir(tdir);
		}
		closedir(tdir);
		// deleting empty directory
		remove(name);
		return true;
	}
	else
	{
		return false;
	}
	
}

void m3_xml_simulator::shutdown()
{
	m3_object *obj;
	 
	if(!current_environment.shell_variables.get("clean_directories_on_exit", obj) || (is_a<m3_string *>(obj) && to<m3_string *>(obj)->get_string().compare("true")==0))
	{
		if(!m3_xml_simulator::del_element(m3_xml_simulator::directory.c_str()))
		{
			cerr << "Unable to delete temporary directory" << endl;
		}
	}
}

/// class m3_xml_driver ///

string m3_xml_driver::get_information()
{
    string info = "";
    info.append("XML driver written by A. Sivieri");
    return info;
}

m3_design_space *m3_xml_driver::get_design_space(m3_env *env) throw(m3_xml_exception)
{
	m3_object *obj;
	m3_design_space *design_space=new m3_design_space();
	m3_xml_design_space reader;
	
	current_environment.shell_variables.get("xml_design_space_file", obj);
	reader.set_doc(reader.read(to<m3_string *>(obj)->get_string()));
	reader.fill(design_space);
	this->simulator=new m3_xml_simulator(reader.get_simulator());
	this->rules=reader.get_rules();
	
	return design_space;
}


bool m3_xml_driver::is_valid(m3_point &point, m3_env *env)
{
	int i;
	
	if(this->rules.size()==0)
	{
		return true;
	}
	for(i=0; i<this->rules.size(); i++)
	{
		if(!this->rules[i]->validate(point))
		{
			return false;
		}
	}
	
	return true;
}

m3_point *m3_xml_driver::simulate(m3_point &point, m3_env *env)
{
    m3_point *simulated_point = new m3_point(point);
    m3_design_space *design_space=env->current_design_space;
    map<string, string> parameters;
    m3_vector statistics, metrics;
    int i;
    map<string, double>::iterator it;
    
	// launch work
    for(i=0; i<design_space->ds_parameters.size(); ++i)
    {
	    m3_parameter par=design_space->ds_parameters[i];
		if(par.type==M3_DS_PERMUTATION)
		{
			vector<int> permutation=design_space->get_permutation(env, &point, par.name);
			parameters.insert(pair<string, string>(par.name, get_elements_as_string(permutation)));
		}
		else if(par.type==M3_DS_ON_OFF_MASK)
		{
			vector<int> mask=design_space->get_mask(env, &point, par.name);
			parameters.insert(pair<string, string>(par.name, get_elements_as_string(mask)));
		}
		else // scalar value, we can insert it as is
		{
			// if the list of this scalar is empty, then we have an integer
			// with step 1, so the point value is the one requested
			if(design_space->scalar_parameters[par.name].list.size()==0)
			{
				parameters.insert(pair<string, string>(par.name, to_string(point[i])));
			}
			else // otherwise, let's pick up the list element
			{
				parameters.insert(pair<string, string>(par.name, design_space->scalar_parameters[par.name].list[point[i]]));
			}
		}
    }
	simulator->run(parameters);
    //cout << " Current directory is " << get_current_dir() << endl;
	if(simulator->get_error_code()==M3_POINT_NO_ERROR)
    {
		for(i=0; i<design_space->metric_names.size(); ++i)
		{
			double value=simulator->get_metrics()[design_space->metric_names[i]];
            m3_double obj(value);
			metrics.insert(i, obj);
		}
	    simulated_point->set_properties("metrics", metrics);
	    simulated_point->set_properties("statistics", statistics);
    }
	else
	{
		simulated_point->set_error(simulator->get_error_code(), simulator->get_error_description());
	}
	
	return simulated_point;
}

string m3_xml_driver::get_name()
{
    string name = "xml_driver";
    return name;
}

m3_xml_driver::m3_xml_driver() throw(m3_xml_exception)
{
	prs_display_message("Loading the xml_driver"); 
    m3_xml_simulator::init();
}

m3_xml_driver::~m3_xml_driver()
{
    prs_display_message("Removing xml_driver"); 
    int i;
	for(i=0; i<this->rules.size(); ++i)
	{
		delete this->rules[i];	
	}
	m3_xml_validator::shutdown();
	m3_xml_simulator::shutdown();
	delete this->simulator;
	// this will be the last call, to be sure to clean up everything
	xmlCleanupParser();
}

/// class m3_xml_exception ///

m3_xml_exception::m3_xml_exception(string msg): m3_exception("XML exception: " + msg)
{
	
}

m3_xml_exception::~m3_xml_exception() throw()
{
	
}

/// extra stuff ///

extern "C"
{

	m3_xml_driver *drv_generate_driver()
    {
        prs_display_message("Creating the xml_driver");
        return new m3_xml_driver();
    }

}

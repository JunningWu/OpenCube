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





/*
 * \file
 * 
 * Author: Vittorio Zaccaria, Politecnico di Milano
 *
 */

#include <string>
#include <cstring>
#include <map>
#include <set>
#include <list>
#include <iostream>
#include <fstream>

#ifndef ARG_HH
#define ARG_HH

using namespace std;

typedef map<string, string> options;
typedef set<string> parameters;

typedef enum 
{
  OPTION_SIMPLE,
  OPTION_MULTIPLE_ARG,
} arg_parser_item_type;

/** The data structure for a program argument */
typedef struct arg_parser_item
{
  arg_parser_item_type  type;
  string                    option_name;
  string                    short_option_name;
  string                    argument_name;
  set<string>               values;   
  string                    comment;
} arg_parser_item;

typedef map<string, arg_parser_item> option_list;
typedef list<string> parameter_list;

/** The parser of program arguments */
typedef struct arg_parser
{
  string          	prog_name;
  string          	parameter_name;
  option_list 		options;
  parameter_list 	parameters;
  string          	bug_report;

  string look_for_option_name(string short_option);

  void insert(const char *option_name, char short_option, const char *argument, const char *comment, arg_parser_item_type type);

  bool option_is_set(string option_name);

  string get_option_value(string option_name);

  bool process_options(int argc, char **argv);

  void print_help();

  void print_short_help();

} arg_parser; 


#endif

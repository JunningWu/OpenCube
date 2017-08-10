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
#include <map>
#include <set>
#include <list>
#include <iostream>
#include <fstream>
#include "m3_arg.h"


string arg_parser::look_for_option_name(string short_option)
{
  option_list::iterator i;
  for(i = options.begin();
      i != options.end();
      i++)
  {
    if(i->second.short_option_name == short_option)
      return i->first;
  }
  return "";
}

void arg_parser::insert(const char *option_name, char short_option, const char *argument, const char *comment, arg_parser_item_type type)
{
  char opt[2];

  opt[1]='\0';
  opt[0]= short_option;

  arg_parser_item item;
  item.option_name = option_name;
  item.short_option_name = opt;
  item.argument_name = argument ? argument: "";
  item.comment = comment ? comment: "";
  item.type = type;

  options.insert(pair<string, arg_parser_item>(string(option_name), item));
}

bool arg_parser::option_is_set(string option_name)
{
  return (options[option_name].values.size() != 0);
}

string arg_parser::get_option_value(string option_name)
{
  return (*(options[option_name].values.begin()));
}

bool arg_parser::process_options(int argc, char **argv)
{
  for(int i=1; i<argc; i++)
  {
    bool complex_option=false;
    int  eq=0;

    char tmpstr1[200];
    char tmpstr2[200];

    string opt; 
    string val; 

    tmpstr1[0]='\0';
    tmpstr2[0]='\0';

    if(!strncmp(argv[i],"--",2))
    {
      int s=0;
      argv[i]=argv[i]+2;
      for(s=0; s<strlen(argv[i]); s++)
      {
        if(argv[i][s]=='=')
        {
          complex_option=true;
          eq=s;	
        }
      }	

      if(complex_option)
      {
        int sz = strlen(argv[i])-(eq+1);
        strncpy(tmpstr1,argv[i], eq);
        tmpstr1[eq]='\0';
        strncpy(tmpstr2,argv[i]+eq+1, sz);
        tmpstr2[sz]='\0';
        opt = tmpstr1; 
        val = tmpstr2; 
      }
      else
      {
        opt = argv[i];
        val = "";
      }
    }
    else
    {
      /** This is an option with only one character */
      if(!strncmp(argv[i],"-",1))
      {
        int s=0;
        char option[2];
        string long_opt;

        option[0] = argv[i][1];
        option[1] = '\0';

        argv[i]=argv[i]+2;
        long_opt = option;

        val = argv[i];
        opt = look_for_option_name(long_opt);

        if(opt == "")
        {
          cout << "Error: unrecognized short option '-" << long_opt << "'" << endl;
          return false;
        }
      }
      else
      {
        opt = "";
        val = argv[i];
      }
    }

    if(opt!="")
    {
      if(!options.count(opt))
      {
        cout << "Error: unrecognized option '" << opt << "'" << endl;
        return false;
      }
      else
      {
        if(val == "" && options[opt].type == OPTION_MULTIPLE_ARG)
        {
          if(i<(argc-1))
          {
            options[opt].values.insert(argv[i+1]);
            i++;
          }
          else
          {
            cout << "Error: option '" << opt << "' needs an extra parameter" << endl;
            return false;
          }
        }
        else
        {
          options[opt].values.insert(val);
        }
      }
    }
    else
    {
      parameters.push_back(val); 
    }
  }	
  return true;
}

#define TABS 30

void arg_parser::print_help()
{
  cout << "Usage: " << prog_name << " [options] FILE(S) " << endl;
  cout << endl;

  cout << "Mandatory arguments to long options are mandatory for short options too." << endl;

  option_list::iterator i;
  for(i = options.begin();
      i != options.end();
      i++)
  {
    unsigned int size ; 
    cout << "  -" << i->second.short_option_name.c_str()[0] << ", --" << i->second.option_name;
    size = strlen(i->second.option_name.c_str());
    if(i->second.type != OPTION_SIMPLE)
    {
      cout << "=" << i->second.argument_name;
      size += 1 + strlen(i->second.argument_name.c_str());
    }

    for(int j=size; j<=TABS; j++)
      cout << " ";
    cout << i->second.comment << endl;
  }
  cout << endl << endl;
  cout << bug_report << endl;
}

void arg_parser::print_short_help()
{
  int n=0;

  option_list::iterator i;
  cout << prog_name << ": ";
  for(i = options.begin();
      i != options.end();
      i++)
  {
    unsigned int size ; 
    cout << "[ -" << i->second.short_option_name.c_str()[0] << " | --" << i->second.option_name;
    size = strlen(i->second.option_name.c_str());
    if(i->second.type != OPTION_SIMPLE)
    {
      cout << "=" << i->second.argument_name;
      size += 1 + strlen(i->second.argument_name.c_str());
    }
    cout << "] ";

    n++;

    if(!(n % 3)) 
    {
      cout << endl;
      for(int k=0; k<strlen(prog_name.c_str()); k++)
        cout << " ";
      cout << "  ";

    }
  }
  cout << parameter_name; 
  cout << endl;
}





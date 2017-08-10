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
#include <m3_commands.h>
#include <m3_arg.h>
#include <m3_env.h>
#include <libgen.h>
#include <unistd.h>

#if !defined(__MAC_OSX__)
/**
 * Reads the directory of the executable
 */
string m3_get_build_path(char *base_m3explorer_name)
{
  int retsz;
  char complete_path[300];
  retsz = readlink("/proc/self/exe", complete_path, 300);
  if(retsz < 0)
    return ".";

  complete_path[retsz]='\0';

  dirname(complete_path);
  dirname(complete_path);
  string final_path = string(complete_path);
  return final_path;
}

#else

string m3_get_curdir()
{
    char s[ 400+ 1];
    string st = getcwd(s,400);
    return st;
}


/** 
 * Under mac we have to play a little bit more.
 * Reads the directory of the executable.
 */
string m3_get_build_path(char *base_m3explorer_name)
{
  int retstatus;
  string command = "type -p ";
  FILE *file;

  char complete_path[100];
  char *internal_path;
  command = command + base_m3explorer_name;
  command = command + " > ._m3explorer_tmp_file";

  //cout << "Executing: " << command << endl;

  retstatus = system(command.c_str());

  if(retstatus)
  {
    cout << "Cannot state the complete path of m3explorer" << endl;
    exit(0);
  }

  file = fopen("._m3explorer_tmp_file", "r");
  fscanf(file, "%s", complete_path);
  fclose(file);

  string curpath = m3_get_curdir();
  internal_path = dirname(complete_path);
  internal_path = dirname(internal_path);
  chdir(internal_path);
  string final_path = m3_get_curdir();
  chdir(curpath.c_str());
  return final_path;
}

#endif

void m3_print_welcome_screen()
{
    cout << " \n";
    cout << "         ____              _                     \n";
    cout << "   _ __ |__ / _____ ___ __| |___ _ _ ___ _ _     \n";
    cout << "  | '  \\ |_ \\/ -_) \\ / '_ \\ / _ \\ '_/ -_) '_|    \n";
    cout << "  |_|_|_|___/\\___/_\\_\\ .__/_\\___/_| \\___|_|      \n";
    cout << "                     |_|                         \n";
    cout << " \n";
    cout << "  Multicube Explorer - Version " << __M3EXPLORER_VERSION__ << endl;
    cout << "  Send bug reports to zaccaria@elet.polimi.it, gpalermo@elet.polimi.it \n";
    cout << "  --\n\n";
}


void m3_setup(int argc, char **argv)
{
    srandom(0);
    m3_string cbp(m3_get_build_path(argv[0]));
    current_environment.shell_variables.insert("current_build_path", cbp);
    m3_init_parse_command();
    m3_init_expression_eval();
}

extern void prs_command_drv_define_driver(string n);

arg_parser current_arg_parser;

int main(int argc, char **argv)
{
    current_arg_parser.prog_name = "m3explorer";
    current_arg_parser.parameter_name = "";
    current_arg_parser.bug_report = "Report bugs to: <vittorio.zaccaria@gmail.com>, <gpalermo@elet.polimi.it>";
    
    current_arg_parser.insert("help", 'H', NULL, "this help", OPTION_SIMPLE);
    current_arg_parser.insert("open_script_file", 'f', "FILE", "specify source script", OPTION_MULTIPLE_ARG);
    current_arg_parser.insert("input_xml_file", 'x', "FILE", "use the XML design space definition in FILE", OPTION_MULTIPLE_ARG);
 
    m3_setup(argc, argv);

    if(!current_arg_parser.process_options(argc, argv))
    {
        current_arg_parser.print_short_help();
        exit(1);
    }
  
    if(current_arg_parser.option_is_set("help"))
    {
        current_arg_parser.print_help();
        exit(0);
    }
    

    if(current_arg_parser.option_is_set("input_xml_file"))
    {
        set<string>::iterator i;
        i = current_arg_parser.options["input_xml_file"].values.begin();

        m3_string xml_file_name(*i);
  
        current_environment.shell_variables.insert("xml_design_space_file", xml_file_name);
        prs_command_drv_define_driver("m3_xml_driver");
    }
    /*
    else
    {
       cout << "Please define the Multicube XML design space definition pathname with -x option" << endl;
       current_arg_parser.print_help();
       exit(0);
    }
    */
  
    if(current_arg_parser.option_is_set("open_script_file"))
    {
        set<string>::iterator i;
        for( i=current_arg_parser.options["open_script_file"].values.begin();
                i!=current_arg_parser.options["open_script_file"].values.end();
                i++)
        {
            /* prs_display_message("Reading commands from file " + *i); */
            if (prs_open_input_file(i->c_str()))
            {
                prs_dont_show_prompt();
            }

        }
    }
    else
    {
        m3_print_welcome_screen();
    }

    prs_display_prompt();
    while (1) yyparse();
}

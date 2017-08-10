//==========================================================================
//	original_options.cpp
//	Author: Juan Castillo, David Quijano
//	Date: lun dic 3 2007
//	Description:
//==========================================================================
//  The following code is derived, directly or indirectly, from SCoPE,
//  released June 30, 2008. Copyright (C) 2006 Design of Systems on Silicon (DS2)
//  The Initial Developer of the Original Code is the University of Cantabria
//  for Design of Systems on Silicon (DS2)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) for more details.
//
//  You should have received a copy of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL)
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  For more information about SCOPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================
/*!
 * \file command_line.cpp
 * \brief Function implementation of command_analyzed class methods
 */
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include "command_line.h"
using namespace std;

#define GCC_PATH "/usr/bin/"

#define VPRINT_CMD(a, b) \
	if (m_verbose == true) { \
		cout << a << b << endl; \
	}

/*******************************************************************/
/*                         Public Methods                          */
/*******************************************************************/

/*!
 * \fn command_analyzer_class::command_analyzer_class(int src_lang)
 * \brief Constructor
 * Instantiates the asm info analizer, reserves space for buffer and
 * gets the necessary environment variables.
 * \param language Languaje of user code (see defines in header file)
 */
command_analyzer_class::command_analyzer_class() {
	char *aux;
	//$SCOPE_HOME:
	aux = getenv("SCOPE_HOME");
	if (aux == NULL) {
		cerr << "#ERROR: SCOPE_HOME not set" << endl;
		exit(1);
	}
	m_scope_home = aux;
	//$SYSTEMC:
	aux = getenv("SYSTEMC");
	if (aux == NULL) {
		cerr << "#ERROR: SYSTEMC not set" << endl;
		exit(1);
	}
	m_systemc = aux;
	//Default options:
	m_cpu = "arm926t";
	m_method = "op-cost";
//	m_method = "";
	m_cross_compiler = "arm-linux-gcc";
	m_api = "posix";
	m_verbose = false;
	m_preserve_files = false;
	m_large_cache = false;
	m_language = "";//"c++";
	m_optmizations = 0;
	m_hw = false;
	m_time = true;
	m_icache = true;
	m_dcache = true;
}


/*!
 * \fn command_analyzer_class::~command_analyzer_class()
 * \brief Default destructor
 */
command_analyzer_class::~command_analyzer_class() {
}


/*!
 * \fn command_analyzer_class::analyze_command_line(int argc, char *argv[])
 * \brief Get options and input files from command line
 * This function detects input files and puts them into the file list.
 * Remaining options are concatenated with their arguments to form the
 * original_options string.
 * It also detects if user provides the name of the output file (-o file).
 * \param argc number of options
 * \param argv options
 */
void command_analyzer_class::analyze_command_line(int argc, char *argv[]) {
	int c;
	int opt_index = 0;
	int opt_flag;
	opterr = 0;
	char *aux;
	string scope_opt;
	
	static struct option long_options[] = {
		{"scope-cpu", required_argument, &opt_flag, 0},
		{"scope-method", required_argument, &opt_flag, 1},
		{"scope-crosscompiler", required_argument, &opt_flag, 2},
		{"scope-api", required_argument, &opt_flag, 3},
		{"scope-verbose", no_argument, &opt_flag, 4},
		{"scope-preserve-files", no_argument, &opt_flag, 5},
		{"scope-large-cache", no_argument, &opt_flag, 6}, /* Similar to solution proposed by Rosenstiel.*/
		{"scope-language", required_argument, &opt_flag, 7},
		{"scope-optimizations", no_argument, &opt_flag, 8},
		{"scope-pg", no_argument, &opt_flag, 9},
		{"scope-hw", no_argument, &opt_flag, 10},
		{"help", no_argument, &opt_flag, 11},
		{"scope-notime",no_argument, &opt_flag, 12},
		{"scope-nodcache",no_argument, &opt_flag, 13},
		{"scope-noicache",no_argument, &opt_flag, 14},
		{"scope-noannotate",no_argument, &opt_flag, 15},
		{0, 0, 0, 0}
	};
	
	for (int i=1 ; i<argc ; i++) {
		m_original_opt += argv[i];
		m_original_opt += " ";
	}
	
	while ((c = getopt_long(argc, argv, GCC_COMMAND_OPTIONS, long_options, &opt_index)) != -1) {
		switch (c) {
			case 0:
// 				cout << "SCoPE Long option " << long_options[opt_index].name;
// 				if (optarg) {
// 					cout << " with arg " << optarg << endl;
// 				}
				switch (opt_index) {
					case 0:
						m_cpu = optarg;
						scope_opt = "--scope-cpu=";
						scope_opt += optarg;
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 1:
						m_method = optarg;
						scope_opt = "--scope-method=";
						scope_opt += optarg;
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 2:
						m_cross_compiler = optarg;
						scope_opt = "--scope-crosscompiler=";
						scope_opt += optarg;
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 3:
						m_api = optarg;
						scope_opt = "--scope-api=";
						scope_opt += optarg;
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 4:
						m_verbose = true;
						scope_opt = "--scope-verbose";
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 5:
						m_preserve_files = true;
						scope_opt = "--scope-preserve-files";
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 6:
						m_large_cache = true;
						scope_opt = "--scope-large-cache";
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 7:
						if(m_language.size()!=0)
							if(m_language != optarg) {
								cerr << "#WARNING: Applying "<<optarg<<" rules to "<<m_input_file<< endl;
							}
						m_language = optarg;
						scope_opt = "--scope-language=";
						scope_opt += optarg;
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 8:
						m_optmizations = 2;
						scope_opt = "--scope-optimizations";
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 9:
						m_pg = true;
						scope_opt = "--scope-pg";
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 10:
						m_hw = true;
						scope_opt = "--scope-hw";
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 11:
						print_help();
						exit(0);
					case 12:
						m_time = false;
						scope_opt = "--scope-notime";
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 13:
//						printf("WARNING: Data cache modeling still not available\n");
						m_dcache = false;
						scope_opt = "--scope-nodcache";
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 14:
						m_icache = false;
						scope_opt = "--scope-noicache";
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					case 15:
						m_time = false;
						m_dcache = false;
						m_icache = false;
						scope_opt = "--scope-noannotate";
						m_original_opt.erase(m_original_opt.find(scope_opt), scope_opt.size());
						break;
					default:
						cerr << "#WARNING: SCoPE option not recognized: " << opt_index << endl;
						break;
				}
				break;
			case 1:
				if (!m_input_file.empty()) {
					cerr << "#ERROR: multiple input files not allowed. Compile them individually" << endl;
					exit(1);
				}
				if ((aux=strstr(optarg, ".c")) != NULL && *(aux+2) == '\0') {
					m_input_file = optarg;
					if(m_language.size()==0)
						m_language = "c";
					else
						if(m_language != "c") {
							cerr << "#WARNING: Applying C++ rules to a .c file."<< endl;
						}
				}
				else if ((aux=strstr(optarg, ".cc")) != NULL && *(aux+3) == '\0') {
					m_input_file = optarg;
					if(m_language.size()==0)
						m_language = "c++";
					else
						if(m_language == "c") {
							cerr << "#WARNING: Applying C rules to a .cc file."<< endl;
						}
				}
				else if ((aux=strstr(optarg, ".cp")) != NULL && *(aux+3) == '\0') {
					m_input_file = optarg;
					if(m_language.size()==0)
						m_language = "c++";
					else
						if(m_language == "c") {
							cerr << "#WARNING: Applying C rules to a .cp file."<< endl;
						}
				}
				else if ((aux=strstr(optarg, ".cxx")) != NULL && *(aux+4) == '\0') {
					m_input_file = optarg;
					if(m_language.size()==0)
						m_language = "c++";
					else
						if(m_language == "c") {
							cerr << "#WARNING: Applying C rules to a .cxx file."<< endl;
						}
				}
				else if ((aux=strstr(optarg, ".cpp")) != NULL && *(aux+4) == '\0') {
					m_input_file = optarg;
					if(m_language.size()==0)
						m_language = "c++";
					else
						if(m_language == "c") {
							cerr << "#WARNING: Applying C rules to a .c++ file."<< endl;
						}
				}
				else if ((aux=strstr(optarg, ".CPP")) != NULL && *(aux+4) == '\0') {
					m_input_file = optarg;
					if(m_language.size()==0)
						m_language = "c++";
					else
						if(m_language == "c") {
							cerr << "#WARNING: Applying C rules to a .CPP file."<< endl;
						}
				}
				else if ((aux=strstr(optarg, ".c++")) != NULL && *(aux+4) == '\0') {
					m_input_file = optarg;
					if(m_language.size()==0)
						m_language = "c++";
					else
						if(m_language == "c") {
							cerr << "#WARNING: Applying C rules to a .c++ file."<< endl;
						}
				}
				else if ((aux=strstr(optarg, ".C")) != NULL && *(aux+2) == '\0') {
					m_input_file = optarg;
					if(m_language.size()==0)
						m_language = "c++";
					else
						if(m_language == "c") {
							cerr << "#WARNING: Applying C rules to a .C file."<< endl;
						}
				}
				else if ((aux=strstr(optarg, ".i")) != NULL && *(aux+2) == '\0') {
					m_input_file = optarg;
					if(m_language.size()==0)
						m_language = "c";
					else
						if(m_language != "c") {
							cerr << "#WARNING: Applying C++ rules to a .i file."<< endl;
						}
				}
				else if ((aux=strstr(optarg, ".ii")) != NULL && *(aux+2) == '\0') {
					m_input_file = optarg;
					if(m_language.size()==0)
						m_language = "c++";
					else
						if(m_language == "c") {
							cerr << "#WARNING: Applying C rules to a .ii file."<< endl;
						}
				}
				break;
			case 'o':
				m_out_file = optarg;
				break;
			default:
				break;
		}
	}
	
	size_t opts = m_original_opt.find("-O");
	if(opts != m_original_opt.npos){
		switch( m_original_opt[opts+2] ){
			case ' ':
			case '1':
/*				if(!m_optmizations){
					cerr << "#WARNING: --scope-optimizations not indicated although optimization appear in the compiler line."<< endl;
					cerr << "#WARNING: Applying -O1."<< endl;
				}*/
				m_optmizations = 1;
				break;
			case '2':
/*				if(!m_optmizations){
					cerr << "#WARNING: --scope-optimizations not indicated although optimization appear in the compiler line."<< endl;
					cerr << "#WARNING: Applying -O2."<< endl;
				}*/
				m_optmizations = 2;
				break;
			case '3':
/*				if(!m_optmizations){
					cerr << "#WARNING: --scope-optimizations not indicated although optimization appear in the compiler line."<< endl;
					cerr << "#WARNING: Applying -O3."<< endl;
				}*/
				m_optmizations = 3;
				break;
			case 's':
/*				if(!m_optmizations){
					cerr << "#WARNING: --scope-optimizations not indicated although optimization appear in the compiler line."<< endl;
					cerr << "#WARNING: Applying -O2."<< endl;
				}
				else{*/
					cerr << "#WARNING: -Os optimization not recognised."<< endl;
					cerr << "#WARNING: Applying -O2"<< endl;
/*				}*/
				m_optmizations = 2;
				break;
			case '0':
//			default :
				if(m_optmizations){
					cerr << "#WARNING: Required optimization when no optimization appear in the compiler line."<< endl;
					cerr << "#WARNING: Applying -O0"<< endl;
				}
				m_optmizations = 0;
		}
	}else{
		if(m_optmizations){
			cerr << "#WARNING: Required optimization when no optimization appear in the compiler line."<< endl;
			cerr << "#WARNING: Applying -O0"<< endl;
		}
		m_optmizations = 0;
	}

// 	cout << "Cadena original: " << m_original_opt << endl;
// 	cout << "Fichero fuente : " << m_input_file << endl;
// 	cout << "Fichero de salida: " << m_out_file << endl;
	
	m_original_opt.erase(m_original_opt.find(m_input_file), m_input_file.size());
	remove_option(m_original_opt, 'o');
// 	cout << "Cadena original modificada: " << m_original_opt << endl;

	if (m_input_file.empty()) {
		cerr << "#ERROR: No input file" << endl;
		print_help();
		exit(1);
	}
	if (m_out_file.empty()) {
		change_file_ext(m_out_file, m_input_file, "o");
	}
	make_filenames();
	make_commands();
}


/*!
 * \fn command_analyzer_class::run(bool rm_files)
 * \brief Executes the compilation process for current file
 */
int command_analyzer_class::run(void) {
	int ret = 0;
	if (m_verbose == true) {
		cout << endl;
		cout << "****************************************************" << endl;
		cout << "* File: " << m_input_file << endl;
		cout << "****************************************************" << endl;
	}
	//Native Preprocess. This stage is performed for both methods:
	VPRINT_CMD("* Native Preprocess: \n", m_cpp_cmd);
	ret = system(m_cpp_cmd.c_str());
	check_error(ret, CPP_ERROR);
	
	if (m_method.compare("asm-sentences") == 0) {
		//Cross Preprocess:
		VPRINT_CMD("* Cross Preprocess: \n", m_cross_cpp_cmd);
		ret = system(m_cross_cpp_cmd.c_str());
		check_error(ret, CPP_ERROR);
		//Native Parser:
		VPRINT_CMD("* Native Parser: \n", m_parser_cmd);
		ret = system(m_parser_cmd.c_str());
		check_error(ret, PARSER_ERROR);
		//Cross Parser:
		VPRINT_CMD("* Cross Parser: \n", m_cross_parser_cmd);
		ret = system(m_cross_parser_cmd.c_str());
		check_error(ret, PARSER_ERROR);
		//Native Builder:
		VPRINT_CMD("* Native XCBuilder: \n", m_xc_cmd);
		ret = system(m_xc_cmd.c_str());
		check_error(ret, BUILDER_ERROR);
		//Cross Builder:
		VPRINT_CMD("* Cross XCBuilder: \n", m_cross_xc_cmd);
		ret = system(m_cross_xc_cmd.c_str());
		check_error(ret, BUILDER_ERROR);
		//Cross compile:
		VPRINT_CMD("* Cross compiler: \n", m_cross_cmd);
		ret = system(m_cross_cmd.c_str());
		check_error(ret, CROSS_COMPILER_ERROR);
		//Asm Analyzer
		VPRINT_CMD("* ASM Analyzer: \n", m_analyzer_cmd);
		ret = system(m_analyzer_cmd.c_str());
		check_error(ret, ASM_ANALYZER_ERROR);
		//Compilation:
		VPRINT_CMD("* Compiler command: \n", m_compiler_cmd);
		ret = system(m_compiler_cmd.c_str());
		check_error(ret, COMPILER_ERROR);
	}
	else if(m_method.compare("op-cost")==0) { //Operator costs
		//Parsing:
		if(m_time) m_op_cost_cmd+=" -t";
		if(m_icache) m_op_cost_cmd+=" -i";

//		m_op_cost_cmd+=" -t -i";
		VPRINT_CMD("* Op Cost Parser: \n", m_op_cost_cmd);
		ret = system(m_op_cost_cmd.c_str());
		check_error(ret, OP_COST_ERROR);
		//Compilation:
		VPRINT_CMD("* Compiler command: \n", m_compiler_cmd);
		ret = system(m_compiler_cmd.c_str());
		check_error(ret, COMPILER_ERROR);
	}
	else if(m_method.compare("none")==0){
		//Parsing:
		VPRINT_CMD("* Op Cost Parser: \n", m_op_cost_cmd);
		ret = system(m_op_cost_cmd.c_str());
		check_error(ret, OP_COST_ERROR);
		VPRINT_CMD("* Compiler command: \n", m_compiler_cmd);
		ret = system(m_compiler_cmd.c_str());
		check_error(ret, COMPILER_ERROR);
	}
	else{
		cerr<<"Error: Estimation method must be provided."<<endl;
		cerr<<" Try 'scope-gcc --help' for more information."<<endl;
		exit(1);
	}
	
	if (m_preserve_files == false) {
		remove_tmp_files();
	}
		
	return ret;
}



/*******************************************************************/
/*                        Private Methods                          */
/*******************************************************************/
/*!
 * \fn command_analyzer_class::make_filenames(int num_file)
 * \brief Makes the names of the auxiliar files to compile the original user file
 * 	Example: \n
 * 		m_input_file = coder.cpp \n
 * 		m_cpp_file = prsd_coder.ii \n
 * 		m_xml_file = prsd_coder.xml \n
 * 		m_marked_file = asm_prsd_coder.ii \n
 * 		m_asm_file = asm_prsd_coder.s \n
 * 		m_annotated_file = uc_prsd_coder.ii \n
 * \return 0 = success
 */
void command_analyzer_class::make_filenames(void) {
	//Preprocessed files:
	if (m_language.compare("c") == 0) {
		change_file_ext(m_cpp_file, m_input_file, "i");
	}
	else { //c++
		change_file_ext(m_cpp_file, m_input_file, "ii");
	}
	add_prefix(m_cpp_file, "prsd_");
	m_cross_cpp_file = m_cpp_file;
	add_prefix(m_cross_cpp_file, "cross_");
	//xml file:
	change_file_ext(m_xml_file, m_cpp_file, "xml");
	m_cross_xml_file = m_xml_file;
	add_prefix(m_cross_xml_file, "cross_");
	//Marked file:
	m_marked_file = m_cpp_file;
	add_prefix(m_marked_file, "asm_");
	m_cross_marked_file = m_marked_file;
	add_prefix(m_cross_marked_file, "cross_");
	//Assemble file:
	change_file_ext(m_asm_file, m_marked_file, "s");
	//Annotated file:
	if (m_language.compare("c") == 0) {
		change_file_ext(m_annotated_file, m_cpp_file, "c");
	}
	else { //c++
		change_file_ext(m_annotated_file, m_cpp_file, "cpp");
	}
	add_prefix(m_annotated_file, "uc_");
	//Table file:
	change_file_ext(m_asm_info_file, m_input_file, "xml");
	change_file_ext(m_asm_header_file, m_input_file, "h");
	add_prefix(m_asm_header_file, "uc_header_");
	add_prefix(m_asm_info_file, "table_");
}


void command_analyzer_class::make_commands(void) {
	make_op_cost_cmd();
	make_cpp_cmd();
	make_parser_cmd();
	make_builder_cmd();
	make_cross_cmd();
	make_analyzer_cmd();
	make_compiler_cmd();
	make_clean_cmd();
}


void command_analyzer_class::make_op_cost_cmd(void) {
	m_op_cost_cmd = "opcost ";
	m_op_cost_cmd += "-s -c ";
	m_op_cost_cmd += m_cpu;
	m_op_cost_cmd += " -o ";
	m_op_cost_cmd += m_annotated_file;
	m_op_cost_cmd += " ";
	m_op_cost_cmd += m_cpp_file;
}


/*!
 * \fn command_analyzer_class::make_cpp_cmd(void)
 * \brief Makes the command to execute the preprocessor
 */
void command_analyzer_class::make_cpp_cmd(void) {
	size_t found;
	//Example: "g++ -E -isystem $SCOPE_HOME/include -include scope.h"

	if (m_language.compare("c") == 0) {
		m_cpp_cmd = GCC_PATH "gcc -E ";
	}
	else { //c++
		m_cpp_cmd = GCC_PATH "g++ -E ";
	}
	//SystemC:
	m_cpp_cmd += "-isystem ";
	m_cpp_cmd += m_systemc;
	m_cpp_cmd += "/include ";
	//API include folder:
	m_cpp_cmd += "-isystem ";
	m_cpp_cmd += m_scope_home;
	m_cpp_cmd += "/include/";
	if (m_api.compare("ucos") == 0) {
		m_cpp_cmd += "ucos ";
	}
	else if (m_api.compare("ucos/posix") == 0) {
		m_cpp_cmd += "ucos ";
		m_cpp_cmd += "-isystem ";
		m_cpp_cmd += m_scope_home;
		m_cpp_cmd += "/include/posix ";
	}
	else { //default
		m_cpp_cmd += "posix ";
	}

	if (m_hw == true) {
		m_cpp_cmd += "-isystem ";
		m_cpp_cmd += m_scope_home;
		m_cpp_cmd += "/include/hw ";
	}
	//SCoPE include folder:
	m_cpp_cmd += "-isystem ";
	m_cpp_cmd += m_scope_home;
	m_cpp_cmd += "/scope ";
	m_cpp_cmd += "-include scope.h ";
	//If op-cost, include cache info:
	if (m_method.compare("asm-sentences") != 0) {
		m_cpp_cmd += "-include rtos/kernel/uc_icache_line.h ";
	}
	m_cpp_cmd += m_original_opt;
	m_cpp_cmd += " ";
	remove_option(m_cpp_cmd, 'o');
	remove_option(m_cpp_cmd, 'l');
	m_cpp_cmd += "-o ";
	m_cpp_cmd += m_cpp_file;
	m_cpp_cmd += " ";
	m_cpp_cmd += m_input_file;
	//For cross file:
	m_cross_cpp_cmd = m_cross_compiler;
	m_cross_cpp_cmd += " -E ";

	m_cross_cpp_cmd += m_original_opt;
	// "-fvisibitility is a non-valid option for some compilers (arm-linux-gcc)
	found = m_cross_cpp_cmd.find("-fvisibility=protected");
	if(found != string::npos){
		m_cross_cpp_cmd.erase(found,strlen("-fvisibility=protected "));
	}

	m_cross_cpp_cmd += " ";
	remove_option(m_cross_cpp_cmd, 'o');
	remove_option(m_cross_cpp_cmd, 'l');
	m_cross_cpp_cmd += "-o ";
	m_cross_cpp_cmd += m_cross_cpp_file;
	m_cross_cpp_cmd += " ";
	m_cross_cpp_cmd += m_input_file;
	
}


/*!
 * \fn command_analyzer_class::make_parser_cmd(void)
 * \brief Makes the command to parse the preprocessed file (native)
 */
void command_analyzer_class::make_parser_cmd(void) {
	//For native file;
	m_parser_cmd = "parser -r ";
	m_parser_cmd += m_cpp_file;
	m_parser_cmd += " -o ";
	m_parser_cmd += m_xml_file;
	//For cross file:
	m_cross_parser_cmd = "parser ";
	m_cross_parser_cmd += m_cross_cpp_file;
	m_cross_parser_cmd += " -o ";
	m_cross_parser_cmd += m_cross_xml_file;
}


void command_analyzer_class::make_builder_cmd(void) {
	//For native file;
	m_xc_cmd = "xcbuilder ";
	m_xc_cmd += m_xml_file;
	m_xc_cmd += " -o ";
	m_xc_cmd += m_marked_file;
	//For cross file:
	m_cross_xc_cmd = "xcbuilder ";
	m_cross_xc_cmd += m_cross_xml_file;
	m_cross_xc_cmd += " -o ";
	m_cross_xc_cmd += m_cross_marked_file;
}


/*!
 * \fn command_analyzer_class::make_cross_cmd(void)
 * \brief Makes the command to cross-compile the source file
 */
void command_analyzer_class::make_cross_cmd(void) {
	m_cross_cmd = m_cross_compiler;
	m_cross_cmd += " -S -w ";
	switch(m_optmizations){
		case 0:
			m_cross_cmd += "-O0 ";
			break;
		case 1:
			m_cross_cmd += "-O1 ";
			break;
		case 2:
			m_cross_cmd += "-O2 ";
			break;
		case 3:
			m_cross_cmd += "-O3 ";
	}
	m_cross_cmd += m_cross_marked_file;
	m_cross_cmd += " -o ";
	m_cross_cmd += m_asm_file;
}


void command_analyzer_class::make_analyzer_cmd(void) {
	m_analyzer_cmd = "asmanalyzer ";
	if (m_large_cache == true) {
		m_analyzer_cmd += " -r ";
	}
	m_analyzer_cmd += " -a ";
	m_analyzer_cmd += m_asm_file;
	m_analyzer_cmd += " -s ";
	m_analyzer_cmd += m_marked_file;
	m_analyzer_cmd += " -o ";
	m_analyzer_cmd += m_annotated_file;
	m_analyzer_cmd += " -m ";
	m_analyzer_cmd += m_asm_info_file;
	m_analyzer_cmd += " -x ";
	m_analyzer_cmd += m_asm_header_file;
	if(m_time)
		m_analyzer_cmd += " -t ";
	if(m_dcache)
		m_analyzer_cmd += " -d ";
	if(m_icache)
		m_analyzer_cmd += " -i ";


	
}


/*!
 * \fn command_analyzer_class::make_compiler_command(void)
 * \brief Makes the command to compile the file
 */
void command_analyzer_class::make_compiler_cmd(void) {
	if (m_language.compare("c") == 0) {
		m_compiler_cmd =  GCC_PATH  "gcc -std=c99 ";
	}
	else { //c++
		m_compiler_cmd =  GCC_PATH  "g++ ";
	}
	m_compiler_cmd += m_original_opt;
	if (m_method.compare("asm-sentences") == 0) {
		m_compiler_cmd += " -include ";
		m_compiler_cmd += m_asm_header_file;
		m_compiler_cmd += " ";
	}
	if(m_pg == true){
		m_compiler_cmd += " -finstrument-functions ";
	}
	m_compiler_cmd += " -o ";
	m_compiler_cmd += m_out_file;
	m_compiler_cmd += " ";
	m_compiler_cmd += m_annotated_file;
}


/*!
 * \fn command_analyzer_class::make_clean_command(void)
 * \brief Makes the command to remove temporal files
 */
void command_analyzer_class::make_clean_cmd(void) {
	m_clean_cmd = "rm -rf ";
	m_clean_cmd += m_cpp_file;
	m_clean_cmd += " ";
	m_clean_cmd += m_cross_cpp_file;
	m_clean_cmd += " ";
	m_clean_cmd += m_xml_file;
	m_clean_cmd += " ";
	m_clean_cmd += m_cross_xml_file;
	m_clean_cmd += " ";
	m_clean_cmd += m_marked_file;
	m_clean_cmd += " ";
	m_clean_cmd += m_cross_marked_file;
	m_clean_cmd += " ";
	m_clean_cmd += m_asm_file;
	m_clean_cmd += " ";
	m_clean_cmd += m_asm_info_file;
	m_clean_cmd += " ";
	m_clean_cmd += m_annotated_file;
	m_clean_cmd += " ";
	m_clean_cmd += m_asm_header_file;
}


/***********************************************************************
 *                 Auxiliar functions
 ***********************************************************************/

void command_analyzer_class::remove_tmp_files(void) {
	int ret;
//	cout << "Clean command line: " << m_clean_command_line << endl;
	ret = system(m_clean_cmd.c_str());
	if (ret < 0) {
		fprintf(stderr, "cleaner has returned an error\n");
		exit(-1);
	}
}


/*!
 * \fn command_analyzer_class::change_file_ext(const char *old_file, char *new_file, char *new_ext)
 * \brief Changes the extension of the indicated file with the new extension
 * \param old_file name of the original file
 * \param new_file buffer to write the new file name
 * \param new_ext new file extension
 */
void command_analyzer_class::change_file_ext(char *new_file, const char *old_file, const char *new_ext) {
	const char *last_dot = strrchr(old_file, '.');
	size_t name_size = (size_t)(last_dot - old_file);
	strncpy(new_file, old_file, name_size);
	new_file[name_size] = '\0';
	strcat(new_file, ".");
	strcat(new_file, new_ext);
}


/*!
 * \fn command_analyzer_class::change_file_ext(string &new_file, string &old_file, const char *new_ext)
 * \brief Changes the extension of the indicated file with the new extension
 * \param old_file name of the original file
 * \param new_file buffer to write the new file name
 * \param new_ext new file extension
 */
void command_analyzer_class::change_file_ext(string &new_file, string &old_file, const char *new_ext) {
	int last_dot;
	new_file = old_file;
	last_dot = new_file.rfind('.');
	new_file.replace(last_dot+1, new_file.length(), new_ext);
}


void command_analyzer_class::add_prefix(char *file, const char *prefix) {
	char *buffer = (char *)malloc(MAX_LINE_SIZE);
	char *slash = strrchr(file, '/');
	if (slash == NULL) {
		//file has been introduced without path
		strcpy(buffer, prefix);
		strcat(buffer, file);
	}
	else {
		//file has been introduced with path
		*slash = '\0';
		strcpy(buffer, file);
		strcat(buffer, "/");
		strcat(buffer, prefix);
		strcat(buffer, slash+1);
	}
	strcpy(file, buffer);
	free (buffer);
}


void command_analyzer_class::add_prefix(string &file, const char *prefix) {
	string buffer;
	int slash = file.rfind('/');
	if (slash == string::npos) {
		//slash not found. Input file has no path
		buffer = prefix;
		buffer += file;
	}
	else {
		//input file has path
		buffer.assign(file, 0, slash+1);
		buffer += prefix;
		buffer.append(file, slash+1, file.size());
	}
	file.swap(buffer);
}


/*!
 * \fn command_analyzer_class::remove_option(char *original_options, char option)
 * \brief Removes the indicated option from a string
 * \param string string to analyze
 * \param option option to remove (for example, 'l' removes all -l options)
 */
void command_analyzer_class::remove_option(char *str, char option) {
	int i;
	char *p = str;
	char opt[4];
	opt[0] = ' ';
	opt[1] = '-';
	opt[2] = option;
	opt[3] = '\0';
	do {
		p = strstr(p, opt);
		if (p == NULL) {
			break;
		}
		else {
			p[0] = ' ';
			p[1] = ' ';
			p[2] = ' ';
			if (p[3] == ' ') {
				i = 4;
			}
			else {
				i = 3;
			}
			while (p[i] != ' ' && p[i] != '\0') {
				p[i] = ' ';
				i++;
			}
		}
	} while (true);
}


void command_analyzer_class::remove_option(string &str, char option) {
	int i, length;
	int p = 0;
	char opt[4];
	opt[0] = ' ';
	opt[1] = '-';
	opt[2] = option;
	opt[3] = '\0';
	p = str.find(opt);
	while (p != string::npos) {
		//Remove current option:
		if (str[p+3] == ' ') {
			i = p+4;
			length = 4;
		}
		else {
			i = p+3;
			length = 3;
		}
		//Search the end of the option (whitespace or end of string):
		while (str[i] != ' ' && str[i] != '\0') {
			i++;
		}
		length += (i - p);
		str.erase(p, length);
		//Move to the next option:
		p = str.find(opt);
	}
}


void command_analyzer_class::check_error(int ret, int error) {
	if (ret == 0) {
		return;
	}
	switch (error) {
		case CPP_ERROR:
			fprintf(stderr, "Preprocessor has returned an error\n");
			break;
		case PARSER_ERROR:
			fprintf(stderr, "Parser has returned an error\n");
			break;
		case BUILDER_ERROR:
			fprintf(stderr, "XCBuilder has returned an error\n");
			break;
		case CROSS_COMPILER_ERROR:
			fprintf(stderr, "Cross-compiler has returned an error\n");
			break;
		case ASM_ANALYZER_ERROR:
			fprintf(stderr, "ASMAnalyzer has returned an error\n");
			break;
		case COMPILER_ERROR:
			fprintf(stderr, "Compiler has returned an error\n");
			break;
		default:
			fprintf(stderr, "Unknown Error\n");
			break;
	}
	if (m_preserve_files == false) {
		remove_tmp_files();
	}
	exit(1);
}

void command_analyzer_class::print_help() {
	
printf( "This is the SCoPE compiler.  Usage:\n\n");
printf( "\tscope-gcc [options] [gcc-options] --scope-method=METHOD input-file\n\n");
printf( "Options:\n\n");
printf( "\t--help\t\t\t\t\tPrint scope-gcc help\n");
printf( "\t--scope-cpu=CPU_NAME\t\t\tName of the cpu where the code is expected to work. For estimation purposes\n");
printf( "\t--scope-api=API\t\t\t\tRTOS API applied: 'posix' for POSIX API, 'ucos' for ucos OS, 'ucos/posix' for doble API\n");
printf( "\t--scope-verbose\t\t\t\tPrint intermediate parsing commands\n");
printf( "\t--scope-preserve-files\t\t\tPreserve intermediate parsing files\n");
printf( "\t--scope-language=LANGUAGE\t\tCode language: 'c', or 'c++'. Default value selected considering the input file extension\n");
printf( "\t--scope-method=METHOD\t\t\tSW Estimation method to be applied:\n");
printf( "\t\t\t\t\t\t\t'op-cost' for source code analysis \n");
printf( "\t\t\t\t\t\t\t'asm-sentences' for binary-based analysis\n");
printf( "\t\t\t\t\t\t\t'none' if no annotation is required. Only RTOS related transformations are applied\n");
printf( "\t--scope-crosscompiler=CROSSCOMPILER\tName of the cross compiler executable. Required for 'asm-sentences' method\n");
//printf( "\t--scope-optimizations\t\t\tCross-compile with -O2. Otherwise, -O0 is applied\n");
printf( "\t--scope-notime\t\t\t\tAvoids annotating time information.\n");
printf( "\t--scope-nodcache\t\t\t\tAvoids annotating data cache marks.\n");
printf( "\t--scope-noicache\t\t\t\tAvoids annotating instruction cache marks. Only for method=asm-sentences\n\n");
printf( "\t--scope-noannotate\t\t\t\tAvoids all annotations. Used to replace only POSIX names\n\n\n");
printf( "\t\t\t\t\t\t\tImplies --scope-notime --scope-nodcache and --scope-noicache \n");
printf( "\t--scope-pg\t\t\t\tGenerates SCoPE profiling information\n");
}

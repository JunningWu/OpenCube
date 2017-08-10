//==========================================================================
//	original_options.h
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
/**
 * \file command_line.h
 * \brief Class definition for command_analyzer class
 */
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <string>
#include "defs.h"
using namespace std;

#define GCC_COMMAND_OPTIONS "-a::b::cd::e::f::g::h::i::j::k::l:m::n::o:p::q::r::s::t::u::v::w::x::y::z::A::B::C::D::E::F::G::H::I:J::K::L:M::N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"

class asm_info_class;
class config_class;

/**
 * \class command_analyzer_class
 * \brief Provides facilities to compile user files
 * This class analizes the original command line passed to gcc (g++),
 * extracting options and source files. It makes the names of temporal files
 * during compilation process and all necessary commands to preprocess,
 * parse and compile sources.\n
 * Compilation consists on 6 steps:\n
 * 		1 - Preprocess source file \n
 * 		2 - Parsing to obtain the equivalent xml file \n
 * 		3 - Rebuild code with asm marks \n
 * 		4 - Cross-compile for target architecture \n
 * 		5 - Analyze assemble code, characterize basic blocks and backannotate values in 
 * 			preprocessed original file \n
 * 		6 - Native compilation \n
 * During the process, 5 temporal files are created: \n
 * 		1 - example.ii \n
 * 		2 - example.xml \n
 * 		3 - uc_example.ii \n
 * 		4 - uc_example.s \n
 * 		5 - prsd_example.ii \n
 */
class command_analyzer_class {
public:
	command_analyzer_class();
	~command_analyzer_class();

	void analyze_command_line(int argc, char *argv[]);
	int run(void);

private:
	void make_filenames(void);
	void make_commands(void);
	void make_op_cost_cmd(void);
	void make_cpp_cmd(void);
	void make_parser_cmd(void);
	void make_builder_cmd(void);
	void make_cross_cmd(void);
	void make_analyzer_cmd(void);
	void make_compiler_cmd(void);
	void make_clean_cmd(void);
	
	void change_file_ext(char *new_file, const char *old_file, const char *new_ext);
	void change_file_ext(string &new_file, string &old_file, const char *new_ext);
	void add_prefix(char *file, const char *prefix);
	void add_prefix(string &file, const char *prefix);
	void remove_option(char *str, char option);
	void remove_option(string &str, char option);
	void remove_tmp_files(void);
	void check_error(int ret, int error);

	void print_help();
	
	//SCoPE options:
	string m_scope_home;			///< Path to SCoPE (from env SCOPE_HOME)
	string m_systemc;
	string m_cpu;					///< Processor (arm926t, ...). Check $SCOPE_HOME/config/processors.xml
	string m_method;				///< Estimation method (operators, asm sentences, ...)
	string m_cross_compiler;		///< Prefix of cross-compiler (eg. arm-linux-)
	string m_api;					///< SCoPE API (posix, ucos, ...)
	bool m_verbose;
	bool m_preserve_files;
	bool m_large_cache;				///< Indicates if Rosenstiel method for cache modeling is applied
	string m_language;				///< c,c++
	int m_optmizations;			///< Indicates if estimation should consider compiler optimzations
	bool m_hw;				///< Indicates if code includes HW modules, and hw includes must be added
	bool m_pg;				///< Activate the SCoPE profiling functons. Provokes compiling with -finstrument-function the SW code

	bool m_time;				///< Activate time annotation
	bool m_dcache;				///< Activate data cache annotation
	bool m_icache;				///< Activate instruction cache annotation
		
	string m_input_file;			///< Input file
	string m_out_file;				///< Output file
	string m_original_opt;			///< String with all options and arguments from user compilation sentence
	
	//Operator costs:
	string m_op_cost_cmd;			///< Command to estimate with the operators cost method
	
	//Preprocessor strings:
	string m_cpp_cmd;				///< Command to preprocess the source file
	string m_cpp_file; 				///< Name of the preprocessed file for native compilation
	string m_cross_cpp_cmd;			///< Command to preprocess the source file
	string m_cross_cpp_file; 		///< Name of the preprocessed file for native compilation
	
	//Parser strings:
	string m_parser_cmd;			///< String with the sentence to parse the file
	string m_xml_file;				///< Parsed file for simulation (in xml format)
	string m_cross_parser_cmd;		///< String with the sentence to parse the file
	string m_cross_xml_file;		///< Parsed file for simulation (in xml format)
	
	//XCBuilder strings:
	string m_xc_cmd;
	string m_marked_file;
	string m_cross_xc_cmd;
	string m_cross_marked_file;
	
	//Cross compiler strings:
	string m_cross_cmd;				///< Command to cross-compile the source code
	string m_asm_file;				///< Assmeble file for target architecture
	
	//ASM info:
	string m_analyzer_cmd;
	string m_asm_info_file;			///< File with the table "block - #instr"
	string m_asm_header_file;		///< Header file generated by the asm analyzer
	string m_annotated_file;		///< Annotated file ready to compile
	
	//Compiler strings:
	string m_compiler_cmd;			///< String with the sentence to compile the parsed file

	string m_clean_cmd;				///< Command to remove temporal files
};

#endif

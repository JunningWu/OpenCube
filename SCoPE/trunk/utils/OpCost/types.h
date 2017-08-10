//==========================================================================
//	types.h
//	Author: David Quijano, Juan Castillo, Héctor Posadas
//	Date: mié may 28 2008
//	Description:
//==========================================================================
//  The following code is derived, directly or indirectly, from PERFidiX,
//  released June 30, 2006. Copyright (C) 2006 Design of Systems on Silicon(DS2)
//  The Initial Developer of the Original Code is the University of Cantabria
//  for Design of Systems on Silicon (DS2)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License (GPL) or GNU Lesser General Public License(LGPL) as published by
//  the Free Software Foundation; either version 2 of the License, or
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
//  For more information about SCope you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCope, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================
/*!
 * \file types.h
 * \brief Typedefs for parser code
 */
#ifndef UC_TYPES_H
#define UC_TYPES_H

#include <stdio.h>

#define MAX_FILENAME_SIZE		256
#define MAX_NESTED_BLOCKS		20
#define MAX_ASM_SIZE			30

/*!
 * \enum block_type_t
 * \brief Possible types of basic blocks
 */
typedef enum {
	FOR_BLOCK,
	OTHER_BLOCK
} block_type_t;


/*!
 * \struct block_s
 * \brief Various characteristics of a basic block of code
 */
typedef struct block_s {
	block_type_t block_type;	///< Type of block (for, other)
	int init_id;				///< Identifier of the mark introduced in the initialization field of the for sentencence
	int inc_id;					///< Identifier of the mark introduced in the increment field of the for sentencence
} block_t;


/*!
 * \struct parsing_options_s
 * \brief Options for parsing
 */
struct parsing_options_s {
	bool verbose_mode;			///< Indicates detailed output
	int current_pass;			///< In case of multi-pass parsing, indicates the current pass number (loaded from command line)
	bool show_marked;			///< Indicates if parser should print error marks
	bool analyze_section;		///< Indicates if code should be parsed (system headers should not be parsed; ej. stdio.h)
	bool annotate_time;		///< Indicates if the code should be annotated adding timing information
	bool parse_syscalls;		///< Indicates if parser should change syscalls by adding a prefix "uc_" in order to call SCoPE API and not native API
	bool c_keywords; 			///< Command line check to print each keywork in stdout
	bool echo_line_number; 		///< Display line number
	bool echo_line_text; 		///< Command line check to print the number of the line analyzed
	char *proc_type;			///< The target processor to simulate (obtained from SCOPE_CPU environment variable)
	bool instruction_cache;		///< Indicates if target processor presents instruction cache
	char *compiler_path;			///< The path of Scope (obtained from SCOPE_HOME environment variable)
	char *input_file;			///< Input file name
	char *output_file;			///< Output file name
	bool provide_out_file;		///< Indicates if user has provided an output file (with -o option)
	FILE *fd_in;				///< Input file descriptor
	FILE *fd_out;				///< Output file descriptor
};
typedef struct parsing_options_s parsing_options_t;


/*!
 * \struct parsing_struct_s
 * \brief Status of the current parsing
 */
struct parsing_struct_s {
	int gramm_status;					///< 0: not in function, 1: in function
	unsigned int block_id;				///< Identifier of current block (incresed sequentially)
	unsigned int for_id;				///< Identifier of for_mark (incresed sequentially)
	int segment_time;					///< Current segment time
	int segment_instr;					///< Current segment instructions
	int total_instr;					///< Total number of instructions
	char *asm_sentence;					///< The asm sentence to introduce in the code as annotation (architecture dependent)
	char *for_mark;						///< The for_mark text to introduce in the code
	block_t block[MAX_NESTED_BLOCKS];	///< A stack with relevant data for each nested block
	int current_block;					///< Index of current nested block to index the block stack (if 0, we are outside any function).
	unsigned int current_address;
};
typedef struct parsing_struct_s parsing_struct_t;


/*!
 * \struct asm_sentences_s
 * \brief Asm jump sentences for  different architectures
 */
struct asm_sentences_s {
	char asm_arm[MAX_ASM_SIZE];			///< The asm jump sentence for ARM architecture
	char asm_mips[MAX_ASM_SIZE];		///< The asm jump sentence for MIPS architecture
};
typedef struct asm_sentences_s asm_sentences_t;

#define DEFAULT_PASS		0
#define FIRST_PASS			1
#define FIRST_PASS_CROSS	2
#define SECOND_PASS			3

#endif

//==========================================================================
//	CxxAnnotation.hxx
//	Author: Hector Posadas, Juan Castillo, David Quijano
//	Date: 12-12-2007
//	Description: Static Parser for execution time estimation and cache modeling.
//  Original grammar obtained from http://willink.me.uk/projects/fog/index.html
//  These files comprises a pre-built demonstration of the superset C++ grammar
//  from FOG.
//==========================================================================
//  Copyright (C) 2008 Design of Systems on Silicon(DS2)
//  Main developer: University of Cantabria
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
//  For more information about SCoPE you can visit
//  http://www.teisa.unican.es/scope or write an e-mail to
//  scope@teisa.unican.es or a letter to SCoPE, GIM - TEISA, University
//  of Cantabria, AV. Los Castros s/n, ETSIIT, 39005, Santander, Spain
//==========================================================================

/*!
 * \file CxxAnnotation.hxx
 * \brief Static Parser for execution time estimation and cache modeling
 */
#ifndef CXX_ANNOTATION_HXX
#define CXX_ANNOTATION_HXX

#include "operators.h"

#define CACHE_SIZE		16384
#define WORD_SIZE		4
#define WORDS_PER_LINE		8
#define WAY_ASSOCIATIVE		64
#define SET_SIZE WORD_SIZE*WORDS_PER_LINE*WAY_ASSOCIATIVE
#define NUM_SETS CACHE_SIZE/(SET_SIZE)
#define LINE_SIZE		WORD_SIZE*WORDS_PER_LINE

extern char last_token[3200];

void enter_annotation(bool linked_mark, bool inc);
void annotate_for(void);
void new_block(const char *token);
void put_block_start();
void put_handler_start();
void put_function_start();
void put_block_end();
void block_end();
void save_init_for_cost(void);
void restore_init_for_cost(void);

void put_end_function(int i=0);
void start_compound_statement();
void function_decl();
void end_statement();
void add_operator_cost(int id);
void mark_literal();
void mark_identifier();
void register_id();
void using_scope_operator();
void load_parsing_files(char* str);
int push_file(char *file_i);
void itoa(int n, char *str);

#endif

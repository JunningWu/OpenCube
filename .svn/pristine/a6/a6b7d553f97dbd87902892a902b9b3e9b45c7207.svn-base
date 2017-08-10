//==========================================================================
//	CxxAnnotation.cxx
//	Author: Hector Posadas, Juan Castillo, David Quijano
//	Date: 12-12-2007
//	Description: Static Parser for execution time estimation and cache modeling.
//  Original grammar obtained from http://willink.me.uk/projects/fog/index.html
//  These files comprises a pre-built demonstration of the superset C++ grammar
//  from FOG.
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
 * \file CxxAnnotation.cxx
 * \brief Static Parser for execution time estimation and cache modeling
 */

//System headers
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
using namespace std;

//Local headers
#include "operators.h"
#include "CxxAnnotation.h"
#include "CxxCosts.h"
#include "CxxToken.h"
#include "CxxSearchContext.h"
#include "types.h"

asm_sentences_t asm_sentences = {
	{"b uc_mark_"},
 	{"b uc_mark_"}
};

char for_mark[] = "for_mark_";

FILE * yyget_out();
YACC_MARK_TYPE mark(void);

extern parsing_struct_t parsing_struct;
extern parsing_options_t parsing_options;

extern char *buffer;
extern char last_token[3200];
extern bool ic_low_level_model;
// extern bool instruction_cache;
extern CxxToken *primed_tokens[3];
extern CxxToken *plusToken;
extern CxxToken *minusToken;

extern int searching_instr[20];
extern int nested_search_ind;

/*
 * These global variables are used to save current parameters when a parsing search
 * is performed to resolve ambiguities (start_search()). Values are restored when a
 * new try is done (advance_search()). Definitive values are annotated when the
 * search is finished (end_search());
 */
int searching_time[20];				//!< Cost accumulated just before start the search (time)
int searching_instr[20];			//!< Cost accumulated just before start the search (instructions)
int searching_nested_loops[20];		//!< Number of nested loops just before start the search
int nested_search_ind = 0;			//!< Number of nested searchs (to access array)

/*
 * In a for(;;) statement, initialization cost must be added only once, not in every
 * new iteration. That means that we must keep the initialization cost in a variable,
 * while iteration body cost is being incremented. When the loop finishes, the iteration
 * cost is added and, once the loop is exited, the initialization cost is added:
 * for (init_cost ; cond_cost ; inc_cost) {
 *		..........;
 *		...body...;
 *		..........;
 *		add(cond_cost + inc_cost + body_cost); } //end of loop
 *		//Outside the loop:
 *		add(init_cost + FOR_cost);
 *
 * The backup variable must be an array because we can have nested loops. The parser
 * rules manage these arrays when a "for" statement is found.
 */
int init_for_time[10];			//!< Initialization cost for each loop (time)
int init_for_instr[10];			//!< Initialization cost for each loop (instructions)
int nested_for_ind = 0;			//!< Number of nested for loops (to access array)


/*!
 * \fn find_last_str(char *buffer, const char *token)
 * \brief Finds last occurrence of token in the code buffer
 * This function must be implemented manually since strrstr() does not exist
 * in the standard library
 * \param buffer Code buffer
 * \param token String to search
 * \param max buffer size where the search take place. It is optional
 * \return pointer to the match text, NULL otherwise
 */
char * find_last_str(char *buffer, const char *token, int max = -1) {
	int len, i, max_val;
	len = strlen(token);
	//strstr() detects the first occurrence. The last one must be searched manually:
	if(max > 0){
		max_val = max;
	}else{
		max_val = strlen(buffer);
	}
	for (i = max_val - strlen(token) ; strncmp(&buffer[i],token,len) && i >= 0 ; i--);
	if (i < 0) {
		return NULL;
	}
	return &buffer[i];
}


/*!
 * \fn error(char *str)
 * \brief Prints an error
 */
void error(char *str) {
	printf(" Error: %s\n", str);
}


/*!
 * \fn copy_str_n(char *buffer, char* tokens, int n)
 * \brief Modification of strncpy
 * This function differs from strncpy() because it copies
 * the '\0' character at the end of the string
 */
void copy_str_n(char *buffer, char* tokens, int n) {
	int i;
	for (i=0 ; i < n && tokens[i] != '\0' ; i++) {
		buffer[i] = tokens[i];
	}
	if (tokens[i] == '\0') {
		buffer[i] = '\0';
	}
}


/*!
 * \fn enter_annotation(bool linked_mark, bool inc)
 * \brief Enters the annotation into source code
 */
void enter_annotation(bool linked_mark, bool inc) {
// 	static int last_block_size = 8;
	int num_set;
	unsigned int start_address;
	int num_instr;
	int num_lines;
	int last_line_size;

	switch (parsing_options.current_pass) {
		case DEFAULT_PASS:
			/*
			 * This is the default pass. Parsing is performed in one step and annotations
			 * are based on the source operators cost readed from processor's xml file
			 */
			start_address = parsing_struct.current_address;
			num_instr = parsing_struct.segment_instr;
			num_lines = num_instr / 8;	//FIXME
			//It is assumed that the remainig instructions are loaded with the first
			//line of the next block
			if ((num_instr % 8) > 0) {
// 				num_lines++;
			}
			if(parsing_options.annotate_time){
				fprintf(yyget_out(), "uc_segment_time+=%d;uc_segment_instr+=%d;", parsing_struct.segment_time,
					parsing_struct.segment_instr);
			}
			if (parsing_options.instruction_cache && num_lines > 0) {
				fprintf(yyget_out(),"static uc_icache_line_t line_list_%d[]={", parsing_struct.block_id);

				//Annotate lines but the last one (to close the block):
				for (int i=0 ; i<num_lines-1 ; i++) {
					num_set = (start_address >> 5) & 0x00000007;
					fprintf(yyget_out(),"{%d,0},", num_set);
					start_address += (8*4);
				}

				//Annotate the last line and close the array:
				num_set = (start_address >> 5) & 0x00000007;
				last_line_size = num_instr % 8;
				if (last_line_size == 0) {
					last_line_size++;
				}
				fprintf(yyget_out(),"{%d,0}};", num_set);
				fprintf(yyget_out(),"for (int ppp=0;ppp<%d;ppp++) { if(line_list_%d[ppp].hit==0) {"
						"ic_insert_line(&(line_list_%d[ppp])); }}", num_lines, parsing_struct.block_id,
	  					parsing_struct.block_id);
			}
			//Increment count variables:
			parsing_struct.block_id++;
			parsing_struct.current_address += parsing_struct.segment_instr;
			parsing_struct.total_instr += parsing_struct.segment_instr;
			//Reset counters:
			parsing_struct.segment_time = 0;
			parsing_struct.segment_instr = 0;
			searching_time[nested_search_ind-1] = 0;
			searching_instr[nested_search_ind-1] = 0;
			break;

		case FIRST_PASS:
		case FIRST_PASS_CROSS:
			/*
			 * This is the first pass of the object code characterization. It introduces asm sentences
			 * to cross-compile the annotated code and extract the number of instructions between
			 * two consecutive marks.
			 * We can have two types of marks: normal and linked. The number of instructions of a linked
			 * mark must be increased or decreased with the instrucions of its linking mark. This technique is
			 * used to provide better results in the number of executed instructions.
			 */
			if (linked_mark == false) {
				fprintf(yyget_out(), "asm(\"%s%d__\");", parsing_struct.asm_sentence, parsing_struct.block_id);
			}
			else {
				fprintf(yyget_out(), "asm(\"%s%d_", parsing_struct.asm_sentence, parsing_struct.block_id);
				if (inc == false) {
					//This is a mark whose number of instructions must be decreased by the number of instructions
					//of its linked block. We add the id of the linked block and two '_' at the end.
					//The mark should result in something like this: uc_mark_100_3__ (mark id = 100, linked block id = 3)
					fprintf(yyget_out(), "%d__", parsing_struct.block[parsing_struct.current_block].init_id);
				}
				else {
					//This is a mark whose number of instructions must be increased by the number of instructions
					//of its linked block. We add the id of the linked block and one '_' at the end.
					//The mark should result in something like this: uc_mark_101_4_ (mark id = 101, linked block id = 4)
					fprintf(yyget_out(), "%d_", parsing_struct.block[parsing_struct.current_block].inc_id);
				}
				fprintf(yyget_out(), "\");", parsing_struct.asm_sentence, parsing_struct.block_id);
			}
			parsing_struct.block_id++;
			break;
		case SECOND_PASS:
			break;
		default:
			cerr << "#ERROR: Invalid pass number " << parsing_options.current_pass << endl;
	}

}


void annotate_for(void) {
	//buffer contains the arguments of the "for" sentence:
	// for example: buffer --> "(i=0 ; i<50 ; i++)"
	char *init, *inc;
	char *buffer_aux;
	char *for_mark;
	char *for_id;
	buffer_aux = (char *)malloc(256);
	for_id = (char *)malloc(64);

// 	printf("\t#AVISO: Lazo FOR detectado\n");
// 	printf("\t\tIndice de bloque actual: %d\n", parsing_struct.current_block);
// 	printf("\t\tMarca de inicializacion: %d\n", parsing_struct.for_id);
// 	printf("\t\tMarca de incremento: %d\n", parsing_struct.for_id+1);
	parsing_struct.block[parsing_struct.current_block].block_type = FOR_BLOCK;
	parsing_struct.block[parsing_struct.current_block].init_id = parsing_struct.for_id;
	parsing_struct.block[parsing_struct.current_block].inc_id = parsing_struct.for_id+1;
	for_mark = parsing_struct.for_mark;

	if (parsing_options.current_pass == FIRST_PASS_CROSS) {
		//Introduce the mark in the initialization field:
		init = strchr(buffer, ';');
		strcpy(buffer_aux, init);
		*init = '\0';
		strcat(buffer, ",");
		strcat(buffer, for_mark);
		itoa(parsing_struct.for_id, for_id);
		strcat(buffer, for_id);
		strcat(buffer, "_() ");
		strcat(buffer, buffer_aux);
	}
	parsing_struct.for_id++;

	if (parsing_options.current_pass == FIRST_PASS_CROSS) {
		//Introduce the other mark in the increment field:
		inc = strrchr(buffer, ')');
		*inc = '\0';
		strcat(buffer, ",");
		strcat(buffer, for_mark);
		itoa(parsing_struct.for_id, for_id);
		strcat(buffer, for_id);
		strcat(buffer, "_() ");
		strcat(buffer, ")");
	}
	parsing_struct.for_id++;
	free(buffer_aux);
	free(for_id);
}


/*!
 * \fn new_block(char *token)
 * \brief Puts a "basic block end" mark with block parameters in the code
 * \param token The token string which starts new block
 */
void new_block(const char *token) {
	char c; //Auxiliar character to insert the annotation
	char *last;
	char *buffer_aux;
	int num;
	bool for_loop = false;
// 	buffer_aux = (char *)malloc(64);
	//Search the last "token" inside the code buffer:
	num = strlen(buffer);
	do{
		last = find_last_str(buffer, token, num);
		if (last <= 0) {
			//Token not found
			return;
		}
		// Check that is a separate word: not an alfanum or _ char before or after the token
		if(!isalnum(last[-1]) && last[-1] != '_' && !isalnum(last[strlen(token)]) && last[strlen(token)] != '_'){
			break;
		}
		num = (int)last - (int)buffer;
	}
	while( 1 );
	if (strncmp(last, "for", 3) == 0) {
		for_loop = true;
	}
	c = *last; //Save the first character of the new block
	*last = '\0'; //Cut the code buffer just before new block
	fprintf(yyget_out(), "%s", buffer); //Dump code buffer until the begining of the new block
	enter_annotation(false, false); //Annotate a common mark
	//Restore cutted character and dump the rest of code buffer (basically, the sentence key word):
	*last = c;
	fprintf(yyget_out(), "%s", last);
	buffer[0] = '\0'; //Erase buffer
// 	free(buffer_aux);
}


/*!
 * \fn print_block(bool prev)
 * \brief Dump the code buffer to file
 * Prints the current buffer, except the last token if prev = true or last token is ';'
 * If last token is not printed it is placed at the beginning of the buffer
 *
 * \param prev If true, last token remains in code buffer (not printed)
 */
void print_block(bool prev) {
	char *last;
	char c;
	// Search last token:
	last = find_last_str(buffer, last_token);
	// Print block:
	if (last != 0) {
		if (*last != ';' || prev) { // Do not dump last token
			c = *last;
			*last = '\0';
			fprintf(yyget_out(), "%s", buffer);
			*last = c;
		}
		else {
			fprintf(yyget_out(), "%s", buffer);
			last = 0;
		}
	}
	else {
		fprintf(yyget_out(), "%s", buffer);
	}
	if (last != 0) { // Insert last token at the beginning of code buffer:
		copy_str_n(buffer, last, strlen(last)+1);
	}
	else { // Empty buffer:
		buffer[0] = '\0';
	}
}


/*!
 * \fn put_block_start()
 * \brief Puts a '{' at the beginning of a new block
 * Adds a '{' if the looping_statement does not have one in case the first token
 * of the looping_statement has been parsed.
 * All looping_statement are shut in curly brackets
 */
void put_block_start() {
	char *str;
// 	printf("put_block_start()\n");
	parsing_struct.current_block++;
	print_block(true);
	str = (char*)malloc(strlen(buffer)+2);
	str[0] = '{';
	strcpy(&str[1], buffer);
	strcpy(buffer, str);
	free(str);
}


/*!
 * \fn start_compound_statement()
 * \brief Puts a "function start" mark and initializes block parameters if required
 * Mark '+' if compound statement is a function, '-' otherwise
 */
void start_compound_statement() {
// 	printf("start_compound_statement()\n");
	fprintf(yyget_out(), "%s", buffer);
	//If we are entering a function, enter the annotation. The rest of compound statements are not
	//annotated at the beginning, but at the end.
	if (parsing_struct.gramm_status == 0 && last_token[0] == '{' && CxxSearchContext::current() ) {
		YACC_MARK_TYPE markIndex = mark();
		primed_tokens[0] = plusToken;
		primed_tokens[1] = 0;
		enter_annotation(false, false);
		add_operator_cost(FUNC_CALL);
		parsing_struct.gramm_status++;
	}
	else {
		primed_tokens[0] = minusToken;
		primed_tokens[1] = 0;
	}
	parsing_struct.current_block++;
// 	printf("\t#AVISO: Incrementando el indice de bloque\n");
// 	printf("\t\tIndice actual: %d\n", parsing_struct.current_block);
	buffer[0] = '\0';
}


/*!
 * \fn put_block_end()
 * \brief Adds a '}' if the looping_statement does not have one
 * All looping_statement are shut in curly brackets
 */
void put_block_end() {
	char *str;
// 	printf("put_block_end()\n");
	block_end();
	str = (char*)malloc(strlen(buffer)+2);
	str[0] = '}';
	strcpy(&str[1], buffer);
	strcpy(buffer, str);
	free(str);
}


/*!
 * \fn block_end()
 * \brief Puts a "basic block end" mark with block parameters in the code
 * Mark is only introduced if there have not been a previous return statement
 */
void block_end() {
	char *str, *last;
// 	printf("block_end()\n");
	parsing_struct.current_block--;
// 	printf("\t#AVISO: Decrementando el indice de bloque\n");
// 	printf("\t\tIndice actual: %d\n", parsing_struct.current_block);

	str = strstr(buffer, "return");
	last = find_last_str(buffer, last_token);
	print_block(false);
	if (str == 0 || str == last) {
		if (parsing_struct.block[parsing_struct.current_block].block_type == FOR_BLOCK) {
// 			printf("\t\t#AVISO: Este bloque corresponde a un FOR\n");
// 			printf("\t\tSu marca de enlace es: %d\n", parsing_struct.block[parsing_struct.current_block].inc_id);
			enter_annotation(true, true);
		}
		else {
			enter_annotation(false, false);
		}
	}
}


// Adds a '{' if the looping_statement does not have one if the looping_statement has not been started
// All looping_statement are shut in curly brackets
void put_handler_start() {
	strcat(buffer,"{");
}


/*!
 * \fn put_end_function(int i)
 * \brief Add a "function end" mark
 * \param i If i=1, comes from return keyword. If i=0, comes from function end (default)
 */
void put_end_function(int i) {
// 	printf("put_end_function()\n");
	// introducir anotacion si no ha habido un return antes
	if (i == 0) {
		parsing_struct.current_block--;
// 		printf("\t#AVISO: Decrementando el indice de bloque\n");
// 		printf("\t\tIndice actual: %d\n", parsing_struct.current_block);
		if (strstr(buffer,"return") == 0) {
			add_operator_cost(END_FUNC);
			print_block(false);
			if (parsing_struct.gramm_status < 1) {
				printf("/* Warning: Detecting and end function outside a function */\n");
//				increment_error_count();
			}
			enter_annotation(false, false);
		}
		parsing_struct.gramm_status--;
	}
	else {
		add_operator_cost(END_FUNC);
		print_block(false);
		enter_annotation(false, false);
	}
}


/*!
 * \fn save_init_for_cost(void)
 * \brief Saves the initialization cost in a for statement
 * Initialization cost must be added only once, so it is keeped
 * in a global variable until it is added at the end of the loop
 */
void save_init_for_cost(void) {
	if (nested_for_ind == 10) {
		cerr << "ERROR: Maximum nested loops overpassed" << endl;
		exit(-1);
	}
	init_for_time[nested_for_ind] = parsing_struct.segment_time;
	init_for_instr[nested_for_ind] = parsing_struct.segment_instr;
	parsing_struct.segment_time = 0;
	parsing_struct.segment_instr = 0;
	nested_for_ind++;
}


/*!
 * \fn restore_init_for_cost(void)
 * \brief Restores the initialization cost in a for statement
 * This value is added only once to the simulation global cost
 */
void restore_init_for_cost(void) {
	nested_for_ind--;
	parsing_struct.segment_time += init_for_time[nested_for_ind];
	parsing_struct.segment_instr += init_for_instr[nested_for_ind];
}


// Marks it is a function declaration
void function_decl() {
}


void end_statement() {
}


/*!
 * \fn add_operator_cost(int id)
 * \brief Adds the cost of an operator to the global count
 * \param id The executed operator
 */
void add_operator_cost(int id) {
	int op, time, instructions;
	if (id >= 43) {
		//Control statement:
		op = id - 43;
		time = get_control_op_time(op);
		instructions = get_control_op_instr(op);
	}
	else if (id >= 35) {
		//Access statement:
		op = id - 35;
		time = get_access_op_time(op);
		instructions = get_access_op_instr(op);
	}
	else {
		op = id;
		time = get_data_op_time(op, INT_TYPE_OP);
		instructions = get_data_op_instr(op, INT_TYPE_OP);
	}
	parsing_struct.segment_time += time;
	parsing_struct.segment_instr += instructions;
}


void mark_literal() {
}


void mark_identifier() {
}


void using_scope_operator() {
}

void itoa(int n, char *str) {
	string s;
	ostringstream oss;
	oss << n;
	s = oss.str();
	strcpy(str, s.c_str());
}

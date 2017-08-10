//==========================================================================
//	CxxLexing.hxx
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
 * \file CxxLexing.hxx
 * \brief Static Parser for execution time estimation and cache modeling
 */
#ifndef CXX_LEXING_HXX
#define CXX_LEXING_HXX

#define YY_STDINIT

//Local headers
// #include "CxxParsing.hxx"
#include "CxxToken.h"
#include "types.h"

/*
 * Global variables
 */
static CxxToken *yyToken;	//!< Token returned by lex

/*
 * Function declarations
 */
static CxxToken *make_character(const char *someText, size_t aLength);
static CxxToken *make_string(const char *someText, size_t aLength);
static CxxToken *make_identifier(const char *someText, size_t aLength);
static void make_line(const char *yyText, size_t yyLeng);
static CxxToken *make_literal_character(const char *someText, size_t aLength);
static CxxToken *make_narrow_character(const char *someText, size_t aLength);
static CxxToken *make_narrow_string(const char *someText, size_t aLength);
static CxxToken *make_number(const char *someText, size_t aLength);
static CxxToken *make_token(size_t aCxxToken);
static CxxToken *make_wide_character(const char *someText, size_t aLength);
static CxxToken *make_wide_string(const char *someText, size_t aLength);

void omit_attribute(char *yytext,int yyleng);
void detect_section();
void add_line();
void add_ws(char *yytext,int yyleng);
FILE * yyget_out(void);
// extern bool analyze_section;
extern parsing_options_t parsing_options;
// extern bool c_keywords;

/*
 * #defines for lexer actions
 */

#define LEX_SAVE_LINE(yyText, yyLeng) 													\
	make_line(yyText, yyLeng);


#define LEX_ASCII_TOKEN(a) 																\
	if (parsing_options.analyze_section == false) {										\
		fprintf(yyget_out(), "%s", yytext);												\
	}																					\
	else {																				\
		yyToken = make_token(a); 														\
		return true;																	\
	}


#define LEX_STATIC_TOKEN(a) 															\
	if (parsing_options.analyze_section == false) {										\
		fprintf(yyget_out(), "%s", yytext);												\
	}																					\
	else {																				\
		static CxxToken theToken(PARSE_TOKEN(a), no_type, yytext, yyleng); 				\
		yyToken = &theToken; 															\
		return true;																	\
	}


#define LEX_C_STATIC_TOKEN(a) 															\
	if (parsing_options.analyze_section == false) {										\
		fprintf(yyget_out(), "%s", yytext);												\
	}																					\
	else {																				\
		if (parsing_options.c_keywords) { 												\
			LEX_IDENTIFIER_TOKEN(yytext, yyleng) 										\
		} 																				\
		else { 																			\
			LEX_STATIC_TOKEN(a) 														\
		} 																				\
		return true;																	\
	}


#define LEX_ESCAPED_TOKEN(yyText, yyLeng) 												\
	if (parsing_options.analyze_section == false) {										\
		fprintf(yyget_out(), "%s", yytext);												\
	}																					\
	else {																				\
		yyToken = make_literal_character(yytext, yyleng); 								\
		return true;																	\
	}


#define LEX_CHARACTER_TOKEN(yyText, yyLeng) 											\
	if (parsing_options.analyze_section == false) {										\
		fprintf(yyget_out(), "%s", yyText);												\
	}																					\
	else {																				\
		yyToken = make_character(yyText, yyLeng); 										\
		return true;																	\
	}


#define LEX_STRING_TOKEN(yyText, yyLeng) 												\
	if (parsing_options.analyze_section == false) {										\
		fprintf(yyget_out(), "%s", yyText);												\
	}																					\
	else {																				\
		yyToken = make_string(yyText, yyLeng); 											\
		return true;																	\
	}


#define LEX_IDENTIFIER_TOKEN(yyText, yyLeng) 											\
	if (parsing_options.analyze_section == false) {										\
		fprintf(yyget_out(), "%s", yyText);												\
	}																					\
	else {																				\
		yyToken = make_identifier(yyText, yyLeng); 										\
		return true;																	\
	}


#define LEX_NUMBER_TOKEN(yyText, yyLeng) 												\
	if (parsing_options.analyze_section == false) {										\
		fprintf(yyget_out(), "%s", yyText);												\
	}																					\
	else {																				\
		yyToken = make_number(yyText, yyLeng); 											\
		return true;																	\
	}


#endif

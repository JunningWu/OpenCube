//==========================================================================
//	CxxLexing.cxx
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
 * \file CxxLexing.cxx
 * \brief Static Parser for execution time estimation and cache modeling
 */

#include <vector>

#ifdef FLEX_PP_CLASS
FLEX_PP_CLASS theLexer;
#define LEX_DOT theLexer .
#else
#define LEX_DOT
#endif
#include "types.h"

extern parsing_options_t parsing_options;

int line_number = 0;				//!< Number of current line

char *buffer;
char last_token[3200];

int yycount = 0;
int yyline = 0;
int yyClauseCount = 0;
void increment_error_count();
int yyinput ();

extern std::vector<char *> syscalls;
extern std::vector<char *> uc_syscalls;

int yywrap() {
	return 1;
}


inline char uc_yyinput() {
	char data;
	yycount++;
	//yyinput() function simply gets the next character from the input stream feeding lex
	data = yyinput();
	if (data == '\n') {
		yyline++;
	}
	if (data != -1) {
		fprintf(yyget_out(), "%c", data);
	}
	return data;
}


CxxToken *yylex_token() {
	char *flag_section;
	static CxxToken theToken(';', no_type, NULL, -1);
	char c = 0;
	int i = 0;
	//Get the next token:
	if (LEX_DOT yylex() == 0) {
		return NULL; //End of file
	}
	if (parsing_options.analyze_section == true) {
		//Parsing activated. yyToken has already been set by flex actions.
		if (strlen(buffer)+strlen(yytext) >= 16000) {
			//Descarga parte del buffer al fichero de salida:
			char str[1024];
			int index = strlen(buffer)-1000;
			strcpy(str, &buffer[index]);
			buffer[index] = '\0';
			fprintf(yyget_out(), "%s", buffer);
			strcpy(buffer, str);
		}
		if (yyToken->_value == Identifier) {
			strcat(buffer, yyToken->_name);
			strcpy(last_token, yyToken->_name);
		}
		else {
			strcat(buffer, yytext);
			strcpy(last_token, yytext);
		}
	}
	else {
		//Returns ';' token (does nothing at all)
		yyToken = &theToken;
	}
	return yyToken;
}


/*!
 * \fn detect_section()
 * \brief Analyzes preprocessor marks
 * This function is called when a preprocessor line is detected within the code
 * (see cpp manual for more info). Scope headers are detected by checking the
 * system header flag or by looking at file path
 */
void detect_section() {
	char *flag_section;
	char *scope_headers_dir;
	scope_headers_dir = (char *)malloc(256);
	strcpy(scope_headers_dir, parsing_options.compiler_path);
	strcat(scope_headers_dir, "/include");
	
	//Detected preprocessor mark. Possible buffered code must be dumped to output file:
	fprintf(yyget_out(), "%s", buffer);
	buffer[0] = '\0';
	
	//Find the end of preprocessor mark:
	flag_section = strrchr(yytext, '"');
	if (flag_section == NULL) {
		fprintf(yyget_out(), "%s", yytext);
		free(scope_headers_dir);
		return;
	}
	if (strstr(flag_section, " 3") != 0 || strstr(yytext, scope_headers_dir) != 0 || strstr(yytext, "/usr/include") != 0) {
		//System header detected. Stop parsing.
		parsing_options.analyze_section = false;
	}
	else {
		//User code. We must activate the.
		parsing_options.analyze_section = true;
	}
	//Dump the preprocessor mark:
	fprintf(yyget_out(), "%s", yytext);
	free(scope_headers_dir);
}


/*!
 * \fn make_token(size_t tokenValue)
 * \brief Return the appropriate 1 of 256 flyweight tokens for the ASCII characters.
 *
 * \param tokenValue id of token to create
 * \return pointer to the created token
 */
CxxToken *make_token(size_t tokenValue) {
	static CxxToken *asciiTokens[256];
	if (tokenValue >= (sizeof(asciiTokens)/sizeof(asciiTokens[0])))
	{
		ERRMSG("Cannot make_token for " << tokenValue);
		return 0;
	}
	CxxToken **p = &asciiTokens[tokenValue];
	CxxToken *theToken = *p;
	if (!theToken) {
		*p = theToken = new CxxToken(tokenValue, no_type, yytext, yyleng);
	}
	return theToken;
}


/*!
 * \fn make_character(const char *someText, size_t aLength)
 * \brief Configure the lexer to reflect successful parsing of a character value, assigning it to yylval.
 *
 * The source someText[aLength] should correspond to the parsed text including any L or ' prefix
 * but excluding any ' suffix. In this way the return can indicate whether a wide character has
 * been detected and the routine can accommodate a variety of erroneous terminations.
 *
 * \param someText Parsed text including any L or ' prefix but excluding any ' suffix
 * \param aLength Length of someText
 * \return
 */
CxxToken *make_character(const char *someText, size_t aLength) {
	bool isWide = false;
	if (someText && aLength) {
		if (*someText == 'L') {
			isWide = true;
			someText++;
			aLength--;
		}
		if (!aLength || (*someText != '\'')) {
			ERRMSG("BUG - bad start of character literal.");
		}
		if (aLength) {
			someText++;
			aLength--;
		}
	}
	if (isWide) {
		return make_wide_character(someText, aLength);
	}
	else {
		return make_narrow_character(someText, aLength);
	}
}


/*!
 * \fn make_identifier(const char *someText, size_t aLength)
 * \brief Make the token correponding to an identifier
 * \param someText The identifier
 * \param aLength Size of identifier (in characters)
 * \return Pointer to the created token
 */
CxxToken *make_identifier(const char *someText, size_t aLength) {
	char *idText;
	if (parsing_options.parse_syscalls == true) {
		//Detect if identifier is a System Call:
		for (int i=0 ; i<syscalls.size() ; i++) {
			if (strcmp(syscalls.at(i), someText) == 0) {
				return new CxxToken(PARSE_TOKEN(Identifier), no_type, uc_syscalls.at(i), aLength+3);
			}
		}
	}
	return new CxxToken(PARSE_TOKEN(Identifier), no_type, someText, aLength);
}


CxxToken *make_literal_character(const char *someText, size_t aLength) {
	return make_narrow_character(someText, aLength);
}


CxxToken *make_number(const char *someText, size_t aLength) {
	return new CxxToken(PARSE_TOKEN(IntegerLiteral), lit_double_t, someText, aLength);
}

//
//	Configure the lexer to reflect successful parsing of a categorised string.
//
//	The source someText[aLength] should correspond to the parsed text including any
//	L or " prefix but excluding any " suffix. In this way the return can indicate whether a wide
//	character has been detected and the routine can accommodate a variety of erroneous terminations.
//
CxxToken *make_string(const char *someText, size_t aLength) {
	bool isWide = false;
	if (someText && aLength) {
		if (*someText == 'L') {
			isWide = true;
			someText++;
			aLength--;
		}
		if (!aLength || (*someText != '"'))
			ERRMSG("BUG - bad start of string literal.");
		if (aLength) {
			someText++;
			aLength--;
		}
	}
	if (isWide)
		return make_wide_string(someText, aLength);
	else
		return make_narrow_string(someText, aLength);
}


CxxToken *make_narrow_character(const char *someText, size_t aLength) {
	return new CxxToken(PARSE_TOKEN(CharacterLiteral), lit_char_t, someText, aLength);
}


CxxToken *make_narrow_string(const char *someText, size_t aLength) {
	return new CxxToken(PARSE_TOKEN(StringLiteral), lit_string_t, someText, aLength);
}


CxxToken *make_wide_character(const char *someText, size_t aLength) {
	return new CxxToken(PARSE_TOKEN(CharacterLiteral), lit_wide_char_t, someText, aLength);
}


CxxToken *make_wide_string(const char *someText, size_t aLength) {
	return new CxxToken(PARSE_TOKEN(StringLiteral), lit_wide_string_t, someText, aLength);
}


/*!
 * \fn make_line(const char *yyText, size_t yyLeng)
 * \brief Buffer the incoming line, before any characters are analysed.
 *
 * This function is executed when a new line of code is detected. If starts
 * with "#", the line is a preprocessor mark, and we must analyze it.
 *
 * \param yyText String that matchs the lex rule
 * \param yyLeng Size of yyText
 */
void make_line(const char *yyText, size_t yyLeng) {
	if (parsing_options.echo_line_text) {
		cout << tokenMarkDepth << ": "  << line_number << ": " << yyText << flush;
	}
	else if (parsing_options.echo_line_number) {
		cout << line_number << ": " << yyText << flush;
	}
	line_number++;
}


void omit_attribute(char *yytext, int yyleng) {
	int i = 0;
	int num_open = 0;
	char c;
	if (parsing_options.analyze_section == false) {
		ECHO;
		return;
	}
	while ((c=yyinput()) != '\0') {
//		printf("character %c %d\n",c,num_open);
		if (c == '(') {
			num_open++;
		}
		else if (c == ')') {
			if (num_open == 1) { return; }
			num_open--;
		}
		i++;
	}
}


void end_code_section() {
	parsing_options.analyze_section = false;
}


void add_ws(char *yytext, int yyleng) {
	if (parsing_options.analyze_section == true) {
		strncat(buffer, yytext, yyleng);
	}
	else {
		fprintf(yyget_out(), "%s", yytext);
	}
}


void add_line() {
	if (parsing_options.analyze_section == true) {
		//Puts the line into the code buffer
		strcat(buffer, "\n");
	}
	else {
		//Dump line directly to output file
		fprintf(yyget_out(), "%s", yytext);
	}
}


void mi_func(void) {
	cout << "mi funcion" << endl;
	//printf("%s", yytext); fflush(stdout);
	cout << yytext; cout << flush;
}

%{
#include <m3_object.h>
#include <m3_point.h>
#include <m3_list.h>
#include <m3_map.h>
#include <m3_shell_variables.h>
#include <m3_sim_utils.h>
#define YYSTYPE m3_object *
#include <math.h>
#include <iostream>
#include <memory> 
#include <m3_parser.h>
#include <m3_commands.h>
#include <m3_driver_utils.h>

void yyerror(const char *);
int yylex();
void prs_include_file(char *name);
void prs_end_include_file();

#define FREE(p) if(p) delete p
%}

%token NUM
%token STRING
%token IDENTIFIER
%token DB_INSERT_POINT_T
%token SET_CMD_T
%token OBJ_REPR_T
%token ENDOFFILE_T
%token FLOAT

%% /* Grammar rules and actions follow */


input:  /* empty */     { }
        | input line    { prs_trash.remove_all(); prs_display_prompt(); }
        ;

line:   db_insert_point_cmd '\n'      		{}
        | SET_CMD_T IDENTIFIER '=' object '\n'	{ prs_command_set($2,$4); }
	| command list_objs '\n' 		{ m3_parse_command(to<m3_string *>($1), to<m3_list *>($2)); }	
	| '\n' 					{}
	| ENDOFFILE_T     			{ prs_end_include_file();}
	;

command: IDENTIFIER { $$=$1; }

db_insert_point_cmd: DB_INSERT_POINT_T full_obj { prs_command_db_insert_point($2);}
		;

object: var 
	| map
	| point
	| vect
	| list
	| string 
	| NUM
	| FLOAT 
	| full_obj
	| IDENTIFIER 
	;
	
list: '{' list_objs '}'	{ $$=$2;}
        ;

list_objs: /* empty */  { $$=prs_trash.insert(new m3_list());}
	   | list_objs object { $$=prs_insert_in_list($1,$2);}
	   ;

point: '<' point_objs '>'  { $$=$2; }
	;

vect: '[' vect_objs ']'  { $$=$2; }
	;

vect_objs: /* empty */ { $$=prs_trash.insert(new m3_vector());}
	| vect_objs object { $$=prs_insert_in_vector($1,$2);}
	;

point_objs: /* empty */    { $$=prs_trash.insert(new m3_point());} 
	| point_objs NUM   { $$=prs_add_coord_to_point($1,$2); } 	
	;

var: '$' IDENTIFIER { $$=prs_command_read_variable($2);}
	; 

string: STRING { $$=$1;}
	;

full_obj:
		OBJ_REPR_T '#' object '#' map '#' { $$=prs_insert_map_as_property($3,$5);}
	;

map: '(' map_elements ')' { $$=$2; }
      ;

map_elements: /* empty */ { $$=prs_trash.insert(new m3_map());}
	| map_elements IDENTIFIER '=' object { $$=prs_add_element_to_map($1,$2,$4);}
	;


%%

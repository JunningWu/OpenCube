%{
#include <memory>
#include <m3_object.h>
#define YYSTYPE m3_object *
#include <m3_grammar.h>
#include <m3_parser.h>
#include <m3_commands.h>

%}
%option noyywrap

DIGIT    [0-9]+
FLOAT	 [0-9]+[\.][0-9]*
SIGN	 [\+\-]
ID       [A-Za-z\_\/\-][A-Za-z0-9\_\.\/\:\-]*
NL	     ([\n])
COM	     [\#][\#]([^\n]*)
WS       ([ \t\r])
IDS      ([^\"]*)

%x incl


%{
YY_BUFFER_STATE old_buffer;  // only one level of include allowed
int            including=0;
FILE * 	       old_input_channel;
void prs_include_file(const char *);
bool prs_open_input_file(const char *name);
void prs_end_include_file();
%}


%%

<<EOF>>				{ return ENDOFFILE_T;}
{SIGN}?{DIGIT}			{ yylval=prs_trash.insert(new m3_integer(atoi(yytext))); return NUM; }
{SIGN}?{FLOAT}			{ yylval=prs_trash.insert(new m3_double(atof(yytext))); return FLOAT; }
{NL}				return '\n';
{COM}	 			{}	
{WS}+	 			{}	
"("|")"|"{"|"}"|"<"|">"|"="|"+"|"-"|"["|"]"|"@"|"#"|"$" { return *yytext;}
"set"                           { return SET_CMD_T;}
"_obj_repr_"                           { return OBJ_REPR_T;}
{ID}				{ yylval=prs_trash.insert(new m3_string(yytext)); return IDENTIFIER;}
\"{IDS}\"			{ yytext[strlen(yytext)-1]='\0'; yylval=prs_trash.insert(new m3_string(yytext+1)); return STRING;}
.				{ string s; prs_display_error(s+"Unrecognized token"+yytext); }
%%

void yyerror(const char *s)
{
    cout << "TOK-PRS: " << s << ": " << *yytext << "\n";
    prs_trash.remove_all();
}

void prs_include_file(const char *name)
{
    old_buffer=YY_CURRENT_BUFFER;
    old_input_channel=yyin;
    yyin=fopen(name,"r");
    if(!yyin)
    {
        prs_display_error("include file does not exist");
        yyin=old_input_channel;
    }
    else 
    {
        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
        including=1;
    }
}

bool prs_open_input_file(const char *name)
{
    yyin=fopen(name,"r");
    if(!yyin)
    {
        prs_display_error("file does not exist");
        return false;
    }
    else 
    {
        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
        return true;
    }
}


void prs_end_include_file()
{
    if(including) {
        yy_delete_buffer(YY_CURRENT_BUFFER);
        yy_switch_to_buffer(old_buffer);
        including=0;
    }
    else
    {
        prs_command_quit();
    }
}


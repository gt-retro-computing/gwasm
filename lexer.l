%{
    #include "helpers.h"
    #include "parser.h"
%}

%option header-file="include/lexer.h"
%option never-interactive

%option noyywrap

COMMA ","
COLON ":"
COMMENT ";"[^\n\r]*
DOT "\."
WHITE_SPACE [ \t]*
LINE_BREAK [\n\r]

IDENT [A-Za-z_][A-Za-z0-9_\-]*

HEX 0[Xx][0-9A-Fa-f]+
OCT 0[0-7]+
DEC [0-9]+

%%

{WHITE_SPACE} {}
{LINE_BREAK} {yylineno++; return TOKEN_LINEBREAK; }
{COMMENT} {}
{COMMA} {return TOKEN_COMMA;}
{COLON} {return TOKEN_COLON;}
{DOT} {return TOKEN_DOT;}
{HEX} { sscanf(yytext, "%x", &yylval.num); return TOKEN_NUM; }
{OCT} { sscanf(yytext, "%o", &yylval.num); return TOKEN_NUM; }
{DEC} { sscanf(yytext, "%i", &yylval.num); return TOKEN_NUM; }
{IDENT} { yylval.ident = copy_str(yytext); return TOKEN_IDENT;}

%%

int yyerror(const char* msg)
{    
    fprintf(stderr,"Error text = %s | Line: %d\n%s\n",yytext, yylineno, msg);
    return 0;
}
%{
    #include "helpers.h"
    #include <stdio.h>
    int yylex(void);
    void yyerror(const char*);
%}

%union {
    int num;
    char *ident;
    arg_t* arg;
    instruction_t* instruction;
    directive_t* directive;
    label_t* label;
}

%defines "include/parser.h"

%token TOKEN_DOT
%token TOKEN_COLON
%token TOKEN_COMMA
%token TOKEN_LINEBREAK

%token <num> TOKEN_NUM
%token <ident> TOKEN_IDENT

%type <arg> arg;
%type <instruction> instruction;
%type <directive> directive;
%type <label> label;

%%

prog :  
    | prog TOKEN_LINEBREAK
    | prog label[l]
    { mk_node_l($l); }
    | prog directive[d]
    { mk_node_d($d); }
    | prog instruction[ins]
    { mk_node_i($ins); }


instruction : TOKEN_IDENT[tok] TOKEN_LINEBREAK
        { $$ = mk_instruction($tok, 0, NULL); }
    | TOKEN_IDENT[tok] arg[a1] TOKEN_LINEBREAK
        { $$ = mk_instruction($tok, 1, $a1); }
    | TOKEN_IDENT[tok] arg[a1] TOKEN_COMMA arg[a2] TOKEN_LINEBREAK
        { $a1 -> next = $a2; $$ = mk_instruction($tok, 2, $a1); }

label : TOKEN_IDENT[lab] TOKEN_COLON
    { $$ = mk_label($lab); }
    | TOKEN_IDENT[lab]
    { $$ = mk_label($lab); }

directive : TOKEN_DOT TOKEN_IDENT[dir] TOKEN_LINEBREAK
        { $$ = mk_directive($dir, 0, NULL); }
    | TOKEN_DOT TOKEN_IDENT[dir] arg[a1] TOKEN_LINEBREAK
        { $$ = mk_directive($dir, 1, $a1); }

arg : TOKEN_NUM
    {$$ = mk_arg(NULL, $1);}
    | TOKEN_IDENT
    {$$ = mk_arg($1, 0); }

%%
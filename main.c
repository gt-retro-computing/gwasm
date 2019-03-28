#include "helpers.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>

extern pnode_t* firstNode;

int main(int argc, char* argv[])
{
    /* yydebug = 1; */
    /* global_prev = NULL; */

    /* The default behaviour of yyparse will call the lexer and
     * then the parser on the contents of stdin. In our case, we're
     * not returning anything; we'll have a pointer to the last parsed
     * inside `global_prev`. */
    if (yyparse())
        return 1;
    print_prog(firstNode);
    printf("=============== FILL_LABEL ==================\n");

    fill_slot();

    print_prog(firstNode);
    // generate_code(firstNode);

    /* print_instrs(global_prev); */
    /* free_instrs(global_prev); */
    return 0;
}

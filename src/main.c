#include "helpers.h"
#include "code_gen.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>

extern uint8_t* binary_out;

int main(int argc, char* argv[])
{
    /* yydebug = 1; */
    /* global_prev = NULL; */

    /* The default behaviour of yyparse will call the lexer and
     * then the parser on the contents of stdin. In our case, we're
     * not returning anything; we'll have a pointer to the last parsed
     * inside `global_prev`. */
    if ((argc > 1) && (freopen(argv[1], "r", stdin) == NULL))
        {
            exit( 1 );
        }
    if (yyparse())
        return 1;
    yylex_destroy();
    print_prog(firstNode);
    printf("=============== FILL_LABEL ==================\n");

    fill_slot();

    print_prog(firstNode);
    // generate_code(firstNode);

    generate_code(firstIns);

    clean_up();
    if (binary_out) free(binary_out);

    /* print_instrs(global_prev); */
    /* free_instrs(global_prev); */
    return 0;
}

#include "code_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint16_t address = 0;

static pnode_t* global_last_node;
static astnode_t* astroot;
static astnode_t* astlastnode;

static FILE* out;

void fail(char* error) {
    fprintf(stderr, "%s\n", error);
    exit(-1);
}

void process_directive(directive_t *d);
void process_instruction(instruction_t *i);

arg_t* getu16arg(arg_t* a, uint16_t* out);

void generate_code_h(pnode_t* root);

void generate_code(pnode_t* root) {
    astroot = malloc(sizeof(astnode_t));
    astroot -> first_child = NULL;
    astroot -> last_child = NULL;
    astroot -> next = NULL;
    if (!astroot) return;
    generate_code_h(root);
}
void generate_code_h(pnode_t* root) {
    if (!root) return;
    if (root -> d) process_directive(root -> d);
    else if (root -> i) process_instruction(root -> i);
    generate_code(root->next);
}

void proc_ins(instruction_type t, instruction_t* i) {
    astnode_t *node = malloc(sizeof(astnode_t));
    if (!node) return;
    node->type = INSTRUCTION;
    node -> numval = (uint16_t)t;
}

void process_instruction(instruction_t* i) {
    if (!strncmp("NOP", i->opcode, 3)) proc_ins(NOP, i);
    else if(!strncmp("MVI", i->opcode, 3)) proc_ins(MVI, i);
}

arg_t* getu16arg(arg_t* a, uint16_t* out) {
    if (!a->key) *out = a->cons;
    else fail("Failed to get number arg");
    return a->next;
}

void process_directive(directive_t *d)
{
    arg_t* a = d -> arg;
    if (!strncmp("org", d->directive, 4)) {
        a = getu16arg(a, &address);
        printf(".org - addressing at 0x%X \n", address);
    }
    else
    {
        printf("UNKNOWN DIRECTIVE: %s \n", d -> directive);
    }
}

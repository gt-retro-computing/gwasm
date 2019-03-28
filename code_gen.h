#pragma once
#include "helpers.h"
#include <stdint.h>

typedef enum
{
 NOP,
 MOV, MVI, LXI, LDA, STA, LHLD, SHLD, LDAX, STAX, XCHG,
 JNZ, JZ, JNC, JC, JPO, JPE, JP, JM,
 ADD, ADI, ADC, ACI, SUB
} instruction_type;


typedef enum
{
 INSTRUCTION,
 IMM8,
 IMM16,
 A8,
 A16,
 ADDR_SLOT,
 REG, REGP
} astnode_type;

typedef struct astnode {
    astnode_type type;
    uint16_t numval;
    char* symbolid;
    struct astnode *first_child;
    struct astnode *last_child;
    struct astnode *next;
} astnode_t;

typedef struct symbol {
    char* id;
    uint16_t address;
} symbol_t;

void generate_code(pnode_t* root);

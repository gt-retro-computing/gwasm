#pragma once
#include <stdint.h>

typedef enum
{
    IMM8,
    IMM8_SLOT,
    IMM16,
    IMM16_SLOT,
    A8,
    A8_SLOT,
    A16,
    A16_SLOT,
    REG, REGP,
    UNDEF
} arg_type;

typedef enum {INVALID_INS, MOV, MVI, LXI, LDA, STA, LHLD, SHLD, LDAX, STAX, XCHG, ADD, ADI, ADC, ACI, SUB, SUI, SBB, SBI, INR, DCR, INX, DCX, DAD, DAA, ANA, ANI, ORA, ORI, XRA, XRI, CMP, CPI, RLC, RRC, RAL, RAR, CMA, CMC, STC, JMP, JNZ, JZ, JNC, JC, JPO, JPE, JP, JM, CALL, CNZ, CZ, CNC, CC, CPO, CPE, CP, CM, RET, RNZ, RZ, RNC, RC, RPO, RPE, RP, RM, RST, PCHL, PUSH, POP, XTHL, SPHL, IN, OUT, EI, DI, HLT, NOP} instruction_type;

typedef enum {INVALID_DIRECTIVE, ORG, END, EQU} directive_type;

typedef enum {
    REG_B,
    REG_C,
    REG_D,
    REG_E,
    REG_H,
    REG_L,
    REG_M,
    REG_A,
    INVALID_REG = -1
} single_reg;

typedef enum {
    REG_BC,
    REG_DE,
    REG_HL,
    REG_SP,
    INVALID_REG_P = -1
} pair_reg;

typedef struct arg {
    char* key;
    int16_t cons;
    arg_type type;
    struct arg *next;
} arg_t;

typedef struct directive {
    directive_type directive;
    int argc;
    arg_t *arg;
} directive_t;

typedef struct instruction {
    instruction_type opcode;
    int argc;
    arg_t* arg;
    struct instruction *next;
} instruction_t;

typedef struct label {
    char* name;
    uint16_t addr;
    struct label *next_label;
} label_t;

typedef enum {
    LABEL,
    INSTRUCTION,
    DIRECTIVE
} nodeType_t;

typedef struct pnode {
    nodeType_t nodeType;
    union {
        label_t *label;
        instruction_t *instruction;
        directive_t *directive;
    };
    struct pnode *next;
} pnode_t;

uint16_t global_address;

pnode_t *firstNode;
pnode_t *lastNode;

instruction_t *firstIns;
instruction_t *lastIns;

label_t *firstLabel;
label_t *lastLabel;

char* copy_str(const char *in);

arg_t* mk_arg(char* key, int16_t cons);
instruction_t* mk_instruction(char* opcode, int argc, arg_t* arg1); 
directive_t* mk_directive(char* opcode, int argc, arg_t* arg);
label_t* mk_label(char* label);

pnode_t* mk_node_i(instruction_t*);
pnode_t* mk_node_d(directive_t*);
pnode_t* mk_node_l(label_t*);

void print_directive(directive_t* dir);
void print_instruction(instruction_t *ins);
void print_prog(pnode_t* p);

void fill_slot();
void clean_up();

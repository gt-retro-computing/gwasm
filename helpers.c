#include "helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define INSCMP(a) !strncmp(a, opcode, 4)

char* instruction_names[] = {"invalid_ins", "mov", "mvi", "lxi", "lda", "sta", "lhld", "shld", "ldax", "stax", "xchg", "add", "adi", "adc", "aci", "sub", "sui", "sbb", "sbi", "inr", "dcr", "inx", "dcx", "dad", "daa", "ana", "ani", "ora", "ori", "xra", "xri", "cmp", "cpi", "rlc", "rrc", "ral", "rar", "cma", "cmc", "stc", "jmp", "jnz", "jz", "jnc", "jc", "jpo", "jpe", "jp", "jm", "call", "cnz", "cz", "cnc", "cc", "cpo", "cpe", "cp", "cm", "ret", "rnz", "rz", "rnc", "rc", "rpo", "rpe", "rp","rm", "rst", "pchl", "push", "pop", "xthl", "sphl", "in", "out", "ei", "di", "hlt", "nop"};

void fail(char* error) {
    fprintf(stderr, "%s\n", error);
    exit(-1);
}

char* copy_str(const char *in)
{
	size_t len = strlen(in);
	char* buf = malloc(len + 1);
	strncpy(buf, in, len);
	buf[len] = '\0';
	return buf;
}

static const char* valid_register_names = "bcdehlma";
static const char* valid_register_pair_names[] = {"bc", "de", "hl", "sp", "b", "d", "h"};

pair_reg get_reg_pair(char *str) {
	if (strlen(str) <= 2) {
		for(size_t i = 0; i < (sizeof(valid_register_pair_names) / sizeof(valid_register_pair_names[0])); i++)
		{
			if (!strcmp(valid_register_pair_names[i], str)) {
				return i % 4;
			} 
		}
		
	}
	return INVALID_REG_P;
}

single_reg get_reg(char *str) {
	if (strlen(str) == 1) {
		char* ptr = strchr(valid_register_names, tolower(*str));
		if (ptr) {
			return (uint8_t)(ptr - valid_register_names);
		}
	}
	return INVALID_REG;
	// return (strlen(str) == 1 && strchr(valid_register_names, tolower(*str)));
}

uint16_t findLabel(char* label) {
	label_t *lookup = firstLabel;
	while (lookup) {
		if (!strcmp(lookup -> name, label)) {
			return lookup -> addr;
		}
		lookup = lookup -> next_label;
	}
	fprintf(stderr, "Undefined label reference '%s'\n", label);
	fail("UNKNOWN TOKEN");
	return 0;
}

arg_t* mk_arg(char* key, int cons){
	arg_t* arg = malloc(sizeof(arg_t));
	if (!arg) return NULL;
	arg -> key = key;
	arg -> type = UNDEF;
	arg -> cons = cons;
	arg -> next = NULL;
	return arg;
}

arg_t* mk_arg_regp(arg_t* arg) {
	if(!arg) fail("NULLPTR MK_ARG_REGP");
	pair_reg rp = get_reg_pair(arg -> key);
	if (rp == INVALID_REG_P) {
		fprintf(stderr, "Invalid Register Pair %s \n", arg -> key);
		fail("INVALID_REG_PAIR");
	}
	free(arg -> key);
	arg -> key = NULL;
	arg -> type = REGP;
	return arg -> next;
}

arg_t* mk_arg_reg(arg_t* arg) {
	if(!arg) fail("NULLPTR MK_ARG_REG");
	single_reg r = get_reg(arg -> key);
	if (r == INVALID_REG) {
		fprintf(stderr, "Invalid Register %s \n", arg -> key);
		fail ("INVALID_REG");
	}
	free (arg->key);
	arg -> type = REG;
	arg -> key = NULL;
	arg -> cons = r;
	return arg->next;
}

arg_t* mk_arg_a16(arg_t* arg) {
	if(!arg) fail("NULLPTR MK_ARG_A16");
	if (! arg -> key) {
		arg->type = A16;
	} else {
		arg -> type = A16_SLOT;
	}
	global_address+=2;
	return arg->next;
}

arg_t* mk_arg_a8(arg_t* arg) {
		if(!arg) fail("NULLPTR MK_ARG_A8");
	arg->type = A8;
	global_address++;
	return arg->next;
}


arg_t* mk_arg_imm16(arg_t* arg) {
	if(!arg) fail("NULLPTR MK_ARG_IMM16");
	arg->type = IMM16;
	global_address+=2;
	return arg->next;
}

arg_t* mk_arg_imm8(arg_t* arg) {
		if(!arg) fail("NULLPTR MK_ARG_IMM8");
	arg->type = IMM8;
	global_address++;
	return arg->next;
}

void free_args(arg_t* arg) {
	if (!arg) return;
	free_args(arg -> next);
	if (arg -> key) free(arg -> key);
	free(arg);
}

// arg_t* mk_ins_opcode(instruction_t *i) {
// 	i -> opcode = t;
// 	return i->arg;
// }

instruction_t* mk_instruction(char* opcode, int argc, arg_t* arg1) {
	instruction_t* ins = malloc(sizeof(instruction_t));
	if(!ins) return NULL;
	ins -> arg = arg1;
	ins -> argc = argc;
	if (opcode) {
		for(size_t i = 0; i < strlen(opcode); i++)
		{
			opcode[i] = tolower(opcode[i]);
		}
	}
	ins -> opcode = 0;
	for(size_t i = 1; i < sizeof(instruction_names) / sizeof(instruction_names[0]); i++)
	{
		if (!strcmp(instruction_names[i], opcode)) {
			ins -> opcode = i;
			break;
		}
	}
	global_address++;
	switch (ins -> opcode)
	{
		// NOARG
		case XCHG:
		case DAA:
		case RLC:
		case RRC:
		case RAL:
		case RAR:
		case CMA:
		case CMC:
		case STC:
		case RET:
		case PCHL:
		case XTHL:
		case SPHL:
		case EI:
		case DI:
		case HLT:
		case NOP:
			free_args(arg1); break;
		// A16
		case LDA:
		case STA:
		case LHLD:
		case SHLD:
		case JMP:
		case JNZ:
		case JZ:
		case JNC:
		case JC:
		case JPO:
		case JPE:
		case JP:
		case JM:
		case CALL:
		case CNZ:
		case CZ:
		case CNC:
		case CC:
		case CPO:
		case CPE:
		case CP:
		case CM:
			free_args(mk_arg_a16(arg1)); break;
		
		// A8
		case IN:
		case OUT:
			free_args(mk_arg_a8(arg1)); break;

		// REG
		case ADD:
		case ADC:
		case SUB:
		case SBB:
		case INR:
		case DCR:
		case ANA:
		case ORA:
		case XRA:
		case CMP:
			free_args(mk_arg_reg(arg1)); break;
		
		// IMM8
		case ADI:
		case ACI:
		case SUI:
		case SBI:
		case ANI:
		case ORI:
		case XRI:
		case CPI:
		case RST:
			free_args(mk_arg_imm8(arg1)); break;

		// REGP
		case LDAX:
		case STAX:
		case INX:
		case DCX:
		case DAD:
		case PUSH:
		case POP:
			free_args(mk_arg_regp(arg1)); break;
		
		// REG_REG
		case MOV:
			free_args(mk_arg_reg(mk_arg_reg(arg1))); break;
	
		// REG_IMM8
		case MVI:
			mk_arg_imm8(mk_arg_reg(arg1)); break;

		// RP_IMM16
		case LXI:
			mk_arg_imm16(mk_arg_regp(arg1)); break;

		case INVALID_INS:
			global_address--;
			fprintf(stderr, "Unknown Instruction %s\n", opcode);
			fail("INVALID INSTRUCTION");
			break;
		default:
			global_address--;
			fprintf(stderr, "Unimplemented Instruction %s\n", opcode);
			fail("INTERNAL ERROR");
			break;
	}

	free(opcode);

	if (!firstIns && !lastIns) {
		firstIns = ins;
		lastIns = ins;
	} else {
		lastIns -> next = ins;
		lastIns = ins;
	}

	return ins;
}

directive_t* mk_directive(char* opcode, int argc, arg_t* arg) {
	directive_t* dir = malloc(sizeof(directive_t));
	if (!dir) return NULL;
	dir -> directive = opcode;
	dir -> argc = argc;
	dir -> arg = arg;
	return dir;
}

label_t* mk_label(char* label) {
    label_t* l = malloc(sizeof(label_t));
    if (!l) return NULL;
	l -> addr = global_address;
    l -> name = label;

	if (!firstLabel && !lastLabel) {
		firstLabel = l;
		lastLabel = l;
	} else {
		lastLabel -> next_label = l;
		lastLabel = l;
	}

    return l;
}

pnode_t* mk_node(label_t *l, instruction_t *i, directive_t *d) {
	pnode_t* n = malloc(sizeof(pnode_t));
	if (!n) return NULL;

	n -> l = l;
	n -> i = i;
	n -> d = d;

	if (firstNode && lastNode) {
		lastNode -> next = n;
		lastNode = n;
	} else {
		firstNode = n;
		lastNode = n;
	}

	return n;
}

void print_arg(arg_t *arg) {
	if (!arg) return;
	switch (arg -> type)
	{
		case IMM8:
		case IMM16:
			printf("(IMM %d) ", arg -> cons);
			break;
		case A8:
		case A16:
			printf("(ADDR 0x%X) ", arg -> cons); break;
		case A16_SLOT:
			printf("(ADDR [%s]) ", arg -> key); break;
		case REG:
			printf("(REG %c)", valid_register_names[arg -> cons]); break;
		case REGP:
			printf("(REGP %s)", valid_register_pair_names[arg -> cons]); break;
		default:
			break;
	}
	print_arg(arg->next);
}

void print_directive(directive_t* dir) {
	printf("(D[%i]): .%s ", dir->argc, dir -> directive);
	print_arg(dir->arg);
	printf("\n");
}

void print_instruction(instruction_t *ins) {
	char * name = "UNKNOWN";
	if (ins -> opcode < sizeof(instruction_names)) {
		name = instruction_names[ins -> opcode];
	}
	printf("(I[%i]): %s ", ins->argc, name);
	print_arg(ins->arg);
	printf("\n");
}

void print_label(label_t* l) {
	printf("(L) [0x%X] %s\n", l -> addr, l -> name);
}

void print_prog(pnode_t* p) {
	if (!p) return;
	if (p -> d) {
		print_directive(p->d);
	}
	if (p->i) print_instruction(p->i);
	if(p->l) print_label(p->l);
	print_prog(p->next);
}

void fill_slot() {
	instruction_t *currentIns = firstIns;
	while(currentIns) {
		arg_t *currentArg = currentIns -> arg;
		while (currentArg) {
			if (currentArg -> type == A16_SLOT) {
				currentArg -> cons = findLabel(currentArg -> key);
				currentArg -> type = A16;
				free(currentArg -> key);
				currentArg -> key = NULL;
			}
			currentArg = currentArg -> next;
		}
		currentIns = currentIns -> next;
	}
}
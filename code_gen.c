#include "code_gen.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE* out;
uint8_t* binary_out;
uint16_t binary_location = 0;

extern char* instruction_names[];

uint8_t instruction_templates[] = { 0x0, 0b01000000, 0b00000110, 0b00000001, 0b00111010, 0b00110010, 0b00101010, 0b00100010, 0b00001010, 0b00000010, 0b11101011, 0b10000000, 0b11000110, 0b10001000, 0b11001110, 0b10010000, 0b11010110, 0b10011000, 0b11011110, 0b00000100, 0b00000101, 0b00000011, 0b00001011, 0b00001001, 0b00100111, 0b10100000, 0b11100110, 0b10110000, 0b11110110, 0b10101000, 0b11101110, 0b10111000, 0b11111110, 0b00000111, 0b00001111, 0b00010111, 0b00011111, 0b00101111, 0b00111111, 0b00110111, 0b11000011, 0b11000010, 0b11000010, 0b11000010, 0b11000010, 0b11000010, 0b11000010, 0b11000010, 0b11000010, 0b11001101, 0b11000100, 0b11000100, 0b11000100, 0b11000100, 0b11000100, 0b11000100, 0b11000100, 0b11000100, 0b11001001, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000000, 0b11000111, 0b11101001, 0b11000101, 0b11000001, 0b11100011, 0b11111001, 0b11011011, 0b11010011, 0b11111011, 0b11110011, 0b01110110, 0b00000000 };

void generate_code_h(instruction_t* root);

void generate_code(instruction_t* root) {
    printf("Instruction Template Size = %lu\n", sizeof(instruction_templates));
    if (binary_out) free(binary_out);
    binary_out = malloc(sizeof(uint8_t) * (global_address));
    binary_location = 0;
    if (!binary_out) return;

    generate_code_h(root);
}

void generate_args(arg_t *a) {
    if (!a) return;
    switch (a -> type) {
    case IMM8:
    case A8:
        binary_out[binary_location++] = (uint8_t)a -> cons;
        break;
    case IMM16:
    case A16:
        binary_out[binary_location++] = (uint8_t) a -> cons;
        binary_out[binary_location++] = (uint8_t) (a -> cons >> 8);
        break;
    default:
        exit(-1);
    }
    generate_args(a->next);
}

void generate_pure_instruction(instruction_t* i) {

}

void generate_code_h(instruction_t* root) {
    if (!root) return;
    switch (root -> opcode) {
        // PureInstruction
    case LDA:
    case STA:
    case LHLD:
    case SHLD:
    case XCHG:
    case ADI:
    case ACI:
    case SUI:
    case SBI:
    case DAA:
    case ANI:
    case ORI:
    case XRI:
    case CPI:
    case RLC:
    case RRC:
    case RAL:
    case RAR:
    case CMA:
    case CMC:
    case STC:
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
    case RET:
    case RNZ:
    case RZ:
    case RNC:
    case RC:
    case RPO:
    case RPE:
    case RP:
    case RM:
    case RST:
    case PCHL:
    case XTHL:
    case SPHL:
    case IN:
    case OUT:
    case EI:
    case DI:
    case HLT:
    case NOP:
        generate_pure_instruction(root); break;
    default:
        fprintf(stderr, "No Code gen rule for instruction %s\n", instruction_names[root->opcode]);
    }
    generate_code_h(root -> next);
}

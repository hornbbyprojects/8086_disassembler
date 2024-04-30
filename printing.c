#include "printing.h"
#include "instruction_data.h"
#include <stdio.h>
#include <stdlib.h>

void print_register_code(u8 code, u8 flag_w) {
  if (flag_w) {
    switch ((enum RegisterCodes)code) {
    case AX:
      printf("ax");
      break;
    case BX:
      printf("bx");
      break;
    case CX:
      printf("cx");
      break;
    case DX:
      printf("dx");
      break;
    case SP:
      printf("sp");
      break;
    case BP:
      printf("bp");
      break;
    case SI:
      printf("si");
      break;
    case DI:
      printf("di");
      break;
    default:
      fprintf(stderr, "Invalid register code %i", code);
      exit(-2);
    }
  } else {
    switch ((enum ShortRegisterCodes)(code)) {
    case AL:
      printf("al");
      break;
    case CL:
      printf("cl");
      break;
    case DL:
      printf("dl");
      break;
    case BL:
      printf("bl");
      break;
    case AH:
      printf("ah");
      break;
    case CH:
      printf("ch");
      break;
    case DH:
      printf("dh");
      break;
    case BH:
      printf("bh");
      break;
    default:
      fprintf(stderr, "Invalid register code %i", code);
      exit(-2);
    }
  }
}
void print_operand(struct Operand *operand) {
  i16 adjusted_offset;
  switch (operand->operand_type) {
  case OPERAND_TYPE_REGISTER:
    print_register_code(operand->_register.code, operand->_register.flag_w);
    break;
  case OPERAND_TYPE_IMMEDIATE:
    switch (operand->immediate.length) {
    case 1:
      printf("byte ");
      break;
    case 2:
      printf("word ");
      break;
    default:
      fprintf(stderr, "Unsupported immediate length %i\n",
              operand->immediate.length);
      break;
    }
    printf("%i", operand->immediate.value);
    break;
  case OPERAND_TYPE_MEMORY:
    printf("[");
    switch (operand->memory.address_type) {

    case ADDRESS_BX_SI:
      printf("BX + SI");
      break;
    case ADDRESS_BX_DI:
      printf("BX + DI");
      break;
    case ADDRESS_BP_SI:
      printf("BP + SI");
      break;
    case ADDRESS_BP_DI:
      printf("BP + DI");
      break;
    case ADDRESS_SI:
      printf("SI");
      break;
    case ADDRESS_DI:
      printf("DI");
      break;
    case ADDRESS_BP:
      printf("BP");
      break;
    case ADDRESS_BX:
      printf("BX");
      break;
    }
    if (operand->memory.is_displaced) {
      if (operand->memory.displacement >= 0) {
        printf(" + %i", operand->memory.displacement);
      } else {
        printf(" - %i", -operand->memory.displacement);
      }
    }
    printf("]");
    break;
  case OPERAND_TYPE_DIRECT_MEMORY:
    printf("[%i]", operand->direct_memory.displacement);
    break;
  case OPERAND_TYPE_MISSING:
    fprintf(stderr, "Instruction is missing an operand!");
    exit(-2);
  case OPERAND_TYPE_JUMP_DESTINATION:
    adjusted_offset = operand->jump_destination.jump_destination + 2;
    if (adjusted_offset >= 0) {
      printf("$+%i", adjusted_offset);
    } else {
      printf("$%i", adjusted_offset);
    }
    break;
  default:
    fprintf(stderr, "Unknown operand type %i\n", operand->operand_type);
    exit(-2);
  }
}
void print_instruction(struct Instruction *instruction) {
  switch (instruction->instruction_type) {
  case INSTRUCTION_TYPE_MOV:
    printf("MOV ");
    break;
  case INSTRUCTION_TYPE_JCXZ:
    printf("JCXZ ");
    break;
  case INSTRUCTION_TYPE_LOOPNE:
    printf("LOOPNE ");
    break;
  case INSTRUCTION_TYPE_LOOPE:
    printf("LOOPE ");
    break;
  case INSTRUCTION_TYPE_LOOP:
    printf("LOOP ");
    break;
  case INSTRUCTION_TYPE_JNS:
    printf("JNS ");
    break;
  case INSTRUCTION_TYPE_JNO:
    printf("JNO ");
    break;
  case INSTRUCTION_TYPE_JNP:
    printf("JNP ");
    break;
  case INSTRUCTION_TYPE_JA:
    printf("JA ");
    break;
  case INSTRUCTION_TYPE_JAE:
    printf("JAE ");
    break;
  case INSTRUCTION_TYPE_JG:
    printf("JG ");
    break;
  case INSTRUCTION_TYPE_JNL:
    printf("JNL ");
    break;
  case INSTRUCTION_TYPE_JNE:
    printf("JNE ");
    break;
  case INSTRUCTION_TYPE_JS:
    printf("JS ");
    break;
  case INSTRUCTION_TYPE_JO:
    printf("JO ");
    break;
  case INSTRUCTION_TYPE_JP:
    printf("JP ");
    break;
  case INSTRUCTION_TYPE_JBE:
    printf("JBE ");
    break;
  case INSTRUCTION_TYPE_JB:
    printf("JB ");
    break;
  case INSTRUCTION_TYPE_JLE:
    printf("JLE ");
    break;
  case INSTRUCTION_TYPE_JL:
    printf("JL ");
    break;
  case INSTRUCTION_TYPE_JE:
    printf("JE ");
    break;
  case INSTRUCTION_TYPE_CMP:
    printf("CMP ");
    break;
  case INSTRUCTION_TYPE_SUB:
    printf("SUB ");
    break;
  case INSTRUCTION_TYPE_ADD:
    printf("ADD ");
    break;
  }
  print_operand(&instruction->destination);
  if (instruction->source.operand_type != OPERAND_TYPE_MISSING) {
    printf(", ");
    print_operand(&instruction->source);
  }
  printf("\n");
}

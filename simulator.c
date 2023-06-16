#include "simulator.h"
#include "instruction_data.h"
#include <stdio.h>
#include <stdlib.h>

void simulate_mov(struct Simulated8086 *comp, struct Instruction *instruction) {
  u16 value = 0;
  u8 _register, is_high;
  switch (instruction->source.operand_type) {
  case OPERAND_TYPE_MISSING:
  case OPERAND_TYPE_REGISTER:
    if(instruction->source._register.flag_w) {
      value = comp->registers[instruction->source._register.code];
    }
    else {
      _register = (instruction->destination._register.code % 4);
      is_high = instruction->destination._register.code >= 4;
      if(is_high) {
        value = comp->registers[_register] & 0xff00 >> 8;
      }
      else {
        value = comp->registers[_register] & 0xff;
      }
    }
      break;
  case OPERAND_TYPE_IMMEDIATE:
    value = instruction ->source.immediate.value;
    break;
  case OPERAND_TYPE_MEMORY:
  case OPERAND_TYPE_DIRECT_MEMORY:
  case OPERAND_TYPE_JUMP_DESTINATION:
    break;
  }
  switch (instruction->destination.operand_type) {
  case OPERAND_TYPE_REGISTER:
    if (instruction->destination._register.flag_w) {
      comp->registers[instruction->destination._register.code] = value;
    } else {
      _register = (instruction->destination._register.code % 4);
      is_high = instruction->destination._register.code >= 4;
      if (is_high) {
        comp->registers[_register] &= 0xff;
        comp->registers[_register] |= (value << 8);
      } else {
        comp->registers[_register] &= 0xff00;
        comp->registers[_register] |= (value & 0xff);
      }
    }
    break;
  case OPERAND_TYPE_MISSING:
  case OPERAND_TYPE_IMMEDIATE:
  case OPERAND_TYPE_MEMORY:
  case OPERAND_TYPE_DIRECT_MEMORY:
  case OPERAND_TYPE_JUMP_DESTINATION:
    fprintf(stderr, "Can't MOV into operand type %i",
            instruction->destination.operand_type);
    exit(-2);
    break;
  }
}
void simulate_instruction(struct Simulated8086 *comp,
                          struct Instruction *instruction) {
  switch (instruction->instruction_type) {
  case INSTRUCTION_TYPE_MOV:
    simulate_mov(comp, instruction);
    break;
  default:
    fprintf(stderr, "Unsupported operation %i\n", instruction -> instruction_type);
  }
}

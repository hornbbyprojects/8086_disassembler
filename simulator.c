#include "simulator.h"
#include "instruction_data.h"
#include <stdio.h>
#include <stdlib.h>

// Base here means not including displacement
u16 get_memory_base_location(struct Simulated8086 *comp, struct Memory *op) {
  switch (op->address_type) {
  case ADDRESS_BX_SI:
    return comp->registers[BX] + comp->registers[SI];
  case ADDRESS_BX_DI:
    return comp->registers[BX] + comp->registers[DI];
  case ADDRESS_BP_SI:
    return comp->registers[BP] + comp->registers[SI];
  case ADDRESS_BP_DI:
    return comp->registers[BP] + comp->registers[DI];
  case ADDRESS_SI:
    return comp->registers[SI];
  case ADDRESS_DI:
    return comp->registers[DI];
  case ADDRESS_BP:
    return comp->registers[BP];
  case ADDRESS_BX:
    return comp->registers[BX];
  default:
    fprintf(stderr, "unknown addressing type %i\n", op->address_type);
    exit(-2);
  }
}
u16 get_memory_location(struct Simulated8086 *comp, struct Memory *op) {
  return get_memory_base_location(comp, op) + op->displacement;
}
u16 read_memory(struct Simulated8086 *comp, u16 location, u8 flag_w) {
  if (flag_w) {
    if (location == 0xff) {
      fprintf(stderr, "Can't wide access last byte!\n");
      exit(-2);
    }
    return comp->memory[location] | (((u16)(comp->memory[location + 1])) << 8);
  } else {
    return comp->memory[location];
  }
}
void set_memory(struct Simulated8086 *comp, u16 location, u8 flag_w,
                u16 value) {
  if (flag_w) {
    if (location == 0xff) {
      fprintf(stderr, "Can't wide set last byte!\n");
      exit(-2);
    }
    comp->memory[location] = value & 0xff;
    comp->memory[location + 1] = (value & 0xff00) >> 8;
  } else {
    comp->memory[location] = (u8)value;
  }
}
u16 get_value_from_operand(struct Simulated8086 *comp,
                           struct Operand *operand) {
  u8 _register, is_high;
  u16 memory_address;
  switch (operand->operand_type) {
  case OPERAND_TYPE_REGISTER:
    if (operand->_register.flag_w) {
      return comp->registers[operand->_register.code];
    } else {
      _register = operand->_register.code % 4;
      is_high = operand->_register.code >= 4;
      if (is_high) {
        return comp->registers[_register] & 0xff00 >> 8;
      } else {
        return comp->registers[_register] & 0xff;
      }
    }
  case OPERAND_TYPE_IMMEDIATE:
    return operand->immediate.value;
  case OPERAND_TYPE_DIRECT_MEMORY:
    return read_memory(comp, operand->direct_memory.displacement,
                       operand->direct_memory.flag_w);
  case OPERAND_TYPE_MEMORY:
    memory_address = get_memory_location(comp, &operand->memory);
    return read_memory(comp, memory_address, operand->memory.flag_w);
  default:
    fprintf(stderr, "Cannot get value from optype %i!\n",
            operand->operand_type);
    exit(-2);
    return 0;
  }
}

void set_operand(struct Simulated8086 *comp, struct Operand *operand,
                 u16 value) {
  u8 _register, is_high;
  u16 memory_address;
  switch (operand->operand_type) {
  case OPERAND_TYPE_REGISTER:
    if (operand->_register.flag_w) {
      comp->registers[operand->_register.code] = value;
    } else {
      _register = (operand->_register.code % 4);
      is_high = operand->_register.code >= 4;
      if (is_high) {
        comp->registers[_register] &= 0xff;
        comp->registers[_register] |= (value << 8);
      } else {
        comp->registers[_register] &= 0xff00;
        comp->registers[_register] |= (value & 0xff);
      }
    }
    break;
  case OPERAND_TYPE_DIRECT_MEMORY:
    set_memory(comp, operand->direct_memory.displacement,
               operand->direct_memory.flag_w, value);
    break;
  case OPERAND_TYPE_MEMORY:
    memory_address = get_memory_location(comp, &operand->memory);
    set_memory(comp, memory_address, operand->memory.flag_w, value);
    break;
  case OPERAND_TYPE_MISSING:
  case OPERAND_TYPE_IMMEDIATE:
  case OPERAND_TYPE_JUMP_DESTINATION:
    fprintf(stderr, "Can't MOV into operand type %i", operand->operand_type);
    exit(-2);
    break;
  }
}

void set_flags(struct Simulated8086 *comp, u16 result) {
  comp->flag_Z = result == 0;
  comp->flag_S = (result >> 15) == 1;
}

void simulate_instruction(struct Simulated8086 *comp,
                          struct Instruction *instruction) {
  u16 source_value = 0, dest_value = 0, set_value = 0;
  switch (instruction->instruction_type) {
  case INSTRUCTION_TYPE_SUB:
    source_value = get_value_from_operand(comp, &instruction->source);
    dest_value = get_value_from_operand(comp, &instruction->destination);
    set_value = dest_value - source_value;
    set_operand(comp, &instruction->destination, set_value);
    set_flags(comp, set_value);
    break;
  case INSTRUCTION_TYPE_CMP:
    source_value = get_value_from_operand(comp, &instruction->source);
    dest_value = get_value_from_operand(comp, &instruction->destination);
    set_value = dest_value - source_value;
    set_flags(comp, set_value);
    break;
  case INSTRUCTION_TYPE_MOV:
    source_value = get_value_from_operand(comp, &instruction->source);
    set_operand(comp, &instruction->destination, source_value);
    break;
  case INSTRUCTION_TYPE_ADD:
    source_value = get_value_from_operand(comp, &instruction->source);
    dest_value = get_value_from_operand(comp, &instruction->destination);
    set_value = source_value + dest_value;
    set_operand(comp, &instruction->destination, set_value);
    set_flags(comp, set_value);
    break;
  case INSTRUCTION_TYPE_JNE:
    if (instruction->destination.operand_type !=
        OPERAND_TYPE_JUMP_DESTINATION) {
      fprintf(stderr, "Jump needs destination, got type %i\n",
              instruction->destination.operand_type);
    }
    if (!comp->flag_Z) {
      comp->pc += instruction->destination.jump_destination.jump_destination;
    }
    break;
  default:
    fprintf(stderr, "Unsupported operation %i\n",
            instruction->instruction_type);
    exit(-2);
  }
}

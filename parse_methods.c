#include "instruction_data.h"
#include "stdio.h"
#include "stdlib.h"

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

// For use with values stored in u8 that are only a few bits
#define NOT_PRESENT 255

unsigned char read_byte(FILE *file) {
  unsigned char ret;
  if (!fread(&ret, 1, 1, file)) {
    if (ferror(file)) {
      fprintf(stderr, "Error reading file: %i\n", ferror(file));
      exit(-2);
    }
    // I hope caller checks feof
    return 0;
  }
  return ret;
}

i16 read_i16_little_endian(FILE *file) {
  return ((u16)read_byte(file)) | (((u16)read_byte(file)) << 8);
}

void read_data(FILE *file, struct InstructionData *instruction) {
  instruction->has_immediate = 1;
  if (instruction->flag_w) {
    if (instruction->flag_s) {
      instruction->data = ((i8)(read_byte(file)));
    } else {
      instruction->data = read_i16_little_endian(file);
    }
  } else {
    instruction->data = read_byte(file);
  }
}

void read_displacement(FILE *file, struct InstructionData *instruction) {
  switch (instruction->mod) {
  case 0:
    if (instruction->r_m != 0b110) {
      break;
    }
    // Fall through to case 2
  case 2:
    instruction->displacement = read_i16_little_endian(file);
    break;
  case 1:
    instruction->displacement = (i8)read_byte(file);
    return;
  case 3:
    instruction->displacement = 0;
    break;
  default:
    fprintf(stderr, "Unexpected mod %i\n", instruction->mod);
    exit(-2);
  }
}

#include "parse_methods.inl"
struct InstructionData parse(FILE *file) {
  struct InstructionData ret;
  long current_pos = ftell(file);
  for (int i = 0; i < ARRAY_LENGTH(parse_methods); ++i) {
    ret.failed = 0;
    ret.r_m = NOT_PRESENT;
    ret.reg = NOT_PRESENT;
    ret.has_immediate = 0;
    ret.has_jump_destination = 0;
    ret.flag_s = 0;
    (*(parse_methods[i]))(&ret, file);
    if (!ret.failed) {
      break;
    }
    fseek(file, current_pos, SEEK_SET);
  }
  return ret;
}

void parse_register(struct Operand *ret, struct InstructionData *data) {
  ret->operand_type = OPERAND_TYPE_REGISTER;
  ret->_register.code = data->reg;
  ret->_register.flag_w = data->flag_w;
}

void parse_r_m_as_register(struct Operand *ret, struct InstructionData *data) {
  ret->operand_type = OPERAND_TYPE_REGISTER;
  ret->_register.code = data->r_m;
  ret->_register.flag_w = data->flag_w;
}

void parse_r_m_as_memory(struct Operand *ret, struct InstructionData *data) {
  ret->operand_type = OPERAND_TYPE_MEMORY;
  ret->memory.address_type = data->r_m;
  ret->memory.is_displaced = data->mod;
  ret->memory.displacement = data->displacement;
  ret->memory.flag_w = data->flag_w;
}

struct Instruction instruction_from_data(struct InstructionData data) {
  struct Instruction ret;
  ret.destination.operand_type = OPERAND_TYPE_MISSING;
  ret.source.operand_type = OPERAND_TYPE_MISSING;
  ret.instruction_type = data.instruction_type;
  if(data.has_jump_destination) {
    ret.destination.operand_type = OPERAND_TYPE_JUMP_DESTINATION;
    ret.destination.jump_destination.jump_destination = data.jump_destination;
    return ret;
  }
  if (data.reg != NOT_PRESENT) {
    if (data.flag_d || data.has_immediate) {
      parse_register(&ret.destination, &data);
    } else {
      parse_register(&ret.source, &data);
    }
  }
  if (data.r_m != NOT_PRESENT) {
    if (data.mod == 0b11) {
      if (data.flag_d && !data.has_immediate) {
        parse_r_m_as_register(&ret.source, &data);

      } else {
        parse_r_m_as_register(&ret.destination, &data);
      }
    } else {
      if (data.mod == 0b00 && data.r_m == 0b110) {
        if (data.flag_d && !data.has_immediate) {
          ret.source.operand_type = OPERAND_TYPE_DIRECT_MEMORY;
          ret.source.direct_memory.displacement = data.displacement;
          ret.source.direct_memory.flag_w = data.flag_w;
        } else {
          ret.destination.operand_type = OPERAND_TYPE_DIRECT_MEMORY;
          ret.destination.direct_memory.displacement = data.displacement;
          ret.destination.direct_memory.flag_w = data.flag_w;
        }
      } else {
        if (data.flag_d && !data.has_immediate) {
          parse_r_m_as_memory(&ret.source, &data);
        } else {
          parse_r_m_as_memory(&ret.destination, &data);
        }
      }
    }
  }
  if (data.has_immediate) {
    ret.source.operand_type = OPERAND_TYPE_IMMEDIATE;
    ret.source.immediate.value = data.data;
    if (data.flag_w) {
      ret.source.immediate.length = 2;
    }
    else {
      ret.source.immediate.length = 1;
    }
  }
  return ret;
}


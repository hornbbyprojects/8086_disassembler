#include "stdio.h"
#include "stdlib.h"
#include <bits/types.h>
#include <stdint.h>

void print_binary(FILE *file, unsigned char byte) {
  while (byte) {
    fprintf(file, "%i", (byte & 0b10000000) >> 7);
    byte = byte << 1;
  }
}

unsigned char read_byte(FILE *file) {
  unsigned char ret;
  if (!fread(&ret, 1, 1, file)) {
    fprintf(stderr, "Unexpected end of file");
  }
  return ret;
}

enum InstructionType {
  INSTRUCTION_TYPE_MOV_REGISTER_TO_REGISTER,
  INSTRUCTION_TYPE_MOV_IMMEDIATE_TO_REGISTER,
  INSTRUCTION_TYPE_MOV_REGISTER_TO_MEMORY

};
struct MovRegisterToRegister {
  unsigned char is_wide;
  unsigned char from_reg;
  unsigned char to_reg;
};
struct MovRegisterToMemory {
  unsigned char is_wide;
  unsigned char reg;
  unsigned char r_m;
  unsigned char displacement_length;
  __int16_t displacement;
  unsigned char flag_d;
};
struct MovImmediateToRegister {
  unsigned char reg;
  __uint16_t data;
  unsigned char is_wide;
};

union InstructionUnion {
  struct MovRegisterToRegister mov_register_to_register;
  struct MovImmediateToRegister mov_immediate_to_register;
  struct MovRegisterToMemory mov_register_to_memory;
};
struct Instruction {
  enum InstructionType instruction_type;
  union InstructionUnion instruction_union;
};

struct Instruction parse_mov_immediate_to_register(unsigned char high_byte,
                                                   FILE *file) {
  struct Instruction ret;
  unsigned char flag_w = (high_byte & 0b00001000) >> 3;
  unsigned char reg = (high_byte & 0b00000111);
  __uint16_t data = 0;

  data = read_byte(file);
  if (flag_w) {
    data |= read_byte(file) << 8;
  }
  ret.instruction_type = INSTRUCTION_TYPE_MOV_IMMEDIATE_TO_REGISTER;
  ret.instruction_union.mov_immediate_to_register.reg = reg;
  ret.instruction_union.mov_immediate_to_register.data = data;
  ret.instruction_union.mov_immediate_to_register.is_wide = flag_w;
  return ret;
}

struct Instruction parse_mov_register_to_register(unsigned char high_byte,
                                                  FILE *file) {
  struct Instruction ret;
  __int16_t displacement;
  unsigned char displacement_length;

  unsigned char low_byte = read_byte(file);
  unsigned char flag_d = high_byte & 0b00000010;
  unsigned char flag_w = high_byte & 1;
  unsigned char mod = (low_byte & 0b11000000) >> 6;
  unsigned char reg = (low_byte & 0b00111000) >> 3;
  unsigned char r_m = (low_byte & 0b00000111);
  switch (mod) {
  case 0b00:
    // Memory
    displacement_length = 0;
    if (mod == 0b110) {
      goto TWO_BYTE_DISPLACEMENT;
    }
    break;
  case 0b01:
    // Memory, 8 bit displacement
    displacement = (char)read_byte(file);
    displacement_length = 1;
    break;
  case 0b10:
    // Memory, 16 bit displacement
    displacement_length = 2;
  TWO_BYTE_DISPLACEMENT:
    displacement = (unsigned char) read_byte(file);
    displacement |= ((__uint16_t) read_byte(file)) << 8;
    break;
  case 0b11:
    // Reg to reg
    ret.instruction_type = INSTRUCTION_TYPE_MOV_REGISTER_TO_REGISTER;
    ret.instruction_union.mov_register_to_register.is_wide = flag_w;
    if (flag_d) {
      ret.instruction_union.mov_register_to_register.from_reg = r_m;
      ret.instruction_union.mov_register_to_register.to_reg = reg;
    } else {
      ret.instruction_union.mov_register_to_register.from_reg = reg;
      ret.instruction_union.mov_register_to_register.to_reg = r_m;
    }
    return ret;
  }
  ret.instruction_type = INSTRUCTION_TYPE_MOV_REGISTER_TO_MEMORY;
  ret.instruction_union.mov_register_to_memory.flag_d = flag_d;
  ret.instruction_union.mov_register_to_memory.is_wide = flag_w;
  ret.instruction_union.mov_register_to_memory.reg = reg;
  ret.instruction_union.mov_register_to_memory.r_m = r_m;
  ret.instruction_union.mov_register_to_memory.displacement = displacement;
  ret.instruction_union.mov_register_to_memory.displacement_length =
      displacement_length;
  return ret;
}

const char *wide_register_strings[] = {"ax", "cx", "dx", "bx",
                                       "sp", "bp", "si", "di"};
const char *short_register_strings[] = {"al", "cl", "dl", "bl",
                                        "ah", "ch", "dh", "bh"};
const char *register_string(unsigned char reg, unsigned char is_wide) {
  if (is_wide) {
    return wide_register_strings[reg];
  } else {
    return short_register_strings[reg];
  }
}
const char *memory_strings[] = {
    "BX + SI", "BX + DI", "BP + SI", "BP + DI", "SI", "DI", "BP", "BX",
};
void write_memory_string(FILE *file, unsigned char r_m,
                         unsigned char displacement_length,
                         __int16_t displacement) {
  /*
    maximum string length is
    [BX + SI] + 65536
   */
  if (displacement_length == 0 && r_m == 0b110) {
    // Direct address
    fprintf(file, "[%i]", displacement);
    return;
  }
  if (displacement_length == 0) {
    fprintf(file, "[%s]", memory_strings[r_m]);
  } else {
    if (displacement > 0) {
      fprintf(file, "[%s + %i]", memory_strings[r_m], displacement);
    }
    else {
      fprintf(file, "[%s - %i]", memory_strings[r_m], -displacement);
    }
  }
}
void write_instruction(FILE *file, struct Instruction instruction) {
  switch (instruction.instruction_type) {
  case INSTRUCTION_TYPE_MOV_REGISTER_TO_REGISTER:
    fprintf(
        file, "mov %s, %s\n",
        register_string(
            instruction.instruction_union.mov_register_to_register.to_reg,
            instruction.instruction_union.mov_register_to_register.is_wide),
        register_string(
            instruction.instruction_union.mov_register_to_register.from_reg,
            instruction.instruction_union.mov_register_to_register.is_wide));
    break;
  case INSTRUCTION_TYPE_MOV_REGISTER_TO_MEMORY:
    if (instruction.instruction_union.mov_register_to_memory.flag_d) {
      fprintf(
          file, "mov %s, ",
          register_string(
              instruction.instruction_union.mov_register_to_memory.reg,
              instruction.instruction_union.mov_register_to_memory.is_wide));
    } else {
      fprintf(file, "mov ");
    }
    write_memory_string(
        file, instruction.instruction_union.mov_register_to_memory.r_m,
        instruction.instruction_union.mov_register_to_memory
            .displacement_length,
        instruction.instruction_union.mov_register_to_memory.displacement);
    if (!instruction.instruction_union.mov_register_to_memory.flag_d) {
      fprintf(
          file, ", %s",
          register_string(
              instruction.instruction_union.mov_register_to_memory.reg,
              instruction.instruction_union.mov_register_to_memory.is_wide));
    }
    fprintf(file, "\n");
    break;
  case INSTRUCTION_TYPE_MOV_IMMEDIATE_TO_REGISTER:
    fprintf(
        file, "mov %s, %i\n",
        register_string(
            instruction.instruction_union.mov_immediate_to_register.reg,
            instruction.instruction_union.mov_immediate_to_register.is_wide),
        instruction.instruction_union.mov_immediate_to_register.data);
    break;
  }
}

struct Instruction parse_instruction(unsigned char high_byte, FILE *file) {
  if ((high_byte & 0b11110000) == 0b10110000) {
    return parse_mov_immediate_to_register(high_byte, file);
  }
  if ((high_byte & 0b11111100) == 0b10001000) {
    return parse_mov_register_to_register(high_byte, file);
  }

  fprintf(stderr, "Unknown first byte of instruction ");
  print_binary(stderr, high_byte);
  fprintf(stderr, "\n");
  exit(-1);
}

int main(int argc, char *argv[]) {
  unsigned char high_byte;
  struct Instruction instruction;
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [[filepath of binary]]\n", argv[0]);
  }
  FILE *infile = fopen(argv[1], "rb");
  if (!infile) {
    fprintf(stderr, "File %s not found!\n", argv[1]);
    exit(-2);
  }
  puts("bits 16");
  while (1) {
    if (!fread(&high_byte, 1, 1, infile)) {
      break;
    }
    instruction = parse_instruction(high_byte, infile);
    write_instruction(stdout, instruction);
  }
  return 0;
}

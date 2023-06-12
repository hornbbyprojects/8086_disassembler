#include "stdio.h"
#include "stdlib.h"
#include <bits/types.h>

unsigned char read_byte(FILE *file) {
  unsigned char ret;
  if (!fread(&ret, 1, 1, file)) {
    fprintf(stderr, "Unexpected end of file\n");
  }
  return ret;
}

struct MovRegRM {
  unsigned char flag_d;
  unsigned char flag_w;
  unsigned char mod;
  unsigned char reg;
  unsigned char r_m;
  unsigned char displacement[2];
};

unsigned char get_displacement_length(unsigned char mod, unsigned char r_m) {
  switch (mod) {
  case 0:
    if (r_m == 0b110) {
      return 2;
    } else {
      return 0;
    }
    break;
  case 1:
    return 1;
  case 2:
    return 2;
  case 3:
    return 0;
  }
  fprintf(stderr, "Unexpected mod %i\n", mod);
  exit(-2);
}

struct MovRegRM parse_mov_reg_rm(FILE *file, unsigned char first_byte) {
  struct MovRegRM ret;
  unsigned char second_byte = read_byte(file);
  ret.flag_d = first_byte & 0b10;
  ret.flag_w = first_byte & 0b1;
  ret.mod = second_byte >> 6;
  ret.reg = (second_byte >> 3) & 0b111;
  ret.r_m = second_byte & 0b111;
  unsigned char displacement_length = get_displacement_length(ret.mod, ret.r_m);
  for (int i = 0; i < displacement_length; ++i) {
    ret.displacement[i] = read_byte(file);
  }
  return ret;
}

struct MovImmRM {
  unsigned char flag_w;
  unsigned char mod;
  unsigned char r_m;
  unsigned char displacement[2];
  unsigned char data[2];
};

struct MovImmRM parse_mov_imm_rm(FILE *file, unsigned char first_byte) {
  struct MovImmRM ret;
  unsigned char second_byte = read_byte(file);
  ret.flag_w = first_byte & 1;
  ret.mod = second_byte >> 6;
  ret.r_m = second_byte & 0b111;
  unsigned char displacement_length = get_displacement_length(ret.mod, ret.r_m);
  for (int i = 0; i < displacement_length; ++i) {
    ret.displacement[i] = read_byte(file);
  }
  ret.data[0] = read_byte(file);
  if (ret.flag_w) {
    ret.data[1] = read_byte(file);
  }
  return ret;
}

struct MovImmReg {
  unsigned char flag_w;
  unsigned char reg;
  unsigned char data[2];
};

struct MovImmReg parse_mov_imm_reg(FILE *file, unsigned char first_byte) {
  struct MovImmReg ret;
  ret.flag_w = (first_byte >> 3) & 1;
  ret.reg = first_byte & 0b111;
  ret.data[0] = read_byte(file);
  if (ret.flag_w) {
    ret.data[1] = read_byte(file);
  }
  return ret;
}

struct MovMemAcc {
  unsigned char flag_w;
  unsigned char data[2];
};

struct MovMemAcc parse_mov_mem_acc(FILE *file, unsigned char first_byte) {
  struct MovMemAcc ret;
  ret.flag_w = first_byte & 1;
  ret.data[0] = read_byte(file);
  if (ret.flag_w) {
    ret.data[1] = read_byte(file);
  }
  return ret;
}

struct MovAccMem {
  unsigned char flag_w;
  unsigned char data[2];
};
struct MovAccMem parse_mov_acc_mem(FILE *file, unsigned char first_byte) {
  struct MovAccMem ret;
  ret.flag_w = first_byte & 1;
  ret.data[0] = read_byte(file);
  if (ret.flag_w) {
    ret.data[1] = read_byte(file);
  }
  return ret;
}
enum InstructionType {
  INSTRUCTION_TYPE_MOV_REG_RM,
  INSTRUCTION_TYPE_MOV_IMM_RM,
  INSTRUCTION_TYPE_MOV_IMM_REG,
  INSTRUCTION_TYPE_MOV_MEM_ACC,
  INSTRUCTION_TYPE_MOV_ACC_MEM,
};

struct Instruction {
  enum InstructionType type;
  union {
    struct MovRegRM mov_reg_rm;
    struct MovImmRM mov_imm_rm;
    struct MovImmReg mov_imm_reg;
    struct MovMemAcc mov_mem_acc;
    struct MovAccMem mov_acc_mem;
  };
};

struct Instruction parse_instruction(FILE *file, unsigned char first_byte) {
  struct Instruction ret;
  if ((first_byte >> 2) == 0b100010) {
    ret.type = INSTRUCTION_TYPE_MOV_REG_RM;
    ret.mov_reg_rm = parse_mov_reg_rm(file, first_byte);
  } else if ((first_byte >> 1) == 0b1100011) {
    ret.type = INSTRUCTION_TYPE_MOV_IMM_RM;
    ret.mov_imm_rm = parse_mov_imm_rm(file, first_byte);
  } else if ((first_byte >> 4) == 0b1011) {
    ret.type = INSTRUCTION_TYPE_MOV_IMM_REG;
    ret.mov_imm_reg = parse_mov_imm_reg(file, first_byte);
  } else if ((first_byte >> 1) == 0b1010000) {
    ret.type = INSTRUCTION_TYPE_MOV_MEM_ACC;
    ret.mov_mem_acc = parse_mov_mem_acc(file, first_byte);
  } else if ((first_byte >> 1) == 0b1010001) {
    ret.type = INSTRUCTION_TYPE_MOV_ACC_MEM;
    ret.mov_acc_mem = parse_mov_acc_mem(file, first_byte);
  } else {
    fprintf(stderr, "Unrecognised first byte %i\n", first_byte);
    exit(-1);
  }
  return ret;
}

// PRINTING

#define NUM_REGISTER_STRINGS 8
const char *wide_register_strings[NUM_REGISTER_STRINGS] = {
    "ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *short_register_strings[NUM_REGISTER_STRINGS] = {
    "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void print_reg(unsigned char reg, unsigned char flag_w) {
  if (reg >= NUM_REGISTER_STRINGS) {
    fprintf(stderr, "Invalid register %i!\n", reg);
    exit(-1);
  }
  if (flag_w) {
    printf("%s", wide_register_strings[reg]);
  } else {
    printf("%s", short_register_strings[reg]);
  }
}
__int16_t one_byte_displacement(unsigned char bytes[2]) {
  return (__int16_t)((signed char)bytes[0]);
}
__int16_t two_byte_displacement(unsigned char bytes[2]) {
  // Second byte is the most significant
  return ((__uint16_t)bytes[1]) << 8 | bytes[0];
}
#define NUM_MEMORY_R_M_STRINGS 8
const char *memory_r_m_strings[NUM_MEMORY_R_M_STRINGS] = {
    "BX + SI", "BX + DI", "BP + SI", "BP + DI", "SI", "DI", "BP", "BX",
};
void print_r_m(unsigned char r_m, unsigned char flag_w, unsigned char mod,
               unsigned char displacement_bytes[2]) {
  __int16_t displacement;
  if (r_m >= NUM_MEMORY_R_M_STRINGS) {
    fprintf(stderr, "Invalid r_m %i!\n", r_m);
    exit(-1);
  }
  switch (mod) {
  case 0:
    if (r_m == 0b110) {
      printf("[%i]", two_byte_displacement(displacement_bytes));
    } else {
      printf("[%s]", memory_r_m_strings[r_m]);
    }
    return;
  case 1:
    displacement = one_byte_displacement(displacement_bytes);
    break;
  case 2:
    displacement = two_byte_displacement(displacement_bytes);
    break;
  case 3:
    print_reg(r_m, flag_w);
    return;
  }
  if (displacement < 0) {
    printf("[%s - %i]", memory_r_m_strings[r_m], -displacement);
  } else {
    printf("[%s + %i]", memory_r_m_strings[r_m], displacement);
  }
}
void print_mov_reg_rm(struct MovRegRM instruction) {
  printf("mov ");
  if (instruction.flag_d) {
    print_reg(instruction.reg, instruction.flag_w);
    printf(", ");
    print_r_m(instruction.r_m, instruction.flag_w, instruction.mod,
              instruction.displacement);
    printf("\n");
  } else {
    print_r_m(instruction.r_m, instruction.flag_w, instruction.mod,
              instruction.displacement);
    printf(", ");
    print_reg(instruction.reg, instruction.flag_w);
    printf("\n");
  }
}

void print_immediate(unsigned char data[2], unsigned char flag_w) {
  if (flag_w) {
    printf("word %i", (((__uint16_t)data[1]) << 8) | (__uint16_t)data[0]);
  } else {
    printf("byte %i", (__uint8_t)data[0]);
  }
}

void print_mov_imm_rm(struct MovImmRM instruction) {
  printf("mov ");
  print_r_m(instruction.r_m, instruction.flag_w, instruction.mod,
            instruction.displacement);
  printf(", ");
  print_immediate(instruction.data, instruction.flag_w);
  printf("\n");
}

void print_mov_imm_reg(struct MovImmReg instruction) {
  printf("mov ");
  print_reg(instruction.reg, instruction.flag_w);
  printf(", ");
  print_immediate(instruction.data, instruction.flag_w);
  printf("\n");
}

void print_mov_mem_acc(struct MovMemAcc instruction) {
  printf("mov ax, [%i]\n", two_byte_displacement(instruction.data));
}

void print_mov_acc_mem(struct MovAccMem instruction) {
  printf("mov [%i], ax\n", two_byte_displacement(instruction.data));
}

void print_instruction(struct Instruction instruction) {
  switch (instruction.type) {
  case INSTRUCTION_TYPE_MOV_REG_RM:
    print_mov_reg_rm(instruction.mov_reg_rm);
    break;
  case INSTRUCTION_TYPE_MOV_IMM_RM:
    print_mov_imm_rm(instruction.mov_imm_rm);
    break;
  case INSTRUCTION_TYPE_MOV_IMM_REG:
    print_mov_imm_reg(instruction.mov_imm_reg);
    break;
  case INSTRUCTION_TYPE_MOV_MEM_ACC:
    print_mov_mem_acc(instruction.mov_mem_acc);
    break;
  case INSTRUCTION_TYPE_MOV_ACC_MEM:
    print_mov_acc_mem(instruction.mov_acc_mem);
    break;
  }
}

int main(int argc, char *argv[]) {
  unsigned char first_byte;
  struct Instruction instruction;
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [[filepath of binary]]\n", argv[0]);
  }
  FILE *infile = fopen(argv[1], "rb");
  if (!infile) {
    fprintf(stderr, "File %s not found!\n", argv[1]);
    exit(-2);
  }
  printf("bits 16\n");
  while (1) {
    if (!fread(&first_byte, 1, 1, infile)) {
      break;
    }
    instruction = parse_instruction(infile, first_byte);
    print_instruction(instruction);
  }
}

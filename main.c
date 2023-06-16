#include "arithmetic.h"
#include "jumps.h"
#include "shared.h"
#include "stdio.h"
#include "stdlib.h"
#include <bits/types.h>

struct MovRegRM {
  unsigned char flag_d;
  unsigned char flag_w;
  unsigned char mod;
  unsigned char reg;
  unsigned char r_m;
  unsigned char displacement[2];
};

struct MovRegRM parse_mov_reg_rm(FILE *file, unsigned char first_byte) {
  struct MovRegRM ret;
  unsigned char second_byte = read_byte(file);
  ret.flag_d = (first_byte >> 1) & 1;
  ret.flag_w = first_byte & 1;
  ret.mod = second_byte >> 6;
  ret.reg = (second_byte >> 3) & 0b111;
  ret.r_m = second_byte & 0b111;
  read_displacement(file, ret.mod, ret.r_m, ret.displacement);
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
  read_displacement(file, ret.mod, ret.r_m, ret.displacement);
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
  INSTRUCTION_TYPE_BINARY_REG_RM,
  INSTRUCTION_TYPE_BINARY_IMM_RM,
  INSTRUCTION_TYPE_BINARY_IMM_ACC,
  INSTRUCTION_TYPE_JUMP,
};

struct Instruction {
  enum InstructionType type;
  union {
    struct MovRegRM mov_reg_rm;
    struct MovImmRM mov_imm_rm;
    struct MovImmReg mov_imm_reg;
    struct MovMemAcc mov_mem_acc;
    struct MovAccMem mov_acc_mem;
    struct BinaryRegRM binary_reg_rm;
    struct BinaryImmRM binary_imm_rm;
    struct BinaryImmAcc binary_imm_acc;
    struct Jump jump;
  };
};

struct Instruction parse_instruction(FILE *file, unsigned char first_byte) {
  struct Instruction ret;
  signed char jump_code_as_ordinal =
    jump_code_to_ordinal(first_byte);
  if (jump_code_as_ordinal != -1) {
    ret.type = INSTRUCTION_TYPE_JUMP;
    ret.jump = parse_jump(file, jump_code_as_ordinal);
  }
  else if ((first_byte >> 2) == 0b100010) {
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
  } else if (((first_byte >> 2) & 0b110001) == 0) {
    ret.type = INSTRUCTION_TYPE_BINARY_REG_RM;
    ret.binary_reg_rm = parse_binary_reg_rm(file, first_byte);
  } else if ((first_byte >> 2) == 0b100000) {
    ret.type = INSTRUCTION_TYPE_BINARY_IMM_RM;
    ret.binary_imm_rm = parse_binary_imm_rm(file, first_byte);
  } else if (((first_byte >> 1) & 0b1100011)  == 0b10) {
    ret.type = INSTRUCTION_TYPE_BINARY_IMM_ACC;
    ret.binary_imm_acc = parse_binary_imm_acc(file, first_byte);
  } else {
    fprintf(stderr, "Unrecognised first byte %i\n", first_byte);
    exit(-1);
  }
  return ret;
}

// PRINTING

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

void print_mov_imm_rm(struct MovImmRM instruction) {
  printf("mov ");
  print_r_m(instruction.r_m, instruction.flag_w, instruction.mod,
            instruction.displacement);
  printf(", ");
  print_immediate(instruction.data, instruction.flag_w, 0);
  printf("\n");
}

void print_mov_imm_reg(struct MovImmReg instruction) {
  printf("mov ");
  print_reg(instruction.reg, instruction.flag_w);
  printf(", ");
  print_immediate(instruction.data, instruction.flag_w, 0);
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
  case INSTRUCTION_TYPE_BINARY_REG_RM:
    print_binary_reg_rm(instruction.binary_reg_rm);
    break;
  case INSTRUCTION_TYPE_BINARY_IMM_RM:
    print_binary_imm_rm(instruction.binary_imm_rm);
    break;
  case INSTRUCTION_TYPE_BINARY_IMM_ACC:
    print_binary_imm_acc(instruction.binary_imm_acc);
    break;
  case INSTRUCTION_TYPE_JUMP:
    print_jump(instruction.jump);
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

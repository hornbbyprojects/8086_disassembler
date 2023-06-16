#include "shared.h"
#include "stdio.h"
#include "stdlib.h"
#include "arithmetic.h"


void print_operation(unsigned char operation) {
  switch(operation) {
  case 0b000:
    printf("add");
    break;
  case 0b101:
    printf("sub");
    break;
  case 0b111:
    printf("cmp");
    break;
  }
  printf(" ");
}

struct BinaryRegRM parse_binary_reg_rm(FILE *file, unsigned char first_byte){
  struct BinaryRegRM ret;
  ret.flag_d = (first_byte >> 1) & 1;
  ret.flag_w = first_byte & 1;
  unsigned char second_byte = read_byte(file);
  ret.mod = second_byte >> 6;
  ret.reg = (second_byte >> 3) & 0b111;
  ret.r_m = second_byte & 0b111;
  ret.operation = (first_byte >> 3) & 0b111;
  read_displacement(file, ret.mod, ret.r_m, ret.displacement);
  return ret;
}

void print_binary_reg_rm(struct BinaryRegRM instruction) {
  print_operation(instruction.operation);
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


struct BinaryImmRM parse_binary_imm_rm(FILE *file, unsigned char first_byte) {
  struct BinaryImmRM ret;
  unsigned char second_byte = read_byte(file);
  ret.flag_w = first_byte & 1;
  ret.flag_s = (first_byte >> 1) & 1;
  ret.mod = second_byte >> 6;
  ret.r_m = second_byte & 0b111;
  ret.operation = (second_byte >> 3) & 0b111;
  read_displacement(file, ret.mod, ret.r_m, ret.displacement);
  ret.data[0] = read_byte(file);
  if (!ret.flag_s && ret.flag_w) {
    ret.data[1] = read_byte(file);
  }
  return ret;
}

void print_binary_imm_rm(struct BinaryImmRM instruction) {
  print_operation(instruction.operation);
  print_r_m(instruction.r_m, instruction.flag_w, instruction.mod,
            instruction.displacement);
  printf(", ");
  print_immediate(instruction.data, instruction.flag_w, instruction.flag_s);
  printf("\n");
}

struct BinaryImmAcc parse_binary_imm_acc(FILE *file, unsigned char first_byte) {
  struct BinaryImmAcc ret;
  ret.flag_w = first_byte & 1;
  ret.data[0] = read_byte(file);
  ret.operation = (first_byte >> 3) & 0b111;
  if (ret.flag_w) {
    ret.data[1] = read_byte(file);
  }
  return ret;
}

void print_binary_imm_acc(struct BinaryImmAcc instruction) {
  print_operation(instruction.operation);
  if(instruction.flag_w) {
    printf("ax, %i\n", two_byte_displacement(instruction.data));
  }
  else {
    printf("al, %i\n", one_byte_displacement(instruction.data));
  }
}

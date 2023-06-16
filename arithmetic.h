#ifndef __ARITHMETIC_H
#define __ARITHMETIC_H

#include <stdio.h>

struct BinaryRegRM {
  unsigned char operation;
  unsigned char flag_d;
  unsigned char flag_w;
  unsigned char mod;
  unsigned char reg;
  unsigned char r_m;
  unsigned char displacement[2];
};

struct BinaryRegRM parse_binary_reg_rm(FILE *file, unsigned char first_byte);
void print_binary_reg_rm(struct BinaryRegRM instruction);

struct BinaryImmRM {
  unsigned char operation;
  unsigned char flag_w;
  unsigned char flag_s;
  unsigned char mod;
  unsigned char r_m;
  unsigned char displacement[2];
  unsigned char data[2];
};
struct BinaryImmRM parse_binary_imm_rm(FILE *file, unsigned char first_byte);
void print_binary_imm_rm(struct BinaryImmRM instruction);

struct BinaryImmAcc {
  unsigned char operation;
  unsigned char flag_w;
  unsigned char data[2];
};
void print_binary_imm_acc(struct BinaryImmAcc instruction);
struct BinaryImmAcc parse_binary_imm_acc(FILE * file, unsigned char first_byte);

#endif

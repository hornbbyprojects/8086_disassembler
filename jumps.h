#ifndef __JUMPS_H
#define __JUMPS_H
#include "stdio.h"

// clang-format off
enum JumpCode {
  JE =     0b01110100,
  JL =     0b01111100,
  JLE =    0b01111110,
  JB =     0b01110010,
  JBE =    0b01110110,
  JP =     0b01111010,
  JO =     0b01110000,
  JS =     0b01111000,
  JNE =    0b01110101,
  JNL =    0b01111101,
  JG =     0b01111111,
  JAE =    0b01110011,
  JA =     0b01110111,
  JNP =    0b01111011,
  JNO =    0b01110001,
  JNS =    0b01111001,
  LOOP =   0b11100010,
  LOOPE =  0b11100001,
  LOOPNE = 0b11100000,
  JCXZ =   0b11100011,
};
// clang-format on
struct Jump {
  unsigned char jump_code_as_ordinal;
  signed char displacement;
};
void print_jump(struct Jump instruction);
struct Jump parse_jump(FILE *file, unsigned char jump_code_as_ordinal);
signed char jump_code_to_ordinal(unsigned char code);
#endif

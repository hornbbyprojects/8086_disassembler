#ifndef __SIMULATOR_H
#define __SIMULATOR_H
#include <bits/types.h>
#include "instruction_data.h"
typedef __uint16_t u16;
typedef unsigned char u8;

struct Simulated8086 {
  u16 registers[8];
  u8 flag_Z, flag_S;
  u8 memory[0xffff];
  i16 pc;
};
void simulate_instruction(struct Simulated8086 *comp,
                          struct Instruction *instruction);
#endif

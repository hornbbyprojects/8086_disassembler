
#ifndef __SIMULATOR_H
#define __SIMULATOR_H
#include <bits/types.h>
#include "instruction_data.h"
typedef __uint16_t u16;
typedef unsigned char u8;

struct Simulated8086 {
  u16 registers[8];
};
void simulate_instruction(struct Simulated8086 *comp,
                          struct Instruction *instruction);
#endif

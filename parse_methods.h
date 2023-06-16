
#ifndef __PARSE_METHODS_H
#define __PARSE_METHODS_H
#include "instruction_data.h"
#include "stdio.h"
struct Instruction instruction_from_data(struct InstructionData data);
struct InstructionData parse(FILE *file);
#endif

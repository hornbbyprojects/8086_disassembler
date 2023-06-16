#include "instruction_data.h"
#include "parse_methods.h"
#include "printing.h"
#include "simulator.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

int main_disassemble(char *binary_path) {
  struct Instruction instruction;
  FILE *infile = fopen(binary_path, "rb");
  if (!infile) {
    fprintf(stderr, "File %s not found!\n", binary_path);
    exit(-2);
  }
  printf("bits 16\n");
  while (1) {
    u8 head = fgetc(infile);
    if (feof(infile)) {
      break;
    }
    if (ferror(infile)) {
      fprintf(stderr, "ERROR %i reading file\n", ferror(infile));
      break;
    }
    ungetc(head, infile);
    struct InstructionData instruction_data = parse(infile);
    if (instruction_data.failed) {
      fprintf(stdout, "No parser succeeded for first byte %i\n", head);
      exit(-2);
      break;
    }
    instruction = instruction_from_data(instruction_data);
    print_instruction(&instruction);
  }
  return 0;
}

enum RegisterCodes PRINT_ORDER[] = {
    AX, BX, CX, DX, SP, BP, SI, DI,
};
int main_simulate(char *binary_path) {
  struct Simulated8086 comp;
  struct Instruction instruction;
  FILE *infile = fopen(binary_path, "rb");
  if (!infile) {
    fprintf(stderr, "File %s not found!\n", binary_path);
    exit(-2);
  }
  while (1) {
    u8 head = fgetc(infile);
    if (feof(infile)) {
      break;
    }
    if (ferror(infile)) {
      fprintf(stderr, "ERROR %i reading file\n", ferror(infile));
      break;
    }
    ungetc(head, infile);
    struct InstructionData instruction_data = parse(infile);
    if (instruction_data.failed) {
      fprintf(stdout, "No parser succeeded for first byte %i\n", head);
      exit(-2);
      break;
    }
    instruction = instruction_from_data(instruction_data);
    simulate_instruction(&comp, &instruction);
  }
  for (int i = 0; i < sizeof(PRINT_ORDER) / sizeof(PRINT_ORDER[0]); i++) {
    print_register_code(PRINT_ORDER[i], 1);
    printf(" = %i\n", comp.registers[PRINT_ORDER[i]]);
  }
  return 0;
}
int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr,
            "Usage: %s [[simulate | disassemble]] [[filepath of binary]]\n",
            argv[0]);
    exit(-2);
  }
  if (!strcmp("simulate", argv[1])) {
    return main_simulate(argv[2]);
  }
  if (!strcmp("disassemble", argv[1])) {
    return main_disassemble(argv[2]);
  }
  fprintf(stderr, "Unrecognised operation %s", argv[1]);
  exit(-1);
}

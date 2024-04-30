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
void print_simulator_state(struct Simulated8086 *comp) {
  for (int i = 0; i < sizeof(PRINT_ORDER) / sizeof(PRINT_ORDER[0]); i++) {
    print_register_code(PRINT_ORDER[i], 1);
    printf(": %1$#.4x (%1$i)\n", comp->registers[PRINT_ORDER[i]]);
  }
  printf("flags: ");
  if (comp->flag_S) {
    printf("S");
  }
  if (comp->flag_Z) {
    printf("Z");
  }
  printf("\n");
}
int main_simulate(char *binary_path, u8 print_intermediate, u8 dump_memory) {
  struct Simulated8086 comp;
  struct Instruction instruction;
  comp.pc = 0;
  FILE *infile = fopen(binary_path, "rb");
  if (!infile) {
    fprintf(stderr, "File %s not found!\n", binary_path);
    exit(-2);
  }
  size_t bytes_read = fread(&comp.memory, 1, 0xffff, infile);
  if (!feof(infile)) {
    fprintf(stderr, "Failed to read whole file!\n");
    exit(-2);
  }
  FILE *memfile = fmemopen(&comp.memory, bytes_read, "r");
  while (1) {
    if (comp.pc >= bytes_read) {
      break;
    }
    fseek(memfile, comp.pc, SEEK_SET);
    struct InstructionData instruction_data = parse(memfile);
    if (instruction_data.failed) {
      fprintf(stdout, "No parser succeeded\n");
      exit(-2);
      break;
    }
    instruction = instruction_from_data(instruction_data);
    comp.pc = ftell(memfile);
    if (print_intermediate) {
      print_instruction(&instruction);
    }
    simulate_instruction(&comp, &instruction);
    if (print_intermediate) {
      print_simulator_state(&comp);
      printf("pc: %i\n", comp.pc);
    }
  }
  printf("Final state:\n");
  print_simulator_state(&comp);
  if (dump_memory) {
    FILE *outfile = fopen("memory.data", "w");
    u8 *writeptr = comp.memory;
    while (1) {
      u16 to_write = 0xffff - (writeptr - comp.memory);
      printf("Trying to write %i bytes\n", to_write);
      size_t bytes_written = fwrite(writeptr, 1, to_write, outfile);
      printf("Wrote %li bytes\n", bytes_written);
      if (bytes_written == 0) {
        if (ferror(outfile)) {
          fprintf(stderr, "Error writing to outfile\n");
        }
        break;
      }
      writeptr += bytes_written;
    }
  }
  return 0;
}
int main(int argc, char *argv[]) {
  u8 print_intermediate = 0, dump_memory = 1;
  if (argc < 3) {
    fprintf(stderr,
            "Usage: %s [[simulate | disassemble]] [[filepath of binary]] "
            "[[opts...]]\n",
            argv[0]);
    exit(-2);
  }
  if (!strcmp("simulate", argv[1])) {
    return main_simulate(argv[2], print_intermediate, dump_memory);
  }
  if (!strcmp("disassemble", argv[1])) {
    return main_disassemble(argv[2]);
  }
  fprintf(stderr, "Unrecognised operation %s", argv[1]);
  exit(-1);
}

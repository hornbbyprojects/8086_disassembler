#include "stdio.h"
#include "stdlib.h"

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

unsigned char read_byte(FILE *file) {
  unsigned char ret;
  if (!fread(&ret, 1, 1, file)) {
    fprintf(stderr, "Unexpected end of file\n");
  }
  return ret;
}

void read_displacement(FILE *file, unsigned char mod, unsigned char r_m, unsigned char buffer[2]) {
  unsigned char displacement_length = get_displacement_length(mod, r_m);
  for (int i = 0; i < displacement_length; ++i) {
    buffer[i] = read_byte(file);
  }
}
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

void print_immediate(unsigned char data[2], unsigned char flag_w, unsigned char flag_s) {
  if (flag_w) {
    if (flag_s) {
      printf("word %i", (__uint8_t)data[0]);
    }
    else {
      printf("word %i", (((__uint16_t)data[1]) << 8) | (__uint16_t)data[0]);
    }
  } else {
    printf("byte %i", (__uint8_t)data[0]);
  }
}

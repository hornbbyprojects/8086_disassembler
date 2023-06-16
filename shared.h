
#ifndef __SHARED_H
#define __SHARED_H
#include "stdio.h"
unsigned char get_displacement_length(unsigned char mod, unsigned char r_m);
void read_displacement(FILE *file, unsigned char mod, unsigned char r_m,
                       unsigned char buffer[2]);
unsigned char read_byte(FILE *file);
void print_r_m(unsigned char r_m, unsigned char flag_w, unsigned char mod,
               unsigned char displacement_bytes[2]);
void print_reg(unsigned char reg, unsigned char flag_w);
__int16_t one_byte_displacement(unsigned char bytes[2]);
__int16_t two_byte_displacement(unsigned char bytes[2]);
void print_immediate(unsigned char data[2], unsigned char flag_w, unsigned char flag_s);

#endif

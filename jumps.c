#include "jumps.h"
#include "shared.h"

signed char jump_code_to_ordinal(unsigned char code) {
  switch (code) {
  case JE:
    return 0;
  case JL:
    return 1;
  case JLE:
    return 2;
  case JB:
    return 3;
  case JBE:
    return 4;
  case JP:
    return 5;
  case JO:
    return 6;
  case JS:
    return 7;
  case JNE:
    return 8;
  case JNL:
    return 9;
  case JG:
    return 10;
  case JAE:
    return 11;
  case JA:
    return 12;
  case JNP:
    return 13;
  case JNO:
    return 14;
  case JNS:
    return 15;
  case LOOP:
    return 16;
  case LOOPE:
    return 17;
  case LOOPNE:
    return 18;
  case JCXZ:
    return 19;
  default:
    return -1;
  };
}

char jump_strings[][7] = {
    "JE",
    "JL",
    "JLE",
    "JB",
    "JBE",
    "JP",
    "JO",
    "JS",
    "JNE",
    "JNL",
    "JG",
    "JAE",
    "JA",
    "JNP",
    "JNO",
    "JNS",
    "LOOP",
    "LOOPE",
    "LOOPNE",
    "JCXZ",
};

struct Jump parse_jump(FILE *file, unsigned char jump_code_as_ordinal) {
  struct Jump ret;
  ret.jump_code_as_ordinal = jump_code_as_ordinal;
  ret.displacement = read_byte(file);
  return ret;
}

void print_jump(struct Jump instruction) {
  printf("%s ", jump_strings[instruction.jump_code_as_ordinal]);
  __int16_t displacement_from_last_instruction = instruction.displacement + 2;
  if (displacement_from_last_instruction >= 0) {
    printf("$+%i\n", displacement_from_last_instruction);
  } else {
    printf("$%i\n", displacement_from_last_instruction);
  }
}

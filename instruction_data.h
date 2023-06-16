#ifndef __INSTRUCTION_DATA_h
#define __INSTRUCTION_DATA_h
#include "bits/types.h"

typedef unsigned char u8;
typedef signed char i8;
typedef __uint16_t u16;
typedef __int16_t i16;

#include "operation_enum.inl"
struct InstructionData {
  enum InstructionType instruction_type;
  u8 mod, reg, r_m, failed, flag_d, flag_w, flag_s, has_immediate, jump_destination, has_jump_destination;
  i16 displacement, data;
};
struct Register {
  u8 code;
  u8 flag_w;
};
struct Immediate {
  i16 value;
  u8 length; // byte, word, etc
};
enum ShortRegisterCodes {
  AL,
  CL,
  DL,
  BL,
  AH,
  CH,
  DH,
  BH
      };
enum RegisterCodes {
  AX,
  CX,
  DX,
  BX,
  SP,
  BP,
  SI,
  DI
};

enum AddressType {
  ADDRESS_BX_SI,
  ADDRESS_BX_DI,
  ADDRESS_BP_SI,
  ADDRESS_BP_DI,
  ADDRESS_SI,
  ADDRESS_DI,
  ADDRESS_BP,
  ADDRESS_BX,
};
struct Memory {
  enum AddressType address_type;
  i16 displacement;
  u8 is_displaced;
};
struct DirectMemory {
  i16 displacement;
};
struct JumpDestination {
  signed char jump_destination;
};
enum OperandType {
  OPERAND_TYPE_MISSING,
  OPERAND_TYPE_REGISTER,
  OPERAND_TYPE_IMMEDIATE,
  OPERAND_TYPE_MEMORY,
  OPERAND_TYPE_DIRECT_MEMORY,
  OPERAND_TYPE_JUMP_DESTINATION,
};
struct Operand {
  enum OperandType operand_type;
  union {
    struct Register _register;
    struct Immediate immediate;
    struct Memory memory;
    struct DirectMemory direct_memory;
    struct JumpDestination jump_destination;
  };
};
struct Instruction {
  enum InstructionType instruction_type;
  struct Operand destination, source;
};

#endif

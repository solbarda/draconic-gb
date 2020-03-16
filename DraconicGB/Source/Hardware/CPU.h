#pragma once

#include "Opcodes.h"

#define FLAG_MASK_ZERO 0b10000000;
#define FLAG_MASK_SUBTRACT 0b01000000;
#define FLAG_MASK_HALFCARRY 0b00100000;
#define FLAG_MASK_CARRY 0b00010000;

struct Registers
{
  unsigned char A;
  unsigned char B;
  unsigned char C;
  unsigned char D;
  unsigned char E;
  unsigned char H;
  unsigned char L;
  unsigned char F; // Flags
  unsigned char SP; // Stack Pointer
  unsigned char PC; // Program Counter
};

struct State
{
  Registers registers;
  class Memory* memory;
};

class CPU
{

public:
  State state;

  void ExecuteOpcode(GBOpcode opcode);
};


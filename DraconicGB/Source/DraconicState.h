#pragma once
#include "Hardware/DraconicMemory.h"

using reg16_t = uint16_t;
using reg8_t = uint8_t;
using address16_t = uint16_t;

// Struct to store the value of the different registers
struct Registers
{
  struct {
    union {
      struct {
        reg8_t F;
        reg8_t A;
      };
      reg16_t AF;
    };
  };
  struct {
    union {
      struct {
        reg8_t C;
        reg8_t B;
      };
      reg16_t BC;
    };
  };
  struct {
    union {
      struct {
        reg8_t E;
        reg8_t D;
      };
      reg16_t DE;
    };
  };
  struct {
    union {
      struct {
        reg8_t L;
        reg8_t H;
      };
      reg16_t HL;
    };
  };
  
  reg16_t SP = 0; // Stack Pointer
  reg16_t PC = 0x100; // Program Counter

  void Reset()
  {
    A = 0;
    B = 0;
    C = 0;
    D = 0;
    E = 0;
    H = 0;
    L = 0;
    F = 0;
    SP = 0;
    PC = 0x100;
  }
};

// State of the application, in this case this means the Memory and the CPU registers
class DraconicState
{
public:
  Registers registers;
  DraconicMemory memory;
  int numCycles = 0;

public:
  // Functions
  void SetFlag(int flag, bool value);
  void ParseOpcode(uint8_t opCode);

public:
  // 8-bit Arithmetic and Logic Instructions

  void ADC(uint8_t& target, uint8_t value);
  void ADC_A_R8(uint8_t value);
  void ADC_A_HL();
  void ADC_A_N8(uint8_t value);

  void ADD(uint8_t& target, uint8_t value);
  void ADD_A_R8(uint8_t value);
  void ADD_A_HL();
  void ADD_A_N8(uint8_t value);

  void AND(uint8_t& target, uint8_t value);
  void AND_A_R8(uint8_t value);
  void AND_A_HL();
  void AND_A_N8(uint8_t value);

  void CP(uint8_t& target, uint8_t value);
  void CP_A_R8(uint8_t value);
  void CP_A_HL();
  void CP_A_N8(uint8_t value);

};

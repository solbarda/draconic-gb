#pragma once
#include "Hardware/DraconicMemory.h"

// Struct to store the value of the different registers
struct Registers
{
  uint8_t A = 0;
  uint8_t B = 0;
  uint8_t C = 0;
  uint8_t D = 0;
  uint8_t E = 0;
  uint8_t H = 0;
  uint8_t L = 0;
  uint8_t F = 0; // Flags
  uint16_t SP = 0; // Stack Pointer
  uint16_t PC = 0x100; // Program Counter

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
};

#pragma once
#include "Hardware/DraconicMemory.h"

// Struct to store the value of the different registers
struct Registers
{
  uint8_t A;
  uint8_t B;
  uint8_t C;
  uint8_t D;
  uint8_t E;
  uint8_t H;
  uint8_t L;
  uint8_t F; // Flags
  uint16_t SP; // Stack Pointer
  uint16_t PC; // Program Counter
};

// State of the application, in this case this means the Memory and the CPU registers
class DraconicState
{
public:
  Registers registers;
  DraconicMemory memory;
};

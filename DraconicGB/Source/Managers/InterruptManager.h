#pragma once
#include "SDL_config.h"
#include "Hardware/BaseHardware.h"

// Handles interruptons of the cpu
class InterruptManager : public BaseHardware{

public:
  // Request an interrupt of the specified bit
  void RequestInterrupt(uint8_t id);
  // Perform internal interrupts
  void PerformInterrupts();
private:
  // Perform a service interrupt such as VBLANK, SERIAL or JOYPAD
  void ServiceInterrupt(uint8_t id);
};



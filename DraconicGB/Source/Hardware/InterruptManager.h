#pragma once
#include "SDL_config.h"
#include "BaseHardware.h"




class InterruptManager : public BaseHardware{

public:
  void RequestInterrupt(uint8_t id);
  void DoInterrupts();
private:
  void ServiceInterrupt(uint8_t id);
};



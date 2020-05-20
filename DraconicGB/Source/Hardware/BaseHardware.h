#pragma once
#include "SDL_config.h"
#include "SDL_events.h"

// Base hardware class that stores a pointer to the state and can be initialized with it
class BaseHardware {

public:
  // Init the hardware with the emulator state
  void Init(class DraconicState* _state);

protected:
  // Store a pointer to the emulator state
  class DraconicState* state;
};



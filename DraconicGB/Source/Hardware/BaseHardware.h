#pragma once
#include "SDL_config.h"
#include "SDL_events.h"


class BaseHardware {

public:
  void Init(class DraconicState* _state);

protected:
  class DraconicState* state;
};



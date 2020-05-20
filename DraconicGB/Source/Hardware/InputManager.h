#pragma once
#include "SDL_config.h"
#include "SDL_events.h"
#include "BaseHardware.h"
#include "Utils.h"


class InputManager : public BaseHardware {

public:
  void OnKeyPressed(SDL_KeyboardEvent key);
  void OnKeyReleased(SDL_KeyboardEvent key);

private:
  EBit GetKeyID(SDL_Keysym key);
  void ProcessInputData(SDL_KeyboardEvent& key, bool bPressed);
};



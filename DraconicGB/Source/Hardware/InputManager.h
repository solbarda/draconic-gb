#pragma once
#include "SDL_config.h"
#include "SDL_events.h"
#include "BaseHardware.h"
#include "Utils.h"

// Class responsible for handling input and altering input registers/memory
class InputManager : public BaseHardware {

public:
  // SDL Event functions called from the emeulator
  void OnKeyPressed(SDL_KeyboardEvent key);
  void OnKeyReleased(SDL_KeyboardEvent key);

private:
  // Convert and SDL Key into a bit
  EBit GetKeyID(SDL_Keysym key);
  // Modify state based on key and if that key is pressed or not
  void ProcessInputData(SDL_KeyboardEvent& key, bool bPressed);
};



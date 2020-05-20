#include "InputManager.h"
#include "Utils/Utils.h"
#include "Hardware/DraconicState.h"
#include "SDL_events.h"


void InputManager::ProcessInputData(SDL_KeyboardEvent& key, bool bPressedNow)
{
  // Get the key id converted to bit
  EBit keyID = GetKeyID(key.keysym);

  // If id is invalid return
  if (keyID == EBit::BIT_INVALID)
    return;

  // Check to see if we are pressing a directional key
  bool bIsDirectionalKey = false;
  if (key.keysym.sym == SDLK_UP || key.keysym.sym == SDLK_DOWN || key.keysym.sym == SDLK_LEFT || key.keysym.sym == SDLK_RIGHT)
  {
    bIsDirectionalKey = true;
  }

  // Set joypad variable to different value depending if directional key or not
  uint8_t joypad = (bIsDirectionalKey) ? state->memory.joypad_arrows : state->memory.joypad_buttons;
  bool bPressedBefore = !IsBitSet(joypad, (EBit)keyID);

  // If we want to press but the key is already pressed or we want to release but the kye is already released do nothing
  if ((bPressedNow && bPressedBefore) || (!bPressedNow && !bPressedBefore))
    return;

  // Set the corresponding bits
  if (bIsDirectionalKey)
    state->memory.joypad_arrows = bPressedNow ? ClearBit(joypad, keyID) : SetBit(joypad, keyID);
  else
    state->memory.joypad_buttons = bPressedNow ? ClearBit(joypad, keyID) : SetBit(joypad, keyID);

  // Request a joypad interrupt
  *state->memory.GetMemoryLocation(Addr_IF) = SetBit(state->memory.GetMemoryLocationData(Addr_IF), (EBit)EInterrupt::INTERRUPT_JOYPAD);
}

void InputManager::OnKeyPressed(SDL_KeyboardEvent key)
{
  // Process ad pressed key
  ProcessInputData(key, true);
}

void InputManager::OnKeyReleased(SDL_KeyboardEvent key)
{
  // Process a releaed key
  ProcessInputData(key, false);
}

EBit InputManager::GetKeyID(SDL_Keysym key)
{
  // A Button and Right
  if (key.sym == SDLK_z || key.sym == SDLK_RIGHT)
    return EBit::BIT_0;
  // B Button and Left
  if (key.sym == SDLK_x || key.sym == SDLK_LEFT)
    return EBit::BIT_1;
  // Select Button and Up
  if (key.sym == SDLK_s || key.sym == SDLK_UP)
    return EBit::BIT_2;
  // Start Button and Down
  if (key.sym == SDLK_a || key.sym == SDLK_DOWN)
    return EBit::BIT_3;

  // If we do not recognize the key return and invalid bit
  return EBit::BIT_INVALID;
}



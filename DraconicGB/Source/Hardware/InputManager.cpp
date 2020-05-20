#include "InputManager.h"
#include "Utils.h"
#include "DraconicState.h"
#include "SDL_events.h"


void InputManager::ProcessInputData(SDL_KeyboardEvent& key, bool bPressed)
{
  // Get key if
  EBit key_id = GetKeyID(key.keysym);

  // If id is invalid return
  if (key_id == EBit::BIT_INVALID)
    return;

  // Check to see if we are pressing a directional key
  bool directional = false;
  if (key.keysym.sym == SDLK_UP || key.keysym.sym == SDLK_DOWN || key.keysym.sym == SDLK_LEFT || key.keysym.sym == SDLK_RIGHT)
  {
    directional = true;
  }

  // Set joypad variable to different value depending if directional key or not
  uint8_t joypad = (directional) ? state->memory.joypad_arrows : state->memory.joypad_buttons;
  bool unpressed = IsBitSet(joypad, (EBit)key_id);

  // If key is already pressed return
  if ((bPressed && !unpressed) || (!bPressed && unpressed))
    return;

  // Set the corresponding bits
  if (directional)
    state->memory.joypad_arrows = bPressed ? ClearBit(joypad, key_id) : SetBit(joypad, key_id);
  else
    state->memory.joypad_buttons = bPressed ? ClearBit(joypad, key_id) : SetBit(joypad, key_id);

  // Request a joypad interrupt
  state->memory.IF.set_bit((EBit)EInterrupt::INTERRUPT_JOYPAD);
}

void InputManager::OnKeyPressed(SDL_KeyboardEvent key)
{
  ProcessInputData(key, true);
}

void InputManager::OnKeyReleased(SDL_KeyboardEvent key)
{
  ProcessInputData(key, false);
}

EBit InputManager::GetKeyID(SDL_Keysym key)
{
  switch (key.sym)
  {
  case SDLK_z:
  case SDLK_RIGHT:
    return EBit::BIT_0;
  case SDLK_x: // B
  case SDLK_LEFT:
    return EBit::BIT_1;
  case SDLK_s: // select
  case SDLK_UP:
    return EBit::BIT_2;
  case SDLK_a:
  case SDLK_DOWN:
    return EBit::BIT_3;
  default:
    return EBit::BIT_INVALID;
  }
}



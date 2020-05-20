#pragma once
#include "SDL_config.h"
#include "Utils.h"

class SpecialRegister
{
private:
  uint8_t* value;

public:
  SpecialRegister();
  SpecialRegister(uint8_t* _data);
  uint8_t get();
  void set(uint8_t data);
  void clear();
  void set_bit(EBit bit);
  void clear_bit(EBit bit);
  bool is_bit_set(EBit bit);
};
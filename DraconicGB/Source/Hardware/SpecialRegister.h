#pragma once
#include "SDL_config.h"

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
  void set_bit(uint8_t bit);
  void clear_bit(uint8_t bit);
  bool is_bit_set(uint8_t bit);
};
#pragma once

#include "Utils.h"
#include "SDL_config.h"
/*
  Helper Functions
*/
unsigned char high_byte(uint16_t target)
{
  return (unsigned char)(target >> 8) & 0xFF;
}

unsigned char low_byte(uint16_t target)
{
  return (unsigned char)target;
}

unsigned char high_nibble(unsigned char target)
{
  return ((target >> 4) & 0xF);
}

unsigned char low_nibble(unsigned char target)
{
  unsigned char test = (target & 0xF);
  return (target & 0xF);
}

uint16_t combine(unsigned char high, unsigned char low)
{
  return (high << 8 | low);
}

bool between(unsigned char target, int low, int high)
{
  return (target >= low && target <= high);
}

unsigned char set_bit(unsigned char data, unsigned char bit)
{
  return data | (1 << bit);
}

unsigned char clear_bit(unsigned char data, unsigned char bit)
{
  return data & (~(1 << bit));
}

bool is_bit_set(unsigned char data, unsigned char bit)
{
  return ((data >> bit) & 1) ? true : false;
}


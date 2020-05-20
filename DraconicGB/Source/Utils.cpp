#pragma once

#include "Utils.h"
#include "SDL_config.h"
/*
  Helper Functions
*/
unsigned char SetBit(unsigned char data, EBit bit)
{
  return data | (1 << (uint8_t)bit);
}

unsigned char ClearBit(unsigned char data, EBit bit)
{
  return data & (~(1 << (uint8_t)bit));
}

bool IsBitSet(unsigned char data, EBit bit)
{
  return ((data >> (uint8_t)bit) & 1) ? true : false;
}


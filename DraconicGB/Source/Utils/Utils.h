#pragma once
#include "SDL_config.h"

const int FLAG_ZERO = 0b10000000;
const int FLAG_SUB = 0b01000000;
const int FLAG_HALF_CARRY = 0b00100000;
const int FLAG_CARRY = 0b00010000;


enum class EBit : uint8_t {
  BIT_7 = 7,
  BIT_6 = 6,
  BIT_5 = 5,
  BIT_4 = 4,
  BIT_3 = 3,
  BIT_2 = 2,
  BIT_1 = 1,
  BIT_0 = 0,
  BIT_INVALID = 255
};


enum class EInterrupt : uint8_t {
  INTERRUPT_VBLANK = 0,
  INTERRUPT_LCDC = 1,
  INTERRUPT_TIMER = 2,
  INTERRUPT_SERIAL = 3,
  INTERRUPT_JOYPAD = 4,
};


unsigned char SetBit(unsigned char data, EBit bit);
unsigned char ClearBit(unsigned char data, EBit bit);
bool IsBitSet(unsigned char data, EBit bit);


struct PixelColor
{
  PixelColor()
  {
    r = 0;
    g = 0;
    b = 0;
    a = 255;
  }
  PixelColor(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
  {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
  }
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

#pragma once
#include "SDL_config.h"

const int FLAG_ZERO = 0b10000000;
const int FLAG_SUB = 0b01000000;
const int FLAG_HALF_CARRY = 0b00100000;
const int FLAG_CARRY = 0b00010000;

const unsigned char BIT_7 = 7;
const unsigned char BIT_6 = 6;
const unsigned char BIT_5 = 5;
const unsigned char BIT_4 = 4;
const unsigned char BIT_3 = 3;
const unsigned char BIT_2 = 2;
const unsigned char BIT_1 = 1;
const unsigned char BIT_0 = 1;

// Interrupt register bit values
const uint8_t INTERRUPT_VBLANK = 0;
const uint8_t INTERRUPT_LCDC = 1;
const uint8_t INTERRUPT_TIMER = 2;
const uint8_t INTERRUPT_SERIAL = 3;
const uint8_t INTERRUPT_JOYPAD = 4;

// Joypad keyboard key IDs
const int A = 0;
const int B = 18;
const int START = 25;
const int SELECT = 23;
const int LEFT = 71;
const int RIGHT = 72;
const int UP = 73;
const int DOWN = 74;



/*
  Helper Functions
*/
unsigned char high_byte(uint16_t target);
unsigned char low_byte(uint16_t target);
unsigned char high_nibble(unsigned char target);
unsigned char low_nibble(unsigned char target);
uint16_t combine(unsigned char high, unsigned char low);
bool between(unsigned char target, int low, int high);
unsigned char set_bit(unsigned char data, unsigned char bit);
unsigned char clear_bit(unsigned char data, unsigned char bit);
bool is_bit_set(unsigned char data, unsigned char bit);

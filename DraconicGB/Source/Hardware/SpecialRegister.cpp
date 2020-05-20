#include "SpecialRegister.h"


SpecialRegister::SpecialRegister() {}

SpecialRegister::SpecialRegister(unsigned char* _data)
{
  value = _data;
}

unsigned char SpecialRegister::get()
{
  return *value;
}

void SpecialRegister::set(unsigned char data)
{
  *value = data;
}

void SpecialRegister::clear()
{
  set(0x00);
}

void SpecialRegister::set_bit(EBit bit)
{
  *value |= 1 << (uint8_t)bit;
}

void SpecialRegister::clear_bit(EBit bit)
{
  *value &= ~(1 << (uint8_t)bit);
}

bool SpecialRegister::is_bit_set(EBit bit)
{
  return ((*value >> (uint8_t)bit) & 1) ? true : false;
}
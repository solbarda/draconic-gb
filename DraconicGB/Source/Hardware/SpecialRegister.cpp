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

void SpecialRegister::set_bit(unsigned char bit)
{
  *value |= 1 << bit;
}

void SpecialRegister::clear_bit(unsigned char bit)
{
  *value &= ~(1 << bit);
}

bool SpecialRegister::is_bit_set(unsigned char bit)
{
  return ((*value >> bit) & 1) ? true : false;
}
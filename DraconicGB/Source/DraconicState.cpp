#include "DraconicState.h"

void DraconicState::SetFlag(int flag, bool value)
{
  if (value)
    registers.F |= flag;
  else
    registers.F &= ~(flag);
}

void DraconicState::ParseOpcode(uint8_t opCode)
{
  uint8_t value8Low = memory.Read(registers.PC + 1);
  uint8_t value8High = memory.Read(registers.PC + 2);
  uint16_t value16 = value8High << 8 | value8Low;

  switch (opCode)
  {
  case 0x87: ADC_A_R8(registers.A); break;
  case 0x80: ADC_A_R8(registers.B); break;
  case 0x81: ADC_A_R8(registers.C); break;
  case 0x82: ADC_A_R8(registers.D); break;
  case 0x83: ADC_A_R8(registers.E); break;
  case 0x84: ADC_A_R8(registers.H); break;
  case 0x85: ADC_A_R8(registers.L); break;
  case 0xC6: ADC_A_N8( value8Low ); break;
  case 0x86: ADD_A_HL(); break;
  case 0x8F: ADC_A_R8( registers.A); break;
  case 0x88: ADC_A_R8( registers.B); break;
  case 0x89: ADC_A_R8( registers.C); break;
  case 0x8A: ADC_A_R8( registers.D); break;
  case 0x8B: ADC_A_R8( registers.E); break;
  case 0x8C: ADC_A_R8( registers.H); break;
  case 0x8D: ADC_A_R8( registers.L); break;
  case 0xCE: ADC_A_N8( value8Low ); break;
  case 0x8E: ADC_A_HL(); break;
  }
}

void DraconicState::ADC(uint8_t& target, uint8_t value)
{
  uint16_t carry = (registers.F & FLAG_CARRY) ? 1 : 0;
  uint16_t result = (uint16_t)target + (uint16_t)value + carry;
  SetFlag(FLAG_HALF_CARRY, ((target & 0x0F) + (value & 0xF) + (uint8_t)carry) > 0x0F);
  SetFlag(FLAG_CARRY, (result > 0xFF));
  SetFlag(FLAG_ZERO, (result & 0xFF) == 0);
  SetFlag(FLAG_SUB, false);
  target = (result & 0xFF);
}

void DraconicState::ADC_A_R8(uint8_t value)
{
  ADC(registers.A, value);
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::ADC_A_HL()
{
  uint8_t val = memory.Read(registers.HL);
  ADC(registers.A, val);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::ADC_A_N8(uint8_t value)
{
  ADC(registers.A, value);
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::ADD(uint8_t& target, uint8_t value)
{
  uint16_t result = (uint16_t)target + (uint16_t)value;
  SetFlag(FLAG_HALF_CARRY, ((target & 0xF) + (value & 0xF)) > 0xF);
  SetFlag(FLAG_CARRY, (result > 0xFF));
  SetFlag(FLAG_ZERO, (result & 0xFF) == 0);
  SetFlag(FLAG_SUB, false);
  target = (result & 0xFF);
}

void DraconicState::ADD_A_R8(uint8_t value)
{
  ADD(registers.A, value);
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::ADD_A_HL()
{
  uint8_t val = memory.Read(registers.HL);
  ADD(registers.A, val);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::ADD_A_N8(uint8_t value)
{
  ADD(registers.A, value);
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::AND(uint8_t& target, uint8_t value)
{
  target &= value;
  SetFlag(FLAG_ZERO, (target == 0));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, true);
  SetFlag(FLAG_CARRY, false);
}

void DraconicState::AND_A_R8(uint8_t value)
{
  AND(registers.A, value);
}

void DraconicState::AND_A_HL()
{
  uint8_t val = memory.Read(registers.HL);
  AND(target, val);
}

void DraconicState::AND_A_N8(uint8_t value)
{
  AND(registers.A, value);
}

void DraconicState::CP(uint8_t& target, uint8_t value)
{

}

void DraconicState::CP_A_R8(uint8_t value)
{

}

void DraconicState::CP_A_HL()
{

}

void DraconicState::CP_A_N8(uint8_t value)
{

}

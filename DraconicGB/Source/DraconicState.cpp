#include "DraconicState.h"
#include "Utils.h"

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
  AND(registers.A, val);
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

void DraconicState::DEC_R8(uint8_t& target)
{

}

void DraconicState::DEC_HL()
{

}

void DraconicState::INC_R8(uint8_t& target)
{

}

void DraconicState::INC_HL()
{

}

void DraconicState::OR_A_R8(uint8_t value)
{

}

void DraconicState::OR_A_HL()
{

}

void DraconicState::OR_A_N8(uint8_t value)
{

}

void DraconicState::SBC_A_R8(uint8_t value)
{

}

void DraconicState::SBC_A_HL()
{

}

void DraconicState::SBC_A_N8(uint8_t value)
{

}

void DraconicState::SUB_A_R8(uint8_t value)
{

}

void DraconicState::SUB_A_HL()
{

}

void DraconicState::SUB_A_N8(uint8_t value)
{

}

void DraconicState::XOR_A_R8(uint8_t value)
{

}

void DraconicState::XOR_A_HL()
{

}

void DraconicState::XOR_A_N8(uint8_t value)
{

}

void DraconicState::ADD_HL_R16(uint16_t value)
{

}

void DraconicState::DEC_R16(uint16_t& target)
{

}

void DraconicState::INC_R16(uint16_t& target)
{

}

void DraconicState::BIT_U3_R8(uint8_t value)
{

}

void DraconicState::BIT_U3_HL()
{

}

void DraconicState::RES_U3_R8(uint8_t value)
{

}

void DraconicState::RES_U3_HL()
{

}

void DraconicState::SET_U3_R8(uint8_t value)
{

}

void DraconicState::SET_U3_HL()
{

}

void DraconicState::SWAP_R8(uint8_t& target)
{

}

void DraconicState::SWAP_HL()
{

}

void DraconicState::RL_R8(uint8_t& target)
{

}

void DraconicState::RL_HL()
{

}

void DraconicState::RLA()
{

}

void DraconicState::RLC_R8(uint8_t& target)
{

}

void DraconicState::RLC_HL()
{

}

void DraconicState::RLCA()
{

}

void DraconicState::RR_R8(uint8_t& target)
{

}

void DraconicState::RR_HL()
{

}

void DraconicState::RR_A()
{

}

void DraconicState::RRC_R8(uint8_t& target)
{

}

void DraconicState::RRC_HL()
{

}

void DraconicState::RRC_A()
{

}

void DraconicState::SLA_R8(uint8_t& target)
{

}

void DraconicState::SLA_HL()
{

}

void DraconicState::SRA_R8(uint8_t& target)
{

}

void DraconicState::SRA_HL()
{

}

void DraconicState::SRL_R8(uint8_t& target)
{

}

void DraconicState::SRL_HL()
{

}

void DraconicState::LD_R8_R8(uint8_t& target, uint8_t value)
{

}

void DraconicState::LD_R8_N8(uint8_t& target, uint8_t value)
{

}

void DraconicState::LD_R16_N16(uint16_t& target, uint16_t value)
{

}

void DraconicState::LD_HL_R8(uint8_t value)
{

}

void DraconicState::LD_HL_N8(uint8_t value)
{

}

void DraconicState::LD_R8_HL(uint8_t& target)
{

}

void DraconicState::LD_r16_A(uint16_t addr)
{

}

void DraconicState::LD_n16_A(uint16_t addr)
{

}

void DraconicState::LDH_n16_A(uint16_t addr)
{

}

void DraconicState::LDH_C_A(uint16_t C)
{

}

void DraconicState::LD_A_R16(uint16_t addr)
{

}

void DraconicState::LD_A_N16(uint16_t addr)
{

}

void DraconicState::LDH_A_N16(uint16_t addr)
{

}

void DraconicState::LDH_A_C(uint16_t C)
{

}

void DraconicState::LD_HLI_A()
{

}

void DraconicState::LD_HLD_A()
{

}

void DraconicState::LD_A_HLI()
{

}

void DraconicState::LD_A_HLD()
{

}

void DraconicState::CALL_N16(uint16_t addr)
{

}

void DraconicState::CALL_CC_N16(uint8_t cc, uint16_t addr)
{

}

void DraconicState::JP_HL()
{

}

void DraconicState::JP_N16(uint16_t addr)
{

}

void DraconicState::JP_CC_N16(uint8_t cc, uint16_t addr)
{

}

void DraconicState::JR_E8(uint8_t offset)
{

}

void DraconicState::JR_CC_E8(uint8_t cc, uint8_t offset)
{

}

void DraconicState::RET_CC(uint8_t cc)
{

}

void DraconicState::RET()
{

}

void DraconicState::RETI()
{

}

void DraconicState::RST_VEC(uint8_t vec)
{

}

void DraconicState::ADD_HL_SP()
{

}

void DraconicState::ADD_SP_E8(uint8_t offset)
{

}

void DraconicState::DEC_SP()
{

}

void DraconicState::LD_SP_N16(uint16_t value)
{

}

void DraconicState::LD_N16_SP(uint16_t addr)
{

}

void DraconicState::LD_HL_SP_E8(uint8_t offset)
{

}

void DraconicState::LD_SP_HL()
{

}

void DraconicState::POP_AF()
{

}

void DraconicState::POP_R16(uint16_t value)
{

}

void DraconicState::PUSH_AF()
{

}

void DraconicState::PUSH_r16(uint16_t value)
{

}

void DraconicState::CCF()
{

}

void DraconicState::CPL()
{

}

void DraconicState::DAA()
{

}

void DraconicState::DI()
{

}

void DraconicState::EI()
{

}

void DraconicState::HALT()
{

}

void DraconicState::NOP()
{

}

void DraconicState::SCF()
{

}

void DraconicState::STOP()
{

}

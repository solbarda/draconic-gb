#include "DraconicState.h"
#include "Utils.h"

void DraconicState::SetFlag(int flag, bool value)
{
  if (value8Low)
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
  case 0x7F: LD_R8_R8(registers, registers); break;
  case 0x78: LD_R8_R8(registers, registers); break;
  case 0x79: LD_R8_R8(registers, registers); break;
  case 0x7A: LD_R8_R8(registers, registers); break;
  case 0x7B: LD_R8_R8(registers, registers); break;
  case 0x7C: LD_R8_R8(registers, registers); break;
  case 0x7D: LD_R8_R8(registers, registers); break;
  case 0x47: LD_R8_R8(registers, registers); break;
  case 0x40: LD_R8_R8(registers, registers); break;
  case 0x41: LD_R8_R8(registers, registers); break;
  case 0x42: LD_R8_R8(registers, registers); break;
  case 0x43: LD_R8_R8(registers, registers); break;
  case 0x44: LD_R8_R8(registers, registers); break;
  case 0x45: LD_R8_R8(registers, registers); break;
  case 0x4F: LD_R8_R8(registers, registers); break;
  case 0x48: LD_R8_R8(registers, registers); break;
  case 0x49: LD_R8_R8(registers, registers); break;
  case 0x4A: LD_R8_R8(registers, registers); break;
  case 0x4B: LD_R8_R8(registers, registers); break;
  case 0x4C: LD_R8_R8(registers, registers); break;
  case 0x4D: LD_R8_R8(registers, registers); break;
  case 0x57: LD_R8_R8(registers, registers); break;
  case 0x50: LD_R8_R8(registers, registers); break;
  case 0x51: LD_R8_R8(registers, registers); break;
  case 0x52: LD_R8_R8(registers, registers); break;
  case 0x53: LD_R8_R8(registers, registers); break;
  case 0x54: LD_R8_R8(registers, registers); break;
  case 0x55: LD_R8_R8(registers, registers); break;
  case 0x5F: LD_R8_R8(registers, registers); break;
  case 0x58: LD_R8_R8(registers, registers); break;
  case 0x59: LD_R8_R8(registers, registers); break;
  case 0x5A: LD_R8_R8(registers, registers); break;
  case 0x5B: LD_R8_R8(registers, registers); break;
  case 0x5C: LD_R8_R8(registers, registers); break;
  case 0x5D: LD_R8_R8(registers, registers); break;
  case 0x67: LD_R8_R8(registers, registers); break;
  case 0x60: LD_R8_R8(registers, registers); break;
  case 0x61: LD_R8_R8(registers, registers); break;
  case 0x62: LD_R8_R8(registers, registers); break;
  case 0x63: LD_R8_R8(registers, registers); break;
  case 0x64: LD_R8_R8(registers, registers); break;
  case 0x65: LD_R8_R8(registers, registers); break;
  case 0x6F: LD_R8_R8(registers, registers); break;
  case 0x68: LD_R8_R8(registers, registers); break;
  case 0x69: LD_R8_R8(registers, registers); break;
  case 0x6A: LD_R8_R8(registers, registers); break;
  case 0x6B: LD_R8_R8(registers, registers); break;
  case 0x6C: LD_R8_R8(registers, registers); break;
  case 0x6D: LD_R8_R8(registers, registers); break;
  case 0x3E: LD_R8_N8(registers.A, value8Low); break;
  case 0x06: LD_R8_N8(registers.B, value8Low); break;
  case 0x0E: LD_R8_N8(registers.C, value8Low); break;
  case 0x16: LD_R8_N8(registers.D, value8Low); break;
  case 0x1E: LD_R8_N8(registers.E, value8Low); break;
  case 0x26: LD_R8_N8(registers.H, value8Low); break;
  case 0x2E: LD_R8_N8(registers.L, value8Low); break;
  case 0x7E: LD_R8_HL(registers.A); break;
  case 0x46: LD_R8_HL(registers.B); break;
  case 0x4E: LD_R8_HL(registers.C); break;
  case 0x56: LD_R8_HL(registers.D); break;
  case 0x5E: LD_R8_HL(registers.E); break;
  case 0x66: LD_R8_HL(registers.H); break;
  case 0x6E: LD_R8_HL(registers.L); break;
    // 86
  case 0x77: LD_HL_R8(registers.A); break;
  case 0x70: LD_HL_R8(registers.B); break;
  case 0x71: LD_HL_R8(registers.C); break;
  case 0x72: LD_HL_R8(registers.D); break;
  case 0x73: LD_HL_R8(registers.E); break;
  case 0x74: LD_HL_R8(registers.H); break;
  case 0x75: LD_HL_R8(registers.L); break;
  case 0x36: LD_HL_N8(value8Low); break;
  case 0x0A: LD_A_R16(registers.BC); break;
  case 0x1A: LD_A_R16(registers.DE); break;
  case 0xF2: LDH_A_C(); break;
    // 87
  case 0xE2: LDH_C_A();break;
  case 0xF0: LDH_A_N16(value8Low); break; // this may need to consume 3 opuint8_ts
  case 0xE0: LDH_N16_A(value8Low); break; // this also
  case 0xFA: LD_A_N16(value16); break; // these may need swapped
  // 88
  case 0xEA: LD_N16_A(value16); break; // these may need swapped
  case 0x2A: LD_A_HLI(); op(1, 2); break;
  case 0x3A: LD_A_HLD(); op(1, 2); break;
  case 0x02: LD_R16_A(registers.BC); break;
  case 0x12: LD_R16_A(registers.DE); break;
    // 89
  case 0x22: LD_HLI_A(); op(1, 2); break;
  case 0x32: LD_HLD_A(); op(1, 2); break;
    // 90
  case 0x01: LD_R16_N16(registers.BC, value16); break;
  case 0x11: LD_R16_N16(registers.DE, value16); break;
  case 0x21: LD_R16_N16(registers.HL, value16); break;
  case 0x31: LD_SP_N16(value16);break;
  case 0xF9: LD_SP_HL(); break;
  case 0xC5: PUSH_R16(registers.BC); op(1, 4); break;
  case 0xD5: PUSH_R16(registers.DE); op(1, 4); break;
  case 0xE5: PUSH_R16(registers.HL); op(1, 4); break;
  case 0xF5: PUSH_R16(registers.AF); op(1, 4); break;
    // 91
  case 0xC1: POP_R16(registers.BC, registers.C); break;
  case 0xD1: POP_R16(registers.DE, registers.E); break;
  case 0xE1: POP_R16(registers.HL, registers.L); break;
  case 0xF1:
    POP(registers.A, registers.F);
    // After failing tests, apparently lower 4 bits of register F
    // (all flags) are set to zero.
    registers.F &= 0xF0;
    op(1, 3);
    break;
  case 0xF8: LDHL(value8Low); op(2, 3); break;
  case 0x08: LDNN(value8Low, value8High); op(3, 5); break;
    // 92
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

void DraconicState::ADC(uint8_t& target, uint8_t value8Low)
{
  uint16_t carry = (registers.F & FLAG_CARRY) ? 1 : 0;
  uint16_t result = (uint16_t)target + (uint16_t)value8Low + carry;
  SetFlag(FLAG_HALF_CARRY, ((target & 0x0F) + (value8Low & 0xF) + (uint8_t)carry) > 0x0F);
  SetFlag(FLAG_CARRY, (result > 0xFF));
  SetFlag(FLAG_ZERO, (result & 0xFF) == 0);
  SetFlag(FLAG_SUB, false);
  target = (result & 0xFF);
}

void DraconicState::ADC_A_R8(uint8_t value8Low)
{
  ADC(registers.A, value8Low);
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

void DraconicState::ADC_A_N8(uint8_t value8Low)
{
  ADC(registers.A, value8Low);
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::ADD(uint8_t& target, uint8_t value8Low)
{
  uint16_t result = (uint16_t)target + (uint16_t)value8Low;
  SetFlag(FLAG_HALF_CARRY, ((target & 0xF) + (value8Low & 0xF)) > 0xF);
  SetFlag(FLAG_CARRY, (result > 0xFF));
  SetFlag(FLAG_ZERO, (result & 0xFF) == 0);
  SetFlag(FLAG_SUB, false);
  target = (result & 0xFF);
}

void DraconicState::ADD_A_R8(uint8_t value8Low)
{
  ADD(registers.A, value8Low);
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

void DraconicState::ADD_A_N8(uint8_t value8Low)
{
  ADD(registers.A, value8Low);
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::AND(uint8_t& target, uint8_t value8Low)
{
  target &= value8Low;
  SetFlag(FLAG_ZERO, (target == 0));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, true);
  SetFlag(FLAG_CARRY, false);
}

void DraconicState::AND_A_R8(uint8_t value8Low)
{
  AND(registers.A, value8Low);
}

void DraconicState::AND_A_HL()
{
  uint8_t val = memory.Read(registers.HL);
  AND(registers.A, val);
}

void DraconicState::AND_A_N8(uint8_t value8Low)
{
  AND(registers.A, value8Low);
}

void DraconicState::CP(uint8_t& target, uint8_t value8Low)
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
  registers.pc += 1;
  numCycles += 4;
}

void DraconicState::LD_R8_N8(uint8_t& target, uint8_t value)
{
  registers.pc += 2;
  numCycles += 8;

}

void DraconicState::LD_R16_N16(uint16_t& target, uint16_t value)
{
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::LD_HL_R8(uint8_t value)
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_HL_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 12;
}

void DraconicState::LD_R8_HL(uint8_t& target)
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_R16_A(uint16_t addr)
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_N16_A(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 16;
}

void DraconicState::LDH_N16_A(uint16_t value)
{
  uint16_t addr = (uint16_t)(0xFF00 + value);
  registers.PC += 2;
  numCycles += 12;
}

void DraconicState::LDH_C_A()
{
  uint16_t addr = (uint16_t)(0xFF00 + registers.C);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_A_R16(uint16_t addr)
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_A_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 16;
}

void DraconicState::LDH_A_N16(uint16_t value)
{
  uint16_t addr = (uint16_t)(0xFF00 + value);
  registers.PC += 2;
  numCycles += 12;
}

void DraconicState::LDH_A_C()
{
  uint16_t addr = (uint16_t)(0xFF00 + registers.C);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_HLI_A()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_HLD_A()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_A_HLI()
{
  registers.PC += 1;
  numCycles += 2;
}

void DraconicState::LD_A_HLD()
{
  registers.PC += 1;
  numCycles += 2;
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
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::LD_N16_SP(uint16_t addr)
{

}

void DraconicState::LD_HL_SP_E8(uint8_t offset)
{

}

void DraconicState::LD_SP_HL()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::POP_AF()
{

}

void DraconicState::POP_R16(uint16_t& value)
{
  registers.PC += 1;
  numCycles += 12;
}

void DraconicState::PUSH_AF()
{

}

void DraconicState::PUSH_R16(uint16_t value)
{
  registers.PC += 1;
  numCycles += 16;
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

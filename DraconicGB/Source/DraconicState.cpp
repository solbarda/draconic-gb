#include "DraconicState.h"
#include "Utils.h"
#include "Hardware/CPU.h"

void DraconicState::SetFlag(int flag, bool value)
{
  if (value)
    registers.F |= flag;
  else
    registers.F &= ~(flag);
}


void DraconicState::ParseOpcodeDeprecated(uint8_t opCode)
{
  switch (opCode)
  {
    // 85
  case 0x7F:
  case 0x78:
  case 0x79:
  case 0x7A:
  case 0x7B:
  case 0x7C:
  case 0x7D:
  case 0x47:
  case 0x40:
  case 0x41:
  case 0x42:
  case 0x43:
  case 0x44:
  case 0x45:
  case 0x4F:
  case 0x48:
  case 0x49:
  case 0x4A:
  case 0x4B:
  case 0x4C:
  case 0x4D:
  case 0x57:
  case 0x50:
  case 0x51:
  case 0x52:
  case 0x53:
  case 0x54:
  case 0x55:
  case 0x5F:
  case 0x58:
  case 0x59:
  case 0x5A:
  case 0x5B:
  case 0x5C:
  case 0x5D:
  case 0x67:
  case 0x60:
  case 0x61:
  case 0x62:
  case 0x63:
  case 0x64:
  case 0x65:
  case 0x6F:
  case 0x68:
  case 0x69:
  case 0x6A:
  case 0x6B:
  case 0x6C:
  case 0x6D:
  case 0x3E:
  case 0x06:
  case 0x0E:
  case 0x16:
  case 0x1E:
  case 0x26:
  case 0x2E:
  case 0x7E:
  case 0x46:
  case 0x4E:
  case 0x56:
  case 0x5E:
  case 0x66:
  case 0x6E:
    // 86
  case 0x77:
  case 0x70:
  case 0x71:
  case 0x72:
  case 0x73:
  case 0x74:
  case 0x75:
  case 0x36:
  case 0x0A:
  case 0x1A:
  case 0xF2:
    // 87
  case 0xE2:
  case 0xF0:
  case 0xE0:
  case 0xFA:
    // 88
  case 0xEA:
  case 0x2A:
  case 0x3A:
  case 0x02:
  case 0x12:
    // 89
  case 0x22:
  case 0x32:
    // 90
  case 0x01:
  case 0x11:
  case 0x21:
  case 0x31:
  case 0xF9:
  case 0xC5:
  case 0xD5:
  case 0xE5:
  case 0xF5:
    // 91
  case 0xC1:
  case 0xD1:
  case 0xE1:
  case 0xF1:
    // 92
  case 0x87:
  case 0x80:
  case 0x81:
  case 0x82 :
  case 0x83 :
  case 0x84:
  case 0x85:
  case 0xC6:
  case 0x86:
  case 0x8F:
  case 0x88:
  case 0x89:
  case 0x8A:
  case 0x8B:
  case 0x8C:
  case 0x8D:
  case 0xCE:
  case 0x8E:
    // 93
  case 0x97:
  case 0x90:
  case 0x91:
  case 0x92:
  case 0x93:
  case 0x94:
  case 0x95:
  case 0xD6:
  case 0x96:
  case 0x9F:
  case 0x98:
  case 0x99:
  case 0x9A:
  case 0x9B:
  case 0x9C:
  case 0x9D:
  case 0xDE:
  case 0x9E:
    // 94
  case 0xA7:
  case 0xA0:
  case 0xA1:
  case 0xA2:
  case 0xA3:
  case 0xA4:
  case 0xA5:
  case 0xE6:
  case 0xA6:
  case 0xB7:
  case 0xB0:
  case 0xB1:
  case 0xB2:
  case 0xB3:
  case 0xB4:
  case 0xB5:
  case 0xF6:
  case 0xB6:
  case 0xAF:
  case 0xA8:
  case 0xA9:
  case 0xAA:
  case 0xAB:
  case 0xAC:
  case 0xAD:
  case 0xEE:
  case 0xAE:
 
    ParseOpcode(opCode);
    return;
  default:
    break;
  }

  DraconicCPU->parse_opcode(opCode);
}

void DraconicState::SetCPU(CPU* _DraconicCPU)
{
  DraconicCPU = _DraconicCPU;
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
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::AND_A_HL()
{
  uint8_t val = memory.Read(registers.HL);
  AND(registers.A, val);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::AND_A_N8(uint8_t value8Low)
{
  AND(registers.A, value8Low);
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::CP(uint8_t& target, uint8_t value8Low)
{

}

void DraconicState::CP_A_R8(uint8_t value)
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::CP_A_HL()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::CP_A_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::DEC_R8(uint8_t& target)
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::DEC_HL()
{
  registers.PC += 1;
  numCycles += 12;
}

void DraconicState::INC_R8(uint8_t& target)
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::INC_HL()
{
  registers.PC += 1;
  numCycles += 12;
}

void DraconicState::OR(uint8_t& target, uint8_t value)
{
  target |= value;
  SetFlag(FLAG_ZERO, (target == 0));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, false);
}

void DraconicState::OR_A_R8(uint8_t value)
{
  OR(registers.A, value);
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::OR_A_HL()
{
  OR(registers.A, memory.Read(registers.HL));
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::OR_A_N8(uint8_t value)
{
  OR(registers.A, value);
  registers.PC += 2;
  numCycles += 4;
}

void DraconicState::SBC(uint8_t& target, uint8_t value)
{
  uint16_t carry = (registers.F & FLAG_CARRY) ? 1 : 0;
  int16_t result = (int16_t)target - (int16_t)value - carry;

  int16_t s_target = (int16_t)target;
  int16_t s_value = (int16_t)value;

  SetFlag(FLAG_HALF_CARRY, (((s_target & 0xF) - (s_value & 0xF) - carry) < 0));
  SetFlag(FLAG_CARRY, result < 0);
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_ZERO, (result & 0xFF) == 0);

  target = (uint8_t)(result & 0xFF);
}

void DraconicState::SBC_A_R8(uint8_t value)
{
  SBC(registers.A, value);
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::SBC_A_HL()
{
  uint8_t val = memory.Read(registers.HL);
  SBC(registers.A, val);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::SBC_A_N8(uint8_t value)
{
  SBC(registers.A, value);
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::SUB(uint8_t& target, uint8_t value)
{
  int16_t result = (int16_t)target - (int16_t)value;
  int16_t s_target = (int16_t)target;
  int16_t s_value = (int16_t)value;
  SetFlag(FLAG_HALF_CARRY, (((s_target & 0xF) - (s_value & 0xF)) < 0));
  SetFlag(FLAG_CARRY, result < 0);
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_ZERO, (result & 0xFF) == 0);
  target = (uint8_t)(result & 0xFF);
}

void DraconicState::SUB_A_R8(uint8_t value)
{
  SUB(registers.A, value);
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::SUB_A_HL()
{
  uint8_t val = memory.Read(registers.HL);
  SUB(registers.A, val);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::SUB_A_N8(uint8_t value)
{
  SUB(registers.A, value);
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::XOR(uint8_t& target, uint8_t value)
{
  target ^= value;
  SetFlag(FLAG_ZERO, (target == 0));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, false);
}


void DraconicState::XOR_A_R8(uint8_t value)
{
  XOR(registers.A, value);
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::XOR_A_HL()
{
  XOR(registers.A, memory.Read(registers.HL));
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::XOR_A_N8(uint8_t value)
{
  XOR(registers.A, value);
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::ADD_HL_R16(uint16_t value)
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::DEC_R16(uint16_t& target)
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::INC_R16(uint16_t& target)
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::BIT_U3_R8(uint8_t value, uint8_t bit)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::BIT_U3_HL(uint8_t bit)
{
  registers.PC += 2;
  numCycles += 12;
}

void DraconicState::RES_U3_R8(uint8_t& target, uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::RES_U3_HL(uint8_t value)
{
  registers.PC += 2;
  numCycles += 16;
}

void DraconicState::SET_U3_R8(uint8_t& target, uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::SET_U3_HL(uint8_t value)
{
  registers.PC += 2;
  numCycles += 16;
}

void DraconicState::SWAP_R8(uint8_t& target)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::SWAP_HL()
{
  registers.PC += 2;
  numCycles += 16;
}

void DraconicState::RL_R8(uint8_t& target)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::RL_HL()
{
  registers.PC += 2;
  numCycles += 16;
}

void DraconicState::RLA()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::RLC_R8(uint8_t& target)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::RLC_HL()
{
  registers.PC += 2;
  numCycles += 16;
}

void DraconicState::RLCA()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::RR_R8(uint8_t& target)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::RR_HL()
{
  registers.PC += 2;
  numCycles += 16;
}

void DraconicState::RR_A()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::RRC_R8(uint8_t& target)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::RRC_HL()
{
  registers.PC += 2;
  numCycles += 16;
}

void DraconicState::RRC_A()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::SLA_R8(uint8_t& target)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::SLA_HL()
{
  registers.PC += 2;
  numCycles += 16;
}

void DraconicState::SRA_R8(uint8_t& target)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::SRA_HL()
{
  registers.PC += 2;
  numCycles += 16;
}

void DraconicState::SRL_R8(uint8_t& target)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::SRL_HL()
{
  registers.PC += 2;
  numCycles += 16;
}

void DraconicState::LD_R8_R8(uint8_t& target, uint8_t value)
{
  target = value;
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::LD_R8_N8(uint8_t& target, uint8_t value)
{
  target = value;
  registers.PC += 2;
  numCycles += 8;

}

void DraconicState::LD_R16_N16(uint16_t& target, uint16_t value)
{
  target = value;
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::LD_HL_R8(uint8_t value)
{
  memory.Write(registers.HL, value);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_HL_N8(uint8_t value)
{
  memory.Write(registers.HL, value);
  registers.PC += 2;
  numCycles += 12;
}

void DraconicState::LD_R8_HL(uint8_t& target)
{
  target = memory.Read(registers.HL);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_R16_A(uint16_t addr)
{
  memory.Write(addr, registers.A);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_N16_A(uint16_t addr)
{
  memory.Write(addr,registers.A);
  registers.PC += 3;
  numCycles += 16;
}

void DraconicState::LDH_N16_A(uint16_t value)
{
  uint16_t addr = (uint16_t)(0xFF00 + value);
  memory.Write(addr, registers.A);
  registers.PC += 2;
  numCycles += 12;
}

void DraconicState::LDH_C_A()
{
  uint16_t addr = (uint16_t)(0xFF00 + registers.C);
  memory.Write(addr, registers.A);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_A_R16(uint16_t addr)
{
  registers.A = memory.Read(addr);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_A_N16(uint16_t addr)
{
  registers.A = memory.Read(addr);
  registers.PC += 3;
  numCycles += 16;
}

void DraconicState::LDH_A_N16(uint16_t value)
{
  uint16_t addr = (uint16_t)(0xFF00 + value);
  registers.A = memory.Read(addr);
  registers.PC += 2;
  numCycles += 12;
}

void DraconicState::LDH_A_C()
{
  uint16_t addr = (uint16_t)(0xFF00 + registers.C);
  registers.A = memory.Read(addr);
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_HLI_A()
{
  memory.Write(registers.HL, registers.A);
  registers.HL++;
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_HLD_A()
{
  memory.Write(registers.HL, registers.A);
  registers.HL--;
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_A_HLI()
{
  registers.A = memory.Read(registers.HL);
  registers.HL++;
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_A_HLD()
{
  registers.A = memory.Read(registers.HL);
  registers.HL--;
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::CALL_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::CALLNZ_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::CALLZ_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::CALLNC_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::CALLC_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::JP(uint16_t target)
{
  numCycles += 4;
}

void DraconicState::JP_HL()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::JP_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 12;
  JP(addr);

}

void DraconicState::JPNZ_N16(uint16_t target)
{
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::JPZ_N16(uint16_t target)
{
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::JPNC_N16(uint16_t target)
{
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::JPC_N16(uint16_t target)
{
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::JR_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::JRNZ_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::JRZ_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::JRNC_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::JRC_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
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
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::RETI()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::RETNZ()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::RETZ()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::RETNC()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::RETC()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::RST_VEC(uint8_t vec)
{
  registers.PC += 1;
  numCycles += 16;
}

void DraconicState::ADD_HL_SP()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::ADD_SP_E8(uint8_t offset)
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::DEC_SP()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::INC_SP()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::LD_SP_N16(uint16_t value)
{
  registers.SP = value;
  registers.PC += 3;
  numCycles += 12;
}

void DraconicState::LD_N16_SP(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 20;
}

void DraconicState::LD_HL_SP_E8(uint8_t offset)
{
  registers.PC += 2;
  numCycles += 12;
}

void DraconicState::LD_SP_HL()
{
  registers.SP = registers.HL;
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::POP_AF()
{

}

void DraconicState::POP_R16(uint16_t& value)
{
  uint8_t low = memory.Read(registers.SP++);
  uint8_t high = memory.Read(registers.SP++);
  value = high << 8 | low;
  registers.PC += 1;
  numCycles += 12;
}

void DraconicState::PUSH_AF()
{

}

void DraconicState::PUSH_R16(uint16_t value)
{
  registers.SP--;
  memory.Write(registers.SP, value >> 8 & 0xFF);
  registers.SP--;
  memory.Write(registers.SP, value & 0xFF);
  registers.PC += 1;
  numCycles += 16;
}

void DraconicState::CCF()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::CPL()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::DAA()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::DI()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::EI()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::HALT()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::NOP()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::SCF()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::STOP()
{
  registers.PC += 1;
  numCycles += 4;
}

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
  case 0x87:
  case 0x80:
  case 0x81:
  case 0x82:
  case 0x83:
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
    ParseOpcode(opCode);
    return;
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

void DraconicState::OR_A_R8(uint8_t value)
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::OR_A_HL()
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::OR_A_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 4;
}

void DraconicState::SBC_A_R8(uint8_t value)
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::SBC_A_HL()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::SBC_A_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::SUB_A_R8(uint8_t value)
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::SUB_A_HL()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::SUB_A_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::XOR_A_R8(uint8_t value)
{
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::XOR_A_HL()
{
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::XOR_A_N8(uint8_t value)
{
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
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::LD_R8_N8(uint8_t& target, uint8_t value)
{
  registers.PC += 2;
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
  numCycles += 8;
}

void DraconicState::LD_A_HLD()
{
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

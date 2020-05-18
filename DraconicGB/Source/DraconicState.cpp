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
  if(opCode == 0xCB)
    DraconicCPU->parse_opcode(opCode);
  else
    ParseOpcode(opCode);
  return;
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
  int result = registers.A - value;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_HALF_CARRY, (((registers.A & 0xF) - (value & 0xF)) < 0));
  SetFlag(FLAG_CARRY, (result < 0));
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::CP_A_HL()
{
  uint8_t value = memory.Read(registers.HL);
  int result = registers.A - value;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_HALF_CARRY, (((registers.A & 0xF) - (value & 0xF)) < 0));
  SetFlag(FLAG_CARRY, (result < 0));
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::CP_A_N8(uint8_t value)
{
  int result = registers.A - value;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_HALF_CARRY, (((registers.A & 0xF) - (value & 0xF)) < 0));
  SetFlag(FLAG_CARRY, (result < 0));
  registers.PC += 2;
  numCycles += 8;
}

void DraconicState::DEC_R8(uint8_t& target)
{
  uint8_t result = target - 1;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_HALF_CARRY, (((target & 0xF) - 1) < 0));
  target = result;
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::DEC_HL()
{
  uint8_t value = memory.Read(registers.HL);
  uint8_t result = value - 1;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_HALF_CARRY, (((value & 0xF) - 1) < 0));
  memory.Write(registers.HL, result);
  registers.PC += 1;
  numCycles += 12;
}

void DraconicState::INC_R8(uint8_t& target)
{
  uint8_t result = target + 1;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, ((((target & 0xF) + 1) & 0x10) != 0));
  target = result;
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::INC_HL()
{
  uint8_t value = memory.Read(registers.HL);
  uint8_t result = value + 1;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, ((((value & 0xF) + 1) & 0x10) != 0));
  memory.Write(registers.HL, result);
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
  uint16_t result = registers.HL + value;
  SetFlag(FLAG_SUB, false); // reset
  SetFlag(FLAG_HALF_CARRY, ((((registers.HL & 0xFFF) + (value & 0xFFF)) & 0x1000) != 0)); // Set if carry from bit 11
  SetFlag(FLAG_CARRY, (result > 0xFFFF)); // Set if carry from bit 15
  registers.HL = result;
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::DEC_R16(uint16_t& target)
{
  target--;
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::INC_R16(uint16_t& target)
{
  target = target + 1;
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
  RL(registers.A, true);
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

void DraconicState::RL(uint8_t& target, bool carry, bool zero_flag)
{
  int bit7 = ((target & 0x80) != 0);
  target = target << 1;
  target |= (carry) ? ((registers.F & FLAG_CARRY) != 0) : bit7;
  SetFlag(FLAG_ZERO, ((zero_flag) ? (target == 0) : false));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, (bit7 != 0));
}

void DraconicState::RLCA()
{
  RL(registers.A, false);
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
  RR(registers.A, true);
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


void DraconicState::RR(uint8_t& target, bool carry, bool zero_flag)
{
  int bit1 = ((target & 0x1) != 0);
  target = target >> 1;

  target |= (carry) ? (((registers.F & FLAG_CARRY) != 0) << 7) : (bit1 << 7);

  SetFlag(FLAG_ZERO, ((zero_flag) ? (target == 0) : false));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, (bit1 != 0));
}
void DraconicState::RRC_A()
{
  RR(registers.A, false);
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


void DraconicState::CALL(uint16_t addr)
{
  registers.SP--;
  memory.Write(registers.SP, registers.PC >> 8 & 0xFF);
  registers.SP--;
  memory.Write(registers.SP, registers.PC & 0xFF);
  JP(addr);
  numCycles += 12;
}

void DraconicState::CALL_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 12;
  CALL(addr);
}

void DraconicState::CALLNZ_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 12;
  if ((registers.F & FLAG_ZERO) == 0)
    CALL(addr);
}

void DraconicState::CALLZ_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 12;
  if ((registers.F & FLAG_ZERO) != 0)
    CALL(addr);
}

void DraconicState::CALLNC_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 12;
  if ((registers.F & FLAG_CARRY) == 0)
    CALL(addr);
}

void DraconicState::CALLC_N16(uint16_t addr)
{
  registers.PC += 3;
  numCycles += 12;
  if ((registers.F & FLAG_CARRY) != 0)
    CALL(addr);
}

void DraconicState::JP(uint16_t target)
{
  registers.PC = target;
  numCycles += 4;
}

void DraconicState::JP_HL()
{
  registers.PC += 1;
  numCycles += 4;
  registers.PC = registers.HL;
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
  if ((registers.F & FLAG_ZERO) == 0)
    JP(target);
}

void DraconicState::JPZ_N16(uint16_t target)
{
  registers.PC += 3;
  numCycles += 12;
  if ((registers.F & FLAG_ZERO) != 0)
    JP(target);
}

void DraconicState::JPNC_N16(uint16_t target)
{
  registers.PC += 3;
  numCycles += 12;
  if ((registers.F & FLAG_CARRY) == 0)
    JP(target);
}

void DraconicState::JPC_N16(uint16_t target)
{
  registers.PC += 3;
  numCycles += 12;
  if ((registers.F & FLAG_CARRY) != 0)
    JP(target);
}

void DraconicState::JR(uint8_t value)
{
  int8_t signed_val = ((int8_t)(value));
  registers.PC += signed_val;
  numCycles += 4;
}

void DraconicState::JR_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
  JR(value);
}

void DraconicState::JRNZ_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
  if ((registers.F & FLAG_ZERO) == 0)
    JR(value);
}

void DraconicState::JRZ_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
  if ((registers.F & FLAG_ZERO) != 0)
    JR(value);
}

void DraconicState::JRNC_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
  if ((registers.F & FLAG_CARRY) == 0)
    JR(value);
}

void DraconicState::JRC_N8(uint8_t value)
{
  registers.PC += 2;
  numCycles += 8;
  if ((registers.F & FLAG_CARRY) != 0)
    JR(value);
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

void DraconicState::RET_Impl()
{
  uint8_t low = memory.Read(registers.SP++);
  uint8_t high = memory.Read(registers.SP++);
  registers.PC = (high << 8 & 0xFF00) | (low & 0xFF);
  numCycles += 12;
}


void DraconicState::RET()
{
  registers.PC += 1;
  numCycles += 4;
  RET_Impl();
}

void DraconicState::RETI()
{
  registers.PC += 1;
  numCycles += 4;
  DraconicCPU->interrupt_master_enable = true;
  RET_Impl();
}

void DraconicState::RETNZ()
{
  registers.PC += 1;
  numCycles += 8;
  if ((registers.F & FLAG_ZERO) == 0)
  {
    RET_Impl();
    numCycles += 8;
  }
}

void DraconicState::RETZ()
{
  registers.PC += 1;
  numCycles += 8;
  if ((registers.F & FLAG_ZERO) != 0)
  {
    RET_Impl();
    numCycles += 8;
  }
}

void DraconicState::RETNC()
{
  registers.PC += 1;
  numCycles += 8;
  if ((registers.F & FLAG_CARRY) == 0)
  {
    RET_Impl();
    numCycles += 8;
  }
}

void DraconicState::RETC()
{
  registers.PC += 1;
  numCycles += 8;
  if ((registers.F & FLAG_CARRY) != 0)
  {
    RET_Impl();
    numCycles += 8;
  }
}

void DraconicState::RST_VEC(uint16_t addr)
{
  registers.PC += 1;
  numCycles += 16;
  registers.SP--;
  memory.Write(registers.SP, (registers.PC >> 8 & 0xFF));
  registers.SP--;
  memory.Write(registers.SP, (registers.PC & 0xFF));
  registers.PC = addr;
}

void DraconicState::ADD_HL_SP()
{
  int result = registers.HL + registers.SP;
  SetFlag(FLAG_SUB, false); // reset
  SetFlag(FLAG_HALF_CARRY, ((((registers.HL & 0xFFF) + (registers.SP & 0xFFF)) & 0x1000) != 0)); // Set if carry from bit 11
  SetFlag(FLAG_CARRY, (result > 0xFFFF)); // Set if carry from bit 15
  registers.HL = result & 0xFFFF;
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::ADD_SP_E8(uint8_t offset)
{
  int16_t val_signed = (int16_t)(int8_t)offset;
  int result = (uint16_t)((int16_t)registers.SP + val_signed);
  SetFlag(FLAG_CARRY, (result & 0xFF) < (registers.SP & 0xFF));
  SetFlag(FLAG_HALF_CARRY, (result & 0xF) < (registers.SP & 0xF));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_ZERO, false);
  registers.SP = result & 0xFFFF;
  registers.PC += 2;
  numCycles += 16;
}

void DraconicState::DEC_SP()
{
  registers.SP--;
  registers.PC += 1;
  numCycles += 8;
}

void DraconicState::INC_SP()
{
  registers.SP++;
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
  uint8_t lsb = registers.SP & 0xFF;
  uint8_t msb = registers.SP >> 8 & 0xFF;

  memory.Write(addr, lsb);
  addr++;
  memory.Write(addr, msb);
  registers.PC += 3;
  numCycles += 20;
}

void DraconicState::LD_HL_SP_E8(uint8_t offset)
{
  int16_t signed_val = (int16_t)(int8_t)offset;
  uint16_t result = (uint16_t)((int16_t)registers.SP + signed_val);

  SetFlag(FLAG_CARRY, (result & 0xFF) < (registers.SP & 0xFF));
  SetFlag(FLAG_HALF_CARRY, (result & 0xF) < (registers.SP & 0xF)); 
  SetFlag(FLAG_ZERO, false);
  SetFlag(FLAG_SUB, false);

  registers.HL = result;
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
  uint8_t low = memory.Read(registers.SP++);
  uint8_t high = memory.Read(registers.SP++);
  registers.AF = high << 8 | low;
  registers.F &= 0xF0;
  registers.PC += 1;
  numCycles += 12;
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
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, ((registers.F & FLAG_CARRY) ? 1 : 0) ^ 1);
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::CPL()
{
  registers.A = ~registers.A;
  SetFlag(FLAG_HALF_CARRY, true);
  SetFlag(FLAG_SUB, true);
  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::DAA()
{
  uint8_t high = registers.A >> 4 & 0xF;
  uint8_t low = registers.A & 0xF;

  bool add = ((registers.F & FLAG_SUB) == 0);
  bool carry = ((registers.F & FLAG_CARRY) != 0);
  bool half_carry = ((registers.F & FLAG_HALF_CARRY) != 0);

  uint16_t result = (uint16_t)registers.A;
  uint16_t correction = (carry) ? 0x60 : 0x00;

  if (half_carry || (add) && ((result & 0x0F) > 9))
    correction |= 0x06;

  if (carry || (add) && (result > 0x99))
    correction |= 0x60;

  if (add)
    result += correction;
  else
    result -= correction;

  if (((correction << 2) & 0x100) != 0)
    SetFlag(FLAG_CARRY, true);

  SetFlag(FLAG_HALF_CARRY, false);
  registers.A = (uint8_t)(result & 0xFF);
  SetFlag(FLAG_ZERO, registers.A == 0);

  registers.PC += 1;
  numCycles += 4;
}

void DraconicState::DI()
{
  registers.PC += 1;
  numCycles += 4;
  DraconicCPU->interrupt_master_enable = false;

}

void DraconicState::EI()
{
  registers.PC += 1;
  numCycles += 4;
  DraconicCPU->interrupt_master_enable = true;
}

void DraconicState::HALT()
{
  registers.PC += 1;
  numCycles += 4;
  DraconicCPU->halted = true;
  registers.PC--;
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
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, true);
}

void DraconicState::STOP()
{
  registers.PC += 1;
  numCycles += 4;
}

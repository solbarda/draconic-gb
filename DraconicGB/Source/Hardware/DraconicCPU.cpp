
#include "DraconicCPU.h"
#include "DraconicState.h"
#include "DraconicMemory.h"


void DraconicCPU::SaveState(std::ofstream& file)
{

}

void DraconicCPU::LoadState(std::ifstream& file)
{

}

void DraconicCPU::SetFlag(int flag, bool value)
{
  if (value)
    state->registers.F |= flag;
  else
    state->registers.F &= ~(flag);
}



void DraconicCPU::ADC(uint8_t& target, uint8_t value8Low)
{
  uint16_t carry = (state->registers.F & FLAG_CARRY) ? 1 : 0;
  uint16_t result = (uint16_t)target + (uint16_t)value8Low + carry;
  SetFlag(FLAG_HALF_CARRY, ((target & 0x0F) + (value8Low & 0xF) + (uint8_t)carry) > 0x0F);
  SetFlag(FLAG_CARRY, (result > 0xFF));
  SetFlag(FLAG_ZERO, (result & 0xFF) == 0);
  SetFlag(FLAG_SUB, false);
  target = (result & 0xFF);
}

void DraconicCPU::ADC_A_R8(uint8_t value8Low)
{
  ADC(state->registers.A, value8Low);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::ADC_A_HL()
{
  uint8_t val = state->memory.Read(state->registers.HL);
  ADC(state->registers.A, val);
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::ADC_A_N8(uint8_t value8Low)
{
  ADC(state->registers.A, value8Low);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::ADD(uint8_t& target, uint8_t value8Low)
{
  uint16_t result = (uint16_t)target + (uint16_t)value8Low;
  SetFlag(FLAG_HALF_CARRY, ((target & 0xF) + (value8Low & 0xF)) > 0xF);
  SetFlag(FLAG_CARRY, (result > 0xFF));
  SetFlag(FLAG_ZERO, (result & 0xFF) == 0);
  SetFlag(FLAG_SUB, false);
  target = (result & 0xFF);
}

void DraconicCPU::ADD_A_R8(uint8_t value8Low)
{
  ADD(state->registers.A, value8Low);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::ADD_A_HL()
{
  uint8_t val = state->memory.Read(state->registers.HL);
  ADD(state->registers.A, val);
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::ADD_A_N8(uint8_t value8Low)
{
  ADD(state->registers.A, value8Low);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::AND(uint8_t& target, uint8_t value8Low)
{
  target &= value8Low;
  SetFlag(FLAG_ZERO, (target == 0));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, true);
  SetFlag(FLAG_CARRY, false);
}

void DraconicCPU::AND_A_R8(uint8_t value8Low)
{
  AND(state->registers.A, value8Low);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::AND_A_HL()
{
  uint8_t val = state->memory.Read(state->registers.HL);
  AND(state->registers.A, val);
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::AND_A_N8(uint8_t value8Low)
{
  AND(state->registers.A, value8Low);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::CP(uint8_t& target, uint8_t value8Low)
{

}

void DraconicCPU::CP_A_R8(uint8_t value)
{
  int result = state->registers.A - value;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_HALF_CARRY, (((state->registers.A & 0xF) - (value & 0xF)) < 0));
  SetFlag(FLAG_CARRY, (result < 0));
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::CP_A_HL()
{
  uint8_t value = state->memory.Read(state->registers.HL);
  int result = state->registers.A - value;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_HALF_CARRY, (((state->registers.A & 0xF) - (value & 0xF)) < 0));
  SetFlag(FLAG_CARRY, (result < 0));
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::CP_A_N8(uint8_t value)
{
  int result = state->registers.A - value;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_HALF_CARRY, (((state->registers.A & 0xF) - (value & 0xF)) < 0));
  SetFlag(FLAG_CARRY, (result < 0));
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::DEC_R8(uint8_t& target)
{
  uint8_t result = target - 1;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_HALF_CARRY, (((target & 0xF) - 1) < 0));
  target = result;
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::DEC_HL()
{
  uint8_t value = state->memory.Read(state->registers.HL);
  uint8_t result = value - 1;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_HALF_CARRY, (((value & 0xF) - 1) < 0));
  state->memory.Write(state->registers.HL, result);
  state->registers.PC += 1;
  state->numCycles += 12;
}

void DraconicCPU::INC_R8(uint8_t& target)
{
  uint8_t result = target + 1;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, ((((target & 0xF) + 1) & 0x10) != 0));
  target = result;
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::INC_HL()
{
  uint8_t value = state->memory.Read(state->registers.HL);
  uint8_t result = value + 1;
  SetFlag(FLAG_ZERO, (result == 0));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, ((((value & 0xF) + 1) & 0x10) != 0));
  state->memory.Write(state->registers.HL, result);
  state->registers.PC += 1;
  state->numCycles += 12;
}

void DraconicCPU::OR(uint8_t& target, uint8_t value)
{
  target |= value;
  SetFlag(FLAG_ZERO, (target == 0));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, false);
}

void DraconicCPU::OR_A_R8(uint8_t value)
{
  OR(state->registers.A, value);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::OR_A_HL()
{
  OR(state->registers.A, state->memory.Read(state->registers.HL));
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::OR_A_N8(uint8_t value)
{
  OR(state->registers.A, value);
  state->registers.PC += 2;
  state->numCycles += 4;
}

void DraconicCPU::SBC(uint8_t& target, uint8_t value)
{
  uint16_t carry = (state->registers.F & FLAG_CARRY) ? 1 : 0;
  int16_t result = (int16_t)target - (int16_t)value - carry;

  int16_t s_target = (int16_t)target;
  int16_t s_value = (int16_t)value;

  SetFlag(FLAG_HALF_CARRY, (((s_target & 0xF) - (s_value & 0xF) - carry) < 0));
  SetFlag(FLAG_CARRY, result < 0);
  SetFlag(FLAG_SUB, true);
  SetFlag(FLAG_ZERO, (result & 0xFF) == 0);

  target = (uint8_t)(result & 0xFF);
}

void DraconicCPU::SBC_A_R8(uint8_t value)
{
  SBC(state->registers.A, value);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::SBC_A_HL()
{
  uint8_t val = state->memory.Read(state->registers.HL);
  SBC(state->registers.A, val);
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::SBC_A_N8(uint8_t value)
{
  SBC(state->registers.A, value);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::SUB(uint8_t& target, uint8_t value)
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

void DraconicCPU::SUB_A_R8(uint8_t value)
{
  SUB(state->registers.A, value);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::SUB_A_HL()
{
  uint8_t val = state->memory.Read(state->registers.HL);
  SUB(state->registers.A, val);
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::SUB_A_N8(uint8_t value)
{
  SUB(state->registers.A, value);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::XOR(uint8_t& target, uint8_t value)
{
  target ^= value;
  SetFlag(FLAG_ZERO, (target == 0));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, false);
}


void DraconicCPU::XOR_A_R8(uint8_t value)
{
  XOR(state->registers.A, value);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::XOR_A_HL()
{
  XOR(state->registers.A, state->memory.Read(state->registers.HL));
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::XOR_A_N8(uint8_t value)
{
  XOR(state->registers.A, value);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::ADD_HL_R16(uint16_t value)
{
  uint16_t result = state->registers.HL + value;
  SetFlag(FLAG_SUB, false); // reset
  SetFlag(FLAG_HALF_CARRY, ((((state->registers.HL & 0xFFF) + (value & 0xFFF)) & 0x1000) != 0)); // Set if carry from bit 11
  SetFlag(FLAG_CARRY, (result > 0xFFFF)); // Set if carry from bit 15
  state->registers.HL = result;
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::DEC_R16(uint16_t& target)
{
  target--;
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::INC_R16(uint16_t& target)
{
  target = target + 1;
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::BIT_U3_R8(uint8_t value, uint8_t bit)
{
  SetFlag(FLAG_ZERO, (((1 << bit) & ~value) != 0));
  SetFlag(FLAG_HALF_CARRY, true);
  SetFlag(FLAG_SUB, false);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::BIT_U3_HL(uint8_t bit)
{
  uint8_t value = state->memory.Read(state->registers.HL);
  SetFlag(FLAG_ZERO, (((1 << bit) & ~value) != 0));
  SetFlag(FLAG_HALF_CARRY, true);
  SetFlag(FLAG_SUB, false);
  state->registers.PC += 2;
  state->numCycles += 12;
}

void DraconicCPU::RES_U3_R8(uint8_t& target, uint8_t bit)
{
  target = (~(1 << bit) & target);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::RES_U3_HL(uint8_t bit)
{
  uint8_t value = state->memory.Read(state->registers.HL);
  value = (~(1 << bit) & value);
  state->memory.Write(state->registers.HL, value);
  state->registers.PC += 2;
  state->numCycles += 16;
}

void DraconicCPU::SET_U3_R8(uint8_t& target, uint8_t bit)
{
  target = (target | (1 << bit));
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::SET_U3_HL(uint8_t bit)
{
  uint8_t value = state->memory.Read(state->registers.HL);
  value = (value | (1 << bit));
  state->memory.Write(state->registers.HL, value);
  state->registers.PC += 2;
  state->numCycles += 16;
}

void DraconicCPU::SWAP_R8(uint8_t& target)
{
  uint8_t first = target >> 4;
  uint8_t second = target << 4;
  uint8_t swapped = first | second;
  target = swapped;
  SetFlag(FLAG_CARRY, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_ZERO, (target == 0));
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::SWAP_HL()
{
  uint8_t value = state->memory.Read(state->registers.HL);
  uint8_t first = value >> 4;
  uint8_t second = value << 4;
  uint8_t swapped = first | second;
  value = swapped;
  SetFlag(FLAG_CARRY, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_ZERO, (value == 0));
  state->memory.Write(state->registers.HL, value);
  state->registers.PC += 2;
  state->numCycles += 16;
}

void DraconicCPU::RL_R8(uint8_t& target)
{
  RL(target, true, true);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::RL_HL()
{
  RL(state->registers.HL, true);
  state->registers.PC += 2;
  state->numCycles += 16;
}

void DraconicCPU::RLA()
{
  RL(state->registers.A, true);
  state->registers.PC += 1;
  state->numCycles += 4;
}



void DraconicCPU::RLC_R8(uint8_t& target)
{
  RL(target, false, true);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::RLC_HL()
{
  RL(state->registers.HL, false);
  state->registers.PC += 2;
  state->numCycles += 16;
}

void DraconicCPU::RL(uint16_t addr, bool carry)
{
  uint8_t value = state->memory.Read(addr);
  RL(value, carry, true);
  state->memory.Write(addr, value);
}

void DraconicCPU::RL(uint8_t& target, bool carry, bool zero_flag)
{
  int bit7 = ((target & 0x80) != 0);
  target = target << 1;
  target |= (carry) ? ((state->registers.F & FLAG_CARRY) != 0) : bit7;
  SetFlag(FLAG_ZERO, ((zero_flag) ? (target == 0) : false));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, (bit7 != 0));
}

void DraconicCPU::RLCA()
{
  RL(state->registers.A, false);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::RR_R8(uint8_t& target)
{
  RR(target, true, true);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::RR_HL()
{
  RR(state->registers.HL, true);
  state->registers.PC += 2;
  state->numCycles += 16;
}

void DraconicCPU::RR_A()
{
  RR(state->registers.A, true);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::RRC_R8(uint8_t& target)
{
  RR(target, false, true);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::RRC_HL()
{
  RR(state->registers.HL, false);
  state->registers.PC += 2;
  state->numCycles += 16;
}

void DraconicCPU::RR(uint16_t addr, bool carry)
{
  uint8_t value = state->memory.Read(addr);
  RR(value, carry, true);
  state->memory.Write(addr, value);
}

void DraconicCPU::RR(uint8_t& target, bool carry, bool zero_flag)
{
  int bit1 = ((target & 0x1) != 0);
  target = target >> 1;

  target |= (carry) ? (((state->registers.F & FLAG_CARRY) != 0) << 7) : (bit1 << 7);

  SetFlag(FLAG_ZERO, ((zero_flag) ? (target == 0) : false));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, (bit1 != 0));
}
void DraconicCPU::RRC_A()
{
  RR(state->registers.A, false);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::SLA_R8(uint8_t& target)
{
  uint8_t result = target << 1;
  SetFlag(FLAG_CARRY, (target & 0x80) != 0);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_ZERO, (result == 0));
  target = result;
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::SLA_HL()
{
  uint8_t data = state->memory.Read(state->registers.HL);
  uint8_t result = data << 1;
  SetFlag(FLAG_CARRY, (data & 0x80) != 0);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_ZERO, (result == 0));
  data = result;
  state->memory.Write(state->registers.HL, data);

  state->registers.PC += 2;
  state->numCycles += 16;
}

void DraconicCPU::SR(uint8_t& target, bool include_top_bit)
{
  bool top_bit_set = IsBitSet(target, EBit::BIT_7);

  uint8_t result;

  if (include_top_bit)
    result = (top_bit_set) ? ((target >> 1) | 0x80) : (target >> 1);
  else
    result = target >> 1;

  SetFlag(FLAG_CARRY, (target & 0x01) != 0);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_ZERO, (result == 0));

  target = result;
}

void DraconicCPU::SRA_R8(uint8_t& target)
{
  SR(target, true);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::SRA_HL()
{
  uint8_t data = state->memory.Read(state->registers.HL);
  SR(data, true);
  state->memory.Write(state->registers.HL, data);
  state->registers.PC += 2;
  state->numCycles += 16;
}

void DraconicCPU::SRL_R8(uint8_t& target)
{
  SR(target, false);
  state->registers.PC += 2;
  state->numCycles += 8;
}

void DraconicCPU::SRL_HL()
{
  uint8_t data = state->memory.Read(state->registers.HL);
  SR(data, false);
  state->memory.Write(state->registers.HL, data);
  state->registers.PC += 2;
  state->numCycles += 16;
}

void DraconicCPU::LD_R8_R8(uint8_t& target, uint8_t value)
{
  target = value;
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::LD_R8_N8(uint8_t& target, uint8_t value)
{
  target = value;
  state->registers.PC += 2;
  state->numCycles += 8;

}

void DraconicCPU::LD_R16_N16(uint16_t& target, uint16_t value)
{
  target = value;
  state->registers.PC += 3;
  state->numCycles += 12;
}

void DraconicCPU::LD_HL_R8(uint8_t value)
{
  state->memory.Write(state->registers.HL, value);
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::LD_HL_N8(uint8_t value)
{
  state->memory.Write(state->registers.HL, value);
  state->registers.PC += 2;
  state->numCycles += 12;
}

void DraconicCPU::LD_R8_HL(uint8_t& target)
{
  target = state->memory.Read(state->registers.HL);
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::LD_R16_A(uint16_t addr)
{
  state->memory.Write(addr, state->registers.A);
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::LD_N16_A(uint16_t addr)
{
  state->memory.Write(addr, state->registers.A);
  state->registers.PC += 3;
  state->numCycles += 16;
}

void DraconicCPU::LDH_N16_A(uint16_t value)
{
  uint16_t addr = (uint16_t)(0xFF00 + value);
  state->memory.Write(addr, state->registers.A);
  state->registers.PC += 2;
  state->numCycles += 12;
}

void DraconicCPU::LDH_C_A()
{
  uint16_t addr = (uint16_t)(0xFF00 + state->registers.C);
  state->memory.Write(addr, state->registers.A);
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::LD_A_R16(uint16_t addr)
{
  state->registers.A = state->memory.Read(addr);
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::LD_A_N16(uint16_t addr)
{
  state->registers.A = state->memory.Read(addr);
  state->registers.PC += 3;
  state->numCycles += 16;
}

void DraconicCPU::LDH_A_N16(uint16_t value)
{
  uint16_t addr = (uint16_t)(0xFF00 + value);
  state->registers.A = state->memory.Read(addr);
  state->registers.PC += 2;
  state->numCycles += 12;
}

void DraconicCPU::LDH_A_C()
{
  uint16_t addr = (uint16_t)(0xFF00 + state->registers.C);
  state->registers.A = state->memory.Read(addr);
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::LD_HLI_A()
{
  state->memory.Write(state->registers.HL, state->registers.A);
  state->registers.HL++;
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::LD_HLD_A()
{
  state->memory.Write(state->registers.HL, state->registers.A);
  state->registers.HL--;
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::LD_A_HLI()
{
  state->registers.A = state->memory.Read(state->registers.HL);
  state->registers.HL++;
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::LD_A_HLD()
{
  state->registers.A = state->memory.Read(state->registers.HL);
  state->registers.HL--;
  state->registers.PC += 1;
  state->numCycles += 8;
}


void DraconicCPU::CALL(uint16_t addr)
{
  state->registers.SP--;
  state->memory.Write(state->registers.SP, state->registers.PC >> 8 & 0xFF);
  state->registers.SP--;
  state->memory.Write(state->registers.SP, state->registers.PC & 0xFF);
  JP(addr);
  state->numCycles += 12;
}

void DraconicCPU::CALL_N16(uint16_t addr)
{
  state->registers.PC += 3;
  state->numCycles += 12;
  CALL(addr);
}

void DraconicCPU::CALLNZ_N16(uint16_t addr)
{
  state->registers.PC += 3;
  state->numCycles += 12;
  if ((state->registers.F & FLAG_ZERO) == 0)
    CALL(addr);
}

void DraconicCPU::CALLZ_N16(uint16_t addr)
{
  state->registers.PC += 3;
  state->numCycles += 12;
  if ((state->registers.F & FLAG_ZERO) != 0)
    CALL(addr);
}

void DraconicCPU::CALLNC_N16(uint16_t addr)
{
  state->registers.PC += 3;
  state->numCycles += 12;
  if ((state->registers.F & FLAG_CARRY) == 0)
    CALL(addr);
}

void DraconicCPU::CALLC_N16(uint16_t addr)
{
  state->registers.PC += 3;
  state->numCycles += 12;
  if ((state->registers.F & FLAG_CARRY) != 0)
    CALL(addr);
}

void DraconicCPU::JP(uint16_t target)
{
  state->registers.PC = target;
  state->numCycles += 4;
}

void DraconicCPU::JP_HL()
{
  state->registers.PC += 1;
  state->numCycles += 4;
  state->registers.PC = state->registers.HL;
}

void DraconicCPU::JP_N16(uint16_t addr)
{
  state->registers.PC += 3;
  state->numCycles += 12;
  JP(addr);

}

void DraconicCPU::JPNZ_N16(uint16_t target)
{
  state->registers.PC += 3;
  state->numCycles += 12;
  if ((state->registers.F & FLAG_ZERO) == 0)
    JP(target);
}

void DraconicCPU::JPZ_N16(uint16_t target)
{
  state->registers.PC += 3;
  state->numCycles += 12;
  if ((state->registers.F & FLAG_ZERO) != 0)
    JP(target);
}

void DraconicCPU::JPNC_N16(uint16_t target)
{
  state->registers.PC += 3;
  state->numCycles += 12;
  if ((state->registers.F & FLAG_CARRY) == 0)
    JP(target);
}

void DraconicCPU::JPC_N16(uint16_t target)
{
  state->registers.PC += 3;
  state->numCycles += 12;
  if ((state->registers.F & FLAG_CARRY) != 0)
    JP(target);
}

void DraconicCPU::JR(uint8_t value)
{
  int8_t signed_val = ((int8_t)(value));
  state->registers.PC += signed_val;
  state->numCycles += 4;
}

void DraconicCPU::JR_N8(uint8_t value)
{
  state->registers.PC += 2;
  state->numCycles += 8;
  JR(value);
}

void DraconicCPU::JRNZ_N8(uint8_t value)
{
  state->registers.PC += 2;
  state->numCycles += 8;
  if ((state->registers.F & FLAG_ZERO) == 0)
    JR(value);
}

void DraconicCPU::JRZ_N8(uint8_t value)
{
  state->registers.PC += 2;
  state->numCycles += 8;
  if ((state->registers.F & FLAG_ZERO) != 0)
    JR(value);
}

void DraconicCPU::JRNC_N8(uint8_t value)
{
  state->registers.PC += 2;
  state->numCycles += 8;
  if ((state->registers.F & FLAG_CARRY) == 0)
    JR(value);
}

void DraconicCPU::JRC_N8(uint8_t value)
{
  state->registers.PC += 2;
  state->numCycles += 8;
  if ((state->registers.F & FLAG_CARRY) != 0)
    JR(value);
}

void DraconicCPU::JP_CC_N16(uint8_t cc, uint16_t addr)
{

}

void DraconicCPU::JR_E8(uint8_t offset)
{

}

void DraconicCPU::JR_CC_E8(uint8_t cc, uint8_t offset)
{

}

void DraconicCPU::RET_CC(uint8_t cc)
{

}

void DraconicCPU::RET_Impl()
{
  uint8_t low = state->memory.Read(state->registers.SP++);
  uint8_t high = state->memory.Read(state->registers.SP++);
  state->registers.PC = (high << 8 & 0xFF00) | (low & 0xFF);
  state->numCycles += 12;
}


void DraconicCPU::RET()
{
  state->registers.PC += 1;
  state->numCycles += 4;
  RET_Impl();
}

void DraconicCPU::RETI()
{
  state->registers.PC += 1;
  state->numCycles += 4;
  state->interrupt_master_enable = true;
  RET_Impl();
}

void DraconicCPU::RETNZ()
{
  state->registers.PC += 1;
  state->numCycles += 8;
  if ((state->registers.F & FLAG_ZERO) == 0)
  {
    RET_Impl();
    state->numCycles += 8;
  }
}

void DraconicCPU::RETZ()
{
  state->registers.PC += 1;
  state->numCycles += 8;
  if ((state->registers.F & FLAG_ZERO) != 0)
  {
    RET_Impl();
    state->numCycles += 8;
  }
}

void DraconicCPU::RETNC()
{
  state->registers.PC += 1;
  state->numCycles += 8;
  if ((state->registers.F & FLAG_CARRY) == 0)
  {
    RET_Impl();
    state->numCycles += 8;
  }
}

void DraconicCPU::RETC()
{
  state->registers.PC += 1;
  state->numCycles += 8;
  if ((state->registers.F & FLAG_CARRY) != 0)
  {
    RET_Impl();
    state->numCycles += 8;
  }
}

void DraconicCPU::RST_VEC(uint16_t addr)
{
  state->registers.PC += 1;
  state->numCycles += 16;
  state->registers.SP--;
  state->memory.Write(state->registers.SP, (state->registers.PC >> 8 & 0xFF));
  state->registers.SP--;
  state->memory.Write(state->registers.SP, (state->registers.PC & 0xFF));
  state->registers.PC = addr;
}

void DraconicCPU::ADD_HL_SP()
{
  int result = state->registers.HL + state->registers.SP;
  SetFlag(FLAG_SUB, false); // reset
  SetFlag(FLAG_HALF_CARRY, ((((state->registers.HL & 0xFFF) + (state->registers.SP & 0xFFF)) & 0x1000) != 0)); // Set if carry from bit 11
  SetFlag(FLAG_CARRY, (result > 0xFFFF)); // Set if carry from bit 15
  state->registers.HL = result & 0xFFFF;
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::ADD_SP_E8(uint8_t offset)
{
  int16_t val_signed = (int16_t)(int8_t)offset;
  int result = (uint16_t)((int16_t)state->registers.SP + val_signed);
  SetFlag(FLAG_CARRY, (result & 0xFF) < (state->registers.SP & 0xFF));
  SetFlag(FLAG_HALF_CARRY, (result & 0xF) < (state->registers.SP & 0xF));
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_ZERO, false);
  state->registers.SP = result & 0xFFFF;
  state->registers.PC += 2;
  state->numCycles += 16;
}

void DraconicCPU::DEC_SP()
{
  state->registers.SP--;
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::INC_SP()
{
  state->registers.SP++;
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::LD_SP_N16(uint16_t value)
{
  state->registers.SP = value;
  state->registers.PC += 3;
  state->numCycles += 12;
}

void DraconicCPU::LD_N16_SP(uint16_t addr)
{
  uint8_t lsb = state->registers.SP & 0xFF;
  uint8_t msb = state->registers.SP >> 8 & 0xFF;

  state->memory.Write(addr, lsb);
  addr++;
  state->memory.Write(addr, msb);
  state->registers.PC += 3;
  state->numCycles += 20;
}

void DraconicCPU::LD_HL_SP_E8(uint8_t offset)
{
  int16_t signed_val = (int16_t)(int8_t)offset;
  uint16_t result = (uint16_t)((int16_t)state->registers.SP + signed_val);

  SetFlag(FLAG_CARRY, (result & 0xFF) < (state->registers.SP & 0xFF));
  SetFlag(FLAG_HALF_CARRY, (result & 0xF) < (state->registers.SP & 0xF));
  SetFlag(FLAG_ZERO, false);
  SetFlag(FLAG_SUB, false);

  state->registers.HL = result;
  state->registers.PC += 2;
  state->numCycles += 12;
}

void DraconicCPU::LD_SP_HL()
{
  state->registers.SP = state->registers.HL;
  state->registers.PC += 1;
  state->numCycles += 8;
}

void DraconicCPU::POP_AF()
{
  uint8_t low = state->memory.Read(state->registers.SP++);
  uint8_t high = state->memory.Read(state->registers.SP++);
  state->registers.AF = high << 8 | low;
  state->registers.F &= 0xF0;
  state->registers.PC += 1;
  state->numCycles += 12;
}

void DraconicCPU::POP_R16(uint16_t& value)
{
  uint8_t low = state->memory.Read(state->registers.SP++);
  uint8_t high = state->memory.Read(state->registers.SP++);
  value = high << 8 | low;
  state->registers.PC += 1;
  state->numCycles += 12;
}

void DraconicCPU::PUSH_AF()
{

}

void DraconicCPU::PUSH_R16(uint16_t value)
{
  state->registers.SP--;
  state->memory.Write(state->registers.SP, value >> 8 & 0xFF);
  state->registers.SP--;
  state->memory.Write(state->registers.SP, value & 0xFF);
  state->registers.PC += 1;
  state->numCycles += 16;
}

void DraconicCPU::CCF()
{
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, ((state->registers.F & FLAG_CARRY) ? 1 : 0) ^ 1);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::CPL()
{
  state->registers.A = ~state->registers.A;
  SetFlag(FLAG_HALF_CARRY, true);
  SetFlag(FLAG_SUB, true);
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::DAA()
{
  uint8_t high = state->registers.A >> 4 & 0xF;
  uint8_t low = state->registers.A & 0xF;

  bool add = ((state->registers.F & FLAG_SUB) == 0);
  bool carry = ((state->registers.F & FLAG_CARRY) != 0);
  bool half_carry = ((state->registers.F & FLAG_HALF_CARRY) != 0);

  uint16_t result = (uint16_t)state->registers.A;
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
  state->registers.A = (uint8_t)(result & 0xFF);
  SetFlag(FLAG_ZERO, state->registers.A == 0);

  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::DI()
{
  state->registers.PC += 1;
  state->numCycles += 4;
  state->interrupt_master_enable = false;

}

void DraconicCPU::EI()
{
  state->registers.PC += 1;
  state->numCycles += 4;
  state->interrupt_master_enable = true;
}

void DraconicCPU::HALT()
{
  state->registers.PC += 1;
  state->numCycles += 4;
  state->halted = true;
  state->registers.PC--;
}

void DraconicCPU::NOP()
{
  state->registers.PC += 1;
  state->numCycles += 4;
}

void DraconicCPU::SCF()
{
  state->registers.PC += 1;
  state->numCycles += 4;
  SetFlag(FLAG_SUB, false);
  SetFlag(FLAG_HALF_CARRY, false);
  SetFlag(FLAG_CARRY, true);
}

void DraconicCPU::STOP()
{
  state->registers.PC += 1;
  state->numCycles += 4;
}
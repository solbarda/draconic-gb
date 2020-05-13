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
  case 0x7F: LD_R8_R8(registers.A, registers.A); break;
  case 0x78: LD_R8_R8(registers.A, registers.B); break;
  case 0x79: LD_R8_R8(registers.A, registers.C); break;
  case 0x7A: LD_R8_R8(registers.A, registers.D); break;
  case 0x7B: LD_R8_R8(registers.A, registers.E); break;
  case 0x7C: LD_R8_R8(registers.A, registers.H); break;
  case 0x7D: LD_R8_R8(registers.A, registers.L); break;
  case 0x47: LD_R8_R8(registers.B, registers.A); break;
  case 0x40: LD_R8_R8(registers.B, registers.B); break;
  case 0x41: LD_R8_R8(registers.B, registers.C); break;
  case 0x42: LD_R8_R8(registers.B, registers.D); break;
  case 0x43: LD_R8_R8(registers.B, registers.E); break;
  case 0x44: LD_R8_R8(registers.B, registers.H); break;
  case 0x45: LD_R8_R8(registers.B, registers.L); break;
  case 0x4F: LD_R8_R8(registers.C, registers.A); break;
  case 0x48: LD_R8_R8(registers.C, registers.B); break;
  case 0x49: LD_R8_R8(registers.C, registers.C); break;
  case 0x4A: LD_R8_R8(registers.C, registers.D); break;
  case 0x4B: LD_R8_R8(registers.C, registers.E); break;
  case 0x4C: LD_R8_R8(registers.C, registers.H); break;
  case 0x4D: LD_R8_R8(registers.C, registers.L); break;
  case 0x57: LD_R8_R8(registers.D, registers.A); break;
  case 0x50: LD_R8_R8(registers.D, registers.B); break;
  case 0x51: LD_R8_R8(registers.D, registers.C); break;
  case 0x52: LD_R8_R8(registers.D, registers.D); break;
  case 0x53: LD_R8_R8(registers.D, registers.E); break;
  case 0x54: LD_R8_R8(registers.D, registers.H); break;
  case 0x55: LD_R8_R8(registers.D, registers.L); break;
  case 0x5F: LD_R8_R8(registers.E, registers.A); break;
  case 0x58: LD_R8_R8(registers.E, registers.B); break;
  case 0x59: LD_R8_R8(registers.E, registers.C); break;
  case 0x5A: LD_R8_R8(registers.E, registers.D); break;
  case 0x5B: LD_R8_R8(registers.E, registers.E); break;
  case 0x5C: LD_R8_R8(registers.E, registers.H); break;
  case 0x5D: LD_R8_R8(registers.E, registers.L); break;
  case 0x67: LD_R8_R8(registers.H, registers.A); break;
  case 0x60: LD_R8_R8(registers.H, registers.B); break;
  case 0x61: LD_R8_R8(registers.H, registers.C); break;
  case 0x62: LD_R8_R8(registers.H, registers.D); break;
  case 0x63: LD_R8_R8(registers.H, registers.E); break;
  case 0x64: LD_R8_R8(registers.H, registers.H); break;
  case 0x65: LD_R8_R8(registers.H, registers.L); break;
  case 0x6F: LD_R8_R8(registers.L, registers.A); break;
  case 0x68: LD_R8_R8(registers.L, registers.B); break;
  case 0x69: LD_R8_R8(registers.L, registers.C); break;
  case 0x6A: LD_R8_R8(registers.L, registers.D); break;
  case 0x6B: LD_R8_R8(registers.L, registers.E); break;
  case 0x6C: LD_R8_R8(registers.L, registers.H); break;
  case 0x6D: LD_R8_R8(registers.L, registers.L); break;
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
    POP_R16(registers.AF);
    // After failing tests, apparently lower 4 bits of register F
    // (all flags) are set to zero.
    registers.F &= 0xF0;
    break;
  case 0xF8: LD_HL_SP_E8(value8Low); break;
  case 0x08: LD_N16_SP(value8Low, value8High); break;
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
    // 93
  case 0x97: SUB_A_R8(registers.A); break;
  case 0x90: SUB_A_R8(registers.B); break;
  case 0x91: SUB_A_R8(registers.C); break;
  case 0x92: SUB_A_R8(registers.D); break;
  case 0x93: SUB_A_R8(registers.E); break;
  case 0x94: SUB_A_R8(registers.H); break;
  case 0x95: SUB_A_R8(registers.L); break;
  case 0xD6: SUB_A_N8(value); break;
  case 0x96: SUB_A_HL(); break;
  case 0x9F: SBC_A_R8(registers.A); break;
  case 0x98: SBC_A_R8(registers.B); break;
  case 0x99: SBC_A_R8(registers.C); break;
  case 0x9A: SBC_A_R8(registers.D); break;
  case 0x9B: SBC_A_R8(registers.E); break;
  case 0x9C: SBC_A_R8(registers.H); break;
  case 0x9D: SBC_A_R8(registers.L); break;
  case 0xDE: SBC_A_N8(value); break;
  case 0x9E: SBC_A_HL(); break;
    // 94
  case 0xA7: AND_A_R8(registers.A); break;
  case 0xA0: AND_A_R8(registers.B); break;
  case 0xA1: AND_A_R8(registers.C); break;
  case 0xA2: AND_A_R8(registers.D); break;
  case 0xA3: AND_A_R8(registers.E); break;
  case 0xA4: AND_A_R8(registers.H); break;
  case 0xA5: AND_A_R8(registers.L); break;
  case 0xE6: AND_A_N8(value); break;
  case 0xA6: AND_A_HL(); break;
  case 0xB7: OR(state->registers.A, state->registers.A); op(1, 1); break;
  case 0xB0: OR(state->registers.A, state->registers.B); op(1, 1); break;
  case 0xB1: OR(state->registers.A, state->registers.C); op(1, 1); break;
  case 0xB2: OR(state->registers.A, state->registers.D); op(1, 1); break;
  case 0xB3: OR(state->registers.A, state->registers.E); op(1, 1); break;
  case 0xB4: OR(state->registers.A, state->registers.H); op(1, 1); break;
  case 0xB5: OR(state->registers.A, state->registers.L); op(1, 1); break;
  case 0xF6: OR(state->registers.A, value); op(2, 2); break;
  case 0xB6: OR(state->registers.A, Pair(state->registers.H, state->registers.L).address()); op(1, 2); break;
  case 0xAF: XOR(state->registers.A, state->registers.A); op(1, 1); break;
  case 0xA8: XOR(state->registers.A, state->registers.B); op(1, 1); break;
  case 0xA9: XOR(state->registers.A, state->registers.C); op(1, 1); break;
  case 0xAA: XOR(state->registers.A, state->registers.D); op(1, 1); break;
  case 0xAB: XOR(state->registers.A, state->registers.E); op(1, 1); break;
  case 0xAC: XOR(state->registers.A, state->registers.H); op(1, 1); break;
  case 0xAD: XOR(state->registers.A, state->registers.L); op(1, 1); break;
  case 0xEE: XOR(state->registers.A, value); op(2, 2); break;
  case 0xAE: XOR(state->registers.A, Pair(state->registers.H, state->registers.L).address()); op(1, 2); break;
    // 95 - 96
  case 0xBF: CP(state->registers.A, state->registers.A); op(1, 1); break;
  case 0xB8: CP(state->registers.A, state->registers.B); op(1, 1); break;
  case 0xB9: CP(state->registers.A, state->registers.C); op(1, 1); break;
  case 0xBA: CP(state->registers.A, state->registers.D); op(1, 1); break;
  case 0xBB: CP(state->registers.A, state->registers.E); op(1, 1); break;
  case 0xBC: CP(state->registers.A, state->registers.H); op(1, 1); break;
  case 0xBD: CP(state->registers.A, state->registers.L); op(1, 1); break;
  case 0xFE: CP(state->registers.A, value); op(2, 2); break;
  case 0xBE: CP(state->registers.A, Pair(state->registers.H, state->registers.L).address()); op(1, 2); break;
  case 0x3C: INC8(state->registers.A); op(1, 1); break;
  case 0x04: INC8(state->registers.B); op(1, 1); break;
  case 0x0C: INC8(state->registers.C); op(1, 1); break;
  case 0x14: INC8(state->registers.D); op(1, 1); break;
  case 0x1C: INC8(state->registers.E); op(1, 1); break;
  case 0x24: INC8(state->registers.H); op(1, 1); break;
  case 0x2C: INC8(state->registers.L); op(1, 1); break;
  case 0x34: INCMem(state->registers.HL); op(1, 3); break;
  case 0x3D: DEC(state->registers.A); op(1, 1); break;
  case 0x05: DEC(state->registers.B); op(1, 1); break;
  case 0x0D: DEC(state->registers.C); op(1, 1); break;
  case 0x15: DEC(state->registers.D); op(1, 1); break;
  case 0x1D: DEC(state->registers.E); op(1, 1); break;
  case 0x25: DEC(state->registers.H); op(1, 1); break;
  case 0x2D: DEC(state->registers.L); op(1, 1); break;
  case 0x35: DEC(Pair(state->registers.H, state->registers.L).address()); op(1, 3); break;
    // 97
  case 0x09: ADDHL(state->registers.BC); op(1, 2); break;
  case 0x19: ADDHL(state->registers.DE); op(1, 2); break;
  case 0x29: ADDHL(state->registers.HL); op(1, 2); break;
  case 0x39: ADDHLSP();                 op(1, 2); break;
  case 0xE8: ADDSP(value); op(2, 4); break;
  case 0x03: INC16(state->registers.BC); op(1, 2); break;
  case 0x13: INC16(state->registers.DE); op(1, 2); break;
  case 0x23: INC16(state->registers.HL); op(1, 2); break;
  case 0x33: INCSP();                 op(1, 2); break;
  case 0x0B: DEC(Pair(state->registers.B, state->registers.C)); op(1, 2); break;
  case 0x1B: DEC(Pair(state->registers.D, state->registers.E)); op(1, 2); break;
  case 0x2B: DEC(Pair(state->registers.H, state->registers.L)); op(1, 2); break;
  case 0x3B: DECSP();                 op(1, 2); break;
    // 98
  case 0x07: RL(state->registers.A, false);  op(1, 1); break; // RLCA
  case 0x17: RL(state->registers.A, true);   op(1, 1); break; // RLA
  case 0x0F: RR(state->registers.A, false);  op(1, 1); break;
  case 0x1F: RR(state->registers.A, true);   op(1, 1); break;
    // 99 - 104
  case 0xCB: parse_bit_op(value); break;
    // 105
  case 0xC3: op(3, 3); JP(Pair(value2, value));   break; // 1 cycle added in JP();
  case 0xC2: op(3, 3); JPNZ(Pair(value2, value)); break;
  case 0xCA: op(3, 3); JPZ(Pair(value2, value));  break;
  case 0xD2: op(3, 3); JPNC(Pair(value2, value)); break;
  case 0xDA: op(3, 3); JPC(Pair(value2, value));  break;
    // 106
  case 0x18: op(2, 2); JR(value); break; // 1 cycle added in JR();
  case 0x20: op(2, 2); JRNZ(value); break;
  case 0x28: op(2, 2); JRZ(value); break;
  case 0x30: op(2, 2); JRNC(value); break;
  case 0x38: op(2, 2); JRC(value); break;
  case 0xE9: op(1, 1); JPHL(); break;
    // 107
  case 0xCD: op(3, 3); CALL(value, value2); break; // 3 cycles added in CALL();
  case 0xC4: op(3, 3); CALLNZ(value, value2); break; // op() must be called before CALL() because it relies on updated PC
  case 0xCC: op(3, 3); CALLZ(value, value2); break;
  case 0xD4: op(3, 3); CALLNC(value, value2); break;
  case 0xDC: op(3, 3); CALLC(value, value2); break;
    // 108
  case 0xC9: op(1, 1); RET(); break; // 3 cycles added in RET();
  case 0xC0: op(1, 2); RETNZ(); break;
  case 0xC8: op(1, 2); RETZ(); break;
  case 0xD0: op(1, 2); RETNC(); break;
  case 0xD8: op(1, 2); RETC(); break;
  case 0xD9: op(1, 1); RETI(); break;
    // 109
  case 0xC7: op(1, 4); RST(0x00); break; // RST() relies on updated PC, op() must be first
  case 0xCF: op(1, 4); RST(0x08); break;
  case 0xD7: op(1, 4); RST(0x10); break;
  case 0xDF: op(1, 4); RST(0x18); break;
  case 0xE7: op(1, 4); RST(0x20); break;
  case 0xEF: op(1, 4); RST(0x28); break;
  case 0xF7: op(1, 4); RST(0x30); break;
  case 0xFF: op(1, 4); RST(0x38); break;
    // 110-111
  case 0x27: DAA(); op(1, 1); break;
  case 0x2F: CPL(); op(1, 1); break;
  case 0x00: NOP(); op(1, 1); break;

    // GBCPUMAN
  case 0xF3: DI(); op(1, 1); break; // Disable interrupts
  case 0xFB: EI(); op(1, 1); break; // Enable interrupts
  // 112
  case 0x76: HALT(); op(1, 1); break;
    // case 0x10: STOP(); op(2, 1); break; // UNIMPLEMENTED

    // Pandocs
  case 0x37: SCF(); op(1, 1); break;
  case 0x3F: CCF(); op(1, 1); break;

  default: op(1, 0); break;



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

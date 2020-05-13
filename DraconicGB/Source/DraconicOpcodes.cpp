#include "DraconicOpcodes.h"

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
  case 0xC5: PUSH_R16(registers.BC); break;
  case 0xD5: PUSH_R16(registers.DE); break;
  case 0xE5: PUSH_R16(registers.HL); break;
  case 0xF5: PUSH_R16(registers.AF); break;
    // 91
  case 0xC1: POP_R16(registers.BC); break;
  case 0xD1: POP_R16(registers.DE); break;
  case 0xE1: POP_R16(registers.HL); break;
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
  case 0xB7: OR_A_R8(registers.A); break;
  case 0xB0: OR_A_R8(registers.B); break;
  case 0xB1: OR_A_R8(registers.C); break;
  case 0xB2: OR_A_R8(registers.D); break;
  case 0xB3: OR_A_R8(registers.E); break;
  case 0xB4: OR_A_R8(registers.H); break;
  case 0xB5: OR_A_R8(registers.L); break;
  case 0xF6: OR_A_N8(value); break;
  case 0xB6: OR_A_HL(); break;
  case 0xAF: XOR_A_R8(registers.A); break;
  case 0xA8: XOR_A_R8(registers.B); break;
  case 0xA9: XOR_A_R8(registers.C); break;
  case 0xAA: XOR_A_R8(registers.D); break;
  case 0xAB: XOR_A_R8(registers.E); break;
  case 0xAC: XOR_A_R8(registers.H); break;
  case 0xAD: XOR_A_R8(registers.L); break;
  case 0xEE: XOR_A_N8(value); break;
  case 0xAE: XOR_A_HL(); break;
    // 95 - 96
  case 0xBF: CP_A_R8(registers.A); break;
  case 0xB8: CP_A_R8(registers.B); break;
  case 0xB9: CP_A_R8(registers.C); break;
  case 0xBA: CP_A_R8(registers.D); break;
  case 0xBB: CP_A_R8(registers.E); break;
  case 0xBC: CP_A_R8(registers.H); break;
  case 0xBD: CP_A_R8(registers.L); break;
  case 0xFE: CP_A_N8(value); break;
  case 0xBE: CP_A_HL(); break;
  case 0x3C: INC_R8(registers.A); break;
  case 0x04: INC_R8(registers.B); break;
  case 0x0C: INC_R8(registers.C); break;
  case 0x14: INC_R8(registers.D); break;
  case 0x1C: INC_R8(registers.E); break;
  case 0x24: INC_R8(registers.H); break;
  case 0x2C: INC_R8(registers.L); break;
  case 0x34: INC_HL(); break;
  case 0x3D: DEC_R8(registers.A); break;
  case 0x05: DEC_R8(registers.B); break;
  case 0x0D: DEC_R8(registers.C); break;
  case 0x15: DEC_R8(registers.D); break;
  case 0x1D: DEC_R8(registers.E); break;
  case 0x25: DEC_R8(registers.H); break;
  case 0x2D: DEC_R8(registers.L); break;
  case 0x35: DEC_HL(); break;
    // 97
  case 0x09: ADD_HL_R16(registers.BC); break;
  case 0x19: ADD_HL_R16(registers.DE); break;
  case 0x29: ADD_HL_R16(registers.HL); break;
  case 0x39: ADD_HL_SP(); break;
  case 0xE8: ADD_SP_E8(value); break;
  case 0x03: INC_R16(registers.BC); break;
  case 0x13: INC_R16(registers.DE); break;
  case 0x23: INC_R16(registers.HL); break;
  case 0x33: INC_SP(); break;
  case 0x0B: DEC_R16(registers.BC); break;
  case 0x1B: DEC_R16(registers.DE); break;
  case 0x2B: DEC_R16(registers.HL); break;
  case 0x3B: DEC_SP(); break;
    // 98
  case 0x07: RLCA(); break; // RLCA
  case 0x17: RLA(); break; // RLA
  case 0x0F: RRC_A(state->registers.A, false); break;
  case 0x1F: RR_A(state->registers.A, true); break;
    // 99 - 104
  case 0xCB: ParseBitOperation(value8Low); break;
    // 105
  case 0xC3: JP_N16(value16);   break; // 1 cycle added in JP();
  case 0xC2: JPNZ_N16(value16); break;
  case 0xCA: JPZ_N16(value16);  break;
  case 0xD2: JPNC_N16(value16); break;
  case 0xDA: JPC_N16(value16);  break;
    // 106
  case 0x18: JR_N8(value); break; // 1 cycle added in JR();
  case 0x20: JRNZ_N8(value); break;
  case 0x28: JRZ_N8(value); break;
  case 0x30: JRNC_N8(value); break;
  case 0x38: JRC_N8(value); break;
  case 0xE9: JP_HL(); break;
    // 107
  case 0xCD: CALL_N16(value16); break; // 3 cycles added in CALL();
  case 0xC4: CALLNZ_N16(value16); break; // op() must be called before CALL() because it relies on updated PC
  case 0xCC: CALLZ_N16(value16); break;
  case 0xD4: CALLNC_N16(value16); break;
  case 0xDC: CALLC_N16(value16); break;
    // 108
  case 0xC9: RET(); break; // 3 cycles added in RET();
  case 0xC0: RETNZ(); break;
  case 0xC8: RETZ(); break;
  case 0xD0: RETNC(); break;
  case 0xD8: RETC(); break;
  case 0xD9: RETI(); break;
    // 109
  case 0xC7: RST_VEC(0x00); break; // RST() relies on updated PC, op() must be first
  case 0xCF: RST_VEC(0x08); break;
  case 0xD7: RST_VEC(0x10); break;
  case 0xDF: RST_VEC(0x18); break;
  case 0xE7: RST_VEC(0x20); break;
  case 0xEF: RST_VEC(0x28); break;
  case 0xF7: RST_VEC(0x30); break;
  case 0xFF: RST_VEC(0x38); break;
    // 110-111
  case 0x27: DAA(); break;
  case 0x2F: CPL(); break;
  case 0x00: NOP(); break;

    // GBCPUMAN
  case 0xF3: DI(); break; // Disable interrupts
  case 0xFB: EI(); break; // Enable interrupts
  // 112
  case 0x76: HALT(); break;
    // case 0x10: STOP(); op(2, 1); break; // UNIMPLEMENTED

    // Pandocs
  case 0x37: SCF(); break;
  case 0x3F: CCF(); break;

  default: registers.PC += 1; break;



  }
}

void DraconicState::ParseBitOperation(uint8_t op)
{
  switch (code)
  {
  case 0x07: RLC_R8(registers.A); break;
  case 0x00: RLC_R8(registers.B); break;
  case 0x01: RLC_R8(registers.C); break;
  case 0x02: RLC_R8(registers.D); break;
  case 0x03: RLC_R8(registers.E); break;
  case 0x04: RLC_R8(registers.H); break;
  case 0x05: RLC_R8(registers.L); break;
  case 0x06: RLC_HL(); break;
  case 0x17: RL_R8(registers.A); break;
  case 0x10: RL_R8(registers.B); break;
  case 0x11: RL_R8(registers.C); break;
  case 0x12: RL_R8(registers.D); break;
  case 0x13: RL_R8(registers.E); break;
  case 0x14: RL_R8(registers.H); break;
  case 0x15: RL_R8(registers.L); break;
  case 0x16: RL_HL(); break;

  case 0x0F: RRC_R8(registers.A); break;
  case 0x08: RRC_R8(registers.B); break;
  case 0x09: RRC_R8(registers.C); break;
  case 0x0A: RRC_R8(registers.D); break;
  case 0x0B: RRC_R8(registers.E); break;
  case 0x0C: RRC_R8(registers.H); break;
  case 0x0D: RRC_R8(registers.L); break;
  case 0x0E: RRC_HL(); break;
  case 0x1F: RR_R8(registers.A); break;
  case 0x18: RR_R8(registers.B); break;
  case 0x19: RR_R8(registers.C); break;
  case 0x1A: RR_R8(registers.D); break;
  case 0x1B: RR_R8(registers.E); break;
  case 0x1C: RR_R8(registers.H); break;
  case 0x1D: RR_R8(registers.L); break;
  case 0x1E: RR_HL(); break; // this could have a different beginning opcode, check manual

  case 0x27: SLA_R8(registers.A); break;
  case 0x20: SLA_R8(registers.B); break;
  case 0x21: SLA_R8(registers.C); break;
  case 0x22: SLA_R8(registers.D); break;
  case 0x23: SLA_R8(registers.E); break;
  case 0x24: SLA_R8(registers.H); break;
  case 0x25: SLA_R8(registers.L); break;
  case 0x26: SLA_HL(); break; // this could actually have a different beginning opcode, check manual

  case 0x2F: SR(state->registers.A, true); op(2, 2); break;
  case 0x28: SR(state->registers.B, true); op(2, 2); break;
  case 0x29: SR(state->registers.C, true); op(2, 2); break;
  case 0x2A: SR(state->registers.D, true); op(2, 2); break;
  case 0x2B: SR(state->registers.E, true); op(2, 2); break;
  case 0x2C: SR(state->registers.H, true); op(2, 2); break;
  case 0x2D: SR(state->registers.L, true); op(2, 2); break;
  case 0x2E: SR(Pair(state->registers.H, state->registers.L).address(), true); op(2, 4); break;

  case 0x3F: SR(state->registers.A, false); op(2, 2); break;
  case 0x38: SR(state->registers.B, false); op(2, 2); break;
  case 0x39: SR(state->registers.C, false); op(2, 2); break;
  case 0x3A: SR(state->registers.D, false); op(2, 2); break;
  case 0x3B: SR(state->registers.E, false); op(2, 2); break;
  case 0x3C: SR(state->registers.H, false); op(2, 2); break;
  case 0x3D: SR(state->registers.L, false); op(2, 2); break;
  case 0x3E: SR(Pair(state->registers.H, state->registers.L).address(), false); op(2, 4); break;

  case 0x37: SWAP(state->registers.A); op(2, 2); break;
  case 0x30: SWAP(state->registers.B); op(2, 2); break;
  case 0x31: SWAP(state->registers.C); op(2, 2); break;
  case 0x32: SWAP(state->registers.D); op(2, 2); break;
  case 0x33: SWAP(state->registers.E); op(2, 2); break;
  case 0x34: SWAP(state->registers.H); op(2, 2); break;
  case 0x35: SWAP(state->registers.L); op(2, 2); break;
  case 0x36: SWAP(Pair(state->registers.H, state->registers.L).address()); op(2, 4); break;

  case 0x47: BIT(state->registers.A, 0); op(2, 2); break;
  case 0x4F: BIT(state->registers.A, 1); op(2, 2); break;
  case 0x57: BIT(state->registers.A, 2); op(2, 2); break;
  case 0x5F: BIT(state->registers.A, 3); op(2, 2); break;
  case 0x67: BIT(state->registers.A, 4); op(2, 2); break;
  case 0x6F: BIT(state->registers.A, 5); op(2, 2); break;
  case 0x77: BIT(state->registers.A, 6); op(2, 2); break;
  case 0x7F: BIT(state->registers.A, 7); op(2, 2); break;
  case 0x40: BIT(state->registers.B, 0); op(2, 2); break;
  case 0x48: BIT(state->registers.B, 1); op(2, 2); break;
  case 0x50: BIT(state->registers.B, 2); op(2, 2); break;
  case 0x58: BIT(state->registers.B, 3); op(2, 2); break;
  case 0x60: BIT(state->registers.B, 4); op(2, 2); break;
  case 0x68: BIT(state->registers.B, 5); op(2, 2); break;
  case 0x70: BIT(state->registers.B, 6); op(2, 2); break;
  case 0x78: BIT(state->registers.B, 7); op(2, 2); break;
  case 0x41: BIT(state->registers.C, 0); op(2, 2); break;
  case 0x49: BIT(state->registers.C, 1); op(2, 2); break;
  case 0x51: BIT(state->registers.C, 2); op(2, 2); break;
  case 0x59: BIT(state->registers.C, 3); op(2, 2); break;
  case 0x61: BIT(state->registers.C, 4); op(2, 2); break;
  case 0x69: BIT(state->registers.C, 5); op(2, 2); break;
  case 0x71: BIT(state->registers.C, 6); op(2, 2); break;
  case 0x79: BIT(state->registers.C, 7); op(2, 2); break;
  case 0x42: BIT(state->registers.D, 0); op(2, 2); break;
  case 0x4A: BIT(state->registers.D, 1); op(2, 2); break;
  case 0x52: BIT(state->registers.D, 2); op(2, 2); break;
  case 0x5A: BIT(state->registers.D, 3); op(2, 2); break;
  case 0x62: BIT(state->registers.D, 4); op(2, 2); break;
  case 0x6A: BIT(state->registers.D, 5); op(2, 2); break;
  case 0x72: BIT(state->registers.D, 6); op(2, 2); break;
  case 0x7A: BIT(state->registers.D, 7); op(2, 2); break;
  case 0x43: BIT(state->registers.E, 0); op(2, 2); break;
  case 0x4B: BIT(state->registers.E, 1); op(2, 2); break;
  case 0x53: BIT(state->registers.E, 2); op(2, 2); break;
  case 0x5B: BIT(state->registers.E, 3); op(2, 2); break;
  case 0x63: BIT(state->registers.E, 4); op(2, 2); break;
  case 0x6B: BIT(state->registers.E, 5); op(2, 2); break;
  case 0x73: BIT(state->registers.E, 6); op(2, 2); break;
  case 0x7B: BIT(state->registers.E, 7); op(2, 2); break;
  case 0x44: BIT(state->registers.H, 0); op(2, 2); break;
  case 0x4C: BIT(state->registers.H, 1); op(2, 2); break;
  case 0x54: BIT(state->registers.H, 2); op(2, 2); break;
  case 0x5C: BIT(state->registers.H, 3); op(2, 2); break;
  case 0x64: BIT(state->registers.H, 4); op(2, 2); break;
  case 0x6C: BIT(state->registers.H, 5); op(2, 2); break;
  case 0x74: BIT(state->registers.H, 6); op(2, 2); break;
  case 0x7C: BIT(state->registers.H, 7); op(2, 2); break;
  case 0x45: BIT(state->registers.L, 0); op(2, 2); break;
  case 0x4D: BIT(state->registers.L, 1); op(2, 2); break;
  case 0x55: BIT(state->registers.L, 2); op(2, 2); break;
  case 0x5D: BIT(state->registers.L, 3); op(2, 2); break;
  case 0x65: BIT(state->registers.L, 4); op(2, 2); break;
  case 0x6D: BIT(state->registers.L, 5); op(2, 2); break;
  case 0x75: BIT(state->registers.L, 6); op(2, 2); break;
  case 0x7D: BIT(state->registers.L, 7); op(2, 2); break;
  case 0x46: BIT(Pair(state->registers.H, state->registers.L).address(), 0); op(2, 3); break;
  case 0x4E: BIT(Pair(state->registers.H, state->registers.L).address(), 1); op(2, 3); break;
  case 0x56: BIT(Pair(state->registers.H, state->registers.L).address(), 2); op(2, 3); break;
  case 0x5E: BIT(Pair(state->registers.H, state->registers.L).address(), 3); op(2, 3); break;
  case 0x66: BIT(Pair(state->registers.H, state->registers.L).address(), 4); op(2, 3); break;
  case 0x6E: BIT(Pair(state->registers.H, state->registers.L).address(), 5); op(2, 3); break;
  case 0x76: BIT(Pair(state->registers.H, state->registers.L).address(), 6); op(2, 3); break;
  case 0x7E: BIT(Pair(state->registers.H, state->registers.L).address(), 7); op(2, 3); break;

  case 0xC7: SET(state->registers.A, 0); op(2, 2); break;
  case 0xCF: SET(state->registers.A, 1); op(2, 2); break;
  case 0xD7: SET(state->registers.A, 2); op(2, 2); break;
  case 0xDF: SET(state->registers.A, 3); op(2, 2); break;
  case 0xE7: SET(state->registers.A, 4); op(2, 2); break;
  case 0xEF: SET(state->registers.A, 5); op(2, 2); break;
  case 0xF7: SET(state->registers.A, 6); op(2, 2); break;
  case 0xFF: SET(state->registers.A, 7); op(2, 2); break;
  case 0xC0: SET(state->registers.B, 0); op(2, 2); break;
  case 0xC8: SET(state->registers.B, 1); op(2, 2); break;
  case 0xD0: SET(state->registers.B, 2); op(2, 2); break;
  case 0xD8: SET(state->registers.B, 3); op(2, 2); break;
  case 0xE0: SET(state->registers.B, 4); op(2, 2); break;
  case 0xE8: SET(state->registers.B, 5); op(2, 2); break;
  case 0xF0: SET(state->registers.B, 6); op(2, 2); break;
  case 0xF8: SET(state->registers.B, 7); op(2, 2); break;
  case 0xC1: SET(state->registers.C, 0); op(2, 2); break;
  case 0xC9: SET(state->registers.C, 1); op(2, 2); break;
  case 0xD1: SET(state->registers.C, 2); op(2, 2); break;
  case 0xD9: SET(state->registers.C, 3); op(2, 2); break;
  case 0xE1: SET(state->registers.C, 4); op(2, 2); break;
  case 0xE9: SET(state->registers.C, 5); op(2, 2); break;
  case 0xF1: SET(state->registers.C, 6); op(2, 2); break;
  case 0xF9: SET(state->registers.C, 7); op(2, 2); break;
  case 0xC2: SET(state->registers.D, 0); op(2, 2); break;
  case 0xCA: SET(state->registers.D, 1); op(2, 2); break;
  case 0xD2: SET(state->registers.D, 2); op(2, 2); break;
  case 0xDA: SET(state->registers.D, 3); op(2, 2); break;
  case 0xE2: SET(state->registers.D, 4); op(2, 2); break;
  case 0xEA: SET(state->registers.D, 5); op(2, 2); break;
  case 0xF2: SET(state->registers.D, 6); op(2, 2); break;
  case 0xFA: SET(state->registers.D, 7); op(2, 2); break;
  case 0xC3: SET(state->registers.E, 0); op(2, 2); break;
  case 0xCB: SET(state->registers.E, 1); op(2, 2); break;
  case 0xD3: SET(state->registers.E, 2); op(2, 2); break;
  case 0xDB: SET(state->registers.E, 3); op(2, 2); break;
  case 0xE3: SET(state->registers.E, 4); op(2, 2); break;
  case 0xEB: SET(state->registers.E, 5); op(2, 2); break;
  case 0xF3: SET(state->registers.E, 6); op(2, 2); break;
  case 0xFB: SET(state->registers.E, 7); op(2, 2); break;
  case 0xC4: SET(state->registers.H, 0); op(2, 2); break;
  case 0xCC: SET(state->registers.H, 1); op(2, 2); break;
  case 0xD4: SET(state->registers.H, 2); op(2, 2); break;
  case 0xDC: SET(state->registers.H, 3); op(2, 2); break;
  case 0xE4: SET(state->registers.H, 4); op(2, 2); break;
  case 0xEC: SET(state->registers.H, 5); op(2, 2); break;
  case 0xF4: SET(state->registers.H, 6); op(2, 2); break;
  case 0xFC: SET(state->registers.H, 7); op(2, 2); break;
  case 0xC5: SET(state->registers.L, 0); op(2, 2); break;
  case 0xCD: SET(state->registers.L, 1); op(2, 2); break;
  case 0xD5: SET(state->registers.L, 2); op(2, 2); break;
  case 0xDD: SET(state->registers.L, 3); op(2, 2); break;
  case 0xE5: SET(state->registers.L, 4); op(2, 2); break;
  case 0xED: SET(state->registers.L, 5); op(2, 2); break;
  case 0xF5: SET(state->registers.L, 6); op(2, 2); break;
  case 0xFD: SET(state->registers.L, 7); op(2, 2); break;
  case 0xC6: SET(Pair(state->registers.H, state->registers.L).address(), 0); op(2, 4); break;
  case 0xCE: SET(Pair(state->registers.H, state->registers.L).address(), 1); op(2, 4); break;
  case 0xD6: SET(Pair(state->registers.H, state->registers.L).address(), 2); op(2, 4); break;
  case 0xDE: SET(Pair(state->registers.H, state->registers.L).address(), 3); op(2, 4); break;
  case 0xE6: SET(Pair(state->registers.H, state->registers.L).address(), 4); op(2, 4); break;
  case 0xEE: SET(Pair(state->registers.H, state->registers.L).address(), 5); op(2, 4); break;
  case 0xF6: SET(Pair(state->registers.H, state->registers.L).address(), 6); op(2, 4); break;
  case 0xFE: SET(Pair(state->registers.H, state->registers.L).address(), 7); op(2, 4); break;

  case 0x87: RES(state->registers.A, 0); op(2, 2); break;
  case 0x8F: RES(state->registers.A, 1); op(2, 2); break;
  case 0x97: RES(state->registers.A, 2); op(2, 2); break;
  case 0x9F: RES(state->registers.A, 3); op(2, 2); break;
  case 0xA7: RES(state->registers.A, 4); op(2, 2); break;
  case 0xAF: RES(state->registers.A, 5); op(2, 2); break;
  case 0xB7: RES(state->registers.A, 6); op(2, 2); break;
  case 0xBF: RES(state->registers.A, 7); op(2, 2); break;
  case 0x80: RES(state->registers.B, 0); op(2, 2); break;
  case 0x88: RES(state->registers.B, 1); op(2, 2); break;
  case 0x90: RES(state->registers.B, 2); op(2, 2); break;
  case 0x98: RES(state->registers.B, 3); op(2, 2); break;
  case 0xA0: RES(state->registers.B, 4); op(2, 2); break;
  case 0xA8: RES(state->registers.B, 5); op(2, 2); break;
  case 0xB0: RES(state->registers.B, 6); op(2, 2); break;
  case 0xB8: RES(state->registers.B, 7); op(2, 2); break;
  case 0x81: RES(state->registers.C, 0); op(2, 2); break;
  case 0x89: RES(state->registers.C, 1); op(2, 2); break;
  case 0x91: RES(state->registers.C, 2); op(2, 2); break;
  case 0x99: RES(state->registers.C, 3); op(2, 2); break;
  case 0xA1: RES(state->registers.C, 4); op(2, 2); break;
  case 0xA9: RES(state->registers.C, 5); op(2, 2); break;
  case 0xB1: RES(state->registers.C, 6); op(2, 2); break;
  case 0xB9: RES(state->registers.C, 7); op(2, 2); break;
  case 0x82: RES(state->registers.D, 0); op(2, 2); break;
  case 0x8A: RES(state->registers.D, 1); op(2, 2); break;
  case 0x92: RES(state->registers.D, 2); op(2, 2); break;
  case 0x9A: RES(state->registers.D, 3); op(2, 2); break;
  case 0xA2: RES(state->registers.D, 4); op(2, 2); break;
  case 0xAA: RES(state->registers.D, 5); op(2, 2); break;
  case 0xB2: RES(state->registers.D, 6); op(2, 2); break;
  case 0xBA: RES(state->registers.D, 7); op(2, 2); break;
  case 0x83: RES(state->registers.E, 0); op(2, 2); break;
  case 0x8B: RES(state->registers.E, 1); op(2, 2); break;
  case 0x93: RES(state->registers.E, 2); op(2, 2); break;
  case 0x9B: RES(state->registers.E, 3); op(2, 2); break;
  case 0xA3: RES(state->registers.E, 4); op(2, 2); break;
  case 0xAB: RES(state->registers.E, 5); op(2, 2); break;
  case 0xB3: RES(state->registers.E, 6); op(2, 2); break;
  case 0xBB: RES(state->registers.E, 7); op(2, 2); break;
  case 0x84: RES(state->registers.H, 0); op(2, 2); break;
  case 0x8C: RES(state->registers.H, 1); op(2, 2); break;
  case 0x94: RES(state->registers.H, 2); op(2, 2); break;
  case 0x9C: RES(state->registers.H, 3); op(2, 2); break;
  case 0xA4: RES(state->registers.H, 4); op(2, 2); break;
  case 0xAC: RES(state->registers.H, 5); op(2, 2); break;
  case 0xB4: RES(state->registers.H, 6); op(2, 2); break;
  case 0xBC: RES(state->registers.H, 7); op(2, 2); break;
  case 0x85: RES(state->registers.L, 0); op(2, 2); break;
  case 0x8D: RES(state->registers.L, 1); op(2, 2); break;
  case 0x95: RES(state->registers.L, 2); op(2, 2); break;
  case 0x9D: RES(state->registers.L, 3); op(2, 2); break;
  case 0xA5: RES(state->registers.L, 4); op(2, 2); break;
  case 0xAD: RES(state->registers.L, 5); op(2, 2); break;
  case 0xB5: RES(state->registers.L, 6); op(2, 2); break;
  case 0xBD: RES(state->registers.L, 7); op(2, 2); break;
  case 0x86: RES(Pair(state->registers.H, state->registers.L).address(), 0); op(2, 4); break;
  case 0x8E: RES(Pair(state->registers.H, state->registers.L).address(), 1); op(2, 4); break;
  case 0x96: RES(Pair(state->registers.H, state->registers.L).address(), 2); op(2, 4); break;
  case 0x9E: RES(Pair(state->registers.H, state->registers.L).address(), 3); op(2, 4); break;
  case 0xA6: RES(Pair(state->registers.H, state->registers.L).address(), 4); op(2, 4); break;
  case 0xAE: RES(Pair(state->registers.H, state->registers.L).address(), 5); op(2, 4); break;
  case 0xB6: RES(Pair(state->registers.H, state->registers.L).address(), 6); op(2, 4); break;
  case 0xBE: RES(Pair(state->registers.H, state->registers.L).address(), 7); op(2, 4); break;
  }
}

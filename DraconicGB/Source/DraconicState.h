#pragma once
#include "Hardware/DraconicMemory.h"

using reg16_t = uint16_t;
using reg8_t = uint8_t;
using address16_t = uint16_t;

// Struct to store the value of the different registers
struct Registers
{
  struct {
    union {
      struct {
        reg8_t F;
        reg8_t A;
      };
      reg16_t AF;
    };
  };
  struct {
    union {
      struct {
        reg8_t C;
        reg8_t B;
      };
      reg16_t BC;
    };
  };
  struct {
    union {
      struct {
        reg8_t E;
        reg8_t D;
      };
      reg16_t DE;
    };
  };
  struct {
    union {
      struct {
        reg8_t L;
        reg8_t H;
      };
      reg16_t HL;
    };
  };
  
  reg16_t SP = 0; // Stack Pointer
  reg16_t PC = 0x100; // Program Counter

  void Reset()
  {
    A = 0;
    B = 0;
    C = 0;
    D = 0;
    E = 0;
    H = 0;
    L = 0;
    F = 0;
    SP = 0;
    PC = 0x100;
  }
};

// State of the application, in this case this means the Memory and the CPU registers
class DraconicState
{
public:
  Registers registers;
  DraconicMemory memory;
  int numCycles = 0;

public:
  // Functions
  void SetFlag(int flag, bool value);
  void ParseOpcode(uint8_t opCode);
  void ParseOpcodeDeprecated(uint8_t opCode);
  void ParseBitOperation(uint8_t op);
  void SetCPU(class CPU* _DraconicCPU);
  CPU* DraconicCPU;

private:

  // 8-bit Arithmetic and Logic Instructions -----------------------------------

  void ADC(uint8_t& target, uint8_t value);
  void ADC_A_R8(uint8_t value);
  void ADC_A_HL();
  void ADC_A_N8(uint8_t value);

  void ADD(uint8_t& target, uint8_t value);
  void ADD_A_R8(uint8_t value);
  void ADD_A_HL();
  void ADD_A_N8(uint8_t value);

  void AND(uint8_t& target, uint8_t value);
  void AND_A_R8(uint8_t value);
  void AND_A_HL();
  void AND_A_N8(uint8_t value);

  void CP(uint8_t& target, uint8_t value);
  void CP_A_R8(uint8_t value);
  void CP_A_HL();
  void CP_A_N8(uint8_t value);

  void DEC_R8(uint8_t& target);
  void DEC_HL();

  void INC_R8(uint8_t& target);
  void INC_HL();

  void OR(uint8_t& target, uint8_t value);
  void OR_A_R8(uint8_t value);
  void OR_A_HL();
  void OR_A_N8(uint8_t value);

  void SBC(uint8_t& target, uint8_t value);
  void SBC_A_R8(uint8_t value);
  void SBC_A_HL();
  void SBC_A_N8(uint8_t value);

  void SUB(uint8_t& target, uint8_t value);
  void SUB_A_R8(uint8_t value);
  void SUB_A_HL();
  void SUB_A_N8(uint8_t value);

  void XOR(uint8_t& target, uint8_t value);
  void XOR_A_R8(uint8_t value);
  void XOR_A_HL();
  void XOR_A_N8(uint8_t value);


  // 16-bit Arithmetic Instructions ---------------------------------------

  void ADD_HL_R16(uint16_t value);
  void DEC_R16(uint16_t& target);
  void INC_R16(uint16_t& target);


  // Bit Operations Instructions ---------------------------------------

  void BIT_U3_R8(uint8_t value, uint8_t bit);
  void BIT_U3_HL(uint8_t bit);

  void RES_U3_R8(uint8_t& target, uint8_t value);
  void RES_U3_HL(uint8_t value);

  void SET_U3_R8(uint8_t& target, uint8_t value);
  void SET_U3_HL(uint8_t value);

  void SWAP_R8(uint8_t& target);
  void SWAP_HL();


  // Bit Shift Instructions ---------------------------------------

  void RL_R8(uint8_t& target);
  void RL_HL();
  void RLA();

  void RLC_R8(uint8_t& target);
  void RLC_HL();
  void RL(uint8_t& target, bool carry, bool zero_flag = false);
  void RLCA();

  void RR_R8(uint8_t& target);
  void RR_HL();
  void RR_A();

  void RRC_R8(uint8_t& target);
  void RRC_HL();
  void RR(uint8_t& target, bool carry, bool zero_flag = false);
  void RRC_A();

  void SLA_R8(uint8_t& target);
  void SLA_HL();

  void SRA_R8(uint8_t& target);
  void SRA_HL();

  void SRL_R8(uint8_t& target);
  void SRL_HL();


  // Load Instructions ---------------------------------------

  void LD_R8_R8(uint8_t& target, uint8_t value);
  void LD_R8_N8(uint8_t& target, uint8_t value);
  void LD_R16_N16(uint16_t& target, uint16_t value);
  void LD_HL_R8(uint8_t value);
  void LD_HL_N8(uint8_t value);
  void LD_R8_HL(uint8_t& target);
  void LD_R16_A(uint16_t addr);
  void LD_N16_A(uint16_t addr);

  void LDH_N16_A(uint16_t addr);
  void LDH_C_A();

  void LD_A_R16(uint16_t addr);
  void LD_A_N16(uint16_t addr);

  void LDH_A_N16(uint16_t addr);
  void LDH_A_C();

  void LD_HLI_A();
  void LD_HLD_A();

  void LD_A_HLI();
  void LD_A_HLD();
  

  void CALL(uint16_t addr);
  // Jumps and Subroutines ---------------------------------------

  void CALL_N16(uint16_t addr);
  void CALLNZ_N16(uint16_t addr);
  void CALLZ_N16(uint16_t addr);
  void CALLNC_N16(uint16_t addr);
  void CALLC_N16(uint16_t addr);

  

  void JP(uint16_t target);
  void JP_HL();
  void JP_N16(uint16_t addr);
  void JPNZ_N16(uint16_t target);
  void JPZ_N16(uint16_t target);
  void JPNC_N16(uint16_t target);
  void JPC_N16(uint16_t target);
  void JR(uint8_t value);
  void JR_N8(uint8_t value);

  void JRNZ_N8(uint8_t value);
  void JRZ_N8(uint8_t value);
  void JRNC_N8(uint8_t value);
  void JRC_N8(uint8_t value);
  void JP_CC_N16(uint8_t cc, uint16_t addr);
  
  void JR_E8(uint8_t offset);
  void JR_CC_E8(uint8_t cc, uint8_t offset);

  void RET();
  void RETI();
  void RETNZ();
  void RETZ();
  void RETNC();
  void RETC();
  void RET_CC(uint8_t cc);
  void RET_Impl();
  void RST_VEC(uint8_t vec);


  // Stack Operations Instructions ---------------------------------------

  void ADD_HL_SP();
  void ADD_SP_E8(uint8_t offset);

  void DEC_SP();
  void INC_SP();
  
  void LD_SP_N16(uint16_t value);
  void LD_N16_SP(uint16_t addr);
  void LD_HL_SP_E8(uint8_t offset);
  void LD_SP_HL();
  
  void POP_AF();
  void POP_R16(uint16_t& value);

  void PUSH_AF();
  void PUSH_R16(uint16_t value);


  // Miscellaneous Instructions ---------------------------------------

  void CCF();
  void CPL();
  void DAA();
  void DI();
  void EI();
  void HALT();
  void NOP();
  void SCF();
  void STOP();



};

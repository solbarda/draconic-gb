//#pragma once
//#include "SDL_config.h"
//#include "../../ThirdParty/nlohmann/json.hpp"
//#include "CPU.h"
//#include "DraconicState.h"
//
//
//enum class EOperationType {
//  VOID,
//  R8_R8,
//  R8_ADDR,
//  R8,
//  ADDR,
//  R8_N,
//  R16_R16,
//  R16,
//  ADDR_R8,
//  ADDR_Value,
//  R16_N
//};
//
//
//class OpCode
//{
//public:
//  uint8_t id;
//  uint8_t numOperands;
//  uint8_t* operandA8 = nullptr;
//  uint8_t* operandB8 = nullptr;
//  uint16_t* operandA16 = nullptr;
//  uint16_t* operandB16 = nullptr;
//  void* code;
//  EOperationType OperationType;
//
//  void Execute(DraconicState* state);
//};
//
//void testINC(uint8_t& target);
//void testINC8(uint8_t& target);
//void testINCMem(uint16_t addr);
//void testNOP();
//void testArguments(uint8_t A, uint8_t B, uint8_t C, uint8_t D);
//void TestADD_HL_SP(uint16_t& HL, uint16_t SP);
//void TestADD_HL_r16(uint16_t& HL, uint16_t r16);
//void TestADD_A_HL(uint8_t& HL, uint16_t HL);
//
//void TestADD(uint8_t& reg, uint8_t value);
//void TestADD(uint16_t& reg, uint16_t value);
//void TestADD(uint8_t& reg, DraconicAddress addr);
//
//bool GetRegisterReference(DraconicState* state, std::string registerName, uint8_t* out_reg);
//bool GetRegisterReference(DraconicState* state, std::string registerName, uint16_t* out_reg);
//
//void TestOpcodes();

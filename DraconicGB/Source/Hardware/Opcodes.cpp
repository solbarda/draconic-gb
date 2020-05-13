#pragma once
#include "Opcodes.h"
#include "DraconicState.h"
#include <iostream>
#include <fstream>
#include "../ThirdParty/nlohmann/json.hpp"
#include <bitset>


void testINC(uint8_t& target) {

}
void testINC8(uint8_t& target) {

}

void testINCMem(uint16_t addr) {

}

void testNOP()
{

}

void testArguments(uint8_t A, uint8_t B, uint8_t C, uint8_t D)
{
  std::cout << "testArguments A: " << std::bitset<8>(A) << " B: " << std::bitset<8>(B) << " C: " << std::bitset<8>(C) << " D: " << std::bitset<8>(D) << std::endl;
}

void TestADD_HL_SP(uint16_t& HL, uint16_t r16)
{

}

void TestADD_HL_r16(uint16_t& HL, uint16_t r16)
{

}

void TestADD_A_HL(uint8_t& HL, uint16_t HL)
{

}

void TestADD(uint8_t& reg, uint8_t value)
{

}

void TestADD(uint16_t& reg, uint16_t value)
{

}

void TestADD(uint8_t& reg, DraconicAddress addr)
{

}

uint16_t& GetRegisterReference(std::string registerName)
{

}

bool GetRegisterReference(DraconicState* state, std::string registerName, uint8_t* out_reg)
{
  if (registerName == "A")
  {
    out_reg = &state->registers.A;
  }
  else if (registerName == "B");
  {
    out_reg = &state->registers.B;
  }
  else if (registerName == "C");
  {
    out_reg = &state->registers.C;
  }
  else if (registerName == "D");
  {
    out_reg = &state->registers.D;
  }
  else if (registerName == "E");
  {
    out_reg = &state->registers.E;
  }
  else if (registerName == "F");
  {
    out_reg = &state->registers.F;
  }
  else if (registerName == "L");
  {
    out_reg = &state->registers.L;
  }
  else if (registerName == "H");
  {
    out_reg = &state->registers.H;
  }
  else
  {
    return false;
  }

  return true;
}

bool GetRegisterReference(DraconicState* state, std::string registerName, uint16_t* out_reg)
{
  if (registerName == "PC")
  {
    out_reg = &state->registers.PC;
  }
  else if (registerName == "SP");
  {
    out_reg = &state->registers.SP;
  }
  else if (registerName == "AF");
  {
    out_reg = &state->registers.AF;
  }
  else if (registerName == "BC");
  {
    out_reg = &state->registers.BC;
  }
  else if (registerName == "DE");
  {
    out_reg = &state->registers.DE;
  }
  else if (registerName == "HL");
  {
    out_reg = &state->registers.HL;
  }
  else
  {
    return false;
  }

  return true;

}

void TestOpcodes()
{

  std::ifstream input_file("./Source/Opcodes.json");
  nlohmann::json opcodes_json;
  input_file >> opcodes_json;

  //std::cout << opcodes_json["unprefixed"]["0x00"].dump(4);

  uint8_t PC = 0;
  int totalCycles = 0;

  std::cout << "Start TestOpcodes" << std::endl;
  std::cout << "PC: " << (int)PC << " totalCycles: " << (int)totalCycles << std::endl;


  //std::cout << "  Running OpCode 0x00  ";
  //auto currentOpCode = opcodes_json["unprefixed"]["0x00"];
  //uint8_t bytes = currentOpCode["bytes"];
  //uint8_t cycles = currentOpCode["cycles"][0];



  DraconicState state;
  /*uint8_t valueA8 = state->memory.Read(state->registers.PC + 1);
  uint8_t valueD8 = state->memory.Read(state->registers.PC + 2);
  uint16_t valueD16 = valueD8 << 8 | valueA8;*/

  uint8_t valueA8 = 1;
  uint8_t valueD8 = 2;
  uint16_t valueD16 = valueD8 << 8 | valueA8;


  std::cout << "  Running OpCode 0x09  ";
  auto currentOpCode = opcodes_json["unprefixed"]["0x09"];
  uint8_t bytes = currentOpCode["bytes"];
  uint8_t cycles = currentOpCode["cycles"][0];
  std::cout << "bytes: " << (int)bytes << " cycles: " << (int)cycles << std::endl;



  


  uint8_t numOperandBytes = 0;
  uint32_t operandsToUse = 0;

  std::string operandAName = "";
  std::string operandBName = "";

  uint8_t* operandA8 = nullptr;
  uint8_t* operandB8 = nullptr;
  uint16_t* operandA16 = nullptr;
  uint16_t* operandB16 = nullptr;


  for (auto operand : currentOpCode["operands"])
  {
    if (operandAName.empty())
    {
      operandAName = operand["name"];
      GetRegisterReference(operandAName, operandA8);
      GetRegisterReference(operandAName, operandA16);
    }
    else
    {
      operandBName = operand["name"];
      GetRegisterReference(operandAName, operandB8);
      GetRegisterReference(operandAName, operandB16);
    }
  }

  if (currentOpCode["mnemonic"] == "ADD") {
    if (operandBName == "HL")
    {
    }
    else
    {
      TestADD(Ope)
    }


      if (operandB == "SP")
      {
        TestADD_HL_SP(state.registers.HL, state.registers.SP)
      }
      else
      {
        TestADD_HL_r16(state.registers.HL, state.registers.BC)
      }
    }
    else if (operandB == "HL")
    {
      TestADD_A_HL(state.registers.A, state.registers.HL);
    }
  }

  


  PC += bytes;
  totalCycles += cycles;
  std::cout << "Final Operands: " << std::bitset<32>(operandsToUse) << " operands Bytes: " << numOperandBytes << std::endl;
  //void (*testArguments_ptr)(uint32_t) = reinterpret_cast<void (*)(uint32_t)> (testArguments);
  void (*testArguments_ptr)(uint8_t,uint8_t, uint8_t, uint8_t) = reinterpret_cast<void (*)(uint8_t, uint8_t, uint8_t, uint8_t)> (testArguments);
  testArguments_ptr((uint8_t)operandsToUse, (uint8_t)(operandsToUse >> 8), (uint8_t)(operandsToUse >> 16), (uint8_t)(operandsToUse >> 24));
  std::cout << "PC: " << (int)PC << " totalCycles: " << (int)totalCycles << std::endl;



 
  OpCode inc{ 0,1, testINC };

  /*uint8_t value8Low = state.memory.Read(state.registers.PC + 1);
  uint8_t value8High = state.memory.Read(state.registers.PC + 2);
  uint16_t value16LowHigh = value8Low << 8 | value8High;
  uint16_t value16HighLow = value8High << 8 | value8Low;*/

  //inc.code();
}

void OpCode::Execute(DraconicState* state)
{

}

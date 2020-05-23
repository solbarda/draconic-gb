#pragma once
#include <vector>
#include "SDL_config.h"
#include <string>
#include "DraconicCartridge.h"

// Addresses of interesting memory locations
const int Addr_WRAM = 0x0000;
const int Addr_ZRAM = 0x2000;
const int Addr_VRAM = 0x2100;
const int Addr_OAM = 0x4100;
const int Addr_P1 = Addr_ZRAM + 0x00;
const int Addr_DIV = Addr_ZRAM + 0x04;
const int Addr_TIMA = Addr_ZRAM + 0x05;
const int Addr_TMA = Addr_ZRAM + 0x06;
const int Addr_TAC = Addr_ZRAM + 0x07;
const int Addr_LCDC = Addr_ZRAM + 0x40;
const int Addr_STAT = Addr_ZRAM + 0x41;
const int Addr_SCY = Addr_ZRAM + 0x42;
const int Addr_SCX = Addr_ZRAM + 0x43;
const int Addr_LY = Addr_ZRAM + 0x44;
const int Addr_LYC = Addr_ZRAM + 0x45;
const int Addr_DMA = Addr_ZRAM + 0x46;
const int Addr_BGP = Addr_ZRAM + 0x47;
const int Addr_OBP0 = Addr_ZRAM + 0x48;
const int Addr_OBP1 = Addr_ZRAM + 0x49;
const int Addr_WY = Addr_ZRAM + 0x4A;
const int Addr_WX = Addr_ZRAM + 0x4B;
const int Addr_IF = Addr_ZRAM + 0x0F;
const int Addr_IE = Addr_ZRAM + 0xFF;

// Size of the different memory regions
const int Size_WRAM = 0x2000;
const int Size_ZRAM = 0x0100;
const int Size_VRAM = 0x2000;
const int Size_OAM = 0x0100;
const int Size_Memory = Size_WRAM + Size_ZRAM + Size_VRAM + Size_OAM;

// Represents the RAM of the gameboy
class DraconicMemory
{
public:
  DraconicMemory();

  // Reset the memory
  void Reset();

  // Store the memory on a vector of bytes
  std::vector<uint8_t> memoryData;

  // Helpful functions to access memory locations
  uint8_t* GetVRAM();
  uint8_t* GetOAM();
  uint8_t* GetWRAM();
  uint8_t* GetZRAM();
  // Get pointer to a memory location direclty
  uint8_t* GetMemoryLocation(int location);
  // Get data at a memory location directly
  uint8_t  GetMemoryLocationData(int location);

  // Cartridge data and ROM
  DraconicCartridge cartridge;

  // State of joypad buttons and joypad arrows
  uint8_t joypadButtons;
  uint8_t joypadArrows;

public:
  // Perform a direct memory transfer
  void PerformDMATransfer();
  // Get the state of the gameboy buttons
  uint8_t GetJoypadState();
  // Load a ROM into RAM and cartridge
  void LoadROM(std::string location);
  // Read a memory location using memory controllers
  uint8_t Read(uint16_t location);
  // Write a memory location using memory controllers
  void Write(uint16_t location, uint8_t data);
  // Write zero page t o location
  void WriteZeroPage(uint16_t location, uint8_t data);
  // Name of the ROM
  std::string romName;

};


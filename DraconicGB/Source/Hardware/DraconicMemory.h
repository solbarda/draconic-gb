#pragma once
#include <vector>
#include "SDL_config.h"
#include <string>


enum class EMemoryManagerType
{
  DEFAULT,
  MBC1,
  MBC2,
  MBC3
};

class MemoryController
{
public:

  EMemoryManagerType MemoryType;
  // $0000 - $7FFF, 32kB Cartridge (potentially dynamic)
  std::vector<unsigned char> CART_ROM;
  // $A000 - $BFFF, 8kB Cartridge external switchable RAM bank
  std::vector<unsigned char> ERAM;

  bool RTC_enabled = false;

  // Bank selectors
  unsigned char ROM_bank_id = 1;
  unsigned char RAM_bank_id = 0;

  bool RAM_bank_enabled = false;
  bool RAM_access_enabled = false;

  // Mode selector
  unsigned char mode = 0;
  const unsigned char MODE_ROM = 0;
  const unsigned char MODE_RAM = 1;

public:
  unsigned char Read(uint16_t location);
  void Write(uint16_t location, unsigned char data);
  void Init(std::vector<unsigned char> cartridge_buffer);

  // Save states
  std::vector<unsigned char> GetRAM();
  void SetRAM(std::vector<unsigned char> data);
  void SaveState(std::ofstream& file);
  void LoadState(std::ifstream& file);
};


class DraconicMemory
{
public:
  DraconicMemory();
  void Reset();

//private:
  // Memory Regions
  std::vector<unsigned char> VRAM;		// $8000 - $9FFF, 8kB Video RAM
  std::vector<unsigned char> OAM;		  // $FE00 - $FEA0, OAM Sprite RAM
  std::vector<unsigned char> WRAM;		// $C000 - $DFFF, 8kB Working RAM
  std::vector<unsigned char> ZRAM;		// $FF80 - $FFFF, 128 unsigned chars of RAM

  MemoryController controller;

public:
  size_t GetTotalMemorySize();
  void LoadROM(std::string location);
  unsigned char Read(uint16_t location);
  void Write(uint16_t location, unsigned char data);
  void WriteZeroPage(uint16_t location, unsigned char data);
  std::string romName;
};


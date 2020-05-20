#pragma once
#include <vector>
#include <fstream>

// Different type of modes that the cartridges can be in
enum class EMemoryBankControllerType
{
  DEFAULT,
  MBC1,
  MBC2,
  MBC3
};

// Class that represents a game cartridge
class DraconicCartridge
{
public:

  EMemoryBankControllerType MemoryType;
  // $0000 - $7FFF, 32kB Cartridge (potentially dynamic)
  std::vector<uint8_t> CART_ROM;
  // $A000 - $BFFF, 8kB Cartridge external switchable RAM bank
  std::vector<uint8_t> ERAM;

  bool RTC_enabled = false;

  // Bank selectors
  uint8_t ROM_bank_id = 1;
  uint8_t RAM_bank_id = 0;

  bool RAM_bank_enabled = false;
  bool RAM_access_enabled = false;

  // Mode selector
  uint8_t mode = 0;
  const uint8_t MODE_ROM = 0;
  const uint8_t MODE_RAM = 1;

public:
  uint8_t Read(uint16_t location);
  void Write(uint16_t location, uint8_t data);
  void Init(std::vector<uint8_t> cartridge_buffer);

  // Save states
  std::vector<uint8_t> GetRAM();
  void SetRAM(std::vector<uint8_t> data);
  void SaveState(std::ofstream& file);
  void LoadState(std::ifstream& file);
};
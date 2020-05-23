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
  // ROM of the cartridge
  std::vector<uint8_t> CART_ROM;
  // External RAM of the cartridge
  std::vector<uint8_t> ERAM;

  bool RTC_enabled = false;

  // Bank selectors
  uint8_t ROMBankID = 1;
  uint8_t RAMBankID = 0;

  bool RAMBankEnabled = false;
  bool RAMAccessEnabled = false;

  // Mode selector
  uint8_t mode = 0;
  const uint8_t MODE_ROM = 0;
  const uint8_t MODE_RAM = 1;

public:
  uint8_t Read(uint16_t location);
  void Write(uint16_t location, uint8_t data);
  void Init(std::vector<uint8_t> cartridge_buffer);

};
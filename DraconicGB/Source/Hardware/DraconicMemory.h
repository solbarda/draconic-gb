#pragma once
#include <vector>
#include "SDL_config.h"
#include <string>
#include "DraconicCartridge.h"
#include "SpecialRegister.h"



class DraconicMemory
{
public:
  DraconicMemory();
  void Reset();

  std::vector<uint8_t> VRAM;		// $8000 - $9FFF, 8kB Video RAM
  std::vector<uint8_t> OAM;		  // $FE00 - $FEA0, OAM Sprite RAM
  std::vector<uint8_t> WRAM;		// $C000 - $DFFF, 8kB Working RAM
  std::vector<uint8_t> ZRAM;		// $FF80 - $FFFF, 128 unsigned chars of RAM

  // Specialized registers for things such as timers, LCD, Sounc, etc

  SpecialRegister P1;
  SpecialRegister DIV;
  SpecialRegister TIMA;
  SpecialRegister TMA;
  SpecialRegister TAC;

  SpecialRegister LCDC;
  SpecialRegister STAT;
  SpecialRegister SCY;
  SpecialRegister SCX;
  SpecialRegister LYC;
  SpecialRegister LY;
  SpecialRegister DMA;

  SpecialRegister BGP;
  SpecialRegister OBP0;
  SpecialRegister OBP1;
  SpecialRegister WY;
  SpecialRegister WX;

  SpecialRegister IF;
  SpecialRegister IE;

  // Cartridge data and ROM
  DraconicCartridge cartridge;

  uint8_t video_mode;
  uint8_t joypad_buttons;
  uint8_t joypad_arrows;


public:
  void PerformDMATransfer();
  uint8_t GetJoypadState();
  size_t GetTotalMemorySize();
  void LoadROM(std::string location);
  uint8_t Read(uint16_t location);
  void Write(uint16_t location, uint8_t data);
  void WriteZeroPage(uint16_t location, uint8_t data);
  std::string romName;

  void write_vector(std::ofstream& file, std::vector<uint8_t>& vec);
  void load_vector(std::ifstream& file, std::vector<uint8_t>& vec);
  void save_state(std::ofstream& file);
  void load_state(std::ifstream& file);
};


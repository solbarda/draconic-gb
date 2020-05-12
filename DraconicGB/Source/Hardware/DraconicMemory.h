#pragma once
#include <vector>
#include "SDL_config.h"
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
  std::vector<unsigned char> ZRAM;		// $FF80 - $FFFF, 128 bytes of RAM

public:
	unsigned char Read(unsigned char PC);
  size_t GetTotalMemorySize();
};


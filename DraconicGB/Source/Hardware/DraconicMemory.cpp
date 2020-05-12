#include "DraconicMemory.h"
#include <xutility>


DraconicMemory::DraconicMemory()
{
  WRAM = std::vector<unsigned char>(0x2000); // $C000 - $DFFF, 8kB Working RAM
  ZRAM = std::vector<unsigned char>(0x0100); // $FF00 - $FFFF, 256 bytes of RAM
  VRAM = std::vector<unsigned char>(0x2000); // $8000 - $9FFF, 8kB Video RAM
  OAM = std::vector<unsigned char>(0x0100); // $FE00 - $FEFF, OAM Sprite RAM, IO RAM

  //// Initialize Memory Register objects for easy reference
  //P1 = MemoryRegister(&ZRAM[0x00]);
  //DIV = MemoryRegister(&ZRAM[0x04]);
  //TIMA = MemoryRegister(&ZRAM[0x05]);
  //TMA = MemoryRegister(&ZRAM[0x06]);
  //TAC = MemoryRegister(&ZRAM[0x07]);
  //LCDC = MemoryRegister(&ZRAM[0x40]);
  //STAT = MemoryRegister(&ZRAM[0x41]);
  //SCY = MemoryRegister(&ZRAM[0x42]);
  //SCX = MemoryRegister(&ZRAM[0x43]);
  //LY = MemoryRegister(&ZRAM[0x44]);
  //LYC = MemoryRegister(&ZRAM[0x45]);
  //DMA = MemoryRegister(&ZRAM[0x46]);
  //BGP = MemoryRegister(&ZRAM[0x47]);
  //OBP0 = MemoryRegister(&ZRAM[0x48]);
  //OBP1 = MemoryRegister(&ZRAM[0x49]);
  //WY = MemoryRegister(&ZRAM[0x4A]);
  //WX = MemoryRegister(&ZRAM[0x4B]);
  //IF = MemoryRegister(&ZRAM[0x0F]);
  //IE = MemoryRegister(&ZRAM[0xFF]);

  Reset();
}

void DraconicMemory::Reset()
{
  fill(WRAM.begin(), WRAM.end(), 0);
  fill(ZRAM.begin(), ZRAM.end(), 0);
  fill(VRAM.begin(), VRAM.end(), 0);
  fill(OAM.begin(), OAM.end(), 0);

  //// The following memory locations are set to the following values after gameboy BIOS runs
  //P1.set(0x00);
  //DIV.set(0x00);
  //TIMA.set(0x00);
  //TMA.set(0x00);
  //TAC.set(0x00);
  //LCDC.set(0x91);
  //SCY.set(0x00);
  //SCX.set(0x00);
  //LYC.set(0x00);
  //BGP.set(0xFC);
  //OBP0.set(0xFF);
  //OBP1.set(0xFF);
  //WY.set(0x00);
  //WX.set(0x00);
  //IF.set(0x00);
  //IE.set(0x00);

  //// Initialize input to HIGH state (unpressed)
  //joypad_buttons = 0xF;
  //joypad_arrows = 0xF;
}

unsigned char DraconicMemory::Read(unsigned char PC)
{
  return 0;
}

size_t DraconicMemory::GetTotalMemorySize()
{
  size_t memorySize = 0;
  memorySize += OAM.size() * sizeof(unsigned char);
  memorySize += VRAM.size() * sizeof(unsigned char);
  memorySize += WRAM.size() * sizeof(unsigned char);
  memorySize += ZRAM.size() * sizeof(unsigned char);
  return memorySize;
}

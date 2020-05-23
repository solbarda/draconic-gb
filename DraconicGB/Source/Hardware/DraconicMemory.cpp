#include "DraconicMemory.h"
#include <xutility>
#include <iosfwd>
#include <iostream>
#include <fstream>

// Memory implementation based on mattbruv code


DraconicMemory::DraconicMemory()
{
  // Initialize memory to the total size of the memory
  memoryData = std::vector<uint8_t>(Size_Memory);
  // Reset the memory
  Reset();
}

void DraconicMemory::Reset()
{
  joypadButtons = 0xF;
  joypadArrows = 0xF;

  fill(memoryData.begin(), memoryData.end(), 0);
}


uint8_t* DraconicMemory::GetVRAM()
{
  return &(memoryData[Addr_VRAM]);
}

uint8_t* DraconicMemory::GetOAM()
{
  return &(memoryData[Addr_OAM]);

}

uint8_t* DraconicMemory::GetWRAM()
{
  return &(memoryData[Addr_WRAM]);

}

uint8_t* DraconicMemory::GetZRAM()
{
  return &(memoryData[Addr_ZRAM]);
}

uint8_t* DraconicMemory::GetMemoryLocation(int location)
{
  return &(memoryData[location]);
}

uint8_t DraconicMemory::GetMemoryLocationData(int location)
{
  return memoryData[location];
}

void DraconicMemory::PerformDMATransfer()
{
  uint16_t  address = (*GetMemoryLocation(Addr_DMA)) << 8;
  for (int i = 0; i < 0xA0; i++)
  {
    Write((0xFE00 + i), Read(address + i));
  }
}

unsigned char DraconicMemory::GetJoypadState()
{
  // The joypad state that we returns depends on the value stored at Addr_P1
  unsigned char joypadTypeRequest = *(GetMemoryLocation(Addr_P1));
  switch (joypadTypeRequest)
  {
  case 0x10:
    return joypadButtons;
  case 0x20:
    return joypadArrows;
  default:
    return 0xFF;
  }
}

void DraconicMemory::LoadROM(std::string location)
{
  std::ifstream input(location, std::ios::binary);
  std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
  // Get the title so we can print it in the debug window
  std::string title = "";
  for (int i = 0x0134; i <= 0x142; i++)
  {
    unsigned char character = buffer[i];
    if (character == 0)
      break;
    else
      title.push_back(tolower(character));
  }
  romName = title;

  unsigned char cartridgeMode = buffer[0x0147];

  // Assign memory controller based on cartridge specification
  switch (cartridgeMode)
  {
  case 0x01:
  case 0x02:
  case 0x03:
    cartridge.MemoryType = EMemoryBankControllerType::MBC1;
    break;
  case 0x05:
  case 0x06:
    cartridge.MemoryType = EMemoryBankControllerType::MBC2;
    break;
  case 0x0F:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
    cartridge.MemoryType = EMemoryBankControllerType::MBC3;
    break;
  default:
    cartridge.MemoryType = EMemoryBankControllerType::DEFAULT;
    break;
  }

  // Initialize controller with cartridge data
  cartridge.Init(buffer);

  input.close();
}



unsigned char DraconicMemory::Read(uint16_t location)
{

  switch (location & 0xF000)
  {
    // ROM
  case 0x0000:
  case 0x1000:
  case 0x2000:
  case 0x3000:
  case 0x4000:
  case 0x5000:
  case 0x6000:
  case 0x7000:
    return cartridge.Read(location);

    // Graphics VRAM
  case 0x8000:
  case 0x9000:
    return GetVRAM()[location & 0x1FFF];

    // External RAM
  case 0xA000:
  case 0xB000:
    return cartridge.Read(location);

    // Working RAM (8kB) and RAM Shadow
  case 0xC000:
  case 0xD000:
  case 0xE000:
    return GetWRAM()[location & 0x1FFF];

    // Remaining Working RAM Shadow, I/O, Zero page RAM
  case 0xF000:
    switch (location & 0x0F00)
    {
      // Remaining Working RAM
    case 0x000: case 0x100: case 0x200: case 0x300:
    case 0x400: case 0x500: case 0x600: case 0x700:
    case 0x800: case 0x900: case 0xA00: case 0xB00:
    case 0xC00: case 0xD00:
      return GetWRAM()[location & 0x1FFF];

      // Sprite OAM
    case 0xE00:
      return GetOAM()[location & 0xFF];

    case 0xF00:
      if (location == 0xFF00)
      {
        return GetJoypadState();

      }
      else 
      {
        return GetZRAM()[location & 0xFF];
      }
        
    }
  default:
    return 0xFF;
  }
}

void DraconicMemory::Write(uint16_t location, uint8_t data)
{
  switch (location & 0xF000)
  {
    // ROM
  case 0x0000:
  case 0x1000:
  case 0x2000:
  case 0x3000:
  case 0x4000:
  case 0x5000:
  case 0x6000:
  case 0x7000:
    cartridge.Write(location, data);
    break;

    // Graphics VRAM
  case 0x8000:
  case 0x9000:
    // Cannot write to VRAM during mode 3 
    GetVRAM()[location & 0x1FFF] = data;
    break;

    // External RAM
  case 0xA000:
  case 0xB000:
    cartridge.Write(location, data);
    break;

    // Working RAM (8kB) and RAM Shadow
  case 0xC000:
  case 0xD000:
  case 0xE000:
    GetWRAM()[location & 0x1FFF] = data;
    break;

    // Remaining Working RAM Shadow, I/O, Zero page RAM
  case 0xF000:
    switch (location & 0x0F00)
    {
      // Remaining Working RAM
    case 0x000: case 0x100: case 0x200: case 0x300:
    case 0x400: case 0x500: case 0x600: case 0x700:
    case 0x800: case 0x900: case 0xA00: case 0xB00:
    case 0xC00: case 0xD00:
      GetWRAM()[location & 0x1FFF] = data;
      break;

      // Sprite OAM
    case 0xE00:
      GetOAM()[location & 0xFF] = data;
      break;

    case 0xF00:
      WriteZeroPage(location, data);
      break;
    }
  }
}


void DraconicMemory::WriteZeroPage(uint16_t location, uint8_t data)
{
  switch (location)
  {
  case 0xFF00:
    GetZRAM()[0x00] = (data & 0x30);
    break;
  case 0xFF04:
    GetZRAM()[0x04] = 0;
    break;
  case 0xFF41:
    GetZRAM()[0x41] = (data & 0xFC) | (memoryData[Addr_STAT] & 0x03);
    break;
  case 0xFF44:
    GetZRAM()[0x44] = 0;
    break;
  case 0xFF46:
    GetZRAM()[0x46] = data;
    PerformDMATransfer();
    break;
  default:
    GetZRAM()[location & 0xFF] = data;
    break;
  }
}


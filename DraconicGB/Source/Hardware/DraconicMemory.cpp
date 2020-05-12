#include "DraconicMemory.h"
#include <xutility>
#include <iosfwd>
#include <iostream>
#include <fstream>


DraconicMemory::DraconicMemory()
{
  WRAM = std::vector<unsigned char>(0x2000); // $C000 - $DFFF, 8kB Working RAM
  ZRAM = std::vector<unsigned char>(0x0100); // $FF00 - $FFFF, 256 bytes of RAM
  VRAM = std::vector<unsigned char>(0x2000); // $8000 - $9FFF, 8kB Video RAM
  OAM = std::vector<unsigned char>(0x0100); // $FE00 - $FEFF, OAM Sprite RAM, IO RAM

  // Initialize Memory Register objects for easy reference
  P1 = SpecialRegister(&ZRAM[0x00]);
  DIV = SpecialRegister(&ZRAM[0x04]);
  TIMA = SpecialRegister(&ZRAM[0x05]);
  TMA = SpecialRegister(&ZRAM[0x06]);
  TAC = SpecialRegister(&ZRAM[0x07]);
  LCDC = SpecialRegister(&ZRAM[0x40]);
  STAT = SpecialRegister(&ZRAM[0x41]);
  SCY = SpecialRegister(&ZRAM[0x42]);
  SCX = SpecialRegister(&ZRAM[0x43]);
  LY = SpecialRegister(&ZRAM[0x44]);
  LYC = SpecialRegister(&ZRAM[0x45]);
  DMA = SpecialRegister(&ZRAM[0x46]);
  BGP = SpecialRegister(&ZRAM[0x47]);
  OBP0 = SpecialRegister(&ZRAM[0x48]);
  OBP1 = SpecialRegister(&ZRAM[0x49]);
  WY = SpecialRegister(&ZRAM[0x4A]);
  WX = SpecialRegister(&ZRAM[0x4B]);
  IF = SpecialRegister(&ZRAM[0x0F]);
  IE = SpecialRegister(&ZRAM[0xFF]);

  Reset();
}

void DraconicMemory::Reset()
{
  fill(WRAM.begin(), WRAM.end(), 0);
  fill(ZRAM.begin(), ZRAM.end(), 0);
  fill(VRAM.begin(), VRAM.end(), 0);
  fill(OAM.begin(), OAM.end(), 0);

  // The following memory locations are set to the following values after gameboy BIOS runs
  P1.set(0x00);
  DIV.set(0x00);
  TIMA.set(0x00);
  TMA.set(0x00);
  TAC.set(0x00);
  LCDC.set(0x91);
  SCY.set(0x00);
  SCX.set(0x00);
  LYC.set(0x00);
  BGP.set(0xFC);
  OBP0.set(0xFF);
  OBP1.set(0xFF);
  WY.set(0x00);
  WX.set(0x00);
  IF.set(0x00);
  IE.set(0x00);

  // Initialize input to HIGH state (unpressed)
  joypad_buttons = 0xF;
  joypad_arrows = 0xF;
}


void DraconicMemory::PerformDMATransfer()
{
  uint16_t  address = DMA.get() << 8; // multiply by 100

  for (int i = 0; i < 0xA0; i++)
  {
    Write((0xFE00 + i), Read(address + i));
  }
}

unsigned char DraconicMemory::GetJoypadState()
{
  unsigned char request = P1.get();

  switch (request)
  {
  case 0x10:
    return joypad_buttons;
  case 0x20:
    return joypad_arrows;
  default:
    return 0xFF;
  }
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

void DraconicMemory::LoadROM(std::string location)
{
  std::ifstream input(location, std::ios::binary);
  std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));

  // print cartrige data
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

  std::cout << "Title: " << title << std::endl;
  unsigned char gb_type = buffer[0x0143];
  std::cout << "Gameboy Type: " << ((gb_type == 0x80) ? "GB Color" : "GB") << std::endl;
  unsigned char functions = buffer[0x0146];
  std::cout << "Use " << ((functions == 0x3) ? "Super " : "") << "Gameboy functions" << std::endl;

  std::string cart_types[0x100];
  cart_types[0x0] = "ROM ONLY";
  cart_types[0x1] = "ROM+MBC1";
  cart_types[0x2] = "ROM+MBC1+RAM";
  cart_types[0x3] = "ROM+MBC1+RAM+BATT";
  cart_types[0x5] = "ROM+MBC2";
  cart_types[0x6] = "ROM+MBC2+BATTERY";
  cart_types[0x8] = "ROM+RAM";
  cart_types[0x9] = "ROM+RAM+BATTERY";
  cart_types[0xB] = "ROM+MMM01";
  cart_types[0xC] = "ROM+MMM01+SRAM";
  cart_types[0xD] = "ROM+MMM01+SRAM+BATT";
  cart_types[0xF] = "ROM+MBC3+TIMER+BATT";
  cart_types[0x10] = "ROM+MBC3+TIMER+RAM+BATT";
  cart_types[0x11] = "ROM+MBC3";
  cart_types[0x12] = "ROM+MBC3+RAM";
  cart_types[0x13] = "ROM+MBC3+RAM+BATT";
  cart_types[0x19] = "ROM+MBC5";
  cart_types[0x1A] = "ROM+MBC5+RAM";
  cart_types[0x1B] = "ROM+MBC5+RAM+BATT";
  cart_types[0x1C] = "ROM+MBC5+RUMBLE";
  cart_types[0x1D] = "ROM+MBC5+RUMBLE+SRAM";
  cart_types[0x1E] = "ROM+MBC5+RUMBLE+SRAM+BATT";
  cart_types[0x1F] = "Pocket Camera";
  cart_types[0xFD] = "Bandai TAMA5";
  cart_types[0xFE] = "Hudson HuC-3";
  cart_types[0xFF] = "Hudson HuC-1";

  unsigned char cart = buffer[0x0147];
  std::cout << "Cartridge Type: " << cart_types[cart] << std::endl;


  // Assign memory controller based on cartridge specification
  switch (cart)
  {
  case 0x01:
  case 0x02:
  case 0x03:
    cartridge.MemoryType = EMemoryBankControllerType::MBC1;
    break;
  case 0x05:
  case 0x06:
    std::cout << "CONTROLLER NOT IMPLEMENTED" << std::endl;
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

  unsigned char rsize = buffer[0x0148];
  std::cout << "ROM Size: " << (32 << rsize) << "kB " << pow(2, rsize + 1) << " banks" << std::endl;
  int size, banks;
  switch (buffer[0x149])
  {
  case 1: size = 2; banks = 1;
  case 2: size = 8; banks = 1;
  case 3: size = 32; banks = 4;
  case 4: size = 128; banks = 16;
  default: size = 0; banks = 0;
  }
  std::cout << "RAM Size: " << size << "kB " << banks << " banks" << std::endl;
  std::cout << "Destination Code: " << (buffer[0x014A] == 1 ? "Non-" : "") << "Japanese" << std::endl;
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
    return VRAM[location & 0x1FFF];

    // External RAM
  case 0xA000:
  case 0xB000:
    return cartridge.Read(location);

    // Working RAM (8kB) and RAM Shadow
  case 0xC000:
  case 0xD000:
  case 0xE000:
    return WRAM[location & 0x1FFF];

    // Remaining Working RAM Shadow, I/O, Zero page RAM
  case 0xF000:
    switch (location & 0x0F00)
    {
      // Remaining Working RAM
    case 0x000: case 0x100: case 0x200: case 0x300:
    case 0x400: case 0x500: case 0x600: case 0x700:
    case 0x800: case 0x900: case 0xA00: case 0xB00:
    case 0xC00: case 0xD00:
      return WRAM[location & 0x1FFF];

      // Sprite OAM
    case 0xE00:
      return OAM[location & 0xFF];

    case 0xF00:
      if (location == 0xFF00)
        return 0;
        //return get_joypad_state();
      else
        return ZRAM[location & 0xFF];
    }
  default:
    return 0xFF;
  }
}

void DraconicMemory::Write(uint16_t location, unsigned char data)
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
    VRAM[location & 0x1FFF] = data;
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
    WRAM[location & 0x1FFF] = data;
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
      WRAM[location & 0x1FFF] = data;
      break;

      // Sprite OAM
    case 0xE00:
      OAM[location & 0xFF] = data;
      break;

    case 0xF00:
      WriteZeroPage(location, data);
      break;
    }
  }
}

void DraconicMemory::WriteZeroPage(uint16_t location, unsigned char data)
{
  switch (location)
  {
    // Joypad Register - only bits 4 & 5 can be written to
  case 0xFF00:
    ZRAM[0x00] = (data & 0x30);
    break;
    // Divider Register - Write as zero no matter content
  case 0xFF04:
    ZRAM[0x04] = 0;
    break;
    // TODO: STAT - writing to match flag resets flag but doesn't change mode
  case 0xFF41:
    ZRAM[0x41] = (data & 0xFC) | (STAT.get() & 0x03);
    break;

    // LY Register - Game cannot write to this register directly 
  case 0xFF44:
    ZRAM[0x44] = 0;
    break;
    // DMA transfer request
  case 0xFF46:
    ZRAM[0x46] = data;
    PerformDMATransfer();
    break;
  default:
    ZRAM[location & 0xFF] = data;
    break;
  }
}

void DraconicMemory::write_vector(std::ofstream& file, std::vector<uint8_t>& vec)
{

}

void DraconicMemory::load_vector(std::ifstream& file, std::vector<uint8_t>& vec)
{

}

void DraconicMemory::save_state(std::ofstream& file)
{

}

void DraconicMemory::load_state(std::ifstream& file)
{

}

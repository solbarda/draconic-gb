#include "DraconicMemory.h"
#include <xutility>
#include <iosfwd>
#include <iostream>
#include <fstream>

bool is_bit_set(unsigned char data, unsigned char bit)
{
  return ((data >> bit) & 1) ? true : false;
}

unsigned char MemoryController::Read(uint16_t location)
{
  switch (MemoryType)
  {
  case EMemoryManagerType::DEFAULT:
  {
    if (location >= 0x0000 && location <= 0x7FFF)
      return CART_ROM[location];
    else if (location >= 0xA000 && location <= 0xBFFF)
      return ERAM[location & 0x1FFF];
    else
      return 0x00;
  }
    break;
  case EMemoryManagerType::MBC1:
  {
    // ROM bank 0 (read only)
    if (location >= 0x0000 && location <= 0x3FFF)
    {
      return CART_ROM[location];
    }
    // ROM banks 01-7F (read only)
    else if (location >= 0x4000 && location <= 0x7FFF)
    {
      // only ROM banks 0x00 - 0x1F can be used during mode 1
      unsigned char temp_id = ROM_bank_id;

      int offset = location - 0x4000;
      int lookup = (temp_id * 0x4000) + offset;

      return CART_ROM[lookup];
    }
    // RAM banks 00 - 03, if any (read/write)
    else if (location >= 0xA000 && location <= 0xBFFF)
    {
      if (RAM_access_enabled == false)
        return 0xFF;

      // only RAM bank 0 can be used during ROM mode
      unsigned char temp_id = (RAM_bank_enabled) ? RAM_bank_id : 0x00;

      int offset = location - 0xA000;
      int lookup = (temp_id * 0x2000) + offset;

      return ERAM[lookup];
    }
  }
    break;
  case EMemoryManagerType::MBC2:
    break;
  case EMemoryManagerType::MBC3:
  {
    // ROM bank 0 (read only)
    if (location >= 0x0000 && location <= 0x3FFF)
    {
      return CART_ROM[location];
    }
    // ROM banks 01-7F (read only)
    else if (location >= 0x4000 && location <= 0x7FFF)
    {
      int offset = location - 0x4000;
      int lookup = (ROM_bank_id * 0x4000) + offset;

      return CART_ROM[lookup];
    }
    // RAM banks 00 - 03, if any (read/write)
    else if (location >= 0xA000 && location <= 0xBFFF)
    {
      if (RTC_enabled)
        return 0x00;

      if (RAM_access_enabled == false)
        return 0xFF;

      int offset = location - 0xA000;
      int lookup = (RAM_bank_id * 0x2000) + offset;

      return ERAM[lookup];
    }
  }
    break;
  }

  return 0;
}

void MemoryController::Write(uint16_t location, unsigned char data)
{
  switch (MemoryType)
  {
  case EMemoryManagerType::DEFAULT:
  {
    if (location >= 0xA000 && location <= 0xBFFF)
      ERAM[location & 0x1FFF] = data;
  }
  break;
  case EMemoryManagerType::MBC1:
    {
    // RAM enable (write only)
    if (location >= 0x0000 && location <= 0x1FFF)
    {
      // Any value with 0x0A in lower 4 bits enables, everything else disables
      RAM_access_enabled = ((data & 0x0A) > 0) ? true : false;
    }
    // ROM bank id low bits select (write only)
    else if (location >= 0x2000 && location <= 0x3FFF)
    {
      // bottom 5 bits represent bank number from 0x00 -> 0x1F
      unsigned char bank_id = data & 0x1F;

      ROM_bank_id = (ROM_bank_id & 0xE0) | bank_id;

      // Prevent bank zero from being accessed
      // TODO: may need to adjust this to include other banks
      switch (ROM_bank_id)
      {
      case 0x00:
      case 0x20:
      case 0x40:
      case 0x60:
        ROM_bank_id++;
        break;
      }
    }
    // RAM bank id, or upper bits of ROM bank id
    else if (location >= 0x4000 && location <= 0x5FFF)
    {
      // extract bottom 2 bits
      unsigned char bank_id = data & 0x03;

      // data represents RAM bank ID
      if (RAM_bank_enabled)
      {
        RAM_bank_id = bank_id;
      }
      // data represents top bits of ROM bank ID
      else
      {
        ROM_bank_id = ROM_bank_id | (bank_id << 5);

        // Adjust bank ID to prevent certain banks from being accessed
        switch (ROM_bank_id)
        {
        case 0x00:
        case 0x20:
        case 0x40:
        case 0x60:
          ROM_bank_id++;
          break;
        }
      }
    }
    // Bank selector
    else if (location >= 0x6000 && location <= 0x7FFF)
    {
      RAM_bank_enabled = is_bit_set(data, 0);
    }
    // RAM banks 00 - 03, if any (read/write)
    else if (location >= 0xA000 && location <= 0xBFFF)
    {
      if (RAM_access_enabled)
      {
        int offset = location - 0xA000;
        int lookup = (RAM_bank_id * 0x2000) + offset;

        ERAM[lookup] = data;
      }
    }
    }
  break;
  case EMemoryManagerType::MBC2:
    {

    }
    break;
  case EMemoryManagerType::MBC3:
  {
    if (location >= 0x0000 && location <= 0x1FFF)
    {
      // Any value with 0x0A in lower 4 bits enables, everything else disables
      if ((data & 0x0A) > 0)
      {
        RAM_access_enabled = true;
        RTC_enabled = true;
      }
      else
      {
        RAM_access_enabled = false;
        RTC_enabled = false;
      }
    }
    else if (location >= 0x2000 && location <= 0x3FFF)
    {
      // bits 0-6 bits represent bank number from 0x00 -> 0x1F
      ROM_bank_id = data & 0x7F;

      if (ROM_bank_id == 0)
        ROM_bank_id++;
    }
    else if (location >= 0x4000 && location <= 0x5FFF)
    {
      // RAM bank
      if (data <= 0x3)
      {
        RTC_enabled = false;
        RAM_bank_id = data;
      }
      // RTC mapped
      else if (data >= 0x08 && data <= 0x0C)
      {
        RTC_enabled = true;
      }
    }
    else if (location >= 0x6000 && location <= 0x7FFF)
    {
      // TODO: Latch clock data
    }
    else if (location >= 0xA000 && location <= 0xBFFF)
    {
      // writing to RAM
      if (!RTC_enabled)
      {
        if (!RAM_access_enabled)
          return;

        int offset = location - 0xA000;
        int lookup = (RAM_bank_id * 0x2000) + offset;

        ERAM[lookup] = data;
      }
      else
      {
        // TODO: RTC writing
      }
    }
  }
  break;
  }
}

void MemoryController::Init(std::vector<unsigned char> cartridge_buffer)
{
	CART_ROM = cartridge_buffer;
	ERAM = std::vector<unsigned char>(0x8000); // $A000 - $BFFF, 8kB switchable RAM bank, size liable to change in future
}

std::vector<unsigned char> MemoryController::GetRAM()
{
	return ERAM;
}

void MemoryController::SetRAM(std::vector<unsigned char> data)
{
	ERAM = data;
}

void MemoryController::SaveState(std::ofstream& file) {
  switch (MemoryType)
  {
  case EMemoryManagerType::MBC1:
    file.write((char*)&ROM_bank_id, sizeof(ROM_bank_id));
    file.write((char*)&RAM_bank_id, sizeof(RAM_bank_id));
    file.write((char*)&RAM_bank_enabled, sizeof(RAM_bank_enabled));
    file.write((char*)&RAM_access_enabled, sizeof(RAM_access_enabled));
    file.write((char*)&mode, sizeof(mode));
    std::cout << "wrote registers" << std::endl;
    break;
  case EMemoryManagerType::MBC3:
    file.write((char*)&ROM_bank_id, sizeof(ROM_bank_id));
    file.write((char*)&RAM_bank_id, sizeof(RAM_bank_id));
    file.write((char*)&RAM_bank_enabled, sizeof(RAM_bank_enabled));
    file.write((char*)&RAM_access_enabled, sizeof(RAM_access_enabled));
    file.write((char*)&mode, sizeof(mode));
    file.write((char*)&RTC_enabled, sizeof(RTC_enabled));
    std::cout << "wrote registers" << std::endl;
    break;
  default:
    std::cout << "did nothing" << std::endl;
  }
}
void MemoryController::LoadState(std::ifstream& file) {

  switch (MemoryType)
  {
  case EMemoryManagerType::MBC1:
    file.read((char*)&ROM_bank_id, sizeof(ROM_bank_id));
    file.read((char*)&RAM_bank_id, sizeof(RAM_bank_id));
    file.read((char*)&RAM_bank_enabled, sizeof(RAM_bank_enabled));
    file.read((char*)&RAM_access_enabled, sizeof(RAM_access_enabled));
    file.read((char*)&mode, sizeof(mode));
    std::cout << "read registers" << std::endl;
    break;
  case EMemoryManagerType::MBC3:
    file.read((char*)&ROM_bank_id, sizeof(ROM_bank_id));
    file.read((char*)&RAM_bank_id, sizeof(RAM_bank_id));
    file.read((char*)&RAM_bank_enabled, sizeof(RAM_bank_enabled));
    file.read((char*)&RAM_access_enabled, sizeof(RAM_access_enabled));
    file.read((char*)&mode, sizeof(mode));
    file.read((char*)&RTC_enabled, sizeof(RTC_enabled));
    std::cout << "read registers" << std::endl;
    break;
  default:
    std::cout << "did nothing" << std::endl;
  }
}


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
    controller.MemoryType = EMemoryManagerType::MBC1;
    break;
  case 0x05:
  case 0x06:
    std::cout << "CONTROLLER NOT IMPLEMENTED" << std::endl;
    controller.MemoryType = EMemoryManagerType::MBC2;
    break;
  case 0x0F:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
    controller.MemoryType = EMemoryManagerType::MBC3;
    break;
  default:
    controller.MemoryType = EMemoryManagerType::DEFAULT;
    break;
  }

  // Initialize controller with cartridge data
  controller.Init(buffer);

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
    return controller.Read(location);

    // Graphics VRAM
  case 0x8000:
  case 0x9000:
    return VRAM[location & 0x1FFF];

    // External RAM
  case 0xA000:
  case 0xB000:
    return controller.Read(location);

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
    controller.Write(location, data);
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
    controller.Write(location, data);
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
    //ZRAM[0x41] = (data & 0xFC) | (STAT.get() & 0x03);
    break;

    // LY Register - Game cannot write to this register directly 
  case 0xFF44:
    ZRAM[0x44] = 0;
    break;
    // DMA transfer request
  case 0xFF46:
    ZRAM[0x46] = data;
    //do_dma_transfer();
    break;
  default:
    ZRAM[location & 0xFF] = data;
    break;
  }
}

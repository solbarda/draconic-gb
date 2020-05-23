#include "DraconicCartridge.h"
#include "Utils/Utils.h"
#include <iostream>


// Memory implementation based on mattbruv code


unsigned char DraconicCartridge::Read(uint16_t location)
{
  switch (MemoryType)
  {
  case EMemoryBankControllerType::DEFAULT:
  {
    if (location >= 0x0000 && location <= 0x7FFF)
      return CART_ROM[location];
    else if (location >= 0xA000 && location <= 0xBFFF)
      return ERAM[location & 0x1FFF];
    else
      return 0x00;
  }
  break;
  case EMemoryBankControllerType::MBC1:
  {
    if (location >= 0x0000 && location <= 0x3FFF)
    {
      return CART_ROM[location];
    }
    else if (location >= 0x4000 && location <= 0x7FFF)
    {
      unsigned char temp_id = ROMBankID;

      int offset = location - 0x4000;
      int lookup = (temp_id * 0x4000) + offset;

      return CART_ROM[lookup];
    }
    else if (location >= 0xA000 && location <= 0xBFFF)
    {
      if (RAMAccessEnabled == false)
        return 0xFF;

      unsigned char temp_id = (RAMBankEnabled) ? RAMBankID : 0x00;

      int offset = location - 0xA000;
      int lookup = (temp_id * 0x2000) + offset;

      return ERAM[lookup];
    }
  }
  break;
  case EMemoryBankControllerType::MBC2:
    break;
  case EMemoryBankControllerType::MBC3:
  {
    if (location >= 0x0000 && location <= 0x3FFF)
    {
      return CART_ROM[location];
    }
    else if (location >= 0x4000 && location <= 0x7FFF)
    {
      int offset = location - 0x4000;
      int lookup = (ROMBankID * 0x4000) + offset;

      return CART_ROM[lookup];
    }
    else if (location >= 0xA000 && location <= 0xBFFF)
    {
      if (RTC_enabled)
        return 0x00;

      if (RAMAccessEnabled == false)
        return 0xFF;

      int offset = location - 0xA000;
      int lookup = (RAMBankID * 0x2000) + offset;

      return ERAM[lookup];
    }
  }
  break;
  }

  return 0;
}

void DraconicCartridge::Write(uint16_t location, uint8_t data)
{
  switch (MemoryType)
  {
  case EMemoryBankControllerType::DEFAULT:
  {
    if (location >= 0xA000 && location <= 0xBFFF)
      ERAM[location & 0x1FFF] = data;
  }
  break;
  case EMemoryBankControllerType::MBC1:
  {
    if (location >= 0x0000 && location <= 0x1FFF)
    {
      RAMAccessEnabled = ((data & 0x0A) > 0) ? true : false;
    }
    else if (location >= 0x2000 && location <= 0x3FFF)
    {
      unsigned char bank_id = data & 0x1F;

      ROMBankID = (ROMBankID & 0xE0) | bank_id;

      switch (ROMBankID)
      {
      case 0x00:
      case 0x20:
      case 0x40:
      case 0x60:
        ROMBankID++;
        break;
      }
    }
    else if (location >= 0x4000 && location <= 0x5FFF)
    {
      unsigned char bank_id = data & 0x03;

      if (RAMBankEnabled)
      {
        RAMBankID = bank_id;
      }
      else
      {
        ROMBankID = ROMBankID | (bank_id << 5);

        switch (ROMBankID)
        {
        case 0x00:
        case 0x20:
        case 0x40:
        case 0x60:
          ROMBankID++;
          break;
        }
      }
    }
    else if (location >= 0x6000 && location <= 0x7FFF)
    {
      RAMBankEnabled = IsBitSet(data, (EBit)0);
    }
    else if (location >= 0xA000 && location <= 0xBFFF)
    {
      if (RAMAccessEnabled)
      {
        int offset = location - 0xA000;
        int lookup = (RAMBankID * 0x2000) + offset;

        ERAM[lookup] = data;
      }
    }
  }
  break;
  case EMemoryBankControllerType::MBC2:
  {

  }
  break;
  case EMemoryBankControllerType::MBC3:
  {
    if (location >= 0x0000 && location <= 0x1FFF)
    {
      if ((data & 0x0A) > 0)
      {
        RAMAccessEnabled = true;
        RTC_enabled = true;
      }
      else
      {
        RAMAccessEnabled = false;
        RTC_enabled = false;
      }
    }
    else if (location >= 0x2000 && location <= 0x3FFF)
    {
      ROMBankID = data & 0x7F;

      if (ROMBankID == 0)
        ROMBankID++;
    }
    else if (location >= 0x4000 && location <= 0x5FFF)
    {
      if (data <= 0x3)
      {
        RTC_enabled = false;
        RAMBankID = data;
      }
      else if (data >= 0x08 && data <= 0x0C)
      {
        RTC_enabled = true;
      }
    }
    else if (location >= 0x6000 && location <= 0x7FFF)
    {
    }
    else if (location >= 0xA000 && location <= 0xBFFF)
    {
      if (!RTC_enabled)
      {
        if (!RAMAccessEnabled)
          return;

        int offset = location - 0xA000;
        int lookup = (RAMBankID * 0x2000) + offset;

        ERAM[lookup] = data;
      }
      else
      {
      }
    }
  }
  break;
  }
}

void DraconicCartridge::Init(std::vector<unsigned char> cartridgeBuffer)
{
  // Initalize the cartridge ROM data based on the buffer and init the external ram
  CART_ROM = cartridgeBuffer;
  ERAM = std::vector<unsigned char>(0x8000);
}


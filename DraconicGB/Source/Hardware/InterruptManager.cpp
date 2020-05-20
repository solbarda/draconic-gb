#include "InterruptManager.h"
#include "Utils.h"
#include "DraconicState.h"

void InterruptManager::RequestInterrupt(uint8_t id)
{
  state->memory.IF.set_bit((EBit)id);
}

void InterruptManager::DoInterrupts()
{
  // If there are any interrupts set
  if (state->memory.IF.get() > 0)
  {
    // Resume  CPU state if halted and interrupts are pending
    if (state->memory.IE.get() > 0)
    {

      if (state->halted)
      {
        state->halted = false;
        state->registers.PC += 1;
      }
    }
    // Loop through each bit and call interrupt for lowest . highest priority bits set
    for (int i = 0; i < 5; i++)
    {
      if (state->memory.IF.is_bit_set((EBit)i))
      {
        if (state->memory.IE.is_bit_set((EBit)i))
        {
          // IME only disables the servicing of interrupts,
          // not all interrupt functionality 
          if (state->interrupt_master_enable)
          {
            ServiceInterrupt(i);
          }
        }
      }
    }
  }
}

void InterruptManager::ServiceInterrupt(uint8_t id)
{
  state->interrupt_master_enable = false;
  state->memory.IF.clear_bit((EBit)id);

  uint8_t PCHigh = (uint8_t)(state->registers.PC >> 8) & 0xFF;
  uint8_t PCLow = (uint8_t)(state->registers.PC) & 0xFF;

  // Push current execution address to stack
  state->memory.Write(--state->registers.SP, PCHigh);
  state->memory.Write(--state->registers.SP, PCLow);

  switch ((EInterrupt)id)
  {
  case EInterrupt::INTERRUPT_VBLANK: state->registers.PC = 0x40; break;
  case EInterrupt::INTERRUPT_LCDC:   state->registers.PC = 0x48; break;
  case EInterrupt::INTERRUPT_TIMER:  state->registers.PC = 0x50; break;
  case EInterrupt::INTERRUPT_SERIAL: state->registers.PC = 0x58; break;
  case EInterrupt::INTERRUPT_JOYPAD: state->registers.PC = 0x60; break;
  }
}



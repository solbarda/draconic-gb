#include "InterruptManager.h"
#include "Utils.h"
#include "DraconicState.h"

void InterruptManager::RequestInterrupt(uint8_t id)
{
  // Set the memory location at Addr_IF with the interrupt bitset
  *state->memory.GetMemoryLocation(Addr_IF) = SetBit(state->memory.GetMemoryLocationData(Addr_IF), (EBit)id);
}

void InterruptManager::PerformInterrupts()
{
  // If the interrupt location has interrupts
  if (state->memory.GetMemoryLocationData(Addr_IF) > 0)
  {
    // RIf we are halted we should resume
    if (state->memory.GetMemoryLocationData(Addr_IE) > 0 && state->bHalted)
    {
      state->bHalted = false;
      state->registers.PC += 1;
    }

    // Loop through each bit and call interrupt for lowest . highest priority bits set
    for (int i = 0; i < 5; i++)
    {
      if(IsBitSet(state->memory.GetMemoryLocationData(Addr_IF),(EBit)i))
      {
        if (IsBitSet(state->memory.GetMemoryLocationData(Addr_IE), (EBit)i))
        {
          // If interrupt master is enabled we perform a Service Interrupt
          if (state->bInterruptMasterEnabled)
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
  // Disable the interrupt master
  state->bInterruptMasterEnabled = false;
  // Clear the interrupt bit from memory
  *state->memory.GetMemoryLocation(Addr_IF) = ClearBit(state->memory.GetMemoryLocationData(Addr_IF), (EBit)id);

  uint8_t PCHigh = (uint8_t)(state->registers.PC >> 8) & 0xFF;
  uint8_t PCLow = (uint8_t)(state->registers.PC) & 0xFF;

  // Push the current PC to the stack
  state->memory.Write(--state->registers.SP, PCHigh);
  state->memory.Write(--state->registers.SP, PCLow);

  // Depending on the interrupt we set the program counter to one value or another
  switch ((EInterrupt)id)
  {
  case EInterrupt::INTERRUPT_VBLANK: state->registers.PC = 0x40; break;
  case EInterrupt::INTERRUPT_LCDC:   state->registers.PC = 0x48; break;
  case EInterrupt::INTERRUPT_TIMER:  state->registers.PC = 0x50; break;
  case EInterrupt::INTERRUPT_SERIAL: state->registers.PC = 0x58; break;
  case EInterrupt::INTERRUPT_JOYPAD: state->registers.PC = 0x60; break;
  }
}



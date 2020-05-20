#include "TimerManager.h"
#include "Utils/Utils.h"
#include "Hardware/DraconicState.h"

TimerManager::TimerManager()
{
  timerCounter = 0;
  timerFrequency = 0;
  dividerCounter = 0;
  dividerFrequency = 16384;
}

// Opcode cycle number may need adjusted, used Nintendo values
void TimerManager::UpdateTimers()
{
  UpdateDivider(state->numCycles);

  // Get the new frequency
  uint8_t NewFrequency = GetTimerFrequency();

  // If our current frequency is different from the new update to match
  if (timerFrequency != NewFrequency)
  {
    SetTimerFrequency();
    timerFrequency = NewFrequency;
  }

  // If the timer is enabled we must make the time pass for them
  if (IsTimerEnabled())
  {
    // Subtract the number of cycles elapsed to the timer counter
    timerCounter -= state->numCycles;

    // If timer counter reaches 0 we update TIMA
    if (timerCounter <= 0)
    {
      uint8_t TimerValue = state->memory.GetMemoryLocationData(Addr_TIMA);
      SetTimerFrequency();

      // If timer is about to overflow call interrupt
      if (TimerValue == 255)
      {
        // Call the interrupt
        *state->memory.GetMemoryLocation(Addr_TIMA) = state->memory.GetMemoryLocationData(Addr_TMA);
        *state->memory.GetMemoryLocation(Addr_IF) = SetBit(state->memory.GetMemoryLocationData(Addr_IF), (EBit)EInterrupt::INTERRUPT_TIMER);
      }
      else
      {
        // Else we increment the timer value and continue counting
        *state->memory.GetMemoryLocation(Addr_TIMA) = TimerValue + 1;
      }
    }
  }
}

bool TimerManager::IsTimerEnabled()
{
  // Return bit of memory that controls if timer is enabled
  return IsBitSet(state->memory.GetMemoryLocationData(Addr_TAC), EBit::BIT_2);
}

uint8_t TimerManager::GetTimerFrequency()
{
  // Return stored timer frequency stored on the first 2 bits
  return state->memory.GetMemoryLocationData(Addr_TAC) & 0x3;
}

void TimerManager::SetTimerFrequency()
{
  // Update the frequency to the selected one
  timerFrequency = GetTimerFrequency();

  switch (timerFrequency)
  {
  case 0: timerCounter = 1024; break; // 4096 Hz
  case 1: timerCounter = 16; break; // 262144 Hz
  case 2: timerCounter = 64; break; // 65536 Hz
  case 3: timerCounter = 256; break; // 16384 Hz
  }
}


void TimerManager::UpdateDivider(int cycles)
{
  // Update the divider coutner based on the cycles
  dividerCounter += cycles;

  // If divider counter exceed the value update the memory location controlling divider
  if (dividerCounter >= 256) // 16384 Hz
  {
    dividerCounter = 0;
    *state->memory.GetMemoryLocation(Addr_DIV) = state->memory.GetMemoryLocationData(Addr_DIV) + 1;
  }
}

#include "TimerManager.h"
#include "Utils.h"
#include "DraconicState.h"

// Opcode cycle number may need adjusted, used Nintendo values
void TimerManager::UpdateTimers()
{
  int cycles = state->numCycles;
  UpdateDivider(cycles);

  // This can be optimized if needed
  uint8_t new_freq = GetTimerFrequency();

  if (timer_frequency != new_freq)
  {
    SetTimerFrequency();
    timer_frequency = new_freq;
  }

  if (IsTimerEnabled())
  {
    timer_counter -= cycles;

    // enough CPU clock cycles have happened to update timer
    if (timer_counter <= 0)
    {
      uint8_t timer_value = state->memory.TIMA.get();
      SetTimerFrequency();

      // Timer will overflow, generate interrupt
      if (timer_value == 255)
      {
        state->memory.TIMA.set(state->memory.TMA.get());
        state->memory.IF.set_bit((EBit)EInterrupt::INTERRUPT_TIMER);
      }
      else
      {
        state->memory.TIMA.set(timer_value + 1);
      }
    }
  }
}

bool TimerManager::IsTimerEnabled()
{
  return state->memory.TAC.is_bit_set(EBit::BIT_2);
}

uint8_t TimerManager::GetTimerFrequency()
{
  return (state->memory.TAC.get() & 0x3);
}

void TimerManager::SetTimerFrequency()
{
  uint8_t frequency = GetTimerFrequency();
  timer_frequency = frequency;

  switch (frequency)
  {
    // timer_counter calculated by (Clock Speed / selected frequency)
  case 0: timer_counter = 1024; break; // 4096 Hz
  case 1: timer_counter = 16; break; // 262144 Hz
  case 2: timer_counter = 64; break; // 65536 Hz
  case 3: timer_counter = 256; break; // 16384 Hz
  }
}


void TimerManager::UpdateDivider(int cycles)
{
  DividerCounter += cycles;

  if (DividerCounter >= 256) // 16384 Hz
  {
    DividerCounter = 0;
    state->memory.DIV.set(state->memory.DIV.get() + 1);
  }
}

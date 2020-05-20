#pragma once
#include "SDL_config.h"
#include "BaseHardware.h"


class TimerManager : public BaseHardware{

private:
  int timer_counter = 0;
  uint8_t timer_frequency = 0;
  bool IsTimerEnabled();
  uint8_t GetTimerFrequency();
  void SetTimerFrequency();

  int DividerCounter = 0;
  int divider_frequency = 16384; // 16384 Hz or every 256 CPU clock cycles
  void UpdateDivider(int cycles);

public:
  void UpdateTimers();
 

  
  

};



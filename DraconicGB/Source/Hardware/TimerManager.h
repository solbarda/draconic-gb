#pragma once
#include "SDL_config.h"
#include "BaseHardware.h"

// Class that handles the timer functionality
class TimerManager : public BaseHardware{


public:
  TimerManager();
  void UpdateTimers();

private:
  int timerCounter;
  // Timer frequency stored in two bits, cna have 4 possible values
  uint8_t timerFrequency;
  // Check if timer is enabled
  bool IsTimerEnabled();
  // Get the frequency of the timer
  uint8_t GetTimerFrequency();
  // Set the timer frequency
  void SetTimerFrequency();

  int dividerCounter;
  int dividerFrequency;
  void UpdateDivider(int cycles);


 

  
  

};



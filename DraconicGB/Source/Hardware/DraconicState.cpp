#include "DraconicState.h"
#include "Utils/Utils.h"

DraconicState::DraconicState()
{
  numCycles = 0;
  bInterruptMasterEnabled = true;
  bHalted = false;
  CPUClockSpeed = 4194304;
}

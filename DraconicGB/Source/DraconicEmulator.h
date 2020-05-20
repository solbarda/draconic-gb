#pragma once

// Draconic Emulator includes
#include "Hardware/DraconicGPU.h"
#include "Hardware/DraconicMemory.h"
#include "Hardware/DraconicCPU.h"
#include "Hardware/TimerManager.h"
#include "Hardware/InterruptManager.h"
#include "Hardware/InputManager.h"
#include "DraconicState.h"
// OpenGL Context
#include <glad/glad.h> 
// Imgui
#include "imgui.h"
#include "examples/imgui_impl_sdl.h"
#include "examples/imgui_impl_opengl3.h"
// SDL
#include <SDL.h>
// Standard libraries
#include <stdio.h>
#include <chrono>

// Main class that handles everything
class DraconicEmulator
{
public:
  DraconicCPU CPU;
  DraconicGPU GPU;
  DraconicState state;
  TimerManager timerManager;
  InterruptManager interruptManager;
  InputManager inputManager;

  // Load a ROM and start the emulation
  void LoadROMAndStart(std::string romPath);
  // Function used to start the emulator initialization and main loop
  int Start();
  // Destroy emulator and intantiated memory
  void Shutdown();

private:
  // Initialize emulator
  int Init();
  // Application main loop, controls timing and rendering
  void  MainLoop();
  // Emulator main loop, controls logic and memory
  void EmulatorMainLoop(float deltaTime);
  // Draw debug windows
  void DebugRender();

  // SDL Context
  struct SDL_Window* window;
  SDL_GLContext gl_context;
  SDL_Renderer* renderer;

  // Application state
  bool Finished = false;
  bool bEmulatorStarted = false;
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  Uint64 PrevFrameTime;
  float DeltaTime;
  float framerate = 60;
  float accumTime = 0;
  // Debug Display flags
  bool bDebugDisplayVRAM = true;
  bool bDebugDisplayOAM;
  bool bDebugDisplayWRAM;
  bool bDebugDisplayZRAM;
  bool bDebugDisplayROM;
  bool bDebugDisplayERAM;
  bool bDebugDisplayGPU = true;


  void SaveState(int id);
  void LoadState(int id);
};


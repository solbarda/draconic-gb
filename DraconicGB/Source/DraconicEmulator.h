#pragma once

#include "Hardware/CPU.h"
#include "Hardware/DraconicGPU.h"
#include "Hardware/DraconicMemory.h"

#include "./../ThirdParty/glad/include/glad/glad.h"
#include "imgui.h"
#include "examples/imgui_impl_sdl.h"
#include "examples/imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#include <chrono>

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING)
#include <glbinding/glbinding.h>  // Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif
#include "DraconicState.h"

class DraconicEmulator
{
  
public:
  CPU DraconicCPU;
  DraconicGPU gpu;
  DraconicState state;

  void LoadROMAndStart(std::string romPath);
  int Start();
  void Shutdown();

private:
  int Init();
  void  MainLoop();

  void EmulatorMainLoop(float deltaTime);

  void DebugRender();

  struct SDL_Window* window;
  SDL_GLContext gl_context;
  SDL_Renderer* renderer;

  // State
  bool Finished = false;
  bool bEmulatorStarted = false;
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  Uint64 PrevFrameTime;
  float DeltaTime;
  float framerate = 60;

  float accumTime = 0;




  bool bDebugDisplayVRAM = true;
  bool bDebugDisplayOAM;
  bool bDebugDisplayWRAM;
  bool bDebugDisplayZRAM;
  bool bDebugDisplayROM;
  bool bDebugDisplayERAM;
  bool bDebugDisplayGPU = true;


  // -------- EVENTS ------- //
  void handle_events();

  // -------- JOYPAD ------- //
  void key_pressed(SDL_KeyboardEvent key);
  void key_released(SDL_KeyboardEvent key);
  int get_key_id(SDL_Keysym key);

  // -------- SAVESTATES ------- //
  void save_state(int id);
  void load_state(int id);

  // --------- DIVIDER --------- //
  int divider_counter = 0;
  int divider_frequency = 16384; // 16384 Hz or every 256 CPU clock cycles
  void update_divider(int cycles);

  // ----------TIMERS ---------- //
  int timer_counter = 0; // this may need to be set to some calculated non zero value
  uint8_t timer_frequency = 0;
  void update_timers(int cycles);
  bool timer_enabled();
  uint8_t get_timer_frequency();
  void set_timer_frequency();

  // ------- INTERRUPTS ------- //
  void request_interrupt(uint8_t id);
  void do_interrupts();
  void service_interrupt(uint8_t id);

  // ------ LCD Display ------ //
  int scanline_counter = 456; // Clock cycles per scanline draw
  void set_lcd_status();
  void update_scanline(int cycles);
};


#pragma once

#include "Hardware/CPU.h"
#include "Hardware/GPU.h"
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

class DraconicEmulator
{
  
public:
  CPU DraconicCPU;
  DraconicMemory memory;
  GPU DraconicGPU;

  int Start();
  void Shutdown();

private:
  int Init();
  void  MainLoop();

  void DebugRender();

  struct SDL_Window* window;
  SDL_GLContext gl_context;

  // State
  bool Finished = false;
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  Uint64 PrevFrameTime;
  float DeltaTime;
  float framerate = 60;

  float accumTime = 0;



  bool bDebugDisplayVRAM;
  bool bDebugDisplayOAM;
  bool bDebugDisplayWRAM;
  bool bDebugDisplayZRAM;


  void UpdateTimers(int numCycles);
  void UpdateScanlines(int numCycles);
  void DoInterrupts();
};


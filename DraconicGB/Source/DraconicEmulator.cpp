#include "DraconicEmulator.h"
#include <iostream>
#include "SDL_config.h"
#include "../ThirdParty/imgui/imgui_memory_editor.h"
#include "Hardware/DraconicState.h"
#include "Utils/Utils.h"
#include <fstream>
#include <iosfwd>
#include "SDL_syswm.h"
#include <bitset>
#include <commdlg.h>


void DraconicEmulator::LoadROMAndStart(std::string romPath)
{
  // Reset memory and registers to default values
  state.memory.Reset();
  state.registers.Reset();
  GPU.Reset();
  // Load the ROM into memory
  state.memory.LoadROM(romPath);
  // We set the bEmulatorStarted flag to true so the the CPU can perform its tasks
  bEmulatorStarted = true;
}

int DraconicEmulator::Start()
{
  // Init the window, context and hardware
  Init();
  // Load a startup ROM
  // Tests
  //LoadROMAndStart("./ROM/cpu_instrs/individual/01-special.gb");
  //LoadROMAndStart("./ROM/cpu_instrs/individual/02-interrupts.gb");
  //LoadROMAndStart("./ROM/cpu_instrs/individual/03-op sp,hl.gb");
  //LoadROMAndStart("./ROM/cpu_instrs/individual/04-op r,imm.gb");
  //LoadROMAndStart("./ROM/cpu_instrs/individual/05-op rp.gb");
  //LoadROMAndStart("./ROM/cpu_instrs/individual/06-ld r,r.gb");
  //LoadROMAndStart("./ROM/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb"); // FAIL
  //LoadROMAndStart("./ROM/cpu_instrs/individual/08-misc instrs.gb");
  //LoadROMAndStart("./ROM/cpu_instrs/individual/09-op r,r.gb");
  //LoadROMAndStart("./ROM/cpu_instrs/individual/10-bit ops.gb");
  //LoadROMAndStart("./ROM/cpu_instrs/individual/11-op a,(hl).gb");
  //LoadROMAndStart("./ROM/uoc.gb");
  
  // Start emulator main loop
  MainLoop();
  return 0;
}

void DraconicEmulator::Shutdown()
{
  // Cleanup ImGui
  //ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  // Cleanup SDL
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int DraconicEmulator::Init()
{
  // SDL Startup
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
  {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  /// Generic SDL Code Start ----------------------------------------------------------
  // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 Core + GLSL 150
  const char* glsl_version = "#version 150";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  window = SDL_CreateWindow("Draconic GB Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  gl_context = SDL_GL_CreateContext(window);
  renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
  bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
  bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
  bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING)
  bool err = false;
  glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name); });
#else
  bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
  if (err)
  {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    return 1;
  }

  /// Generic SDL Code Finish ----------------------------------------------------------




  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  //ImGui_ImplOpenGL3_Init(glsl_version);
  ImGui_ImplOpenGL2_Init();

  // Init the emulator hardware
  CPU.Init(&state);
  GPU.Init(&state, window, renderer);
  timerManager.Init(&state);
  interruptManager.Init(&state);
  inputManager.Init(&state);

  return 0;
}

void DraconicEmulator::MainLoop()
{
  
  // Main loop, until the window is closed we will not exit from here
  while (!Finished)
  {
    // Get time elapsed between frames
    static Uint64 frequency = SDL_GetPerformanceFrequency();
    Uint64 CurrentTime = SDL_GetPerformanceCounter();
    DeltaTime = PrevFrameTime > 0 ? (float)((double)(CurrentTime - PrevFrameTime) / frequency) : (float)(1.0f / 60.0f);

    // Render ImGUI for debugging purposes
    DebugRender();

    // If the emulation has started perform the emulator main loop
    if (bEmulatorStarted)
    {
      EmulatorMainLoop(DeltaTime);
    }
  }
}

void DraconicEmulator::EmulatorMainLoop(float deltaTime)
{
  // First we accomulate de delta time
  accumTime += DeltaTime;
  // Then if accomulated time exceeds the time between frames we perform an update
  float timeBetweenFrames = 1.0f / framerate;
  if (accumTime >= timeBetweenFrames)
  {
    // We will need to perform as many cycles as our current framerate needs us to do
    float cyclesPerFrame = state.CPUClockSpeed / framerate;
    int currentCycle = 0;
    while (currentCycle < cyclesPerFrame)
    {
      // Get the next opcode to execute
      unsigned char opCode = state.memory.Read(state.registers.PC);

      // Run the opcode on the CPU
      CPU.ParseOpcode(opCode);
   
      // Increment the cycleCount base on the number of cycles elapsed on the CPU
      currentCycle += state.numCycles;
      // Update the CPU Timers based on the cycles
      timerManager.UpdateTimers();
      // Update the scanlines
      GPU.UpdateScanline();
      //Perform the interrupts
      interruptManager.PerformInterrupts();
      // Reset the CPU current cycles
      state.numCycles = 0;
    }
    // Once we have finished we reset the current cycle count to 0 and
    // subtract the elasepd time from our accumulated time
    currentCycle = 0;
    accumTime -= timeBetweenFrames;
    GPU.scanlines_rendered = 0;
  }
}

void DraconicEmulator::DebugRender()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT)
      Finished = true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
      Finished = true;
    if (event.type == SDL_KEYDOWN) 
      inputManager.OnKeyPressed(event.key);
    if (event.type == SDL_KEYUP)
      inputManager.OnKeyReleased(event.key);
  }

  // Start the Dear ImGui frame
  //ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();

  // Draw main menus
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Open", "Ctrl+O"))
      {
        char filename[MAX_PATH];

        OPENFILENAME ofn;
        ZeroMemory(&filename, sizeof(filename));
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
        ofn.lpstrFilter = "Gameboy Rom File\0*.gb\0Any File\0*.*\0";
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = "Select a .gb file";
        ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameA(&ofn))
        {
          LoadROMAndStart(filename);
        }

      }
      if (ImGui::BeginMenu("Open Preset Files"))
      {
        if (ImGui::MenuItem("2048.gb"))
          LoadROMAndStart("./ROM/2048.gb");
        if (ImGui::MenuItem("Mona_And_The_Witch_Hat.gb"))
          LoadROMAndStart("./ROM/Mona_And_The_Witch_Hat.gb");
        if (ImGui::MenuItem("PrettyPrincessCastleEscape.gb"))
          LoadROMAndStart("./ROM/PrettyPrincessCastleEscape.gb");
        //if (ImGui::MenuItem("SuperPrincess.gb"))
        //  LoadROMAndStart("./ROM/SuperPrincess.gb");
        if(ImGui::MenuItem("tobu.gb"))
          LoadROMAndStart("./ROM/tobu.gb");
        if (ImGui::MenuItem("CPU_instrs.gb"))
          LoadROMAndStart("./ROM/cpu_instrs.gb");
        if (ImGui::MenuItem("m3_bgp_change.gb"))
          LoadROMAndStart("./ROM/m3_bgp_change.gb");
        if (ImGui::MenuItem("m3_lcdc_bg_en_change.gb"))
          LoadROMAndStart("./ROM/m3_lcdc_bg_en_change.gb");
        if (ImGui::MenuItem("m3_wx_4_change.gb"))
          LoadROMAndStart("./ROM/m3_wx_4_change.gb");
        
        
        ImGui::EndMenu();
      }
      ImGui::Separator();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Color Palette"))
    {
      /*ImGui::PushID(1);
      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(50 / 7.0f, 0.6f, 0.6f));
      ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(200 / 7.0f, 0.6f, 0.6f));
      ImGui::Button("Click");
      ImGui::PopStyleColor(3);
      ImGui::PopID();*/
      static int currentPalette = 0;
      if (ImGui::MenuItem("Palette A", currentPalette == 0 ? "Current" : "")) {
        currentPalette = 0;
        GPU.SetColorPalette(
          PixelColor(90, 57, 33, 255),
          PixelColor(107, 140, 66, 255),
          PixelColor(123, 198, 123, 255),
          PixelColor(255, 255, 181, 255)
        );
      }
      if (ImGui::MenuItem("Palette B", currentPalette == 1 ? "Current" : "")) {
        currentPalette = 1;
        GPU.SetColorPalette(
          PixelColor(51, 30, 80, 255),
          PixelColor(166, 55, 37, 255),
          PixelColor(214, 142, 73, 255),
          PixelColor(253, 247, 231, 255)
        );
      }
      if (ImGui::MenuItem("Palette C", currentPalette == 2 ? "Current" : "")) {
        currentPalette = 2;
        GPU.SetColorPalette(
          PixelColor(0, 0, 0, 255),
          PixelColor(127, 127, 127, 255),
          PixelColor(198, 198, 198, 255),
          PixelColor(255, 255, 255, 255)
        );
      }
      if (ImGui::MenuItem("Palette D", currentPalette == 3 ? "Current" : "")) {
        currentPalette = 3;
        GPU.SetColorPalette(
          PixelColor(16, 37, 51, 255),
          PixelColor(66, 103, 142, 255),
          PixelColor(111, 158, 223, 255),
          PixelColor(206, 206, 206, 255)
        );
      }
      if (ImGui::MenuItem("Palette E", currentPalette == 4 ? "Current" : "")) {
        currentPalette = 4;
        GPU.SetColorPalette(
          PixelColor(124, 63, 88, 255),
          PixelColor(235, 107, 111, 255),
          PixelColor(249, 168, 117, 255),
          PixelColor(255, 246, 211, 255)
        );
      }
      if (ImGui::MenuItem("Palette F", currentPalette == 5 ? "Current" : "")) {
        currentPalette = 5;
        GPU.SetColorPalette(
          PixelColor(45, 27, 0, 255),
          PixelColor(30, 96, 110, 255),
          PixelColor(90, 185, 168, 255),
          PixelColor(196, 240, 194, 255)
        );
      }
      
      ImGui::EndMenu();
    }
    //if (ImGui::BeginMenu("Edit"))
    //{
    //  if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
    //  if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
    //  ImGui::Separator();
    //  if (ImGui::MenuItem("Cut", "CTRL+X")) {}
    //  if (ImGui::MenuItem("Copy", "CTRL+C")) {}
    //  if (ImGui::MenuItem("Paste", "CTRL+V")) {}
    //  ImGui::EndMenu();
    //}

    if (ImGui::BeginMenu("Debug"))
    {
      if (ImGui::MenuItem("Display VRAM", NULL, &bDebugDisplayVRAM)) {}
      if (ImGui::MenuItem("Display OAM", NULL, &bDebugDisplayOAM)) {}
      if (ImGui::MenuItem("Display WRAM", NULL, &bDebugDisplayWRAM)) {}
      if (ImGui::MenuItem("Display ZRAM", NULL, &bDebugDisplayZRAM)) {}
      if (bEmulatorStarted)
      {
        if (ImGui::MenuItem("Display ROM", NULL, &bDebugDisplayROM)) {}
        if (ImGui::MenuItem("Display ERAM", NULL, &bDebugDisplayERAM)) {}
      }
      if (ImGui::MenuItem("Display GPU", NULL, &bDebugDisplayGPU)) {}
      
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  // Draw Stats
  {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Stats");
    ImGui::Text("Rom Name: %s", state.memory.romName.c_str());
    ImGui::Text("ElapsedTime = %.2fms FPS: %.1f", DeltaTime * 1000.0f, 1.0f / DeltaTime);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("AccumTime  = %.2fms", accumTime*1000.0f);
    ImGui::Text("TargetTime = %.2fms", 1000.0f/ framerate);
    ImGui::Text("Registers: -------------");
   
    ImGui::Text("A: %s   B: %s   C: %s   D: %s",
      std::bitset<8>(state.registers.A).to_string().c_str(), std::bitset<8>(state.registers.B).to_string().c_str(), std::bitset<8>(state.registers.C).to_string().c_str(), std::bitset<8>(state.registers.D).to_string().c_str());
    ImGui::Text("E: %s   H: %s   L: %s   F: %s",
      std::bitset<8>(state.registers.E).to_string().c_str(), std::bitset<8>(state.registers.H).to_string().c_str(), std::bitset<8>(state.registers.L).to_string().c_str(), std::bitset<8>(state.registers.F).to_string().c_str());
    ImGui::Text("SP: %s    PC: %s",
      std::bitset<16>(state.registers.SP).to_string().c_str(), std::bitset<16>(state.registers.PC).to_string().c_str());
    ImGui::Text("Special Registers: -------------");
    ImGui::Text("P1:   %s   DIV:  %s   TIMA: %s   TMA:  %s",
      std::bitset<8>(state.memory.GetMemoryLocationData(Addr_P1)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_DIV)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_TIMA)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_TMA)).to_string().c_str());
    ImGui::Text("TAC:  %s   LCDC: %s   STAT: %s   SCY:  %s",
      std::bitset<8>(state.memory.GetMemoryLocationData(Addr_TAC)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_LCDC)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_STAT)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_SCY)).to_string().c_str());
    ImGui::Text("SCX:  %s   LYC:  %s   LY:   %s   DMA:  %s",
      std::bitset<8>(state.memory.GetMemoryLocationData(Addr_SCX)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_LYC)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_LY)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_DMA)).to_string().c_str());
    ImGui::Text("BGP:  %s   OBP0: %s   OBP1: %s   WY:   %s",
      std::bitset<8>(state.memory.GetMemoryLocationData(Addr_BGP)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_OBP0)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_OBP1)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_WY)).to_string().c_str());
    ImGui::Text("WX:   %s   IF:   %s   IE:   %s",
      std::bitset<8>(state.memory.GetMemoryLocationData(Addr_WX)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_IF)).to_string().c_str(), std::bitset<8>(state.memory.GetMemoryLocationData(Addr_IE)).to_string().c_str());

    ImGui::End();
  }


  // Draw memory editor
  {
    if (bDebugDisplayVRAM)
    {
      static MemoryEditor memoryEditorVRAM;
      size_t dataSizeVRAM = Size_VRAM * sizeof(uint8_t);
      memoryEditorVRAM.DrawWindow("VRAM state.memory Editor", state.memory.GetMemoryLocation(Addr_VRAM), dataSizeVRAM);
      bDebugDisplayVRAM = memoryEditorVRAM.Open;
    }
    if (bDebugDisplayOAM)
    {
      static MemoryEditor memoryEditorOAM;
      size_t dataSizeOAM = Size_OAM * sizeof(uint8_t);
      memoryEditorOAM.DrawWindow("OAM state.memory Editor", state.memory.GetMemoryLocation(Addr_OAM), dataSizeOAM);
      bDebugDisplayOAM = memoryEditorOAM.Open;
    }
    if (bDebugDisplayWRAM)
    {
      static MemoryEditor memoryEditorWRAM;
      size_t dataSizeWRAM = Size_WRAM * sizeof(uint8_t);
      memoryEditorWRAM.DrawWindow("WRAM state.memory Editor", state.memory.GetMemoryLocation(Addr_WRAM), dataSizeWRAM);
      bDebugDisplayWRAM = memoryEditorWRAM.Open;
    }
    if (bDebugDisplayZRAM)
    {
      static MemoryEditor memoryEditorZRAM;
      size_t dataSizeZRAM = Size_ZRAM * sizeof(uint8_t);
      memoryEditorZRAM.DrawWindow("ZRAM state.memory Editor", state.memory.GetMemoryLocation(Addr_ZRAM), dataSizeZRAM);
      bDebugDisplayZRAM = memoryEditorZRAM.Open;
    }
    if (bEmulatorStarted && bDebugDisplayROM)
    {
      static MemoryEditor memoryEditorROM;
      size_t dataSizeROM = state.memory.cartridge.CART_ROM.size() * sizeof(unsigned char);
      memoryEditorROM.DrawWindow("ROM state.memory Editor", state.memory.cartridge.CART_ROM.data(), dataSizeROM);
      bDebugDisplayROM = memoryEditorROM.Open;
    }
    if (bEmulatorStarted && bDebugDisplayERAM)
    {
      static MemoryEditor memoryEditorERAM;
      size_t dataSizeERAM = state.memory.cartridge.ERAM.size() * sizeof(unsigned char);
      memoryEditorERAM.DrawWindow("ERAM state.memory Editor", state.memory.cartridge.ERAM.data(), dataSizeERAM);
      bDebugDisplayERAM = memoryEditorERAM.Open;
    }
  }

  // Draw debug gpu frames
  if (bDebugDisplayGPU)
  {
    ImGui::Begin("GPU Debug",&bDebugDisplayGPU);
    ImGui::Text("Background");
    ImGui::Image((void*)(intptr_t)GPU.bg_texture, ImVec2((float)GPU.width, (float)GPU.height));
    ImGui::Text("Window");
    ImGui::Image((void*)(intptr_t)GPU.window_texture, ImVec2((float)GPU.width, (float)GPU.height));
    ImGui::Text("Sprite");
    ImGui::Image((void*)(intptr_t)GPU.sprites_texture, ImVec2((float)GPU.width, (float)GPU.height));
    ImGui::End();
  }

  // Draw main emulator screen
  ImGui::Begin("GB Screen");
  ImGui::Image((void*)(intptr_t)GPU.final_texture, ImVec2(GPU.width * 2.0f, GPU.height * 2.0f), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
  ImGui::End();

  // Render everything
  ImGui::Render();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(window);
}




void DraconicEmulator::SaveState(int id)
{
  std::ofstream file;
  std::string filename = "./saves/" + state.memory.romName + "_" + std::to_string(id) + ".sav";
  file.open(filename, std::ios::binary | std::ios::trunc);

  if (!file.bad())
  {
    //state.memory.save_state(file);
    file.close();

    std::cout << "wrote save state " << id << std::endl;
  }
}

void DraconicEmulator::LoadState(int id)
{
  std::string filename = "./saves/" + state.memory.romName + "_" + std::to_string(id) + ".sav";
  std::ifstream file(filename, std::ios::binary);

  if (file.is_open())
  {
    //state.memory.load_state(file);
    file.close();

    std::cout << "loaded state " << id << std::endl;
  }
}
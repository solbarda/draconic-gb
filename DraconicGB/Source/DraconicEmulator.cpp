#include "DraconicEmulator.h"
#include <iostream>
#include "SDL_config.h"
#include "../ThirdParty/imgui/imgui_memory_editor.h"
#include "DraconicState.h"
#include "Utils.h"
#include <fstream>
#include <iosfwd>
#include "SDL_syswm.h"
#include <bitset>
#include <commdlg.h>


void DraconicEmulator::LoadROMAndStart(std::string romPath)
{
  state.memory.Reset();
  state.registers.Reset();
  // First we load the ROM into memory
  state.memory.LoadROM(romPath);
  // We set the bEmulatorStarted flag to true so the the CPU can perform its tasks
  bEmulatorStarted = true;
}

int DraconicEmulator::Start()
{
  //TestOpcodes();
  // Init the window, context and hardware
  Init();
  // Load a startup ROM

  // Tests
  //LoadROMAndStart("./ROM/CPU_instrs/individual/01-special.gb");
  //LoadROMAndStart("./ROM/CPU_instrs/individual/02-interrupts.gb");
  //LoadROMAndStart("./ROM/CPU_instrs/individual/03-op sp,hl.gb");
  //LoadROMAndStart("./ROM/CPU_instrs/individual/04-op r,imm.gb");
  //LoadROMAndStart("./ROM/CPU_instrs/individual/05-op rp.gb");
  //LoadROMAndStart("./ROM/CPU_instrs/individual/06-ld r,r.gb");
  //LoadROMAndStart("./ROM/CPU_instrs/individual/07-jr,jp,call,ret,rst.gb"); // FAIL
  //LoadROMAndStart("./ROM/CPU_instrs/individual/08-misc instrs.gb");
  //LoadROMAndStart("./ROM/CPU_instrs/individual/09-op r,r.gb");
  //LoadROMAndStart("./ROM/CPU_instrs/individual/10-bit ops.gb");
  //LoadROMAndStart("./ROM/CPU_instrs/individual/11-op a,(hl).gb");


  //LoadROMAndStart("./ROM/mealybug-tearoom/m2_win_en_toggle.gb");
  //LoadROMAndStart("./ROM/mealybug-tearoom/m3_bgp_change.gb");
  //LoadROMAndStart("./ROM/LinkAwakening.gb");
  //LoadROMAndStart("./ROM/Tetris (World) (Rev A).gb");
  //LoadROMAndStart("./ROM/Tetris (Japan) (En).gb");
  //LoadROMAndStart("./ROM/CPU_instrs.gb");
  //LoadROMAndStart("./ROM/bgbtest.gb");
  //LoadROMAndStart("./ROM/Kirby.gb");
  LoadROMAndStart("./ROM/2048.gb");
  
  // Start emulator main loop
  MainLoop();
  return 0;
}

void DraconicEmulator::Shutdown()
{
  // Cleanup ImGui
  ImGui_ImplOpenGL3_Shutdown();
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
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'docs/FONTS.txt' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);



  // Init the emulator hardware
  CPU.Init(&state);
  GPU.Init(&state, window, renderer);

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
    float cyclesPerFrame = state.CLOCK_SPEED / framerate;
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
      update_timers(state.numCycles);
      // Update the scanlines
      update_scanline(state.numCycles);
      //Perform the interrupts
      do_interrupts();
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
  // Poll and handle events (inputs, window resize, etc.)
   // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
   // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
   // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
   // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT)
      Finished = true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
      Finished = true;
    if (event.type == SDL_KEYDOWN) 
      key_pressed(event.key);
    if (event.type == SDL_KEYUP)
      key_released(event.key);
    
    
  }

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();


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
        ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = "Select a File, yo!";
        ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameA(&ofn))
        {
          LoadROMAndStart(filename);
        }

      }
      if (ImGui::BeginMenu("Open Preset Files"))
      {
        if (ImGui::MenuItem("CPU_instrs.gb"))
          LoadROMAndStart("./ROM/CPU_instrs/CPU_instrs.gb");
        if (ImGui::MenuItem("01-special.gb"))
          LoadROMAndStart("./ROM/CPU_instrs/individual/01-special.gb");
        if (ImGui::MenuItem("Link.gb"))
          LoadROMAndStart("./ROM/Link.gb");
        if (ImGui::MenuItem("Kirby.gb"))
          LoadROMAndStart("./ROM/Kirby.gb");
        if (ImGui::MenuItem("Tetris.gb"))
          LoadROMAndStart("./ROM/Tetris.gb");
        if (ImGui::MenuItem("Mona_And_The_Witch_Hat.gb"))
          LoadROMAndStart("./ROM/zgb/Mona_And_The_Witch_Hat.gb");
        if (ImGui::MenuItem("PrettyPrincessCastleEscape.gb"))
          LoadROMAndStart("./ROM/zgb/PrettyPrincessCastleEscape.gb");
        if (ImGui::MenuItem("SuperPrincess.gb"))
          LoadROMAndStart("./ROM/zgb/SuperPrincess.gb");
        if(ImGui::MenuItem("tobu.gb"))
          LoadROMAndStart("./ROM/tobu.gb");
        if (ImGui::MenuItem("airaki.gb"))
          LoadROMAndStart("./ROM/airaki.gb");
        if (ImGui::MenuItem("2048.gb"))
          LoadROMAndStart("./ROM/2048.gb");
        
        ImGui::EndMenu();
      }
      ImGui::Separator();
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

  // Stats
  {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Stats");                          // Create a window called "Hello, world!" and append into it.
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
      std::bitset<8>(state.memory.P1.get()).to_string().c_str(), std::bitset<8>(state.memory.DIV.get()).to_string().c_str(), std::bitset<8>(state.memory.TIMA.get()).to_string().c_str(), std::bitset<8>(state.memory.TMA.get()).to_string().c_str());
    ImGui::Text("TAC:  %s   LCDC: %s   STAT: %s   SCY:  %s",
      std::bitset<8>(state.memory.TAC.get()).to_string().c_str(), std::bitset<8>(state.memory.LCDC.get()).to_string().c_str(), std::bitset<8>(state.memory.STAT.get()).to_string().c_str(), std::bitset<8>(state.memory.SCY.get()).to_string().c_str());
    ImGui::Text("SCX:  %s   LYC:  %s   LY:   %s   DMA:  %s",
      std::bitset<8>(state.memory.SCX.get()).to_string().c_str(), std::bitset<8>(state.memory.LYC.get()).to_string().c_str(), std::bitset<8>(state.memory.LY.get()).to_string().c_str(), std::bitset<8>(state.memory.DMA.get()).to_string().c_str());
    ImGui::Text("BGP:  %s   OBP0: %s   OBP1: %s   WY:   %s",
      std::bitset<8>(state.memory.BGP.get()).to_string().c_str(), std::bitset<8>(state.memory.OBP0.get()).to_string().c_str(), std::bitset<8>(state.memory.OBP1.get()).to_string().c_str(), std::bitset<8>(state.memory.WY.get()).to_string().c_str());
    ImGui::Text("WX:   %s   IF:   %s   IE:   %s",
      std::bitset<8>(state.memory.WX.get()).to_string().c_str(), std::bitset<8>(state.memory.IF.get()).to_string().c_str(), std::bitset<8>(state.memory.IE.get()).to_string().c_str());

    ImGui::End();
  }


  // state.memory editor
  {
    if (bDebugDisplayVRAM)
    {
      static MemoryEditor memoryEditorVRAM;
      size_t dataSizeVRAM = state.memory.VRAM.size() * sizeof(unsigned char);
      memoryEditorVRAM.DrawWindow("VRAM state.memory Editor", state.memory.VRAM.data(), dataSizeVRAM);
      bDebugDisplayVRAM = memoryEditorVRAM.Open;
    }
    if (bDebugDisplayOAM)
    {
      static MemoryEditor memoryEditorOAM;
      size_t dataSizeOAM = state.memory.OAM.size() * sizeof(unsigned char);
      memoryEditorOAM.DrawWindow("OAM state.memory Editor", state.memory.OAM.data(), dataSizeOAM);
      bDebugDisplayOAM = memoryEditorOAM.Open;
    }
    if (bDebugDisplayWRAM)
    {
      static MemoryEditor memoryEditorWRAM;
      size_t dataSizeWRAM = state.memory.WRAM.size() * sizeof(unsigned char);
      memoryEditorWRAM.DrawWindow("WRAM state.memory Editor", state.memory.WRAM.data(), dataSizeWRAM);
      bDebugDisplayWRAM = memoryEditorWRAM.Open;
    }
    if (bDebugDisplayZRAM)
    {
      static MemoryEditor memoryEditorZRAM;
      size_t dataSizeZRAM = state.memory.ZRAM.size() * sizeof(unsigned char);
      memoryEditorZRAM.DrawWindow("ZRAM state.memory Editor", state.memory.ZRAM.data(), dataSizeZRAM);
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

  if (bDebugDisplayGPU)
  {
    ImGui::Begin("GPU Debug",&bDebugDisplayGPU);
    ImGui::Text("Background");
    ImGui::Image((void*)(intptr_t)GPU.bg_texture, ImVec2(GPU.width, GPU.height));
    ImGui::Text("Window");
    ImGui::Image((void*)(intptr_t)GPU.window_texture, ImVec2(GPU.width, GPU.height));
    ImGui::Text("Sprite");
    ImGui::Image((void*)(intptr_t)GPU.sprites_texture, ImVec2(GPU.width, GPU.height));
    ImGui::End();
  }
  


  ImGui::Begin("GB Screen");
  ImGui::Image((void*)(intptr_t)GPU.final_texture, ImVec2(GPU.width*2, GPU.height*2), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
  ImGui::End();

 

  // Rendering
  ImGui::Render();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(window);
}


// Hanlde window events and IO
void DraconicEmulator::handle_events()
{
  /*sf::Event event;

  while (gpu.window.pollEvent(event))
  {
    switch (event.type)
    {
    case sf::Event::Closed:
      gpu.window.close();
      break;
    case sf::Event::KeyPressed:
      key_pressed(event.key.code);
      break;
    case sf::Event::KeyReleased:
      key_released(event.key.code);
      break;
    }
  }*/
}

void DraconicEmulator::key_pressed(SDL_KeyboardEvent key)
{
  
  //// Function keys F1 thru F12
  //if (key.keysym.sym >= 85 && key.keysym.sym <= 96)
  //{
  //  int id = key.keysym.sym - 84;
  //  if (sf::Keyboard::isKeyPressed(Key::LShift))
  //    save_state(id);
  //  else
  //    load_state(id);
  //  return;
  //}

  if (key.keysym.sym == SDLK_SPACE)
  {
    state.CLOCK_SPEED *= 100;
    return;
  }

  int key_id = get_key_id(key.keysym);

  if (key_id < 0)
    return;

  bool directional = false;

  if (key.keysym.sym == SDLK_UP || key.keysym.sym == SDLK_DOWN || key.keysym.sym == SDLK_LEFT|| key.keysym.sym == SDLK_RIGHT)
  {
    directional = true;
  }

  uint8_t joypad = (directional) ? state.memory.joypad_arrows : state.memory.joypad_buttons;
  bool unpressed = is_bit_set(joypad, key_id);

  if (!unpressed)
    return;

  if (directional)
    state.memory.joypad_arrows = clear_bit(joypad, key_id);
  else
    state.memory.joypad_buttons = clear_bit(joypad, key_id);

  request_interrupt(INTERRUPT_JOYPAD);
}

void DraconicEmulator::key_released(SDL_KeyboardEvent key)
{
  if (key.keysym.sym == SDLK_SPACE)
  {
    state.CLOCK_SPEED /= 100;
  }

  int key_id = get_key_id(key.keysym);

  if (key_id < 0)
    return;

  bool directional = false;

  if (key.keysym.sym == SDLK_UP || key.keysym.sym == SDLK_DOWN || key.keysym.sym == SDLK_LEFT || key.keysym.sym == SDLK_RIGHT)
  {
    directional = true;
  }

  uint8_t joypad = (directional) ? state.memory.joypad_arrows : state.memory.joypad_buttons;
  bool unpressed = is_bit_set(joypad, key_id);

  if (unpressed)
    return;

  if (directional)
    state.memory.joypad_arrows = set_bit(joypad, key_id);
  else
    state.memory.joypad_buttons = set_bit(joypad, key_id);
}

int DraconicEmulator::get_key_id(SDL_Keysym key)
{
  switch (key.sym)
  {
  case SDLK_z:
  case SDLK_RIGHT:
    return BIT_0;
  case SDLK_x: // B
  case SDLK_LEFT:
    return BIT_1;
  case SDLK_s: // select
  case SDLK_UP:
    return BIT_2;
  case SDLK_a:
  case SDLK_DOWN:
    return BIT_3;
  default:
    return -1;
  }
}

void DraconicEmulator::update_divider(int cycles)
{
  divider_counter += cycles;

  if (divider_counter >= 256) // 16384 Hz
  {
    divider_counter = 0;
    state.memory.DIV.set(state.memory.DIV.get() + 1);
  }
}

// Opcode cycle number may need adjusted, used Nintendo values
void DraconicEmulator::update_timers(int cycles)
{
  update_divider(cycles);

  // This can be optimized if needed
  uint8_t new_freq = get_timer_frequency();

  if (timer_frequency != new_freq)
  {
    set_timer_frequency();
    timer_frequency = new_freq;
  }

  if (timer_enabled())
  {
    timer_counter -= cycles;

    // enough CPU clock cycles have happened to update timer
    if (timer_counter <= 0)
    {
      uint8_t timer_value = state.memory.TIMA.get();
      set_timer_frequency();

      // Timer will overflow, generate interrupt
      if (timer_value == 255)
      {
        state.memory.TIMA.set(state.memory.TMA.get());
        request_interrupt(INTERRUPT_TIMER);
      }
      else
      {
        state.memory.TIMA.set(timer_value + 1);
      }
    }
  }
}

bool DraconicEmulator::timer_enabled()
{
  return state.memory.TAC.is_bit_set(BIT_2);
}

uint8_t DraconicEmulator::get_timer_frequency()
{
  return (state.memory.TAC.get() & 0x3);
}

void DraconicEmulator::set_timer_frequency()
{
  uint8_t frequency = get_timer_frequency();
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

void DraconicEmulator::request_interrupt(uint8_t id)
{
  state.memory.IF.set_bit(id);
}

void DraconicEmulator::do_interrupts()
{
  // If there are any interrupts set
  if (state.memory.IF.get() > 0)
  {
    // Resume  CPU state if halted and interrupts are pending
    if (state.memory.IE.get() > 0)
    {
      
      if (state.halted)
      {
        state.halted = false;
        state.registers.PC += 1;
      }
    }
    // Loop through each bit and call interrupt for lowest . highest priority bits set
    for (int i = 0; i < 5; i++)
    {
      if (state.memory.IF.is_bit_set(i))
      {
        if (state.memory.IE.is_bit_set(i))
        {
          // IME only disables the servicing of interrupts,
          // not all interrupt functionality 
          if (state.interrupt_master_enable)
          {
            service_interrupt(i);
          }
        }
      }
    }
  }
}

void DraconicEmulator::service_interrupt(uint8_t id)
{
  state.interrupt_master_enable = false;
  state.memory.IF.clear_bit(id);

  // Push current execution address to stack
  state.memory.Write(--state.registers.SP, high_byte(state.registers.PC));
  state.memory.Write(--state.registers.SP, low_byte(state.registers.PC));

  switch (id)
  {
  case INTERRUPT_VBLANK: state.registers.PC = 0x40; break;
  case INTERRUPT_LCDC:   state.registers.PC = 0x48; break;
  case INTERRUPT_TIMER:  state.registers.PC = 0x50; break;
  case INTERRUPT_SERIAL: state.registers.PC = 0x58; break;
  case INTERRUPT_JOYPAD: state.registers.PC = 0x60; break;
  }
}

void DraconicEmulator::set_lcd_status()
{
  uint8_t status = state.memory.STAT.get();

  uint8_t current_line = state.memory.LY.get();
  // extract current LCD mode
  uint8_t current_mode = status & 0x03;

  uint8_t mode = 0;
  bool do_interrupt = false;

  // in VBLANK, set mode to 1
  if (current_line >= 144)
  {
    mode = 1; // In vertical blanking period
    // 1 binary
    status = set_bit(status, BIT_0);
    status = clear_bit(status, BIT_1);
    do_interrupt = is_bit_set(status, BIT_4);

  }
  else
  {
    int mode2_threshold = 456 - 80;
    int mode3_threshold = mode2_threshold - 172;

    if (scanline_counter >= mode2_threshold)
    {
      mode = 2; // Searching OAM RAM
      // 2 binary
      status = set_bit(status, BIT_1);
      status = clear_bit(status, BIT_0);
      do_interrupt = is_bit_set(status, BIT_5);
    }
    else if (scanline_counter >= mode3_threshold)
    {
      mode = 3; // Transferring data to LCD driver
      // 3 binary
      status = set_bit(status, BIT_1);
      status = set_bit(status, BIT_0);
    }
    else
    {
      mode = 0; // CPU has access to all display RAM

      // If first time encountering H-blank, update the scanline
      if (current_mode != mode)
      {
        // draw current scanline to screen
        if (current_line < 144 && GPU.scanlines_rendered <= 144)
          GPU.update_scanline(current_line);
      }

      // 0 binary
      status = clear_bit(status, BIT_1);
      status = clear_bit(status, BIT_0);
      do_interrupt = is_bit_set(status, BIT_3);
    }
  }

  // Entered new mode, request interrupt
  if (do_interrupt && (mode != current_mode))
    request_interrupt(INTERRUPT_LCDC);

  // check coincidence flag, set bit 2 if it matches
  if (state.memory.LY.get() == state.memory.LYC.get())
  {
    status = set_bit(status, BIT_2);

    if (is_bit_set(status, BIT_6))
      request_interrupt(INTERRUPT_LCDC);
  }
  // clear bit 2 if not
  else
    status = clear_bit(status, BIT_2);

  state.memory.STAT.set(status);
  state.memory.video_mode = mode;
}

void DraconicEmulator::update_scanline(int cycles)
{
  scanline_counter -= cycles;

  set_lcd_status();

  if (state.memory.LY.get() > 153)
    state.memory.LY.clear();

  // Enough time has passed to draw the next scanline
  if (scanline_counter <= 0)
  {
    uint8_t current_scanline = state.memory.LY.get();

    // increment scanline and reset counter
    state.memory.LY.set(++current_scanline);
    scanline_counter = 456;

    // Entered VBLANK period
    if (current_scanline == 144)
    {
      request_interrupt(INTERRUPT_VBLANK);
      if (GPU.scanlines_rendered <= 144)
        GPU.render();
    }
    // Reset counter if past maximum
    else if (current_scanline > 153)
      state.memory.LY.clear();
  }
}

void DraconicEmulator::save_state(int id)
{
  std::ofstream file;
  std::string filename = "./saves/" + state.memory.romName + "_" + std::to_string(id) + ".sav";
  file.open(filename, std::ios::binary | std::ios::trunc);

  if (!file.bad())
  {
    CPU.SaveState(file);
    state.memory.save_state(file);
    file.close();

    std::cout << "wrote save state " << id << std::endl;
  }
}

void DraconicEmulator::load_state(int id)
{
  std::string filename = "./saves/" + state.memory.romName + "_" + std::to_string(id) + ".sav";
  std::ifstream file(filename, std::ios::binary);

  if (file.is_open())
  {
    CPU.LoadState(file);
    state.memory.load_state(file);
    file.close();

    std::cout << "loaded state " << id << std::endl;
  }
}
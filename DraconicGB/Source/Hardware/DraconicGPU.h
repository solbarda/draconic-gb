#pragma once
#include "SDL_config.h"
#include "SDL_syswm.h"
#include "../ThirdParty/glad/include/glad/glad.h"
#include "Utils/Utils.h"

class DraconicGPU
{

private:
  int scanline_counter = 456;
  void SetLCDStatus();
public:
  class DraconicState* state;
	int scanlines_rendered = 0;

  // debug variables
  bool debug_enabled = false;
  bool force_bg_map = false;
  bool force_bg_loc = false;


  const int width = 160;  
  const int height = 144;

  GLuint bg_texture;
  GLuint window_texture;
  GLuint sprites_texture;
  GLuint final_texture;

  uint8_t* bg_data;
  uint8_t* window_data;
  uint8_t* sprite_data;
  uint8_t* final_data;

  PixelColor shades_of_gray[4];

  void SetColorPalette(PixelColor Darkest, PixelColor Dark, PixelColor Light, PixelColor Lightest);



  struct SDL_Window* window;
  struct SDL_Renderer* renderer;

public:
	void Init(class DraconicState* newState, struct SDL_Window* newWindow, struct SDL_Renderer* newRenderer);
  void UpdateScanline();
  // Scanline updating
  void UpdateScanline(uint8_t current_scanline);
  // Output all scanlines as a single frame
  void Render();

  void update_bg_scanline(uint8_t current_scanline);
  void update_window_scanline(uint8_t current_scanline);
  void update_bg_tile_pixel(uint8_t palette, int display_x, int display_y, int tile_x, int tile_y, uint8_t tile_id);
  void update_window_tile_pixel(uint8_t palette, int display_x, int display_y, int tile_x, int tile_y, uint8_t tile_id);
  bool is_lcd_enabled();
  PixelColor get_pixel_color(uint8_t palette, uint8_t top, uint8_t bottom, int bit, bool is_sprite);
  void clear_window();
  void render_sprites();
  void render_sprite_tile(uint8_t pallete, int start_x, int start_y, uint8_t tile_id, uint8_t flags);
};


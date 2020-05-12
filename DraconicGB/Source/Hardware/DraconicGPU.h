#pragma once
#include "SDL_config.h"
#include "SDL_syswm.h"
#include "../ThirdParty/glad/include/glad/glad.h"

struct PixelColor
{
  PixelColor() 
  {

  }
  PixelColor(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
  {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
  }
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

class DraconicGPU
{
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

  uint8_t* bg_data;
  uint8_t* window_data;
  uint8_t* sprite_data;

  PixelColor shades_of_gray[4];



  struct SDL_Window* window;
  struct SDL_Renderer* renderer;

public:
	void Init(class DraconicState* newState, struct SDL_Window* newWindow, struct SDL_Renderer* newRenderer);
  // Scanline updating
  void update_scanline(uint8_t current_scanline);
  // Output all scanlines as a single frame
  void render();

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


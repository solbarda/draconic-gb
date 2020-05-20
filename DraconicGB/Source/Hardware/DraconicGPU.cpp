#include "DraconicGPU.h"
#include "DraconicMemory.h"
#include "../DraconicState.h"
#include "Utils.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "../ThirdParty/glad/include/glad/glad.h"

void DraconicGPU::SetColorPalette(PixelColor Darkest, PixelColor Dark, PixelColor Light, PixelColor Lightest)
{
  shades_of_gray[0x0] = Lightest; // 0x0 - White
  shades_of_gray[0x1] = Light; // 0x1 - Light Gray
  shades_of_gray[0x2] = Dark; // 0x2 - Drak Gray
  shades_of_gray[0x3] = Darkest;       // 0x3 - Black/**/
}

void DraconicGPU::Init(DraconicState* newState, SDL_Window* newWindow, SDL_Renderer* newRenderer)
{
  state = newState;
  window = newWindow;
  renderer = newRenderer;

 /* SetColorPalette(
      PixelColor(0, 0, 0, 255),
      PixelColor(127, 127, 127, 255),
      PixelColor(198, 198, 198, 255),
      PixelColor(255, 255, 255, 255)
  );*/

  SetColorPalette(
    PixelColor(90, 57, 33, 255),
    PixelColor(107, 140, 66, 255),
    PixelColor(123, 198, 123, 255),
    PixelColor(255, 255, 181, 255)
  );

  bg_data = new unsigned char[160 * 144 * 4];
  window_data = new unsigned char[160 * 144 * 4];
  sprite_data = new unsigned char[160 * 144 * 4];
  final_data = new unsigned char[160 * 144 * 4];

  for (int i = 0; i < 160 * 144 * 4; ++i)
  {
    bg_data[i] = 64;
  }

  for (int i = 0; i < 160 * 144 * 4; ++i)
  {
    window_data[i] = 64;
  }

  for (int i = 0; i < 160 * 144 * 4; ++i)
  {
    sprite_data[i] = 64;
  }

  for (int i = 0; i < 160 * 144 * 4; ++i)
  {
    final_data[i] = 64;
  }



  // Create a OpenGL texture identifier
  glGenTextures(1, &bg_texture);
  glBindTexture(GL_TEXTURE_2D, bg_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Upload pixels into texture
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bg_data);

  // Enable textures
  glEnable(GL_TEXTURE_2D);



  // Create a OpenGL texture identifier
  glGenTextures(1, &window_texture);
  glBindTexture(GL_TEXTURE_2D, window_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Upload pixels into texture
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, window_data);



  // Create a OpenGL texture identifier
  glGenTextures(1, &sprites_texture);
  glBindTexture(GL_TEXTURE_2D, sprites_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Upload pixels into texture
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite_data);


  // Create a OpenGL texture identifier
  glGenTextures(1, &final_texture);
  glBindTexture(GL_TEXTURE_2D, final_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Upload pixels into texture
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, final_data);

}


void DraconicGPU::SetLCDStatus()
{
  uint8_t status = state->memory.GetMemoryLocationData(Addr_STAT);

  uint8_t current_line = state->memory.GetMemoryLocationData(Addr_LY);
  // extract current LCD mode
  uint8_t current_mode = status & 0x03;

  uint8_t mode = 0;
  bool do_interrupt = false;

  // in VBLANK, set mode to 1
  if (current_line >= 144)
  {
    mode = 1; // In vertical blanking period
    // 1 binary
    status = SetBit(status, EBit::BIT_0);
    status = ClearBit(status, EBit::BIT_1);
    do_interrupt = IsBitSet(status, EBit::BIT_4);

  }
  else
  {
    int mode2_threshold = 456 - 80;
    int mode3_threshold = mode2_threshold - 172;

    if (scanline_counter >= mode2_threshold)
    {
      mode = 2; // Searching OAM RAM
      // 2 binary
      status = SetBit(status, EBit::BIT_1);
      status = ClearBit(status, EBit::BIT_0);
      do_interrupt = IsBitSet(status, EBit::BIT_5);
    }
    else if (scanline_counter >= mode3_threshold)
    {
      mode = 3; // Transferring data to LCD driver
      // 3 binary
      status = SetBit(status, EBit::BIT_1);
      status = SetBit(status, EBit::BIT_0);
    }
    else
    {
      mode = 0; // CPU has access to all display RAM

      // If first time encountering H-blank, update the scanline
      if (current_mode != mode)
      {
        // draw current scanline to screen
        if (current_line < 144 && scanlines_rendered <= 144)
          UpdateScanline(current_line);
      }

      // 0 binary
      status = ClearBit(status, EBit::BIT_1);
      status = ClearBit(status, EBit::BIT_0);
      do_interrupt = IsBitSet(status, EBit::BIT_3);
    }
  }

  // Entered new mode, request interrupt
  if (do_interrupt && (mode != current_mode))
    *state->memory.GetMemoryLocation(Addr_IF) = SetBit(state->memory.GetMemoryLocationData(Addr_IF), (EBit)EInterrupt::INTERRUPT_LCDC);

  // check coincidence flag, set bit 2 if it matches
  if(state->memory.GetMemoryLocationData(Addr_LY) == state->memory.GetMemoryLocationData(Addr_LYC))
  {
    status = SetBit(status, EBit::BIT_2);

    if (IsBitSet(status, EBit::BIT_6))
      *state->memory.GetMemoryLocation(Addr_IF) = SetBit(state->memory.GetMemoryLocationData(Addr_IF), (EBit)EInterrupt::INTERRUPT_LCDC);
  }
  // clear bit 2 if not
  else
    status = ClearBit(status, EBit::BIT_2);

  *state->memory.GetMemoryLocation(Addr_STAT) = status;
  state->memory.video_mode = mode;
}

void DraconicGPU::UpdateScanline()
{
  int cycles = state->numCycles;
  scanline_counter -= cycles;

  SetLCDStatus();

  if (state->memory.GetMemoryLocationData(Addr_LY) > 153)
    *state->memory.GetMemoryLocation(Addr_LY) = 0x00;

  // Enough time has passed to draw the next scanline
  if (scanline_counter <= 0)
  {
    uint8_t current_scanline = state->memory.GetMemoryLocationData(Addr_LY);

    // increment scanline and reset counter
    current_scanline++;
    *state->memory.GetMemoryLocation(Addr_LY) = current_scanline;
    scanline_counter = 456;

    // Entered VBLANK period
    if (current_scanline == 144)
    {
      // Interrupt
      *state->memory.GetMemoryLocation(Addr_IF) = SetBit(state->memory.GetMemoryLocationData(Addr_IF), (EBit)EInterrupt::INTERRUPT_VBLANK);
      if (scanlines_rendered <= 144)
        Render();
    }
    // Reset counter if past maximum
    else if (current_scanline > 153)
      *state->memory.GetMemoryLocation(Addr_LY) = 0x00;
  }
}

void DraconicGPU::UpdateScanline(uint8_t current_scanline)
{
  scanlines_rendered++;

  bool do_background = IsBitSet(state->memory.GetMemoryLocationData(Addr_LCDC), EBit::BIT_0);
  bool do_window = IsBitSet(state->memory.GetMemoryLocationData(Addr_LCDC), EBit::BIT_5);

  if (do_background)
    update_bg_scanline(current_scanline);

  if (do_window)
    update_window_scanline(current_scanline);
}

void DraconicGPU::Render()
{
  if (!is_lcd_enabled())
    return;


  // clear existig sprite and window data
  for (int i = 0; i < 160 * 144 * 4; ++i)
  {
    sprite_data[i] = 0;
  }

  bool do_sprites = IsBitSet(state->memory.GetMemoryLocationData(Addr_LCDC), EBit::BIT_1);

  if (do_sprites)
    render_sprites();

  for (int i = 0; i < 160 * 144; ++i)
  {
    final_data[i * 4] = bg_data[i * 4];
    final_data[i * 4+1] = bg_data[i * 4+1];
    final_data[i * 4+2] = bg_data[i * 4+2];
    final_data[i * 4+3] = bg_data[i * 4+3];
    if (sprite_data[i*4 + 3] > 0)
    {
      final_data[i * 4] = sprite_data[i * 4];
      final_data[i * 4 + 1] = sprite_data[i * 4 + 1];
      final_data[i * 4 + 2] = sprite_data[i * 4 + 2];
      final_data[i * 4 + 3] = sprite_data[i * 4 + 3];
    }
  }


  glBindTexture(GL_TEXTURE_2D, bg_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bg_data);
  glBindTexture(GL_TEXTURE_2D, window_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, window_data);
  glBindTexture(GL_TEXTURE_2D, sprites_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite_data);
  glBindTexture(GL_TEXTURE_2D, final_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, final_data);

}

void DraconicGPU::update_bg_scanline(uint8_t current_scanline)
{
  bool bg_code_area = IsBitSet(state->memory.GetMemoryLocationData(Addr_LCDC), EBit::BIT_3);

  if (debug_enabled)
  {
    bg_code_area = force_bg_map;
  }

  uint16_t tile_map_location = (bg_code_area) ? 0x9C00 : 0x9800;

  uint8_t scroll_x = state->memory.GetMemoryLocationData(Addr_SCX);
  uint8_t scroll_y = state->memory.GetMemoryLocationData(Addr_SCY);

  uint8_t palette = state->memory.GetMemoryLocationData(Addr_BGP);

  // For each pixel in the 160x1 scanline:
  // 1. Calculate where the pixel resides in the overall 256x256 background map
  // 2. Get the tile ID where that pixel is located
  // 3. Get the pixel color based on that coordinate relative to the 8x8 tile grid
  // 4. Plot pixel in 160x144 display view

  int y = current_scanline;

  // Iterate from left to right of display screen (x = 0 -> 160)
  for (int x = 0; x < 160; x++)
  {
    // 1. Get pixel X,Y in overall background map, offset by ScrollX & Y
    int map_x = (int)scroll_x + x;
    int map_y = (int)scroll_y + y;

    // Adjust map coordinates if they exceed the 256x256 area to loop around
    map_x = (map_x >= 256) ? map_x - 256 : map_x;
    map_y = (map_y >= 256) ? map_y - 256 : map_y;

    // 2. Get the tile ID where that pixel is located
    int tile_col = (int)floor(map_x / 8);
    int tile_row = (int)floor(map_y / 8);
    int tile_map_id = (tile_row * 32) + tile_col;
    uint16_t loc = tile_map_location + tile_map_id;
    uint8_t tile_id = state->memory.Read(loc);

    // 3. Get the pixel color based on that coordinate relative to the 8x8 tile grid
    // 4. Plot pixel in 160x144 display view
    int tile_x_pixel = map_x % 8;
    int tile_y_pixel = map_y % 8;

    // Invert x pixels because they are stored backwards
    tile_x_pixel = abs(tile_x_pixel - 7);

    update_bg_tile_pixel(palette, x, y, tile_x_pixel, tile_y_pixel, tile_id);
  }
}

void DraconicGPU::update_window_scanline(uint8_t current_scanline)
{
  // Get current window tile map
  bool window_code_area = IsBitSet(state->memory.GetMemoryLocationData(Addr_LCDC), EBit::BIT_6);

  uint16_t tile_map_location = (window_code_area) ? 0x9C00 : 0x9800;

  int window_x = (int)state->memory.GetMemoryLocationData(Addr_WX);
  int window_y = (int)state->memory.GetMemoryLocationData(Addr_WY);

  uint8_t palette = state->memory.GetMemoryLocationData(Addr_BGP);
  
  // For each pixel in the 160x1 scanline:
  // 1. Calculate where the pixel resides in the overall 256x256 background map
  // 2. Get the tile ID where that pixel is located
  // 3. Get the pixel color based on that coordinate relative to the 8x8 tile grid
  // 4. Plot pixel in 160x144 display view

  int y = (int)current_scanline;
  // Iterate from left to right of display screen (x = 0 -> 160)
  for (int x = 0; x < 160; x++)
  {
    // WINDOW IS RELATIVE TO THE SCREEN
    // Shift X & Y pixels based on window register value
    int display_x = x + window_x - 7;
    int display_y = y;

    // 1. Get the tile ID where that pixel is located
    int tile_col = (int)floor(x / 8);
    int tile_row = (int)floor((y - window_y) / 8);
    int tile_map_id = (tile_row * 32) + tile_col;
    uint16_t loc = tile_map_location + tile_map_id;
    uint8_t tile_id = state->memory.Read(loc);

    // 2. Get the pixel color based on that coordinate relative to the 8x8 tile grid
    // 3. Plot pixel in 160x144 display view
    int tile_x_pixel = x % 8;
    int tile_y_pixel = y % 8;

    // Invert x pixels because they are stored backwards
    tile_x_pixel = abs(tile_x_pixel - 7);

    if (current_scanline == 128)
      bool breakpoint = true;

    if (current_scanline < window_y)
    {
      //window_array.setPixel(x, y, sf::Color::Transparent);
    }
    else
    {
      //window_array.setPixel(x, y, sf::Color::Blue);
      update_window_tile_pixel(palette, display_x, display_y, tile_x_pixel, tile_y_pixel, tile_id);
    }
  }
}

void DraconicGPU::update_bg_tile_pixel(uint8_t palette, int display_x, int display_y, int tile_x, int tile_y, uint8_t tile_id)
{
  bool bg_char_selection = IsBitSet(state->memory.GetMemoryLocationData(Addr_LCDC), EBit::BIT_4);

  if (debug_enabled)
  {
    bg_char_selection = force_bg_loc;
  }

  // Figure out where the current background character data is being stored
  // if selection=0 bg area is 0x8800-0x97FF and tile ID is determined by SIGNED -128 to 127
  // 0x9000 represents the zero ID address in that range
  uint16_t bg_data_location = (bg_char_selection) ? 0x8000 : 0x9000;
  uint16_t offset;

  // 0x8000 - 0x8FFF unsigned 
  if (bg_char_selection)
  {
    offset = (tile_id * 16) + bg_data_location;
  }
  // 0x8800 - 0x97FF signed
  else
  {
    int8_t direction = (int8_t)tile_id;
    uint16_t temp_offset = (bg_data_location)+(direction * 16);
    offset = (uint16_t)temp_offset;
  }

  uint8_t
    high = state->memory.Read(offset + (tile_y * 2) + 1),
    low = state->memory.Read(offset + (tile_y * 2));

  PixelColor color= get_pixel_color(palette, low, high, tile_x, false);
  bg_data[display_y * 160 * 4 + display_x * 4] = color.r;
  bg_data[display_y * 160 * 4 + display_x * 4+1] = color.g;
  bg_data[display_y * 160 * 4 + display_x * 4+2] = color.b;
  bg_data[display_y * 160 * 4 + display_x * 4+3] = color.a;

}

void DraconicGPU::update_window_tile_pixel(uint8_t palette, int display_x, int display_y, int tile_x, int tile_y, uint8_t tile_id)
{
  if (display_x >= 160 || display_x < 0)
    return;
  if (display_y >= 144 || display_y < 0)
    return;

  //bool bg_char_selection = state->memory.LCDC.is_bit_set(EBit::BIT_4);
  bool bg_char_selection = IsBitSet(state->memory.GetMemoryLocationData(Addr_LCDC), EBit::BIT_4);

  if (debug_enabled)
  {
    bg_char_selection = force_bg_loc;
  }

  // Figure out where the current background character data is being stored
  // if selection=0 bg area is 0x8800-0x97FF and tile ID is determined by SIGNED -128 to 127
  // 0x9000 represents the zero ID address in that range
  uint16_t bg_data_location = (bg_char_selection) ? 0x8000 : 0x9000;
  uint16_t offset;

  // 0x8000 - 0x8FFF unsigned 
  if (bg_char_selection)
  {
    offset = (tile_id * 16) + bg_data_location;
  }
  // 0x8800 - 0x97FF signed
  else
  {
    int8_t direction = (int8_t)tile_id;
    uint16_t temp_offset = (bg_data_location)+(direction * 16);
    offset = (uint16_t)temp_offset;
  }

  uint8_t
    high = state->memory.Read(offset + (tile_y * 2) + 1),
    low = state->memory.Read(offset + (tile_y * 2));

  PixelColor color = get_pixel_color(palette, low, high, tile_x, false);
  window_data[display_y * 160 * 4 + display_x * 4] = color.r;
  window_data[display_y * 160 * 4 + display_x * 4 + 1] = color.g;
  window_data[display_y * 160 * 4 + display_x * 4 + 2] = color.b;
  window_data[display_y * 160 * 4 + display_x * 4 + 3] = color.a;

}

bool DraconicGPU::is_lcd_enabled()
{
  return IsBitSet(state->memory.GetMemoryLocationData(Addr_LCDC), EBit::BIT_7);
}

PixelColor DraconicGPU::get_pixel_color(uint8_t palette, uint8_t top, uint8_t bottom, int bit, bool is_sprite)
{
  // Figure out what colors to apply to each color code based on the palette data
  uint8_t color_3_shade = (palette >> 6);        // extract bits 7 & 6
  uint8_t color_2_shade = (palette >> 4) & 0x03; // extract bits 5 & 4
  uint8_t color_1_shade = (palette >> 2) & 0x03; // extract bits 3 & 2
  uint8_t color_0_shade = (palette & 0x03);      // extract bits 1 & 0

  // Get color code from the two defining bytes
  uint8_t first = (uint8_t)IsBitSet(top, (EBit)bit);
  uint8_t second = (uint8_t)IsBitSet(bottom, (EBit)bit);
  uint8_t color_code = (second << 1) | first;

  PixelColor result;

  switch (color_code)
  {
  case 0x0: return (is_sprite) ? PixelColor(0,0,0,0) : shades_of_gray[color_0_shade];
  case 0x1: return shades_of_gray[color_1_shade];
  case 0x2: return shades_of_gray[color_2_shade];
  case 0x3: return shades_of_gray[color_3_shade];
  default:  return PixelColor(255, 0, 255, 0); // error color
  }
}

void DraconicGPU::clear_window()
{

  for (int i = 0; i < 160 * 144 * 4; ++i)
  {
    window_data[i] = 0;
  }
}

void DraconicGPU::render_sprites()
{
  uint16_t sprite_data_location = 0xFE00;
  uint8_t palette_0 = state->memory.GetMemoryLocationData(Addr_OBP0);
  uint8_t palette_1 = state->memory.GetMemoryLocationData(Addr_OBP1);

  bool use_8x16_sprites = IsBitSet(state->memory.GetMemoryLocationData(Addr_LCDC), EBit::BIT_2);

  // 160 bytes of sprite data / 4 bytes per sprite = 40 potential sprites to render maximum
  // Start at 39 -> to have right priority
  for (int sprite_id = 39; sprite_id >= 0; sprite_id--)
  {
    uint16_t offset = sprite_data_location + (sprite_id * 4);
    int y_pos = ((int)state->memory.Read(offset)) - 16;
    int x_pos = ((int)state->memory.Read(offset + 1)) - 8;

    uint8_t tile_id = state->memory.Read(offset + 2);
    uint8_t flags = state->memory.Read(offset + 3);

    bool use_palette_1 = IsBitSet(flags, EBit::BIT_4);
    uint8_t sprite_palette = (use_palette_1) ? palette_1 : palette_0;

    // If in 8x16 mode, the tile pattern for top is VAL & 0xFE
    // Lower 8x8 tile is VAL | 0x1

    if (use_8x16_sprites)
    {
      tile_id = tile_id & 0xFE;
      uint8_t tile_id_bottom = tile_id | 0x01;
      render_sprite_tile(sprite_palette, x_pos, y_pos, tile_id, flags);
      render_sprite_tile(sprite_palette, x_pos, y_pos + 8, tile_id_bottom, flags);
    }
    else
    {
      render_sprite_tile(sprite_palette, x_pos, y_pos, tile_id, flags);
    }
  }
}

void DraconicGPU::render_sprite_tile(uint8_t pallete, int start_x, int start_y, uint8_t tile_id, uint8_t flags)
{
  uint16_t sprite_data_location = 0x8000;

  bool priority = IsBitSet(flags, EBit::BIT_7);
  bool mirror_y = IsBitSet(flags, EBit::BIT_6);
  bool mirror_x = IsBitSet(flags, EBit::BIT_5);

  // If set to zero then sprite always rendered above bg
  // If set to 1, sprite is hidden behind the background and window
  // unless the color of the background or window is white, it's then rendered on top
  bool hide_sprite = IsBitSet(flags, EBit::BIT_7);

  for (int y = 0; y < 8; y++)
  {
    int offset = (tile_id * 16) + sprite_data_location;

    uint8_t
      high = state->memory.Read(offset + (y * 2) + 1),
      low = state->memory.Read(offset + (y * 2));

    for (int x = 0; x < 8; x++)
    {
      int pixel_x = (mirror_x) ? (start_x + x) : (start_x + 7 - x);
      int pixel_y = (mirror_y) ? (start_y + 7 - y) : (start_y + y);

      // prevent pixels from being drawn off screen
      //sf::Vector2u bounds = sprites_array.getSize();

      if (pixel_x < 0 || pixel_x >= width)
        continue;
      if (pixel_y < 0 || pixel_y >= height)
       continue;

      PixelColor color = get_pixel_color(pallete, low, high, x, true);

      PixelColor bg_color;
      bg_color.r = bg_data[pixel_y * 160 * 4 + pixel_x * 4];
      bg_color.g = bg_data[pixel_y * 160 * 4 + pixel_x * 4+1];
      bg_color.b = bg_data[pixel_y * 160 * 4 + pixel_x * 4+2];
      bg_color.a = bg_data[pixel_y * 160 * 4 + pixel_x * 4+3];

      if (priority)
      {
        if (bg_color.r != shades_of_gray[0x0].r && bg_color.g != shades_of_gray[0x0].g && bg_color.b != shades_of_gray[0x0].b && bg_color.a != shades_of_gray[0x0].a)
        {
          continue;
          //color = sf::Color::Transparent;
        }
      }
      sprite_data[pixel_y * 160 * 4 + pixel_x * 4] = color.r;
      sprite_data[pixel_y * 160 * 4 + pixel_x * 4 + 1] = color.g;
      sprite_data[pixel_y * 160 * 4 + pixel_x * 4 + 2] = color.b;
      sprite_data[pixel_y * 160 * 4 + pixel_x * 4 + 3] = color.a;
    }
  }
}

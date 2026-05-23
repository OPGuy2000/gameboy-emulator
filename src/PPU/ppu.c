#include "ppu.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

// Define addresses for PPU needed I/O registers
const unsigned short LCDC_ADDRESS = 0xFF40,       // LCD Control Register
    STAT_ADDRESS = 0xFF41,                        // LCD Status Register
    SCY_ADDRESS = 0xFF42, SCX_ADDRESS = 0xFF43,   // Scroll Y and X
    LY_ADDRESS = 0xFF44,                          // LCD Y Coordinate
    LYC_ADDRESS = 0xFF45,                         // LY Compare
    DMA_ADDRESS = 0xFF46,                         // DMA Transfer and Start Address
    BGP_ADDRESS = 0xFF47,                         // Background Palette Register
    OBP0_ADDRESS = 0xFF48, OBP1_ADDRESS = 0xFF49, // Object Palette 0 Register
    WY_ADDRESS = 0xFF4A, WX_ADDRESS = 0xFF4B;     // Window Y and X

struct OAMEntry {
    unsigned char x_pos;
    unsigned char tile_index;
    unsigned char attributes;
    unsigned char tile_row_offset;
};
struct OAMEntry oam_buffer[10];     // Buffer for OAM search results (up to 10 sprites per line)
static unsigned char oam_index = 0; // Index for OAM search results

static SDL_Window *window;              // Pointer to the SDL window
static SDL_Renderer *renderer;          // Pointer to SDL renderer
static unsigned char mode;              // Current PPU mode (0-3)
static unsigned short scanline_dot = 0; // Counts dots for timing

static SDL_Color get_color_from_palette(unsigned char palette, unsigned char color_id) {
    // Each color is represented by 2 bits in the palette
    unsigned char color_bits = (palette >> (color_id * 2)) & 0x03;
    switch (color_bits) {
    case 0:
        return (struct SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}; // White
    case 1:
        return (struct SDL_Color){0xAA, 0xAA, 0xAA, 0xFF}; // Light_Gray
    case 2:
        return (struct SDL_Color){0x55, 0x55, 0x55, 0xFF}; // Dark_Gray
    case 3:
        return (struct SDL_Color){0x00, 0x00, 0x00, 0xFF}; // Black
    default:
        return (struct SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}; // Default white
    }
}

static short mode3_penalty = 0;
void tick(unsigned char dots) {
    unsigned char lcdc = mem_read(LCDC_ADDRESS);
    unsigned lcdc_enabled = lcdc & 0x80;
    if (!lcdc_enabled) {
        mode = 2; // Reset PPU while off
        scanline_dot = 0;
        return;
    }
    if (scanline_dot == 0) {
        oam_index = 0; // Reset OAM index at the start of each scanline
    }
    // Dot counting per CPU instruction
    switch (mode) {
    case 0: // HBlank
    {
        scanline_dot += dots;
        if (scanline_dot >= 456) // Each scanline takes 456 dots
        {
            unsigned short diff = scanline_dot - 456;
            scanline_dot = 0;

            // Increment LY and check for LYC match
            unsigned char ly = mem_read(LY_ADDRESS);
            ly = (ly + 1) % 154; // LY goes from 0 to 153
            mem_write(LY_ADDRESS, ly);

            if (ly == 144)
                mode = 1; // Enter VBlank after the last visible line
            else
                mode = 2; // Start OAM Search for the next line

            tick(diff); // Process remaining dots
        }
        break;
    }
    case 1: // VBlank
            // ***do interrupt things later***!!
    {

        scanline_dot += dots;
        if (scanline_dot >= 456) // Each scanline takes 456 dots
        {
            unsigned short diff = scanline_dot - 456;
            scanline_dot = 0;

            // Increment LY and check for LYC match
            unsigned char ly = mem_read(LY_ADDRESS);
            ly = (ly + 1) % 154; // LY goes from 0 to 153
            mem_write(LY_ADDRESS, ly);

            if (ly == 0)
                mode = 2; // Start OAM Search for the next frame

            tick(diff); // Process remaining dots
        }
        break;
    }
    case 2: // OAM Search
    {
        unsigned char obj_size = lcdc & 0x04;
        unsigned char entries_to_read = dots / 2;
        if (dots % 2 != 0)
            printf("OAM Search received odd dot count: %d\n", dots);

        unsigned char ly = mem_read(LY_ADDRESS);
        for (int i = 0; i < entries_to_read && oam_index < 10; i++) {
            // Perform OAM search logic here
            unsigned char y_pos = mem_read(0xFE00 + oam_index * 4);
            unsigned char x_pos = mem_read(0xFE00 + oam_index * 4 + 1);
            unsigned char tile_index = mem_read(0xFE00 + oam_index * 4 + 2);
            unsigned char attributes = mem_read(0xFE00 + oam_index * 4 + 3);

            unsigned char sprite_y = y_pos - 16;
            unsigned char sprite_height = obj_size ? 16 : 8;
            if (ly >= sprite_y &&
                ly < sprite_y +
                         sprite_height) // Check if the sprite is visible on the current scanline
            {
                oam_buffer[oam_index].x_pos = x_pos - 8;
                oam_buffer[oam_index].tile_index = tile_index;
                oam_buffer[oam_index].attributes = attributes;
                oam_buffer[oam_index].tile_row_offset = ly - sprite_y;
                oam_index++;
            }
        }

        scanline_dot += dots;
        if (scanline_dot >= 80) // OAM Search takes 80 dots
        {
            unsigned short diff = scanline_dot - 80;
            scanline_dot -= diff;
            mode = 3;   // Enter Pixel Transfer
            tick(diff); // Process remaining dots
        }
        break;
    }
    case 3: // Pixel Transfer
    {
        unsigned char window_tile_map_select = lcdc & 0x40, window_enabled = lcdc & 0x20,
                      bg_and_window_tile_data_select = lcdc & 0x10,
                      bg_tile_map_select = lcdc & 0x08, obj_enabled = lcdc & 0x02,
                      bg_and_window_enabled = lcdc & 0x01;

        if (bg_and_window_enabled) {
            // Window Rendering
            if (window_enabled) {
            }

            // Background Rendering
        }

        // Sprite Rendering
        if (obj_enabled) {
        }

        break;
    }
    }
}

unsigned char get_mode() { return mode; }

int init_ppu() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    window = SDL_CreateWindow("SDL2 Pixel Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        printf("Window Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize PPU boot settings
    mode = 2;
    oam_index = 0;
    scanline_dot = 0;

    return 0;
}

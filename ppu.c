#include "ppu.h"

// Define addresses for PPU needed I/O registers
const unsigned short
    LCDC_ADDRESS = 0xFF40,                        // LCD Control Register
    STAT_ADDRESS = 0xFF41,                        // LCD Status Register
    SCY_ADDRESS = 0xFF42, SCX_ADDRESS = 0xFF43,   // Scroll Y and X
    LY_ADDRESS = 0xFF44,                          // LCD Y Coordinate
    LYC_ADDRESS = 0xFF45,                         // LY Compare
    DMA_ADDRESS = 0xFF46,                         // DMA Transfer and Start Address
    BGP_ADDRESS = 0xFF47,                         // Background Palette Register
    OBP0_ADDRESS = 0xFF48, OBP1_ADDRESS = 0xFF49, // Object Palette 0 Register
    WY_ADDRESS = 0xFF4A, WX_ADDRESS = 0xFF4B;

struct PPU
{
    Tigr *screen;                  // Screen variable
    unsigned short scanline_cycle; // Counts cycles for timing
    char mode;                     // PPU mode (0-3)
};

struct PPU ppu;

TPixel get_color_from_palette(unsigned char palette, unsigned char color_id)
{
    // Each color is represented by 2 bits in the palette
    unsigned char color_bits = (palette >> (color_id * 2)) & 0x03;
    switch (color_bits)
    {
    case 0:
        return tigrRGB(0xFF, 0xFF, 0xFF); // White
    case 1:
        return tigrRGB(0xAA, 0xAA, 0xAA); // Light Gray
    case 2:
        return tigrRGB(0x55, 0x55, 0x55); // Dark Gray
    case 3:
        return tigrRGB(0x00, 0x00, 0x00); // Black
    default:
        return tigrRGB(0xFF, 0xFF, 0xFF); // Default to white
    }
}

void tick(unsigned char cycles)
{
    // Cycle counting per CPU instruction
    switch (ppu.mode)
    {
    case 0: // HBlank
        ppu.scanline_cycle += cycles;
        if (ppu.scanline_cycle >= 456) // Each scanline takes 456 cycles
        {
            unsigned short diff = ppu.scanline_cycle - 456;
            ppu.scanline_cycle = 0;

            // Increment LY and check for LYC match
            unsigned char ly = read_byte(LY_ADDRESS);
            ly = (ly + 1) % 154; // LY goes from 0 to 153
            write_byte(LY_ADDRESS, ly);

            if (ly == 144)
                ppu.mode = 1; // Enter VBlank after the last visible line
            else
                ppu.mode = 2; // Start OAM Search for the next line

            tick(diff); // Process remaining cycles
        }
        break;
    case 1: // VBlank
            // ***do interrupt things later***!!
        ppu.scanline_cycle += cycles;
        if (ppu.scanline_cycle >= 456) // Each scanline takes 456 cycles
        {
            unsigned short diff = ppu.scanline_cycle - 456;
            ppu.scanline_cycle = 0;

            // Increment LY and check for LYC match
            unsigned char ly = read_byte(LY_ADDRESS);
            ly = (ly + 1) % 154; // LY goes from 0 to 153
            write_byte(LY_ADDRESS, ly);

            if (ly == 0)
                ppu.mode = 2; // Start OAM Search for the next frame

            tick(diff); // Process remaining cycles
        }
        break;
    case 2: // OAM Search
    case 3: // Pixel Transfer
    }
}

void ppu_init()
{
    ppu.screen = tigrWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Gameboy Emulator", 0);
    ppu.mode = 0; // Start in mode 0 (HBlank)
    ppu.scanline_cycle = 0;
}
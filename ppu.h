#ifndef PPU_H
#define PPU_H

#include "memory.h"
#include "tigr.h"

// Screen dimensions
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144


TPixel get_color_from_palette(unsigned char palette, unsigned char color_id);


#endif
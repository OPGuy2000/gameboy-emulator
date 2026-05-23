#pragma once

#ifndef PPU_H
#define PPU_H

#include "../memory.h"
#include <SDL2/SDL.h>

// Screen dimensions
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

void tick(unsigned char cycles);
unsigned char get_mode();
int init_ppu();

#endif

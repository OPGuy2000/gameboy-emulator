#pragma once

#ifndef PPU_H
#define PPU_H

#include "../memory.h"
#include <SDL2/SDL.h>

// Screen dimensions
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

void tick(uint8_t cycles);
uint8_t get_mode();
void init_ppu();

#endif

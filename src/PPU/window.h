#pragma once

#include "stdio.h"
#include <SDL2/SDL.h>

// Scaled screen dimensions
#define SCALE 6
#define SCREEN_WIDTH 160 * SCALE
#define SCREEN_HEIGHT 144 * SCALE

void present_frame();
void update_framebuffer(uint32_t color, uint8_t x, uint8_t y);
void window_init();

#pragma once

#include "stdio.h"
#include <SDL2/SDL.h>

// Scaled screen dimensions
#define SCREEN_WIDTH 160 * 4
#define SCREEN_HEIGHT 144 * 4

void present_frame();
void update_framebuffer(uint32_t color, uint8_t x, uint8_t y);
void window_init();

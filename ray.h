#ifndef RAY_H
#define RAY_H

#include "SDL_render.h"
#include <stdlib.h>

#define COLOR_WHITE (Color){.r = 255, .g = 255, .b = 255}

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Color;

void render_ray_tracing(SDL_Renderer *renderer);

#endif

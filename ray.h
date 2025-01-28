#ifndef RAY_H
#define RAY_H

#include "SDL_render.h"
#include <stdlib.h>

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Color;

typedef struct {
  float x;
  float y;
  float r;
} Circle;

void ray_render(SDL_Renderer *renderer);

#endif

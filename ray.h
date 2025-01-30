#ifndef RAY_H
#define RAY_H

#include "SDL_events.h"
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

void ray_render(SDL_Renderer *renderer, SDL_Event *event);
void ray_poll_event(SDL_Event *event);
void ray_load_textures(SDL_Window *window, SDL_Renderer *renderer);

#endif

#include "ray.h"
#include "SDL_render.h"

void render_ray_tracing(SDL_Renderer *renderer) {
  SDL_Rect rect = (SDL_Rect){.x = 200, .y = 200, .w = 200, .h = 200};
  SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b,
                         255);
  SDL_RenderFillRect(renderer, &rect);
}

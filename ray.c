#include "SDL_render.h"
#include "math.h"

#include "ray.h"
#include "raylib.h"

static void ray_render_grid(SDL_Renderer *renderer) {
  for (int i = CELL_W; i < WINDOW_W; i += CELL_W) {
    SDL_RenderDrawLine(renderer, i, 0, i + 1, WINDOW_H);
  }

  for (int i = CELL_H; i < WINDOW_H; i += CELL_H) {
    SDL_RenderDrawLine(renderer, 0, i, WINDOW_W, i + 1);
  }
}

static void ray_fill_circle(SDL_Renderer *renderer, Circle *c) {
  float r_squared = powf(c->r, 2.0);
  for (float i = c->x - c->r; i < c->x + c->r; i++) {
    for (float j = c->y - c->r; j < c->y + c->r; j++) {
      if (powf(i - c->x, 2.0) + powf(j - c->y, 2.0) < r_squared) {
        SDL_RenderDrawPoint(renderer, i, j);
      }
    }
  }
}

static void ray_trace(SDL_Renderer *renderer, Circle *c) {
  float angle = M_PI / 8;
  float i = 0;
  for (;;) {
    float x = i * cos(angle);
    float y = i * sin(angle);
    if (x < 0 || x >= WINDOW_W || y < 0 || y >= WINDOW_H)
      break;
    SDL_RenderDrawPoint(renderer, c->x + x, c->y + y);
    i++;
  }
}

void ray_render(SDL_Renderer *renderer) {
  SDL_Rect rect = (SDL_Rect){.x = 200, .y = 200, .w = 200, .h = 200};
  SDL_SetRenderDrawColor(renderer, COLOR_WHITE.r, COLOR_WHITE.g, COLOR_WHITE.b,
                         128);
  ray_render_grid(renderer);

  Circle c1 = {.x = 350, .y = 400, .r = 5};
  Circle c2 = {.x = 1200, .y = 600, .r = 200};
  ray_fill_circle(renderer, &c1);
  ray_fill_circle(renderer, &c2);

  SDL_SetRenderDrawColor(renderer, COLOR_YELLOW.r, COLOR_YELLOW.g,
                         COLOR_YELLOW.b, 128);
  ray_trace(renderer, &c1);
}

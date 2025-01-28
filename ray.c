#include "SDL_render.h"
#include "math.h"

#include "ray.h"
#include "raylib.h"
#include <stdio.h>

static void ray_render_grid(SDL_Renderer *renderer, Color c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 128);

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

static void ray_trace_circle(SDL_Renderer *renderer, Circle *c, Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 128);

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

static Color *RAY_MAP[CELL_COUNT] = {
    NULL, NULL, NULL, NULL, NULL,         NULL, NULL, NULL, &COLOR_WHITE, NULL,
    NULL, NULL, NULL, NULL, &COLOR_WHITE, NULL, NULL, NULL, &COLOR_WHITE, NULL,
    NULL, NULL, NULL, NULL, NULL,
};

static void ray_render_map(SDL_Renderer *renderer, Color **map) {
  for (int i = 0; i < CELL_H_COUNT; i++) {
    for (int j = 0; j < CELL_W_COUNT; j++) {
      Color *c = map[i * CELL_W_COUNT + j];
      if (c == NULL)
        continue;

      SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, 255);
      SDL_RenderFillRect(renderer, &(SDL_Rect){.x = j * CELL_W,
                                               .y = i * CELL_H,
                                               .w = CELL_W,
                                               .h = CELL_H});
    }
  }
};

void ray_render(SDL_Renderer *renderer) {
  SDL_Rect rect = (SDL_Rect){.x = 200, .y = 200, .w = 200, .h = 200};
  ray_render_grid(renderer, COLOR_WHITE);
  ray_render_map(renderer, RAY_MAP);

  Circle c1 = {.x = 50, .y = 100, .r = 10};
  ray_fill_circle(renderer, &c1);
  ray_trace_circle(renderer, &c1, COLOR_YELLOW);
}

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

static void ray_trace_inf(SDL_Renderer *renderer, Circle *c, Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 128);

  float a = 60.0 / 360.0 * 2 * M_PI;
  float t = a * 2 * M_PI;
  float n = 0;
  while (1) {
    float x = c->x + n * cos(t);
    float y = c->y + n * sin(t);
    if (OOB(x, y))
      break;
    SDL_RenderDrawPoint(renderer, x, y);
    n++;
  }
}

static Vec2 ray_trace_step(SDL_Renderer *renderer, Vec2 *v2, float angle) {
  float t = tan(angle);
  float x1 = v2->x;
  float y1 = v2->y;

  int nx = floor(x1 / CELL_W) * CELL_W;
  int ny = floor(y1 / CELL_H) * CELL_H;
  if (angle < M_PI / 2) {
    nx += CELL_W;
    ny += CELL_H;
  } else if (angle < M_PI) {
    ny += CELL_H;
  } else if (angle < M_PI * 3 / 2) {
  } else {
    nx += CELL_W;
  }

  // x = (y - y1) / t + x1
  // y = t(x - x1) + y1
  float x = (ny - y1) / t + x1;
  float y = t * (nx - x1) + y1;
  if (fabs(x1 - x) < fabs(y1 - y)) {
    float ry = t * (x - x1) + y1;
    return (Vec2){.x = x, .y = ry};
  } else {
    float rx = (y - y1) / t + x1;
    return (Vec2){.x = rx, .y = y};
  }
}

static void ray_trace(SDL_Renderer *renderer, Circle *p, float angle, Color c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 128);
  float x = p->x;
  float y = p->y;

  while (1) {
    Vec2 snap = ray_trace_step(renderer, &(Vec2){.x = x, .y = y}, angle);
    if (OOB(snap.x, snap.y)) {
      return;
    }
    SDL_RenderDrawLine(renderer, p->x, p->y, snap.x, snap.y);
    x = snap.x + 1;
    y = snap.y + 1;
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

  Circle c1 = {.x = 50, .y = 150, .r = 10};
  ray_fill_circle(renderer, &c1);

  float angle = 60.0 / 360.0 * 2 * M_PI;
  ray_trace(renderer, &c1, angle, COLOR_YELLOW);
}

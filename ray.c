#include "SDL_events.h"
#include "SDL_render.h"
#include "math.h"
#include <stdio.h>

#include "ray.h"
#include "raylib.h"

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

static Color *RAY_MAP[CELL_COUNT] = {
    NULL, NULL, NULL, NULL, NULL,         NULL, NULL, NULL, &COLOR_WHITE, NULL,
    NULL, NULL, NULL, NULL, &COLOR_WHITE, NULL, NULL, NULL, &COLOR_WHITE, NULL,
    NULL, NULL, NULL, NULL, NULL,
};

static void ray_trace(SDL_Renderer *renderer, Vec2 *v2, float t, Color c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
  int n = 0;
  while (1) {
    float x = v2->x + n * cos(t);
    float y = v2->y + n * sin(t);
    if (OOB(x, y) || GET_CELL_COOR(y, x) != NULL) {
      break;
    }
    SDL_RenderDrawPoint(renderer, x, y);
    n++;
  }
}

static void ray_render_map(SDL_Renderer *renderer) {
  for (int i = 0; i < CELL_H_COUNT; i++) {
    for (int j = 0; j < CELL_W_COUNT; j++) {
      Color *c = GET_CELL(i, j);
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

static Circle player = {.x = 50, .y = 150, .r = 10};
static int player_direction = 10;

void ray_poll_event(SDL_Event *event) {
  if (event->type == SDL_KEYDOWN) {
    if (event->key.keysym.sym == SDLK_LEFT) {
      player_direction = (player_direction - 5) % 360;

    } else if (event->key.keysym.sym == SDLK_RIGHT) {
      player_direction = (player_direction + 5) % 360;
    }

    else if (event->key.keysym.sym == SDLK_UP) {
      float t = THETA(player_direction);
      float x = player.x + VELOCITY * cos(t);
      float y = player.y + VELOCITY * sin(t);
      if (!OOB(x, y)) {
        player = (Circle){.x = x, .y = y, .r = player.r};
      }
    }

    else if (event->key.keysym.sym == SDLK_DOWN) {
      float t = THETA(player_direction);
      float x = player.x - VELOCITY * cos(t);
      float y = player.y - VELOCITY * sin(t);
      if (!OOB(x, y)) {
        player = (Circle){.x = x, .y = y, .r = player.r};
      }
    }
  }
}

void ray_render(SDL_Renderer *renderer, SDL_Event *event) {
  ray_render_grid(renderer, COLOR_WHITE);
  ray_render_map(renderer);
  ray_fill_circle(renderer, &player);

  Vec2 player_pos = {.x = player.x, .y = player.y};
  ray_trace(renderer, &player_pos, THETA(player_direction), COLOR_YELLOW);
}

#include "SDL_events.h"
#include "SDL_render.h"
#include "math.h"
#include <stdio.h>

#include "ray.h"
#include "raylib.h"

static void ray_render_grid(SDL_Renderer *renderer, Color c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);

  for (int i = MN_X + MN_W / CELL_COUNT_W; i < MN_X + MN_W;
       i += MN_W / CELL_COUNT_W) {
    SDL_RenderDrawLine(renderer, i, MN_Y, i, MN_H);
  }

  for (int i = MN_Y + MN_H / CELL_COUNT_H; i < MN_Y + MN_H;
       i += MN_H / CELL_COUNT_H) {
    SDL_RenderDrawLine(renderer, MN_X, i, MN_X, i);
  }
}

static void ray_fill_circle(SDL_Renderer *renderer, Vec2 *pos, float r) {
  float r_squared = powf(r, 2.0);
  for (float i = pos->x - r; i < pos->x + r; i++) {
    for (float j = pos->y - r; j < pos->y + r; j++) {
      if (powf(i - pos->x, 2.0) + powf(j - pos->y, 2.0) < r_squared) {
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
  for (int i = 0; i < CELL_COUNT_H; i++) {
    for (int j = 0; j < CELL_COUNT_W; j++) {
      Color *c = GET_CELL(i, j);
      if (c == NULL)
        continue;

      SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, 255);
      SDL_RenderFillRect(renderer,
                         &(SDL_Rect){.x = MN_X + j * MN_W / CELL_COUNT_W,
                                     .y = MN_Y + i * MN_H / CELL_COUNT_H,
                                     .w = MN_W / CELL_COUNT_W,
                                     .h = MN_H / CELL_COUNT_H});
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
      if (!OOB(x, y) && GET_CELL_COOR(y, x) == NULL) {
        player = (Circle){.x = x, .y = y, .r = player.r};
      }
    }

    else if (event->key.keysym.sym == SDLK_DOWN) {
      float t = THETA(player_direction);
      float x = player.x - VELOCITY * cos(t);
      float y = player.y - VELOCITY * sin(t);
      if (!OOB(x, y) && GET_CELL_COOR(y, x) == NULL) {
        player = (Circle){.x = x, .y = y, .r = player.r};
      }
    }
  }
}

void ray_render_minimap(SDL_Renderer *renderer) {
  ray_render_grid(renderer, COLOR_WHITE);
  ray_render_map(renderer);

  Vec2 player_pos = {.x = player.x, .y = player.y};
  ray_fill_circle(renderer, &player_pos, player.r);
  ray_trace(renderer, &player_pos, THETA(player_direction), COLOR_YELLOW);
}

void ray_render(SDL_Renderer *renderer, SDL_Event *event) {
  ray_render_minimap(renderer);
}

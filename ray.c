#include "SDL_events.h"
#include "SDL_render.h"
#include "math.h"
#include <stdio.h>

#include "ray.h"
#include "raylib.h"

static void ray_render_grid(SDL_Renderer *renderer, Color c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);

  for (int i = 0; i <= WINDOW_W / MN_SCALE; i += CELL_W / MN_SCALE) {
    SDL_RenderDrawLine(renderer, i, 0, i, WINDOW_H / MN_SCALE);
  }

  for (int i = 0; i <= WINDOW_H / MN_SCALE; i += CELL_H / MN_SCALE) {
    SDL_RenderDrawLine(renderer, 0, i, WINDOW_W / MN_SCALE, i);
  }
}

static void ray_fill_circle(SDL_Renderer *renderer, Circle *c) {
  float r_squared = powf(c->r, 2.0);
  for (float i = c->x - c->r; i < c->x + c->r; i++) {
    for (float j = c->y - c->r; j < c->y + c->r; j++) {
      if (powf(i - c->x, 2.0) + powf(j - c->y, 2.0) < r_squared) {
        SDL_RenderDrawPoint(renderer, i / MN_SCALE, j / MN_SCALE);
      }
    }
  }
}

static Color *RAY_MAP[CELL_COUNT] = {
    NULL, NULL, NULL, NULL, NULL,         NULL, NULL, NULL, &COLOR_WHITE, NULL,
    NULL, NULL, NULL, NULL, &COLOR_WHITE, NULL, NULL, NULL, &COLOR_WHITE, NULL,
    NULL, NULL, NULL, NULL, NULL,
};

static Vec2 ray_trace(SDL_Renderer *renderer, Vec2 *v2, float t) {
  int n = 0;
  float x = v2->x;
  float y = v2->y;
  while (1) {
    x = v2->x + n * cos(t);
    y = v2->y + n * sin(t);
    if (OOB(x, y) || GET_CELL_COOR(y, x) != NULL) {
      break;
    }
    n++;
  }
  return (Vec2){.x = x, .y = y};
}

static Vec2 ray_trace_dist(SDL_Renderer *renderer, Vec2 *v2, float t,
                           int limit) {
  int n = 0;
  float x = v2->x;
  float y = v2->y;
  while (n < limit) {
    x = v2->x + n * cos(t);
    y = v2->y + n * sin(t);
    if (OOB(x, y) || GET_CELL_COOR(y, x) != NULL) {
      break;
    }
    n++;
  }
  return (Vec2){.x = x, .y = y};
}

static void ray_render_map(SDL_Renderer *renderer) {
  for (int i = 0; i < CELL_COUNT_H; i++) {
    for (int j = 0; j < CELL_COUNT_W; j++) {
      Color *c = GET_CELL(i, j);
      if (c == NULL)
        continue;

      SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, 255);
      SDL_RenderFillRect(renderer, &(SDL_Rect){.x = j * CELL_W / MN_SCALE,
                                               .y = i * CELL_H / MN_SCALE,
                                               .w = CELL_W / MN_SCALE,
                                               .h = CELL_H / MN_SCALE});
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

static void ray_render_minimap(SDL_Renderer *renderer) {
  ray_render_grid(renderer, COLOR_WHITE);
  ray_render_map(renderer);
  ray_fill_circle(renderer, &player);
  SDL_SetRenderDrawColor(renderer, COLOR_YELLOW.r, COLOR_YELLOW.g,
                         COLOR_YELLOW.b, 255);
  Vec2 fovl = ray_trace_dist(renderer, &(Vec2){.x = player.x, .y = player.y},
                             THETA(player_direction - FOV / 2), 50);
  Vec2 fovr = ray_trace_dist(renderer, &(Vec2){.x = player.x, .y = player.y},
                             THETA(player_direction + FOV / 2), 50);
  SDL_RenderDrawLine(renderer, player.x / MN_SCALE, player.y / MN_SCALE,
                     fovl.x / MN_SCALE, fovl.y / MN_SCALE);
  SDL_RenderDrawLine(renderer, player.x / MN_SCALE, player.y / MN_SCALE,
                     fovr.x / MN_SCALE, fovr.y / MN_SCALE);
  SDL_RenderDrawLine(renderer, fovl.x / MN_SCALE, fovl.y / MN_SCALE,
                     fovr.x / MN_SCALE, fovr.y / MN_SCALE);
}

static void ray_render_game(SDL_Renderer *renderer) {
  /*for (int d = player_direction - FOV / 2; d < player_direction + FOV / 2;*/
  /*     d++) {*/
  /*  Vec2 trace =*/
  /*      ray_trace(renderer, &(Vec2){.x = player.x, .y = player.y},
   * THETA(d));*/
  /*  Color *c = GET_CELL_COOR(trace.y, trace.x);*/
  /*  if (c != NULL) {*/
  /*    SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, 255);*/
  /*    SDL_RenderDrawLine(renderer, WINDOW_W / 2, 0, WINDOW_W / 2, WINDOW_H);*/
  /*  }*/
  /*}*/
}

void ray_render(SDL_Renderer *renderer, SDL_Event *event) {
  ray_render_minimap(renderer);
  ray_render_game(renderer);
}

#include "SDL.h"
#include "SDL_events.h"
#include "SDL_image.h"
#include "SDL_render.h"
#include "math.h"
#include <stdio.h>

#include "ray.h"
#include "raylib.h"

static SDL_Texture *TEX[TEX_MAX] = {0};
static int TEX_C = 0;

static Color COLOR_WHITE = {.r = 255, .g = 255, .b = 255};
static Color COLOR_BLACK = {.r = 0, .g = 0, .b = 0};
static Color COLOR_GRAY = {.r = 200, .g = 200, .b = 200};
static Color COLOR_DARKGRAY = {.r = 50, .g = 50, .b = 50};
static Color COLOR_YELLOW = {.r = 255, .g = 252, .b = 127};
static Color COLOR_SKYBLUE = {.r = 173, .g = 216, .b = 230};
static Color *RAY_MAP[CELL_COUNT] = {
    NULL, NULL, NULL, NULL, NULL,         NULL, NULL, NULL, &COLOR_YELLOW, NULL,
    NULL, NULL, NULL, NULL, &COLOR_WHITE, NULL, NULL, NULL, &COLOR_WHITE,  NULL,
    NULL, NULL, NULL, NULL, NULL,
};

void ray_load_textures(SDL_Window *window, SDL_Renderer *renderer) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, "./png/cat.png");
  if (!texture) {
    fprintf(stderr, "Texture load fail");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    exit(1);
  }

  if (TEX_C + 1 > TEX_MAX) {
    fprintf(stderr, "Exceed max textures");
    exit(2);
  }

  TEX[TEX_C++] = texture;
}

static inline void ray_color_set(SDL_Renderer *renderer, Color *c) {
  SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, 255);
}

static void ray_render_grid(SDL_Renderer *renderer, Color *c) {
  ray_color_set(renderer, &COLOR_BLACK);
  SDL_RenderFillRect(renderer, &(SDL_Rect){.x = 0,
                                           .y = 0,
                                           .w = WINDOW_W / MN_SCALE,
                                           .h = WINDOW_W / MN_SCALE});

  ray_color_set(renderer, c);
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

static Vec2 ray_cast(SDL_Renderer *renderer, Vec2 *v2, float t) {
  float x = v2->x;
  float y = v2->y;

  while (1) {
    float tx = x + cos(t);
    float ty = y + sin(t);
    if (OOB(tx, ty) || GET_CELL_COOR(y, x) != NULL) {
      break;
    }
    x = tx;
    y = ty;
  }
  return (Vec2){.x = x, .y = y};
}

static Vec2 ray_cast_dist(SDL_Renderer *renderer, Vec2 *v2, float t,
                          int limit) {
  int n = 0;
  float x = v2->x;
  float y = v2->y;
  while (n < limit) {
    float tx = x + cos(t);
    float ty = y + sin(t);
    if (OOB(tx, ty) || GET_CELL_COOR(y, x) != NULL) {
      break;
    }
    x = tx;
    y = ty;
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

      ray_color_set(renderer, c);
      SDL_RenderFillRect(renderer, &(SDL_Rect){.x = j * CELL_W / MN_SCALE,
                                               .y = i * CELL_H / MN_SCALE,
                                               .w = CELL_W / MN_SCALE,
                                               .h = CELL_H / MN_SCALE});
    }
  }
};

static Circle player = {.x = 50, .y = 150, .r = 10};
static int player_direction = 0;

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

static void ray_render_fov(SDL_Renderer *renderer, Color *color) {
  ray_color_set(renderer, color);
  Vec2 fovl = ray_cast_dist(renderer, &(Vec2){.x = player.x, .y = player.y},
                            THETA(player_direction - FOV / 2), 50);
  Vec2 fovr = ray_cast_dist(renderer, &(Vec2){.x = player.x, .y = player.y},
                            THETA(player_direction + FOV / 2), 50);
  SDL_RenderDrawLine(renderer, player.x / MN_SCALE, player.y / MN_SCALE,
                     fovl.x / MN_SCALE, fovl.y / MN_SCALE);
  SDL_RenderDrawLine(renderer, player.x / MN_SCALE, player.y / MN_SCALE,
                     fovr.x / MN_SCALE, fovr.y / MN_SCALE);
  SDL_RenderDrawLine(renderer, fovl.x / MN_SCALE, fovl.y / MN_SCALE,
                     fovr.x / MN_SCALE, fovr.y / MN_SCALE);
}

static void ray_render_minimap(SDL_Renderer *renderer) {
  ray_render_grid(renderer, &COLOR_WHITE);
  ray_render_map(renderer);
  ray_fill_circle(renderer, &player);
  ray_render_fov(renderer, &COLOR_YELLOW);
}

static void ray_render_tex_strip(SDL_Renderer *renderer, SDL_Texture *tex,
                                 int x) {
  int tex_w, tex_h = 0;
  SDL_QueryTexture(tex, NULL, NULL, &tex_w, &tex_h);

  SDL_Rect srcrect = {.x = 0, .y = 0, .w = x, .h = tex_h};
  SDL_Rect dstrect = {.x = 0, .y = 0, .w = x, .h = WINDOW_H};
  SDL_RenderCopy(renderer, tex, &srcrect, &dstrect);
}

static void ray_render_world(SDL_Renderer *renderer) {
  // sky
  ray_color_set(renderer, &COLOR_GRAY);
  SDL_RenderFillRect(
      renderer, &(SDL_Rect){.x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H / 2});

  // floor
  ray_color_set(renderer, &COLOR_DARKGRAY);
  SDL_RenderFillRect(
      renderer,
      &(SDL_Rect){.x = 0, .y = WINDOW_H / 2, .w = WINDOW_W, .h = WINDOW_H / 2});

  // walls
  for (int i = 0; i < WINDOW_W; i++) {
    float angle = player_direction - FOV / 2.0 + (i / (float)WINDOW_W) * FOV;
    float angle_theta = THETAF(angle);
    Vec2 cast =
        ray_cast(renderer, &(Vec2){.x = player.x, .y = player.y}, angle_theta);
    Color *c = GET_CELL_COOR(cast.y, cast.x);
    if (c != NULL) {
      float raw_dist = DISTF(cast, player);
      float dist = raw_dist * cos(angle_theta);
      float height = WINDOW_H * FAR_CLIPPING_PLANE / dist;
      ray_color_set(
          renderer,
          &(Color){.r = CLAMPF(c->r / dist * BRIGHTNESS_FACTOR, 0, c->r),
                   .g = CLAMPF(c->g / dist * BRIGHTNESS_FACTOR, 0, c->g),
                   .b = CLAMPF(c->b / dist * BRIGHTNESS_FACTOR, 0, c->b)});
      SDL_RenderDrawLine(renderer, i, WINDOW_H / 2.0 - height / 2.0, i,
                         WINDOW_H / 2.0 + height / 2.0);
    }
  }
}

void ray_render(SDL_Renderer *renderer, SDL_Event *event) {
  ray_render_world(renderer);
  ray_render_minimap(renderer);
}

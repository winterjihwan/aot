#include "ray.h"
#include <SDL.h>
#include <stdio.h>

#define WINDOW_W 1600
#define WINDOW_H 900
#define COLOR_WHITE (Color){.r = 255, .g = 255, .b = 255}

int main(void) {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "SDL2 init fail");
    return EXIT_FAILURE;
  }

  SDL_Window *window =
      SDL_CreateWindow("Raylib", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);

  if (!window) {
    fprintf(stderr, "SDL2 create window fail");
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    fprintf(stderr, "SDL2 create renderer fail");
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Event e;
  int should_end = 0;
  while (!should_end) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        should_end = 1;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    render_ray_tracing(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();

  return EXIT_SUCCESS;
}

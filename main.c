#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "SDL_render.h"
#include "ray.h"
#include "raylib.h"

int main(void) {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "SDL2 init fail");
    return EXIT_FAILURE;
  }

  if (!IMG_Init(IMG_INIT_PNG)) {
    fprintf(stderr, "IMG init fail");
    return EXIT_FAILURE;
  }

  SDL_Window *window =
      SDL_CreateWindow("Raylib", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);

  if (!window) {
    fprintf(stderr, "SDL2 create window fail");
    IMG_Quit();
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    fprintf(stderr, "SDL2 create renderer fail");
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return EXIT_FAILURE;
  }

  ray_load_textures(window, renderer);

  SDL_Event e;
  int should_end = 0;
  while (!should_end) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        should_end = 1;
      }
      ray_poll_event(&e);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    ray_render(renderer, &e);
    SDL_RenderPresent(renderer);

    SDL_Delay(1000 / FPS);
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  IMG_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}

#include <SDL2/SDL.h>
#include "SDL_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdbool.h>
#include <stdio.h>

struct SDLPack* initialize();

struct SDLPack {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

int main(int, char**) {
    struct SDLPack* sdlPack = initialize();
    SDL_Surface* surface = IMG_Load("resources/square.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(sdlPack->renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderClear(sdlPack->renderer);
    SDL_Rect r = {0, 0, 100, 100};
    SDL_RenderCopy(sdlPack->renderer, texture, NULL, &r);
    SDL_RenderPresent(sdlPack->renderer);

    bool running = true;
    char c;
    while (running) {
        scanf(" %c", &c);
        if (c == 'q') {
            running = false;
        }
    }

    SDL_DestroyWindow(sdlPack->window);
    SDL_Quit();
    // int w, h, ch;
    // unsigned char* image_data = NULL;

    // image_data = stbi_load("resources/square.png", &w, &h, &ch, 0);

    // printf("%i\n", image_data[(0 * 100 + 1) * ch + 0]);

    // stbi_image_free(image_data);
}

struct SDLPack* initialize() {
    struct SDLPack* sdlPack = malloc(sizeof(struct SDLPack));
    SDL_Init(SDL_INIT_EVERYTHING);
    sdlPack->window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
    sdlPack->renderer = SDL_CreateRenderer(sdlPack->window, -1, SDL_RENDERER_ACCELERATED);
    return sdlPack;
}
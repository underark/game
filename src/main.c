#include <SDL2/SDL.h>
#include "SDL_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdbool.h>
#include <stdio.h>

struct SDLPack* initialize();
struct Character* createCharacter(char* filename, SDL_Renderer* renderer, int x, int y);
void draw(SDL_Renderer* renderer, struct Character* character);

struct SDLPack {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

struct Character {
    SDL_Texture* sprite;
    int x;
    int y;
};

int main(int, char**) {
    struct SDLPack* sdlPack = initialize();
    struct Character* player = createCharacter("resources/square.png", sdlPack->renderer, 100, 0);

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
        draw(sdlPack->renderer, player);
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
    SDL_SetRenderDrawColor(sdlPack->renderer, 255, 255, 255, 255);
    return sdlPack;
}

struct Character* createCharacter(char* filename, SDL_Renderer* renderer, int x, int y) {
    SDL_Surface* surface = IMG_Load(filename);
    struct Character* c = malloc(sizeof(struct Character));
    c->x = x;
    c->y = y;
    c->sprite = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return c;
}

void draw(SDL_Renderer* renderer, struct Character* character) {
    SDL_RenderClear(renderer);
    SDL_Rect r = {character->x, character->y, 100, 100};
    SDL_RenderCopy(renderer, character->sprite, NULL, &r);
    SDL_RenderPresent(renderer);
}
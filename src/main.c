#include <SDL2/SDL.h>
#include "SDL_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdbool.h>
#include <stdio.h>

#define CHARACTER_SIZE 50
#define WINDOW_SIZE 600
#define MAX_ENEMIES 10

struct SDLPack* initialize();
struct Character* createCharacter(char* filename, SDL_Renderer* renderer, int x, int y, float speed);
void draw(SDL_Renderer* renderer, struct Character* character, struct Character* enemies[]);
void freeEnemies(struct Character* enemies[]);

bool keys[SDL_NUM_SCANCODES] = {false};

struct Character* enemies[10];

struct SDLPack {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

struct Character {
    SDL_Texture* sprite;
    int x;
    int y;
    int speed;
};

int main(int, char**) {
    struct SDLPack* sdlPack = initialize();
    struct Character* player = createCharacter("resources/square.png", sdlPack->renderer, 0, 0, 5);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i] = createCharacter("resources/square.png", sdlPack->renderer, 200, 200, 5);
    }

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    keys[e.key.keysym.scancode] = true;
                    break;
                case SDL_KEYUP:
                    keys[e.key.keysym.scancode] = false;
                    break;
            }
        }

        if (keys[SDL_SCANCODE_W] && player->y >= 0 && !keys[SDL_SCANCODE_S]) {
            player->y -= player->speed;
        }

        if (keys[SDL_SCANCODE_A] && player->x >= 0 && !keys[SDL_SCANCODE_D]) {
            player->x -= player->speed;
        }

        if (keys[SDL_SCANCODE_S] && (player->y + CHARACTER_SIZE) <= WINDOW_SIZE && !keys[SDL_SCANCODE_W]) {
            player->y += player->speed;
        }

        if (keys[SDL_SCANCODE_D] && (player->x + CHARACTER_SIZE) <= WINDOW_SIZE && !keys[SDL_SCANCODE_A]) {
            player->x += player->speed;
        }

        draw(sdlPack->renderer, player, enemies);
        SDL_Delay(16);
    }

    SDL_DestroyWindow(sdlPack->window);
    SDL_Quit();
    free(sdlPack);
    free(player);
    freeEnemies(enemies);
}

struct SDLPack* initialize() {
    struct SDLPack* sdlPack = malloc(sizeof(struct SDLPack));
    SDL_Init(SDL_INIT_EVERYTHING);
    sdlPack->window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
    sdlPack->renderer = SDL_CreateRenderer(sdlPack->window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(sdlPack->renderer, 255, 255, 255, 255);
    return sdlPack;
}

struct Character* createCharacter(char* filename, SDL_Renderer* renderer, int x, int y, float speed) {
    SDL_Surface* surface = IMG_Load(filename);
    struct Character* c = malloc(sizeof(struct Character));
    c->x = x;
    c->y = y;
    c->speed = speed;
    c->sprite = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return c;
}

void draw(SDL_Renderer* renderer, struct Character* character, struct Character* enemies[]) {
    SDL_RenderClear(renderer);
    SDL_Rect r = {character->x, character->y, CHARACTER_SIZE, CHARACTER_SIZE};
    SDL_RenderCopy(renderer, character->sprite, NULL, &r);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        SDL_Rect e = {enemies[i]->x, enemies[i]->y, CHARACTER_SIZE, CHARACTER_SIZE};
        SDL_RenderCopy(renderer, enemies[i]->sprite, NULL, &e);
    }

    SDL_RenderPresent(renderer);
}

void freeEnemies(struct Character* enemies[]) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        free(enemies[i]);
    }
}
#include <SDL2/SDL.h>
#include "SDL_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdbool.h>
#include <stdio.h>

#define CHARACTER_SIZE 50
#define WINDOW_SIZE 600
#define MAX_ENEMIES 1
#define FRAME_TIME 16

struct SDLPack* initialize();
struct Character* createCharacter(int x, int y, float speed);
void render(SDL_Renderer* renderer, char* spriteName, int x, int y, int size);
void freeEnemies(struct Character* enemies[]);

bool keys[SDL_NUM_SCANCODES] = {false};

struct Character* enemies[MAX_ENEMIES];

struct SDLPack {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

struct Character {
    int x;
    int y;
    float speed;
};

int main(int, char**) {
    struct SDLPack* sdlPack = initialize();
    struct Character* player = createCharacter(0, 0, 5);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i] = createCharacter(500, 500, 0.1);
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

        for (int i = 0; i < MAX_ENEMIES; i++) {
            float vectorX = player->x - enemies[i]->x;
            float vectorY = player->y - enemies[i]->y;
            float magnitude = sqrt((vectorX * vectorX) + (vectorY * vectorY));
            vectorX /= magnitude;
            vectorY /= magnitude;
            enemies[i]->x += vectorX * FRAME_TIME * enemies[i]->speed;
            enemies[i]->y += vectorY * FRAME_TIME * enemies[i]->speed;
        }

        SDL_RenderClear(sdlPack->renderer);
        render(sdlPack->renderer, "resources/square.png", player->x, player->y, CHARACTER_SIZE);
        for (int i = 0; i < MAX_ENEMIES; i++) {
            render(sdlPack->renderer, "resources/square.png", enemies[i]->x, enemies[i]->y, CHARACTER_SIZE);
        }
        SDL_RenderPresent(sdlPack->renderer);
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

struct Character* createCharacter(int x, int y, float speed) {
    struct Character* c = malloc(sizeof(struct Character));
    c->x = x;
    c->y = y;
    c->speed = speed;
    return c;
}

void render(SDL_Renderer* renderer, char* spriteName, int x, int y, int size) {
    SDL_Surface* surface = IMG_Load(spriteName);
    SDL_Texture* sprite = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect r = {x, y, size, size};
    SDL_RenderCopy(renderer, sprite, NULL, &r);
}

void freeEnemies(struct Character* enemies[]) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        free(enemies[i]);
    }
}
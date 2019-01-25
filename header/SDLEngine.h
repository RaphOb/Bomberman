//
// Created by geoff on 16/01/2019.
//

#ifndef BOMBERMAN_SDLENGINE_H
#define BOMBERMAN_SDLENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "../header/game.h"
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

typedef struct {
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;

} sdl_t;

sdl_t *initSDL(game_t *game);
void destroySDL(sdl_t *pSDL);
void displayBlock(sdl_t *pSDL,int x, int y);
void displayMap(game_t *game, sdl_t *pSDL);
int getBit(const char c[], int indexArray, int indexBit);
void displayTrump(sdl_t *pSDL);

#endif //BOMBERMAN_SDLENGINE_H

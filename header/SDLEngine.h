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

typedef struct {
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
} sdl_t;

sdl_t *initSDL(game_t *game);
void destroySDL(sdl_t *pSDL);
void destroyGame(game_t *game);
void displayBlock(sdl_t *pSDL,int x, int y, SDL_Surface *block);
void displayMap(game_t *game, sdl_t *pSDL, SDL_Surface *block);
int getBit(const char c[], int indexArray, int indexBit);
void clear(SDL_Renderer *sdl_renderer);
void displayGame(game_t *game, sdl_t *sdl);
#endif //BOMBERMAN_SDLENGINE_H

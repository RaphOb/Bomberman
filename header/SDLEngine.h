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
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    SDL_Texture *textureMap;
    SDL_Texture *textureBlock;
    //SDL_Rect dst_trump;
    //SDL_Texture *textureTrump;

} sdl_t;

sdl_t *initSDL(game_t *game);
void destroySDL(sdl_t *pSDL);
void destroyGame(game_t *game);
void displayBlock(sdl_t *pSDL,int x, int y);
void displayMap(game_t *game, sdl_t *pSDL);
int getBit(const char c[], int indexArray, int indexBit);
void initTrump(sdl_t *pSDL, game_t *game);
void draw_game(sdl_t *pSDL, game_t *game);

void clear(SDL_Renderer *sdl_renderer);
#endif //BOMBERMAN_SDLENGINE_H

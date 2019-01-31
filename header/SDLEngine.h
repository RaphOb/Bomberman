//
// Created by geoff on 16/01/2019.
//

#ifndef BOMBERMAN_SDLENGINE_H
#define BOMBERMAN_SDLENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "../header/map.h"

typedef struct {
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    SDL_Texture *textureMap;
    SDL_Texture *textureBlock;
    SDL_Rect dst_trump;
    SDL_Texture *textureTrump;
    SDL_Rect dst_bomb;
    SDL_Texture* textureBomb;

} sdl_t;

sdl_t *initSDL();
void destroySDL(sdl_t *pSDL);
void renderBlock(sdl_t *pSDL, int x, int y);
void renderMap(map_t map, sdl_t *pSdl);
int getBit(const char c[], int indexArray, int indexBit);
void initPlayerSDL(sdl_t *pSDL);
void initMap(sdl_t *pSDL);
void initBlock(sdl_t *pSDL);
void initBomb(sdl_t *pSDL);

void clear(SDL_Renderer *sdl_renderer);
#endif //BOMBERMAN_SDLENGINE_H
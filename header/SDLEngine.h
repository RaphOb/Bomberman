//
// Created by geoff on 16/01/2019.
//

#ifndef BOMBERMAN_SDLENGINE_H
#define BOMBERMAN_SDLENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "../header/map.h"

#define BOMB_PNG_W 30
#define BOMB_PNG_H 32

typedef struct button_s {
    SDL_Texture *textureButton[2];
    int hover;
    SDL_Rect dstRect;
} button_t;

struct sdl_s {
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    SDL_Texture *textureMap;
    SDL_Texture *textureBlock;
    SDL_Rect dst_player;
    SDL_Texture *texturePlayer;
    SDL_Rect dst_bomb;
    SDL_Texture* textureBomb;
    SDL_Texture* textureExplosion;
    SDL_Texture* textureExplosion2;
    SDL_Rect dst_explosion;
    SDL_Rect dst_explosion2;
    button_t *buttonPlay;
    button_t *buttonQuit;
    button_t *buttonConnect;
    button_t *buttonHost;
    SDL_Texture *textureMenuLogo;
};
typedef struct sdl_s sdl_t;

struct text_s {
    SDL_Rect pos;
    int length;
    char *text;
    SDL_Color color;
};
typedef struct text_s text_t;

sdl_t *initSDL();
void destroySDL(sdl_t *pSDL);
void initPlayerSDL(sdl_t *pSDL);
void initMap(sdl_t *pSDL);
void initBlock(sdl_t *pSDL);
void initBomb(sdl_t *pSDL);
void initExplosion(sdl_t *pSDL);
void initMenu(sdl_t *pSDL);
button_t *initButton(SDL_Rect rect, SDL_Texture *textureOn, SDL_Texture *textureOff);

void clear(SDL_Renderer *sdl_renderer);

#endif //BOMBERMAN_SDLENGINE_H
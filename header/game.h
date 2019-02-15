//
// Created by geoff on 16/01/2019.
//

#ifndef BOMBERMAN_GAME_H
#define BOMBERMAN_GAME_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../header/SDLEngine.h"
#include "../header/player.h"
#include "../header/map.h"

#define NB_MAPS 8
#define MAX_PLAYER 4
#define FPS 60

typedef struct game_s {
    map_t map;
    int current_map;
    player_t *players[MAX_PLAYER];
    sdl_t *pSDL;
} game_t;

game_t *initGame(sdl_t *sdl);
int gameEvent(game_t *game);
void placeBomb(game_t *game);
void makeExplosion(game_t *game);

#endif //BOMBERMAN_GAME_H
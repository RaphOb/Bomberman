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
    player_t players[MAX_PLAYER];
    int nb_client_serv;
    sdl_t *pSDL;
    son_t* son;
} game_t;

game_t *initGame(sdl_t *sdl);
int gameEvent(game_t *game);
void placeBomb(sdl_t *pSDL, player_t *player);
void makeExplosion(player_t *player);
void checkBombDamage(map_t map, bomb_t b);
void destroyBlock(map_t map, int pos_x, int pos_y);
void checkBombPlayer(player_t *player, bomb_t b);

#endif //BOMBERMAN_GAME_H
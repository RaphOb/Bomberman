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

typedef struct game_s{
    map_t map;
    int current_map;
    player_t *players[MAX_PLAYER];
    sdl_t *pSDL;
} game_t;

game_t *initGame(sdl_t *sdl);
int game_event(game_t *game);
void game_moveT(game_t *game, SDL_Keycode direction);
void placeBomb(game_t *game);
void draw_game(game_t *game);
void renderBomb(sdl_t *pSDL);
void renderBackground(sdl_t *pSDL);
void renderPlayer(sdl_t *pSDL, player_t *player);


#endif //BOMBERMAN_GAME_H
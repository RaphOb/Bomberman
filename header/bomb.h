//
// Created by geoff on 21/03/2019.
//

#ifndef BOMBERMAN_BOMB_H
#define BOMBERMAN_BOMB_H

#include "game.h"

void placeBomb(sdl_t *pSDL, player_t *player, bomb_t *bomb);
int getIndexBomb(player_t *p);
void makeExplosion(bomb_t *bomb, son_t* son);
void checkBombDamage(map_t map, bomb_t b);
int destroyBlock(map_t map, int pos_x, int pos_y);
void checkBombPlayer(player_t *player, bomb_t b);
void checkExplosion(game_t *game, bomb_t b);
bomb_t *getBomb(int x, int y, player_t player[MAX_PLAYER]);


#endif //BOMBERMAN_BOMB_H

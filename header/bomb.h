//
// Created by geoff on 21/03/2019.
//

#ifndef BOMBERMAN_BOMB_H
#define BOMBERMAN_BOMB_H

#include "game.h"

void placeBomb(player_t *player, bomb_t *bomb);
int getIndexBomb(player_t *p);
void updateBombForAnim(bomb_t *bomb);
void makeExplosion(bomb_t *bomb);
void checkBombDamage(map_t map, bomb_t b);
int destroyBlock(map_t map, int pos_x, int pos_y);
void checkBombPlayer(player_t *player, bomb_t b, map_t map);
void checkExplosion(map_t map, bomb_t b);
int isBombOnCell(map_t map, int x, int y);

#endif //BOMBERMAN_BOMB_H

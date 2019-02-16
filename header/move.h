//
// Created by geoff on 14/02/2019.
//

#ifndef BOMBERMAN_MOVE_H
#define BOMBERMAN_MOVE_H

#include "../header/game.h"

typedef struct move_s {
    void (*func_move)(player_t *player, map_t map);
    SDL_Scancode key;
} move_t;

int doMove(const Uint8 *keystates, player_t *player, map_t map);
void moveUp(player_t *player, map_t map);
void moveDown(player_t *player, map_t map);
void moveLeft(player_t *player, map_t map);
void moveRight(player_t *player, map_t map);

#endif //BOMBERMAN_MOVE_H

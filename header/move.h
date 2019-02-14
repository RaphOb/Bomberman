//
// Created by geoff on 14/02/2019.
//

#ifndef BOMBERMAN_MOVE_H
#define BOMBERMAN_MOVE_H

#include "../header/game.h"

typedef struct move_s {
    void (*func_move)(player_t *player);
    SDL_Scancode key;
} move_t;

int doMove(const Uint8 *keystates, player_t *player);
void moveUp(player_t *player);
void moveDown(player_t *player);
void moveLeft(player_t *player);
void moveRight(player_t *player);

#endif //BOMBERMAN_MOVE_H

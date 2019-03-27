//
// Created by geoff on 14/02/2019.
//

#ifndef BOMBERMAN_MOVE_H
#define BOMBERMAN_MOVE_H

#include "../header/game.h"

typedef struct move_s {
    void (*func_move)(player_t *player, map_t map, int request_x, int request_y);
    int code;
} move_t;

int doMove(int code, player_t *player, map_t map, int request_x, int request_y);
void moveUp(player_t *player, map_t map, int request_x, int request_y);
void moveDown(player_t *player, map_t map, int request_x, int request_y);
void moveLeft(player_t *player, map_t map, int request_x, int request_y);
void moveRight(player_t *player, map_t map, int request_x, int request_y);

#endif //BOMBERMAN_MOVE_H

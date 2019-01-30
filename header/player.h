//
// Created by geoff on 30/01/2019.
//

#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H

typedef struct {
    char alive;
    unsigned int number;
    int x_pos;
    int y_pos;
    int bombs_left;
    int frags;
} player_t;

player_t *initPlayer();

#endif //BOMBERMAN_PLAYER_H

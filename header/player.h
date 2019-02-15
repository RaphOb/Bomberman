//
// Created by geoff on 30/01/2019.
//

#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H

#include "../header/map.h"

#define TEXTURE_WIDTH 72
#define TEXTURE_HEIGHT 88
#define FRAME_WIDTH TEXTURE_WIDTH / 3
#define FRAME_HEIGHT TEXTURE_HEIGHT / 4
#define PLAYER_WIDTH FRAME_WIDTH * 3
#define PLAYER_HEIGHT FRAME_HEIGHT * 3

typedef struct player_s {
    char alive;
    unsigned int number;
    int map_x[2];
    int map_y[2];
    int x_pos;
    int y_pos;
    int bomb;
    int bombs_left;
    int frags;
    int current_frame;
    int frame_time;
    int direction;
    int still;
    int explosion;
    int tickBombDropped;
    int tickExplosion;
} player_t;

player_t *initPlayer();
void updatePlayerCell(player_t *player);
int collideWith(map_t map, player_t *player, int x, int y);

#endif //BOMBERMAN_PLAYER_H

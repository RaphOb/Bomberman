//
// Created by geoff on 30/01/2019.
//

#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H

#include <pthread.h>
#include "../header/map.h"

#define TEXTURE_WIDTH 72
#define TEXTURE_HEIGHT 84
#define FRAME_WIDTH TEXTURE_WIDTH / 3
#define FRAME_HEIGHT TEXTURE_HEIGHT / 4
#define PLAYER_WIDTH FRAME_WIDTH * 2
#define PLAYER_HEIGHT FRAME_HEIGHT * 2

typedef struct bombs_s {
    int x_pos;
    int y_pos;
    int explosion;
    int tickBombDropped;
    int tickExplosion;
} bomb_t;

typedef struct player_s {
    char alive;
    int code_reseau;
    int number;
    int map_x[2];
    int map_y[2];
    int x_pos;
    int y_pos;
    int bombPosed;
    int bombs_left;
    int frags;
    int current_frame;
    int frame_time;
    int direction;
    int still;
    bomb_t *bomb;
    pthread_mutex_t mutex_player;
} player_t;

player_t initPlayer();
void updatePlayerCell(player_t *player);
int collideWith(map_t map, player_t *player, int x, int y);
int isPlayerOnOneCell(player_t *player);

#endif //BOMBERMAN_PLAYER_H

//
// Created by geoff on 30/01/2019.
//

#ifndef BOMBERMAN_PLAYER_H
#define BOMBERMAN_PLAYER_H

#include <pthread.h>
#include "../header/map.h"

#define TEXTURE_WIDTH 78
#define TEXTURE_HEIGHT 128
#define FRAME_WIDTH TEXTURE_WIDTH / 3
#define FRAME_HEIGHT TEXTURE_HEIGHT / 4
#define PLAYER_WIDTH (int) (FRAME_WIDTH + (FRAME_WIDTH / 2))
#define PLAYER_HEIGHT (int) (FRAME_HEIGHT + (FRAME_HEIGHT / 2))

typedef struct bombs_s {
    int x_pos;
    int y_pos;
    int range;
    int explosion;
    int tickBombDropped;
    int tickExplosion;
} bomb_t;

typedef struct player_s {
    char alive;
    int code_reseau;
    int co_is_ok;
    int number;
    int checksum;
    int map_x[2];
    int map_y[2];
    int x_pos;
    int y_pos;
    int speed;
    int bombPosed;
    int bombs_left;
    int frags;
    int current_frame;
    int frame_time;
    int direction;
    int still;
    pthread_mutex_t mutex_player;
    bomb_t bomb;
    char *name;
} player_t;

player_t initPlayer();
void updatePlayerCell(player_t *player);
int collideWith(map_t map, player_t *player, int x, int y);
int canPlayerPlaceBomb(player_t *player);
int isPlayerOnOneCell(player_t *player);

#endif //BOMBERMAN_PLAYER_H

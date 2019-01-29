//
// Created by geoff on 16/01/2019.
//

#ifndef BOMBERMAN_GAME_H
#define BOMBERMAN_GAME_H

#include <stdlib.h>

#define NB_MAPS 8
#define MAP_X 9
#define MAP_Y 13
#define MAP_SIZE (MAP_X * MAP_Y)
#define BASE_MAP_W 240
#define BASE_MAP_H 160
#define SIZE_M 5
#define MAP_SIZE_W (BASE_MAP_W * SIZE_M)
#define MAP_SIZE_H (BASE_MAP_H * SIZE_M)
#define START_X_MAP 0
#define START_Y_MAP 0

typedef char map_t[MAP_X][MAP_Y];

typedef struct game_s{
    map_t map;
    int current_map;
} game_t;

game_t *initGame();


#endif //BOMBERMAN_GAME_H

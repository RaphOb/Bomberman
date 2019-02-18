//
// Created by geoff on 22/01/2019.
//

#ifndef BOMBERMAN_MAP_H
#define BOMBERMAN_MAP_H

#define MAP_X               9
#define MAP_Y               13
#define SIZE_M              5
#define BLOCK_SIZE          16
#define BASE_MAP_W          240
#define BASE_MAP_H          160
#define START_X_BACKGROUND  0
#define START_Y_BACKGROUND  0
#define MAP_SIZE_W          (BASE_MAP_W * SIZE_M)
#define MAP_SIZE_H          (BASE_MAP_H * SIZE_M)
#define MAP_SIZE            MAP_X * MAP_Y
#define START_X_MAP         START_X_BACKGROUND + (BLOCK_SIZE * SIZE_M)
#define START_Y_MAP         START_Y_BACKGROUND + (BLOCK_SIZE / 2 * SIZE_M)
#define FILENAME            "../resources/map.txt"

typedef char map_t[MAP_X][MAP_Y];

int extractArrayFromFile(map_t map);
void insertLineIntoArray(map_t map, int index, char *line);
void fillArray(map_t);
void displayArray(map_t map);

#endif //BOMBERMAN_MAP_H
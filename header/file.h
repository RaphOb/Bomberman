//
// Created by geoff on 22/01/2019.
//

#ifndef BOMBERMAN_FILE_H
#define BOMBERMAN_FILE_H

#include "game.h"
#define FILENAME "../resources/map.txt"

int extractArrayFromFile(map_t map);
void insertLineIntoArray(map_t map, int index, char *line);
void fillArray(map_t);
void setBit(char c[], int indexArray, int indexBit);
void displayArray(map_t map);
void printBits(const char c);

#endif //BOMBERMAN_FILE_H

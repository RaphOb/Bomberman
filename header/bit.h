//
// Created by geoff on 30/01/2019.
//

#ifndef BOMBERMAN_BIT_H
#define BOMBERMAN_BIT_H

void toggleBit(char *line, int indexCol, int indexBit);
void setBit(char *line, int indexCol, int indexBit);
void printBits(char c);
int getBit(const char line[], int indexCol, int indexBit);

#endif //BOMBERMAN_BIT_H

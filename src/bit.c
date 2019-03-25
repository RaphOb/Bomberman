//
// Created by geoff on 30/01/2019.
//

#include <SDL2/SDL_log.h>
#include "../header/bit.h"

/**
 * function : Same as getBit but you toggle the bit.
 * When bit is 1, it changes to 0 and vice versa
 * @param line
 * @param indexCol
 * @param indexBit
 */
void toggleBit(char *line, int indexCol, int indexBit)
{
    int pos = indexBit % 8;
//    unsigned char flag = 1;   // flag = 00000001
//   flag = flag << pos;      // flag = 0000...010...000   (shifted k positions)
    line[indexCol] ^= 1U << pos;
}

void setBit(char *line, int indexCol, int indexBit)
{
    int pos = indexBit % 8;
//    unsigned char flag = 1;   // flag = 00000001
//   flag = flag << pos;      // flag = 0000...010...000   (shifted k positions)
    line[indexCol] |= 1U << pos;
}

/**
 * function: Get the bit (0 or 1) at the position "indexBit" from a character in a two dimensional array.
 * "line" represents a one dimensional array and "indexCol" is the index of the character you want the bit from.
 * @param line
 * @param indexCol
 * @param indexBit
 * @return
 */
int getBit(const char line[], int indexCol, int indexBit)
{
    return (1U & (line[indexCol] >> indexBit));
}


/**
 * function : Prints the bits from a character variable
 * @param c
 */
void printBits(const char c)
{
    // parcourt bit par bit le char
/*    printf("Right to left order\n");
    for (int i = 7; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
    }
    printf("\n");
*/
    SDL_Log("Left to right order\n");
    for (int i = 0; i < 8; i++) {
        SDL_Log("%d", (c >> i) & 1);
    }
}

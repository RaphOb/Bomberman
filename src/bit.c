//
// Created by geoff on 30/01/2019.
//

#include <SDL_log.h>
#include "../header/bit.h"

/** TODO
 * function : comment plz
 * @param c
 * @param indexArray
 * @param indexBit
 */
void setBit(char c[], int indexArray, int indexBit)
{
    int pos = indexBit % 8;
//    unsigned char flag = 1;   // flag = 00000001
//   flag = flag << pos;      // flag = 0000...010...000   (shifted k positions)
    c[indexArray] |= 1 << pos;
}

/** TODO
 * function: whos knows ?!
 * @param c
 * @param indexArray
 * @param indexBit
 * @return
 */
int getBit(const char c[], int indexArray, int indexBit)
{
    return (1 & (c[indexArray] >> indexBit));
}


/** TODO
 * function : comment plz
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

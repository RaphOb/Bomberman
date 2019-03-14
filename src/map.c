//
// Created by geoff on 22/01/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_log.h>
#include "../header/map.h"
#include "../header/bit.h"

/**
 * function : Build the map from the file "map.txt"
 * @param map
 * @return 1 if all went good, 0 if not
 */
int extractArrayFromFile(map_t map)
{
    const int BUFSIZE = 27;
    FILE *ptr;
    char line[BUFSIZE];
    int i = 0;

    ptr = fopen(FILENAME, "r");
    if (ptr == NULL) {
        SDL_Log("error opening file");
        return (0);
    }

    fillArray(map);
    while (fgets(line, BUFSIZE, ptr) != NULL) {
        insertLineIntoArray(map, i, line);
        i++;
    }
    fclose(ptr);
    return (1);
}

/**
 * function : Fill the map with 0.
 * @param map
 */
void fillArray(map_t map)
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 13; j++) {
            map[i][j] = 0;
        }
    }
}

/**
 * function : Toggle bit(s) in the map depending on the character when reading through the line from the file
 * @param map
 * @param index
 * @param line
 */
void insertLineIntoArray(map_t map, int index, char *line)
{
    int i = 0;
    for (int j = 0; j <= 24; j += 2, i++) {
        switch (line[j]) {
            case '0' :
                // On ne fait rien puisque tous les bits sont deja à 0
                break;
            case '1' :
                toggleBit(map[index], i, 1);
                toggleBit(map[index], i, 2);
                break;
            case 'X' :
                toggleBit(map[index], i, 1);
                break;
            default:
                break;
        }
    }
}

/**
 * function : Dev function to see the bits in the map
 * @param map
 */
void displayArray(map_t map)
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 13; j++) {
            SDL_Log("[%d][%d]: %c", i,j, map[i][j]);
            printBits(map[i][j]);

        }
    }
}
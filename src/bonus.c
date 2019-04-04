//
// Created by geoff on 14/03/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_log.h>
#include "../header/bonus.h"
#include "../header/bit.h"
#include "../header/player.h"
#include "../header/client.h"

const bonus_t bonus[] = {
        {applyBonusRangeBomb, bRangeBombe},
        {applyMalusRangeBomb, mRangeBombe},
        {applyBonusNbBombe,   bNbBombe},
        {applyMalusNbBombe,   mNbBombe},
        {applyBonusSpeed,     bVitesse},
        {applyMalusSpeed,     mVitesse},
        {NULL,                typeUnknown}
};

/**
 * function : Check if a bonus is on the cell from the parameter
 * @param map
 * @param cell_x
 * @param cell_y
 * @return 1 if true 0 if not
 */
int isBonusOnCell(map_t map, int cell_x, int cell_y) {
    return getBit(map[cell_y], cell_x, 4);
}

/**
 * function : Get the type bonus in the cell from the parameter
 * @param map
 * @param cell_x
 * @param cell_y
 * @return the enum type of bonus
 */
typeBonus_e getBonus(map_t map, int cell_x, int cell_y) {
    char type[3];

    for (int i = 5; i <= 7; i++) {
        char *tmp = malloc(sizeof(char) * 1);
        sprintf(tmp, "%d", getBit(map[cell_y], cell_x, i));
        strcat(type, tmp);
    }
    if (strcmp(type, "000") == 0) {
        return bRangeBombe;
    } else if (strcmp(type, "001") == 0) {
        return mRangeBombe;
    } else if (strcmp(type, "010") == 0) {
        return bNbBombe;
    } else if (strcmp(type, "011") == 0) {
        return mNbBombe;
    } else if (strcmp(type, "100") == 0) {
        return bVitesse;
    } else if (strcmp(type, "101") == 0) {
        return mVitesse;
    }
    return typeUnknown;
}

/**
 * Function : Spawn a random bonus on the cell from the parameter
 * @param map
 * @param cell_x
 * @param cell_y
 */
void spawnBonus(map_t map, int cell_x, int cell_y) {
    typeBonus_e type = rand() % 6;
    toggleBit(map[cell_y], cell_x, 4);
    if (type == mRangeBombe) {
        toggleBit(map[cell_y], cell_x, 7);
    } else if (type == bNbBombe) {
        toggleBit(map[cell_y], cell_x, 6);
    } else if (type == mNbBombe) {
        toggleBit(map[cell_y], cell_x, 6);
        toggleBit(map[cell_y], cell_x, 7);
    } else if (type == bVitesse) {
        toggleBit(map[cell_y], cell_x, 5);
    } else if (type == mVitesse) {
        toggleBit(map[cell_y], cell_x, 5);
        toggleBit(map[cell_y], cell_x, 7);
    }
}

/**
 * Function : Go through the array of structure "bonus" to call a function depending on the parameter "type"
 * @param type
 * @param player
 */
void doBonus(typeBonus_e type, player_t *player) {
    int i = 0;

    while (bonus[i].type != typeUnknown) {
        if (type == bonus[i].type) {
            bonus[i].func_bonus(player);
            break;
        }
        i++;
    }
}

/**
 * Function : Speak itself
 * @param player
 */
void applyBonusRangeBomb(player_t *player) {
    for (int i = 0; i < MAX_BOMBE; i++) {
        if (player->bomb[i].range <= 8) {
            player->bomb[i].range += 1;
        }
    }
}

/**
 * Function : Speak itself
 * @param player
 */
void applyMalusRangeBomb(player_t *player) {
    for (int i = 0; i < MAX_BOMBE; i++) {
        if (player->bomb[i].range > 1) {
            player->bomb[i].range -= 1;
        }
    }
}

/**
 * Function : Speak itself
 * @param player
 */
void applyBonusNbBombe(player_t *player) {
    if (player->nbBombe <= MAX_BOMBE) {
        player->nbBombe += 1;
    }
}

/**
 * Function : Speak itself
 * @param player
 */
void applyMalusNbBombe(player_t *player) {
    if (player->nbBombe > 1) {
        player->nbBombe -= 1;
    }
}

/**
 * Function : Speak itself
 * @param player
 */
void applyBonusSpeed(player_t *player) {
    if (player->speed <= 6) {
        player->speed += 1;
    }
}

/**
 * Function : Speak itself
 * @param player
 */
void applyMalusSpeed(player_t *player) {
    if (player->speed > 3)
        player->speed -= 1;
}

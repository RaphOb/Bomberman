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
 * function : is Bonus on Cell?
 * @param map
 * @param cell_x
 * @param cell_y
 * @return
 */
int isBonusOnCell(map_t map, int cell_x, int cell_y) {
    return getBit(map[cell_y], cell_x, 4);
}

/**
 * function
 * @param map
 * @param cell_x
 * @param cell_y
 * @return
 */
typeBonus_e getBonus(map_t map, int cell_x, int cell_y) {
    char type[3];

    for (int i = 5; i <= 7; i++) {
        char *tmp = malloc(sizeof(char) * 1);
        sprintf(tmp, "%d", getBit(map[cell_y], cell_x, i));
        strcat(type, tmp);
//        free(tmp);
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
 *
 * @param map
 * @param cell_x
 * @param cell_y
 */
void spawnBonus(map_t map, int cell_x, int cell_y) {
    SDL_Log("spanw");
    typeBonus_e type = rand() % 6;
    SDL_Log("cell_x : %d, cell_y: %d", cell_x, cell_y);
    toggleBit(map[cell_y], cell_x, 4);
    SDL_Log("type: %d, mRangeBOmbe: %d", type, mRangeBombe);
    if (type == mRangeBombe) {
        SDL_Log("malus range bombe");
        toggleBit(map[cell_y], cell_x, 7);
    } else if (type == bNbBombe) {
        SDL_Log("bonus nb bombe");
        toggleBit(map[cell_y], cell_x, 6);
    } else if (type == mNbBombe) {
        SDL_Log("malus nb bombe");
        toggleBit(map[cell_y], cell_x, 6);
        toggleBit(map[cell_y], cell_x, 7);
    } else if (type == bVitesse) {
        SDL_Log("bonus vitesse");
        toggleBit(map[cell_y], cell_x, 5);
    } else if (type == mVitesse) {
        SDL_Log("malus vitesse");
        toggleBit(map[cell_y], cell_x, 5);
        toggleBit(map[cell_y], cell_x, 7);
    }
}

/**
 *
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
 *
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
 *
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
 *
 * @param player
 */
void applyBonusNbBombe(player_t *player) {
    if (player->nbBombe <= MAX_BOMBE) {
        player->nbBombe += 1;
    }
}

void applyMalusNbBombe(player_t *player) {
    if (player->nbBombe > 1) {
        player->nbBombe -= 1;
    }
}

void applyBonusSpeed(player_t *player) {
    if (player->speed <= 6) {
        player->speed += 1;
    }
}

void applyMalusSpeed(player_t *player) {
    if (player->speed > 3)
        player->speed -= 1;
}

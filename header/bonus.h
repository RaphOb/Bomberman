//
// Created by geoff on 14/03/2019.
//

#ifndef BOMBERMAN_BONUS_H
#define BOMBERMAN_BONUS_H

#include "map.h"
#include "player.h"

enum typeBonus {bRangeBombe, mRangeBombe, bNbBombe, mNbBombe, bVitesse, mVitesse, typeUnknown};
typedef enum typeBonus typeBonus_e;

typedef struct bonus_s {
    void (*func_bonus)(player_t *player);
    typeBonus_e type;
} bonus_t;

int isBonusOnCell(map_t map, int cell_x, int cell_y);
typeBonus_e getBonus(map_t map, int cell_x, int cell_y);
void spawnBonus(map_t map, int cell_x, int cell_y);
void doBonus(typeBonus_e type, player_t *player);
void applyBonusRangeBomb(player_t *player);
void applyMalusRangeBomb(player_t *player);
void applyBonusNbBombe(player_t *player);
void applyMalusNbBombe(player_t *player);
void applyBonusSpeed(player_t *player);
void applyMalusSpeed(player_t *player);


#endif //BOMBERMAN_BONUS_H

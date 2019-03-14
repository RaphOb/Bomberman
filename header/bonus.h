//
// Created by geoff on 14/03/2019.
//

#ifndef BOMBERMAN_BONUS_H
#define BOMBERMAN_BONUS_H

#include "../header/map.h"

enum typeBonus {bRangeBombe, mRangeBombe, bNbBombe, mNbBombe, bVitesse, mVitesse };
typedef enum typeBonus typeBonus_e;
int isBonusOnCell(map_t map, int cell_x, int cell_y);
typeBonus_e getBonus(map_t map, int cell_x, int cell_y);


#endif //BOMBERMAN_BONUS_H

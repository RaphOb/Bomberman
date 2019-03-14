//
// Created by geoff on 14/03/2019.
//

#include "../header/bonus.h"
#include "../header/bit.h"


int isBonusOnCell(map_t map, int cell_x, int cell_y)
{
    return getBit(map[cell_y], cell_x, 4);
}

typeBonus_e getBonus(map_t map, int cell_x, int cell_y)
{
    const unsigned int bonus;

    return bRangeBombe;
}



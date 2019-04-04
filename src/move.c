//
// Created by geoff on 14/02/2019.
//

#include "../header/move.h"
#include "../header/reseau.h"
#include "../header/bonus.h"
#include "../header/bit.h"

/**
 * An array of the structure "move_t" containing a function pointer and the key corresponding
 */
const move_t move[] = {
        {moveUp,    UP_CODE},
        {moveDown,  DOWN_CODE},
        {moveLeft,  LEFT_CODE},
        {moveRight, RIGHT_CODE},
        {NULL, 0}
};

/**
 * function : Main function to move the player on the map depending on the key he pressed
 * @param keystates
 * @param player
 * @param map
 * @return
 */
int doMove(int code, player_t *player, map_t map, int request_x, int request_y) {
    int i = 0;
    player->still = 0;

    while (move[i].code != 0) {
        if (code == move[i].code) {
            move[i].func_move(player, map, request_x, request_y);
            updatePlayerCell(player);
            if (isPlayerOnOneCell(player) && isBonusOnCell(map, player->map_x[0], player->map_y[0])) {
                typeBonus_e type = getBonus(map, player->map_x[0], player->map_y[0]);
                doBonus(type, player);
                toggleBit(map[player->map_y[0]], player->map_x[0], 4);
            }
            return (1);
        }
        i++;
    }
}

/**
 * function : Move the player up
 * @param player
 * @param map
 */
void moveUp(player_t *player, map_t map, int request_x, int request_y) {
    player->direction = 3;
    if (player->y_pos > START_Y_MAP && collideWith(map, player, request_x, request_y) == 0) {
        player->y_pos = request_y;
    }
//    c_emission(player, UP_CODE);
}

/**
 * function : Move the player right
 * @param player
 * @param map
 */
void moveRight(player_t *player, map_t map, int request_x, int request_y) {
    player->direction = 2;
    if (player->x_pos < ((START_X_BACKGROUND + MAP_SIZE_W) - (PLAYER_WIDTH + (REAL_BLOCK_SIZE)))
        && collideWith(map, player, request_x, request_y) == 0) {
        player->x_pos = request_x;
    }
//    c_emission(player, RIGHT_CODE);
}

/**
 * function : Move the player down
 * @param player
 * @param map
 */
void moveDown(player_t *player, map_t map, int request_x, int request_y) {
    player->direction = 0;
    if (player->y_pos < ((START_Y_BACKGROUND + MAP_SIZE_H) - (PLAYER_HEIGHT + (REAL_BLOCK_SIZE / 2)))
        && collideWith(map, player, request_x, request_y) == 0) {
        player->y_pos = request_y;
    }
//    c_emission(player, DOWN_CODE);
}

/**
 * function : Move the player left
 * @param player
 * @param map
 */
void moveLeft(player_t *player, map_t map, int request_x, int request_y) {
    player->direction = 1;
    if (player->x_pos > START_X_MAP && collideWith(map, player, request_x, request_y) == 0) {
        player->x_pos = request_x;
    }
//    c_emission(player, LEFT_CODE);
}
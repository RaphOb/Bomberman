//
// Created by geoff on 30/01/2019.
//

#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL_log.h>
#include "../header/player.h"
#include "../header/map.h"
#include "../header/bit.h"
#include "../header/game.h"
#include "../header/bomb.h"

/**
 * function : init the player
 * @return a player initialised with default variables
 */
player_t initPlayer() {
    player_t p;
    char str[20] = {'\0'};
    p.alive = 'Y';
    p.co_is_ok = 0;
    p.bombPosed = 0;
    p.nbBombe = 1;
    p.frags = 0;
    p.map_x[0] = 0;
    p.map_x[1] = 0;
    p.map_y[0] = 0;
    p.map_y[1] = 0;
    p.x_pos = START_X_MAP;
    p.y_pos = START_Y_MAP;
    p.number = -1;
    p.speed = 3;
    p.current_frame = 1;
    p.frame_time = 0;
    p.direction = 2;
    p.still = 1;
    strcpy(p.name, str);
    pthread_mutex_init(&p.mutex_player, NULL);
    for (int i = 0; i < MAX_BOMBE; i++) {
        p.bomb[i] = createBomb();
    }
//    p.bomb = createBomb();
    return p;
}

bomb_t createBomb() {
    bomb_t b;

    b.range = 1;
    b.isPosed = 0;
    b.explosion = 0;
    b.frame = 0;
    b.tickExplosion = 0;
    b.tickBombDropped = 0;
    b.cell_x = -1;
    b.cell_y = -1;
    b.pos_x = -1;
    b.pos_y = -1;
    b.width = BOMB_PNG_W;
    b.height = BOMB_PNG_H;
    return b;
}

/**
 * function : Update the player's cells. A player can be on two different cells.
 * @param player
 */
void updatePlayerCell(player_t *player) {
    const int pos_x = player->x_pos - START_X_MAP;
    const int pos_y = player->y_pos - START_Y_MAP;

    player->map_x[0] = (pos_x + 1) / REAL_BLOCK_SIZE;
    player->map_y[0] = (pos_y + 1) / REAL_BLOCK_SIZE;

    player->map_x[1] = (pos_x + PLAYER_WIDTH - 1) / REAL_BLOCK_SIZE;
    player->map_y[1] = (pos_y + PLAYER_HEIGHT - 1) / REAL_BLOCK_SIZE;

}

/**
 * Function : Check if the player is colliding with something like walls
 * @param map
 * @param player
 * @param x
 * @param y
 * @return 1 if the player is colling with something, 0 if not
 */
int collideWith(map_t map, player_t *player, int x, int y) {
    const int pos_x = x - START_X_MAP;
    const int pos_y = y - START_Y_MAP;

    int cell_x = (pos_x + 1) / REAL_BLOCK_SIZE;
    int cell_y = (pos_y + 1) / REAL_BLOCK_SIZE;
    int cell_x2 = (pos_x + PLAYER_WIDTH - 1) / REAL_BLOCK_SIZE;
    int cell_y2 = (pos_y + PLAYER_HEIGHT - 1) / REAL_BLOCK_SIZE;

    // Down
    if (player->direction == 0) {
        cell_y = (pos_y + PLAYER_HEIGHT - 1) / REAL_BLOCK_SIZE;
        // Right
    } else if (player->direction == 2) {
        cell_x = (pos_x + PLAYER_WIDTH - 1) / REAL_BLOCK_SIZE;
        // Left
    } else if (player->direction == 1) {
        cell_x2 = (pos_x + 1) / REAL_BLOCK_SIZE;
        // Up
    } else {
        cell_y2 = (pos_y + 1) / REAL_BLOCK_SIZE;
    }


//    SDL_Log("y: %d, x: %d , bit : %d", cell_x, cell_y, getBit(map[cell_y], cell_x, 1));
//    SDL_Log("y2: %d, x2: %d , bit : %d", cell_x2, cell_y2, getBit(map[cell_y2], cell_x2, 1));

    return (getBit(map[cell_y], cell_x, 1) || getBit(map[cell_y2], cell_x2, 1));
//            || getBit(map[cell_y], cell_x, 3) || getBit(map[cell_y2], cell_x2, 3));
}

/**
 * function : Check if the player can placed a bomb at his current position.
 * If he is more than 60% inside a cell then he can place the bomb.
 * @param player
 * @return 1 if the player can place a bomb, 0 if not
 */
int canPlayerPlaceBomb(player_t *player, bomb_t *bomb, map_t map) {
    const float percentage = 0.6f;
    if (isPlayerOnOneCell(player)) {
        bomb->cell_x = player->map_x[0];
        bomb->cell_y = player->map_y[0];
        return !isBombOnCell(map, bomb->cell_x, bomb->cell_y);
    } else {
        if (player->map_x[0] != player->map_x[1]) {
            const int middle_x = player->map_x[1] * REAL_BLOCK_SIZE;
            const int abs_x = abs(middle_x - (player->x_pos - REAL_BLOCK_SIZE));
            const int abs_x2 = abs(middle_x - (player->x_pos - REAL_BLOCK_SIZE + PLAYER_WIDTH));

            if (abs_x >= percentage * PLAYER_WIDTH || abs_x2 >= percentage * PLAYER_WIDTH) {
                bomb->cell_x = (abs_x >= percentage * PLAYER_WIDTH) ? player->map_x[0] : player->map_x[1];
                bomb->cell_y = player->map_y[0];
                return !isBombOnCell(map, bomb->cell_x, bomb->cell_y);
            } else {
                return 0;
            }
        } else {
            const int middle_y = player->map_y[1] * REAL_BLOCK_SIZE;
            const int abs_y = abs(middle_y - (player->y_pos - (REAL_BLOCK_SIZE / 2)));
            const int abs_y2 = abs(middle_y - (player->y_pos - (REAL_BLOCK_SIZE / 2) + PLAYER_HEIGHT));

            if (abs_y >= percentage * PLAYER_HEIGHT || abs_y2 >= percentage * PLAYER_HEIGHT) {
                bomb->cell_x = player->map_x[0];
                bomb->cell_y = (abs_y >= percentage * PLAYER_HEIGHT) ? player->map_y[0] : player->map_y[1];
                return !isBombOnCell(map, bomb->cell_x, bomb->cell_y);
            } else {
                return 0;
            }
        }
    }
}

/**
 * Function : Check if the player is only in one cell
 * @param player
 * @return 1 if he is in one cell, 0 if not
 */
int isPlayerOnOneCell(player_t *player) {
    return (player->map_x[0] == player->map_x[1] && player->map_y[0] == player->map_y[1]);
}

void setPlayerXYDir(player_t *p) {
    switch (p->number) {
        case 0:
            p->x_pos = START_X_MAP;
            p->y_pos = START_Y_MAP;
            p->direction = 2;
            break;
        case 1:
            p->x_pos = MAP_SIZE_W - (REAL_BLOCK_SIZE + PLAYER_WIDTH);
            p->y_pos = START_Y_MAP;
            p->direction = 0;
            break;
        case 2:
            p->x_pos = START_X_MAP;
            p->y_pos = MAP_SIZE_H;
            p->direction = 1;
            break;
        case 3:
            p->x_pos = MAP_SIZE_W - (REAL_BLOCK_SIZE + PLAYER_WIDTH);
            p->y_pos = MAP_SIZE_H;
            p->direction = 3;
            break;
        default:
            break;
    }
}

/**
* function : return 1 if all player is dead exept MyPlayer
* @param game
* @param number
* @return
*/
int isPlayerDead(player_t players[MAX_PLAYER], int number) {
    int res = 0;
    int nbkill = 0;
    for (int i = 0; i < MAX_PLAYER; i++) {
        if (players[i].alive == 'N' && players[i].number != number) {
            nbkill++;
        }
    }
    if (nbkill == getNbPlayer(players) - 1) {
        res = 1;
        return res;
    }
    return res;
}
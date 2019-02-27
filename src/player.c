//
// Created by geoff on 30/01/2019.
//

#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL_log.h>
#include "../header/player.h"
#include "../header/map.h"
#include "../header/bit.h"

player_t *initPlayer()
{
    player_t *p = malloc(sizeof(player_t));
    if(!p) {
      SDL_Log("Erreur malloc player");
      return NULL;
    }
    bomb_t *b = malloc(sizeof(bomb_t));
    if (!b) {
        SDL_Log("Erreur malloc bombPosed");
        return NULL;
    }
    p->alive = 'Y';
    p->bombPosed = 0;
    p->bombs_left = 20;
    p->frags = 0;
    p->map_x[0] = 0;
    p->map_x[1] = 0;
    p->map_y[0] = 0;
    p->map_y[1] = 0;
    p->x_pos = START_X_MAP;
    p->y_pos = START_Y_MAP;
    p->speed = 1;
    p->number = 1;
    p->current_frame = 1;
    p->frame_time = 0;
    p->direction = 2;
    p->still = 1;
    b->explosion = 0;
    p->bomb = b;

    return p;
}

void updatePlayerCell(player_t *player)
{
    const int pos_x = player->x_pos - 80;
    const int pos_y = player->y_pos - 40;
    const int pSizeBlock = BLOCK_SIZE * SIZE_M;

    player->map_x[0] = (pos_x) / pSizeBlock;
    player->map_y[0] = (pos_y) / pSizeBlock;

    player->map_x[1] = (pos_x + PLAYER_WIDTH) / pSizeBlock;
    player->map_y[1] = (pos_y + PLAYER_HEIGHT) / pSizeBlock;

//    SDL_Log("player_map_x[0] : %d, player_map_x[1] : %d, player_map_y[0] : %d, player_map_y[1] : %d", player->map_x[0], player->map_x[1], player->map_y[0], player->map_y[1]);


}

int collideWith(map_t map, player_t *player, int x, int y)
{
    const int pSizeBlock = BLOCK_SIZE * SIZE_M;
    const int pos_x = x - 80;
    const int pos_y = y - 40;
    int cell_x = (pos_x + 1) / pSizeBlock;
    int cell_y = (pos_y + 1) / pSizeBlock;
//    SDL_Log("%d, %d", PLAYER_WIDTH, PLAYER_HEIGHT);
    int cell_x2 = (pos_x + PLAYER_WIDTH - 1) / pSizeBlock;
    int cell_y2 = (pos_y + PLAYER_HEIGHT - 1) / pSizeBlock;

    // Down
    if (player->direction == 0) {
        cell_y = (pos_y + PLAYER_HEIGHT - 1) / pSizeBlock;
    // Right
    } else if (player->direction == 2) {
        cell_x = (pos_x + PLAYER_WIDTH - 1) / pSizeBlock;
    // Left
    } else if (player->direction == 1) {
        cell_x2 = (pos_x + 1) / pSizeBlock;
    // Up
    } else {
        cell_y2 = (pos_y + 1) / pSizeBlock;
    }

//    SDL_Log("y: %d, x: %d , bit : %d", cell_x, cell_y, getBit(map[cell_y], cell_x, 1));
//    SDL_Log("y2: %d, x2: %d , bit : %d", cell_x2, cell_y2, getBit(map[cell_y2], cell_x2, 1));

    return (getBit(map[cell_y], cell_x, 1) == 1 || getBit(map[cell_y2], cell_x2, 1) == 1);
}

int canPlayerPlaceBomb(player_t *player)
{
    const float percentage = 0.6f;
    if (isPlayerOnOneCell(player)) {
        player->bomb->x_pos = player->map_x[0];
        player->bomb->y_pos = player->map_y[0];
        return 1;
    } else {
        if (player->map_x[0] != player->map_x[1]) {
            const int pos_x = player->x_pos - 80;
            const int pos_x2 = player->x_pos - 80 + PLAYER_WIDTH;
            const int middle_x = player->map_x[1] * BLOCK_SIZE * SIZE_M;
            const int abs_x = abs(middle_x - pos_x);
            const int abs_x2 = abs(middle_x - pos_x2);
//            SDL_Log("abs_x: %d", abs_x);
//            SDL_Log("abs_x2: %d", abs_x2);
//            SDL_Log("0.8 * player_width: %f", percentage * PLAYER_WIDTH);
//            SDL_Log("1: %d, 2: %d", abs_x >= percentage * PLAYER_WIDTH, abs_x2 >= percentage * PLAYER_WIDTH);
            if (abs_x >= percentage * PLAYER_WIDTH || abs_x2 >= percentage * PLAYER_WIDTH) {
                player->bomb->x_pos = (abs_x >= percentage * PLAYER_WIDTH) ? player->map_x[0] : player->map_x[1];
                player->bomb->y_pos = player->map_y[0];
//                SDL_Log("x_pos: %d, y_pos: %d", player->bomb->x_pos, player->bomb->y_pos);
                return 1;
            } else {
                return 0;
            }
        } else {
            const int pos_y = player->y_pos - 40;
            const int pos_y2 = player->y_pos - 40 + PLAYER_HEIGHT;
            const int middle_y = player->map_y[1] * BLOCK_SIZE * SIZE_M;
            const int abs_y = abs(middle_y - pos_y);
            const int abs_y2 = abs(middle_y - pos_y2);
//            SDL_Log("abs_y: %d", abs_y);
//            SDL_Log("abs_y2: %d", abs_y2);
//            SDL_Log("0.8 * player_height: %f", percentage * PLAYER_HEIGHT);
//            SDL_Log("1: %d, 2: %d", abs_y >= percentage * PLAYER_HEIGHT, abs_y2 >= percentage * PLAYER_HEIGHT);
            if (abs_y >= percentage * PLAYER_HEIGHT || abs_y2 >= percentage * PLAYER_HEIGHT) {
                player->bomb->x_pos = player->map_x[0];
                player->bomb->y_pos = (abs_y >= percentage * PLAYER_HEIGHT) ? player->map_y[0] : player->map_y[1];
//                SDL_Log("x_pos: %d, y_pos: %d", player->bomb->x_pos, player->bomb->y_pos);
                return 1;
            } else {
                return 0;
            }

        }
    }
}

int isPlayerOnOneCell(player_t *player)
{
    return (player->map_x[0] == player->map_x[1] && player->map_y[0] == player->map_y[1]);
}
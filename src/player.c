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

player_t initPlayer()
{
    player_t p;

    p.alive = 'Y';
    p.co_is_ok = 0;
    p.bombPosed = 0;
    p.bombs_left = 20;
    p.frags = 0;
    p.map_x[0] = 0;
    p.map_x[1] = 0;
    p.map_y[0] = 0;
    p.map_y[1] = 0;
    p.x_pos = START_X_MAP;
    p.y_pos = START_Y_MAP;
    p.number = -1;
    p.current_frame = 1;
    p.frame_time = 0;
    p.direction = 2;
    p.still = 1;
    pthread_mutex_init(&p.mutex_player, NULL);
    bomb_t b;
    b.range = 1;
    b.explosion = 0;
    p.bomb = b;

    return p;
}

void updatePlayerCell(player_t *player)
{
    const int pos_x = player->x_pos - 80;
    const int pos_y = player->y_pos - 40;

    player->map_x[0] = (pos_x + 1) / REAL_BLOCK_SIZE;
    player->map_y[0] = (pos_y + 1) / REAL_BLOCK_SIZE;

    player->map_x[1] = (pos_x + PLAYER_WIDTH - 1) / REAL_BLOCK_SIZE;
    player->map_y[1] = (pos_y + PLAYER_HEIGHT - 1) / REAL_BLOCK_SIZE;

//    SDL_Log("player_map_x[0] : %d, player_map_x[1] : %d, player_map_y[0] : %d, player_map_y[1] : %d", player->map_x[0], player->map_x[1], player->map_y[0], player->map_y[1]);


}

int collideWith(map_t map, player_t *player, int x, int y)
{
    const int pos_x = x - 80;
    const int pos_y = y - 40;
    int cell_x = (pos_x + 1) / REAL_BLOCK_SIZE;
    int cell_y = (pos_y + 1) / REAL_BLOCK_SIZE;
//    SDL_Log("%d, %d", PLAYER_WIDTH, PLAYER_HEIGHT);
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

    return (getBit(map[cell_y], cell_x, 1) == 1 || getBit(map[cell_y2], cell_x2, 1) == 1);
}

int canPlayerPlaceBomb(player_t *player)
{
    const float percentage = 0.6f;
    if (isPlayerOnOneCell(player)) {
        player->bomb.x_pos = player->map_x[0];
        player->bomb.y_pos = player->map_y[0];
        return 1;
    } else {
        if (player->map_x[0] != player->map_x[1]) {
            const int middle_x = player->map_x[1] * REAL_BLOCK_SIZE;
            const int abs_x = abs(middle_x - (player->x_pos - REAL_BLOCK_SIZE));
            const int abs_x2 = abs(middle_x - (player->x_pos - REAL_BLOCK_SIZE + PLAYER_WIDTH));

            if (abs_x >= percentage * PLAYER_WIDTH || abs_x2 >= percentage * PLAYER_WIDTH) {
                player->bomb.x_pos = (abs_x >= percentage * PLAYER_WIDTH) ? player->map_x[0] : player->map_x[1];
                player->bomb.y_pos = player->map_y[0];
                return 1;
            } else
                return 0;
        } else {
            const int middle_y = player->map_y[1] * REAL_BLOCK_SIZE;
            const int abs_y = abs(middle_y - (player->y_pos - (REAL_BLOCK_SIZE / 2)));
            const int abs_y2 = abs(middle_y - (player->y_pos - (REAL_BLOCK_SIZE / 2) + PLAYER_HEIGHT));

            if (abs_y >= percentage * PLAYER_HEIGHT || abs_y2 >= percentage * PLAYER_HEIGHT) {
                player->bomb.x_pos = player->map_x[0];
                player->bomb.y_pos = (abs_y >= percentage * PLAYER_HEIGHT) ? player->map_y[0] : player->map_y[1];
                return 1;
            } else
                return 0;
            }
    }
}

int isPlayerOnOneCell(player_t *player)
{
    return (player->map_x[0] == player->map_x[1] && player->map_y[0] == player->map_y[1]);
}
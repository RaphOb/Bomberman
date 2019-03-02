//
// Created by geoff on 30/01/2019.
//

#include <stdlib.h>
#include <SDL2/SDL_log.h>
#include "../header/player.h"
#include "../header/map.h"
#include "../header/bit.h"

player_t initPlayer()
{
    player_t p;

    bomb_t *b = malloc(sizeof(bomb_t));
    if (!b) {
        SDL_Log("Erreur malloc bombPosed");
        //return NULL;
    }
    p.alive = 'Y';
    p.bombPosed = 0;
    p.bombs_left = 20;
    p.frags = 0;
    p.map_x[0] = 0;
    p.map_x[1] = 0;
    p.map_y[0] = 0;
    p.map_y[1] = 0;
    p.x_pos = START_X_MAP;
    p.y_pos = START_Y_MAP;
    p.number = 1;
    p.current_frame = 1;
    p.frame_time = 0;
    p.direction = 2;
    p.still = 1;
    b->explosion = 0;
    p.bomb = b;
    pthread_mutex_init(&p.mutex_player, NULL);

    return p;
}

void updatePlayerCell(player_t *player)
{
    const int pos_x = player->x_pos - 80;
    const int pos_y = player->y_pos - 40;
    const int pSizeBlock = BLOCK_SIZE * SIZE_M;

    player->map_x[0] = pos_x / pSizeBlock;
    player->map_y[0] = pos_y / pSizeBlock;

    player->map_x[1] = (pos_x + PLAYER_WIDTH) / pSizeBlock;
    player->map_y[1] = (pos_y + PLAYER_WIDTH) / pSizeBlock;

//    SDL_Log("player_map_x[0] : %d, player_map_x[1] : %d, player_map_y[0] : %d, player_map_y[1] : %d", player->map_x[0], player->map_x[1], player->map_y[0], player->map_y[1]);


}

int collideWith(map_t map, player_t *player, int x, int y)
{
    const int pSizeBlock = BLOCK_SIZE * SIZE_M;
    const int pos_x = x - 80;
    const int pos_y = y - 40;
    int cell_x = (pos_x + 1) / pSizeBlock;
    int cell_y = (pos_y + 1) / pSizeBlock;
    int cell_x2 = (pos_x + PLAYER_WIDTH - 1) / pSizeBlock;
    int cell_y2 = (pos_y + PLAYER_HEIGHT - 1) / pSizeBlock;

    // Down
    if (player->direction == 2) {
        cell_y = (pos_y + PLAYER_HEIGHT - 1) / pSizeBlock;
//        cell_x = (pos_x + PLAYER_WIDTH - 5) / pSizeBlock;
//        cell_x2 = (pos_x + PLAYER_WIDTH - 5) / pSizeBlock;
    // Right
    } else if (player->direction == 1) {
        cell_x = (pos_x + PLAYER_WIDTH - 5) / pSizeBlock;
    // Left
    } else if (player->direction == 3) {
        cell_x2 = (pos_x + 1) / pSizeBlock;
    // Up
    } else {
        cell_y2 = (pos_y + 1) / pSizeBlock;
    }

//    SDL_Log("y: %d, x: %d , bit : %d", cell_x, cell_y, getBit(map[cell_y], cell_x, 1));
//    SDL_Log("y2: %d, x2: %d , bit : %d", cell_x2, cell_y2, getBit(map[cell_y2], cell_x2, 1));

    return (getBit(map[cell_y], cell_x, 1) == 1 || getBit(map[cell_y2], cell_x2, 1) == 1);
}

int isPlayerOnOneCell(player_t *player) {
    return (player->map_x[0] == player->map_x[1] && player->map_y[0] == player->map_y[1]);
}
//
// Created by geoff on 30/01/2019.
//

#include <stdlib.h>
#include <SDL_log.h>
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
    p->alive = 'Y';
    p->bomb = 0;
    p->bombs_left = 20;
    p->frags = 0;
    p->map_x[0] = 0;
    p->map_x[1] = 0;
    p->map_y[0] = 0;
    p->map_y[1] = 0;
    p->x_pos = START_X_MAP;
    p->y_pos = START_Y_MAP;
    p->number = 1;
    p->current_frame = 1;
    p->frame_time = 0;
    p->direction = 2;
    p->still = 1;

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

    SDL_Log("player_map_x[0] : %d, player_map_x[1] : %d, player_map_y[0] : %d, player_map_y[1] : %d", player->map_x[0], player->map_x[1], player->map_y[0], player->map_y[1]);


}

int collideWith(map_t map, player_t *player, int x, int y)
{
    const int pSizeBlock = BLOCK_SIZE * SIZE_M;
    const int pos_x = x - 80;
    const int pos_y = y - 40;
    int cell_x = (pos_x) / pSizeBlock;
    int cell_y = (pos_y + 10) / pSizeBlock;
    int cell_x2 = (pos_x + PLAYER_WIDTH - 5) / pSizeBlock;
    int cell_y2 = (pos_y + PLAYER_HEIGHT - 5) / pSizeBlock;
//    if  (player->direction == 3) {
//        cell_x = x / pSizeBlock;
//        SDL_Log("x / pSizeBlock: %d", x / pSizeBlock);
    if (player->direction == 2) {
        cell_y = (pos_y + PLAYER_HEIGHT) / pSizeBlock;
        SDL_Log(" (x + PLAYER_WIDTH) / pSizeBlock : %d", (pos_x + PLAYER_WIDTH) / pSizeBlock);
//    } else if (player->direction == 0) {
//        cell_y = y / pSizeBlock;
//        SDL_Log("y / pSizeBlock: %d", y / pSizeBlock);
    } else if (player->direction == 1) {
        cell_x = (pos_x + PLAYER_WIDTH - 5) / pSizeBlock;
        SDL_Log("(y + PLAYER_HEIGHT) / pSizeBlock: %d", (pos_y + PLAYER_HEIGHT) / pSizeBlock);
    } else if (player->direction == 3) {
        cell_x2 = (pos_x) / pSizeBlock;
    } else {
        cell_y2 = (pos_y + 10) / pSizeBlock;
    }
//    const int cell_x = (player->direction == 3) ? x / pSizeBlock : (x + PLAYER_WIDTH) / pSizeBlock;
//    const int cell_y = (player->direction == 0) ? y / pSizeBlock : (y + PLAYER_HEIGHT) / pSizeBlock;

//    displayArray(map);
    SDL_Log("y: %d, x: %d , bit : %d", cell_x, cell_y, getBit(map[cell_x], cell_y, 1));
    if (getBit(map[cell_y], cell_x, 1) == 1 || getBit(map[cell_y2], cell_x2, 1) == 1) {
        SDL_Log("blocked");
        return (1);
    } else {
        return (0);
    }
//    return (0);
}
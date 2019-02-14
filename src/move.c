//
// Created by geoff on 14/02/2019.
//

#include "../header/move.h"

const move_t move[] = {
        {moveUp, SDL_SCANCODE_UP},
        {moveDown, SDL_SCANCODE_DOWN},
        {moveLeft, SDL_SCANCODE_LEFT},
        {moveRight, SDL_SCANCODE_RIGHT},
        {NULL, 0}
};

int doMove(const Uint8 *keystates, player_t *player)
{
    int i = 0;
    player->still = 0;

    while (move[i].key != 0) {
        if (keystates[move[i].key]) {
            move[i].func_move(player);
            return (1);
        }
        i++;
    }
    player->still = 1;
    return (0);
}

void moveUp(player_t *player)
{
    player->direction = 0;
    if (player->y_pos > START_Y_MAP) {
        player->y_pos -= 3;
    }
}

void moveRight(player_t *player)
{
    player->direction = 1;
    if (player->x_pos < ((START_X_BACKGROUND + MAP_SIZE_W) - (PLAYER_WIDTH + (BLOCK_SIZE * SIZE_M)))) {//player->pSDL->dst_player.w + 70))) {
        player->x_pos += 3;
    }
}

void moveDown(player_t *player)
{
    player->direction = 2;
    if (player->y_pos < ((START_Y_BACKGROUND + MAP_SIZE_H) - (PLAYER_HEIGHT + (BLOCK_SIZE * SIZE_M / 2)))) {//player->pSDL->dst_player.h + 30))) {
        player->y_pos += 3;
    }
}

void moveLeft(player_t *player)
{
    player->direction = 3;
    if (player->x_pos > START_X_MAP) {
        player->x_pos -= 3;
    }
}
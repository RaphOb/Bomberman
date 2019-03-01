//
// Created by geoff on 14/02/2019.
//

#include "../header/move.h"
#include "../header/reseau.h"

const move_t move[] = {
        {moveUp, SDL_SCANCODE_UP},
        {moveDown, SDL_SCANCODE_DOWN},
        {moveLeft, SDL_SCANCODE_LEFT},
        {moveRight, SDL_SCANCODE_RIGHT},
        {NULL, 0}
};

int doMove(const Uint8 *keystates, player_t *player, map_t map)
{
    int i = 0;
    player->still = 0;

    while (move[i].key != 0) {
        if (keystates[move[i].key]) {
            move[i].func_move(player, map);
            updatePlayerCell(player);
            return (1);
        }
        i++;
    }
    player->still = 1;
    return (0);
}

void moveUp(player_t *player, map_t map)
{
    c_emission(UP_CODE);
    player->direction = 3;
    if (player->y_pos > START_Y_MAP && collideWith(map, player, player->x_pos, player->y_pos - 3) == 0) {
        player->y_pos -= 3 * player->speed;
    }
}

void moveRight(player_t *player, map_t map)
{
    c_emission(RIGHT_CODE);
    player->direction = 2;
    if (player->x_pos < ((START_X_BACKGROUND + MAP_SIZE_W) - (PLAYER_WIDTH + (REAL_BLOCK_SIZE)))
        && collideWith(map, player, player->x_pos + 3, player->y_pos) == 0) {
        player->x_pos += 3 * player->speed;
    }
}

void moveDown(player_t *player, map_t map)
{
    c_emission(DOWN_CODE);
    player->direction = 0;
    if (player->y_pos < ((START_Y_BACKGROUND + MAP_SIZE_H) - (PLAYER_HEIGHT + (REAL_BLOCK_SIZE / 2)))
        && collideWith(map, player, player->x_pos, player->y_pos + 3) == 0) {
        player->y_pos += 3 * player->speed;
    }
}

void moveLeft(player_t *player, map_t map)
{
    c_emission(LEFT_CODE);
    player->direction = 1;
    if (player->x_pos > START_X_MAP && collideWith(map, player, player->x_pos - 3, player->y_pos) == 0) {
        player->x_pos -= 3 * player->speed;
    }
}
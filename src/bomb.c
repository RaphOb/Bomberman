//
// Created by geoff on 21/03/2019.
//

#include "../header/bomb.h"
#include "../header/bit.h"
#include "../header/bonus.h"

/**
 * function : When a bomb explode, this function is called and it updates some variables to trigger the animation
 * and play the sound.
 * @param player
 * @param son
 */
void makeExplosion(bomb_t *bomb, son_t* son)
{
//    SDL_Log("x: %d, y: %d", pSDL->dst_bomb.x, pSDL->dst_bomb.y);
    bomb->tickBombDropped = 0;
    bomb->isPosed = 0;
    bomb->explosion = 1;
    bomb->tickExplosion = SDL_GetTicks();
    playSound(son);

}

int getIndexBomb(player_t *p)
{
    for (int i = 0; i < p->nbBombe; i++) {
//        if (i == 0 && p->bombPosed == 0) return 0;
        if (p->bomb[i].isPosed == 0 && p->bomb[i].explosion == 0) {
            return i;
        }
    }

}

/**
 * function : Update the position of the bomb to place it in the middle of the cell where the player is and update some variables to trigger the animation
 * @param pSDL
 * @param player
 * @param map
 */
void placeBomb(sdl_t *pSDL, player_t *player, bomb_t *bomb)
{
    int pos_x = START_X_MAP + (bomb->cell_x * REAL_BLOCK_SIZE) + (REAL_BLOCK_SIZE / 2) - (BOMB_PNG_W / 2);
    int pos_y = START_Y_MAP + (bomb->cell_y * REAL_BLOCK_SIZE) + (REAL_BLOCK_SIZE / 2) - (BOMB_PNG_H / 2);

    bomb->pos_x = pos_x;
    bomb->pos_y = pos_y;
    bomb->width = BOMB_PNG_W;
    bomb->height = BOMB_PNG_H;
    pSDL->dst_bomb[player->bombPosed].h = BOMB_PNG_H;
    pSDL->dst_bomb[player->bombPosed].w = BOMB_PNG_W;
    pSDL->dst_bomb[player->bombPosed].x = pos_x;
    pSDL->dst_bomb[player->bombPosed].y = pos_y;
//    SDL_Log("bombposed: %d, x: %d, y: %d", player->bombPosed, pSDL->dst_bomb[player->bombPosed].x, pSDL->dst_bomb[player->bombPosed].y);
    bomb->isPosed = 1;
    player->bombPosed++;
    bomb->tickBombDropped = SDL_GetTicks();

}
/**
 * function : check if player is within a bomb's range
 * @param player
 * @param b
 * @param pSDL
 */
void checkBombPlayer(player_t *player, bomb_t b) {
    const int bpos_x = b.cell_x;
    const int bpos_y = b.cell_y;
    const int ppos_x = player->map_x[0];
    const int ppos_y = player->map_y[0];
//    SDL_Log("%d", player->map_x[0]);
//    SDL_Log("%d", b.cell_x);

    //left
    if ((bpos_x - b.range == ppos_x || bpos_x == ppos_x) && bpos_y == ppos_y) {
        player->alive = 'N';
        SDL_Log("leffft");
    }
        //right
    else if ((bpos_x + b.range == ppos_x) && bpos_y == ppos_y) {
        player->alive = 'N';
        SDL_Log("right");
    }
        //top
    else if ((bpos_y - b.range == ppos_y) && bpos_x == ppos_x) {
        player->alive = 'N';
        SDL_Log("top");
    }
        //bottom
    else if ((bpos_y + b.range == ppos_y || bpos_y == ppos_y) && bpos_x == ppos_x) {
        player->alive = 'N';
        SDL_Log("bottom");
    }
//    c_emission(player, 0);
}

void checkExplosion(game_t *game, bomb_t bomb)
{
    if (bomb.explosion == 1) {
        toggleBit(game->map[bomb.cell_y], bomb.cell_x , 3);
        for (int i = 0; i < MAX_PLAYER; i++) {
//            SDL_Log("alive: %c", game->players[i].alive);
            if (game->players[i].alive == 'Y') {
                checkBombPlayer(&game->players[i], bomb);
            }
        }
    }
}

/**
 * function : check if block is within a bomb's range
 * @param map
 * @param b
 */
void checkBombDamage(map_t map, bomb_t b)
{
    const int pos_x = b.cell_x;
    const int pos_y = b.cell_y;
    int destroyed[4] = {0};
    int block[4] = {0};
    for (int i = 1; i <= b.range; i++) {
        // Left
        if (pos_x - i >= 0 && destroyed[0] == 0 && block[0] == 0) {
            destroyed[0] = destroyBlock(map, pos_x - i, pos_y);
            block[0] = (getBit(map[pos_y], pos_x - i, 1) && !getBit(map[pos_y], pos_x - i, 2));
        }
        // Right
        if (pos_x + i <= 12 && destroyed[1] == 0 && block[1] == 0) {
            destroyed[1] = destroyBlock(map, pos_x + i, pos_y);
            block[1] = (getBit(map[pos_y], pos_x + i, 1) && !getBit(map[pos_y], pos_x + i, 2));
        }
        // Up
        if (pos_y - i >= 0 && destroyed[2] == 0 && block[2] == 0) {
            destroyed[2] = destroyBlock(map, pos_x, pos_y - i);
            block[2] = (getBit(map[pos_y - i], pos_x, 1) && !getBit(map[pos_y - i], pos_x, 2));
        }
        // Down
        if (pos_y + i <= 8 && destroyed[3] == 0 && block[3] == 0) {
            destroyed[3] = destroyBlock(map, pos_x, pos_y + i);
            block[3] = (getBit(map[pos_y + i], pos_x, 1) && !getBit(map[pos_y + i], pos_x, 2));
        }
    }
}

/**
 * function : Toggle bit to destroy block
 * @param map
 * @param pos_x
 * @param pos_y
 */
int destroyBlock(map_t map, int pos_x, int pos_y)
{
    if (getBit(map[pos_y], pos_x, 1) == 1 && getBit(map[pos_y], pos_x, 2) == 1) {
        toggleBit(map[pos_y], pos_x, 1);
        toggleBit(map[pos_y], pos_x, 2);
        if ((rand() % 4) == 1) {
            spawnBonus(map, pos_x, pos_y);
        }
        return 1;
    }
    return 0;
}

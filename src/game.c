//
// Created by geoff on 16/01/2019.
//

#include <SDL2/SDL_events.h>
#include <stdio.h>
#include "../header/game.h"
#include "../header/map.h"
#include "../header/map.h"
#include "../header/move.h"
#include "../header/bit.h"
#include "../header/renderer.h"
#include "../header/reseau.h"
#include "../header/bonus.h"

/**
 * function : init game
 * @return
 */
game_t *initGame(sdl_t *pSDL)
{
    game_t *game = malloc(sizeof(game_t));
    if (!game) {
        return (NULL);
    }

    if (extractArrayFromFile(game->map) == 0) {
        return (NULL);
    }
    game->pSDL = pSDL;
    for (int i = 0; i < MAX_PLAYER ; i++) {
        game->players[i].number = -1;
    }
    return game;
}


player_t *getMyPlayer(game_t *g)
{
    return &g->players[g->nb_client_serv];
}

/**
 * function : key press event
 * @param game
 * @return : int
 */
int gameEvent(game_t *game)
{
    int res = 0;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    player_t *p = getMyPlayer(game);

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            c_emission(p, DISCONNECT_CODE);
            res = -1;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE :
                    c_emission(p, DISCONNECT_CODE);
                    res = -1;
                    break;
                case SDLK_b:
                    c_emission(p, BOMB_CODE);
                    if (p->bombPosed == 0 && canPlayerPlaceBomb(p)) {
                        placeBomb(game->pSDL, p, game->map);
                    }
                    break;
                default :
//                    SDL_Log("touche inconnue %d\n", event.key.keysym.sym);
                    break;
            }
        }
    }
        
    if (p->bomb.explosion == 1) {
        toggleBit(game->map[p->bomb.y_pos], p->bomb.x_pos , 3);
        for (int i = 0; i < MAX_PLAYER; i++) {
            if (game->players[i].alive == 'Y') {
                checkBombPlayer(&game->players[i], game->players[i].bomb);
            }
        }
    }
    doMove(keystates, p, game->map);
    return res;
}

/**
 * function : When a bomb explode, this function is called and it updates some variables to trigger the animation
 * and play the sound.
 * @param player
 * @param son
 */
void makeExplosion(player_t *player, son_t* son)
{
//    SDL_Log("x: %d, y: %d", pSDL->dst_bomb.x, pSDL->dst_bomb.y);
    player->bomb.explosion = 1;
    player->bomb.tickExplosion = SDL_GetTicks();
    playSound(son);

}

/**
 * function : Update the position of the bomb to place it in the middle of the cell where the player is and update some variables to trigger the animation
 * @param pSDL
 * @param player
 * @param map
 */
void placeBomb(sdl_t *pSDL, player_t *player, map_t map)
{
    int cell_x = START_X_MAP + (player->bomb.x_pos * REAL_BLOCK_SIZE) + (REAL_BLOCK_SIZE / 2) - (BOMB_PNG_W / 2);
    int cell_y = START_Y_MAP + (player->bomb.y_pos * REAL_BLOCK_SIZE) + (REAL_BLOCK_SIZE / 2) - (BOMB_PNG_H / 2);

    pSDL->dst_bomb.h = BOMB_PNG_H;
    pSDL->dst_bomb.w = BOMB_PNG_W;
    pSDL->dst_bomb.x = cell_x;
    pSDL->dst_bomb.y = cell_y;
    toggleBit(map[player->bomb.x_pos], player->bomb.y_pos, 3);
//    SDL_Log("x: %d, y: %d", game->pSDL->dst_bomb.x, game->pSDL->dst_bomb.y);

    player->bombPosed = 1;
    player->bomb.tickBombDropped = SDL_GetTicks();

}
/**
 * function : check if player is within a bomb's range
 * @param player
 * @param b
 * @param pSDL
 */
void checkBombPlayer(player_t *player, bomb_t b) {
    const int bpos_x = b.x_pos;
    const int bpos_y = b.y_pos;
    const int ppos_x = player->map_x[0];
    const int ppos_y = player->map_y[0];
//    SDL_Log("%d", player->map_x[0]);
//    SDL_Log("%d", b.y_pos);

    //left
    if ((bpos_x - b.range == ppos_x || bpos_x == ppos_x) && bpos_y == ppos_y ) {
        player->alive = 'N';
        //SDL_Log("leffft");
    }
    //right
    else if ((bpos_x + b.range == ppos_x) && bpos_y == ppos_y) {
        player->alive = 'N';
        //SDL_Log("right");
    }
    //top
    else if ((bpos_y - b.range == ppos_y) && bpos_x == ppos_x) {
        player->alive = 'N';
        //SDL_Log("top");
    }
    //bottom
    else if ((bpos_y + b.range == ppos_y || bpos_y == ppos_y) && bpos_x == ppos_x) {
        player->alive = 'N';
        //SDL_Log("bottom");
    }
}
/**
 * function : check if block is within a bomb's range
 * @param map
 * @param b
 */
void checkBombDamage(map_t map, bomb_t b)
{
    const int pos_x = b.x_pos;
    const int pos_y = b.y_pos;
    int destroyedUp = 0;
    int destroyedDown = 0;
    int destroyedLeft = 0;
    int destroyedRight = 0;
    for (int i = 1; i <= b.range; i++) {
        // Left
        if (pos_x - i >= 0 && destroyedLeft == 0) {
            destroyedLeft = destroyBlock(map, pos_x - i, pos_y);
        }
        // Right
        if (pos_x + i <= 12 && destroyedRight == 0) {
            destroyedRight = destroyBlock(map, pos_x + i, pos_y);
        }
        // Up
        if (pos_y - i >= 0 && destroyedUp == 0) {
            destroyedUp = destroyBlock(map, pos_x, pos_y - i);
        }
        // Down
        if (pos_y + i <= 8 && destroyedDown == 0) {
            destroyedDown = destroyBlock(map, pos_x, pos_y + i);
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
            SDL_Log("random");
            spawnBonus(map, pos_x, pos_y);
        }
        return 1;
    }
    return 0;
}

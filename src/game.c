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
    return game;
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

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            res = -1;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE :
                    c_emission(&game->players[0], DISCONNECT_CODE);
                    res = -1;
                    break;
                case SDLK_b:
                    c_emission(&game->players[0], BOMB_CODE);
                    if (game->players[0].bombPosed == 0 && canPlayerPlaceBomb(&game->players[0])) {
                        placeBomb(game->pSDL, &game->players[0]);
                    }
                    break;
                default :
                    fprintf(stderr,"touche inconnue %d\n", event.key.keysym.sym);
                    break;
            }
        }
    }
        
    if (game->players[0].bomb.explosion == 1) {
        checkBombDamage(game->map, game->players[0].bomb);
        checkBombPlayer(&game->players[0], game->players[0].bomb);
    }
    doMove(keystates, &game->players[0], game->map);
    return res;
}

void makeExplosion(player_t *player, son_t* son)
{
//    SDL_Log("x: %d, y: %d", pSDL->dst_bomb.x, pSDL->dst_bomb.y);
    player->bomb.explosion = 1;
    player->bomb.tickExplosion = SDL_GetTicks();
    playSound(son);

}

void placeBomb(sdl_t *pSDL, player_t *player)
{
    int cell_x = START_X_MAP + (player->bomb.x_pos * REAL_BLOCK_SIZE) + (REAL_BLOCK_SIZE / 2) - (BOMB_PNG_W / 2);
    int cell_y = START_Y_MAP + (player->bomb.y_pos * REAL_BLOCK_SIZE) + (REAL_BLOCK_SIZE / 2) - (BOMB_PNG_H / 2);

    pSDL->dst_bomb.h = BOMB_PNG_H;
    pSDL->dst_bomb.w = BOMB_PNG_W;
    pSDL->dst_bomb.x = cell_x;
    pSDL->dst_bomb.y = cell_y;
//    SDL_Log("x: %d, y: %d", game->pSDL->dst_bomb.x, game->pSDL->dst_bomb.y);

    player->bombPosed = 1;
    player->bomb.tickBombDropped = SDL_GetTicks();

}
/**
 * function : check  if player within bombrange
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
    if ((bpos_x - 1 == ppos_x || bpos_x == ppos_x) && bpos_y == ppos_y ) {
        //SDL_Log("leffft");
    }
    //right
    if ((bpos_x + 1 == ppos_x) && bpos_y == ppos_y) {
        //SDL_Log("right");
    }
    //top
    if ((bpos_y - 1 == ppos_y) && bpos_x == ppos_x) {
        //SDL_Log("top");
    }
    //bottom
    if ((bpos_y + 1 == ppos_y || bpos_y == ppos_y) && bpos_x == ppos_x) {
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

    // Left
    if (pos_x - 1 >= 0) {
        destroyBlock(map, pos_x - 1, pos_y);
    }
    // Right
    if (pos_x + 1 <= 12) {
        destroyBlock(map, pos_x + 1, pos_y);
    }
    // Up
    if (pos_y - 1 >= 0) {
        destroyBlock(map, pos_x, pos_y - 1);
    }
    // Down
    if (pos_y + 1 <= 8) {
        destroyBlock(map, pos_x, pos_y + 1);
    }
}

void destroyBlock(map_t map, int pos_x, int pos_y)
{
    if (getBit(map[pos_y], pos_x, 1) == 1 && getBit(map[pos_y], pos_x, 2) == 1) {
        toggleBit(map[pos_y], pos_x, 1);
        toggleBit(map[pos_y], pos_x, 2);

    }
}

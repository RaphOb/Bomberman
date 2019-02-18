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

    game->current_map = 0;
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
            //
            switch (event.key.keysym.sym) {
                case SDLK_p:
                    game->intmenu = 1;
                    break;
                case SDLK_ESCAPE :
                    res = -1;
                    break;
                case SDLK_b:
                    if (game->players[0]->bombPosed == 0 && game->players[0]->bomb->explosion == 0 && isPlayerOnOneCell(game->players[0]))
                        placeBomb(game);
                    break;
                default :
                    fprintf(stderr,"touche inconnue %d\n", event.key.keysym.sym);
                    break;
            }
        }
    }
    if (game->players[0]->bomb->explosion == 1) {
        destroyBlock(game->map, game->players[0]->bomb);
    }
    doMove(keystates, game->players[0], game->map);

    return res;
}

void makeExplosion(game_t *game)
{
    SDL_Log("x: %d, y: %d", game->pSDL->dst_bomb.x, game->pSDL->dst_bomb.y);
    game->pSDL->dst_explosion2.h = 1;
    game->pSDL->dst_explosion2.w = 1;
    game->players[0]->bomb->explosion = 1;
    game->players[0]->bomb->tickExplosion = SDL_GetTicks();
//    game->pSDL->dst_explosion2.x = game->pSDL->dst_bomb.x + ((game->pSDL->dst_bomb.w - game->pSDL->dst_explosion2.w) / 2);
//    game->pSDL->dst_explosion2.y = game->pSDL->dst_bomb.y + ((game->pSDL->dst_bomb.h - game->pSDL->dst_explosion2.h) / 2);
//    SDL_Log("explo : x: %d, y: %d", game->pSDL->dst_explosion2.x, game->pSDL->dst_explosion2.y);

//    game->pSDL->dst_explosion.x = game->pSDL->dst_explosion2.x + 40;
//    game->pSDL->dst_explosion.y = game->pSDL->dst_explosion2.y + 30;
//    game->pSDL->dst_explosion.h = 10;
//    game->pSDL->dst_explosion.h = BLOCK_SIZE * SIZE_M * 2;
//    game->pSDL->dst_explosion.w = 10;
//    game->pSDL->dst_explosion.w = BLOCK_SIZE * SIZE_M * 2;

}

void placeBomb(game_t *game)
{
    const int pSizeBlock = BLOCK_SIZE * SIZE_M;
    int cell_x = START_X_MAP + game->players[0]->map_x[0] * pSizeBlock + (pSizeBlock / 2) - (BOMB_PNG_W / 2);
    int cell_y = START_Y_MAP + game->players[0]->map_y[0] * pSizeBlock + (pSizeBlock / 2) - (BOMB_PNG_H / 2);

    game->pSDL->dst_bomb.h = BOMB_PNG_H;
    game->pSDL->dst_bomb.w = BOMB_PNG_W;
    game->pSDL->dst_bomb.x = cell_x;
    game->pSDL->dst_bomb.y = cell_y;
    SDL_Log("x: %d, y: %d", game->pSDL->dst_bomb.x, game->pSDL->dst_bomb.y);

    game->players[0]->bomb->x_pos = game->players[0]->map_x[0];
    game->players[0]->bomb->y_pos = game->players[0]->map_y[0];
    game->players[0]->bombPosed = 1;
    game->players[0]->bomb->tickBombDropped = SDL_GetTicks();

}

void destroyBlock(map_t map, bomb_t *b)
{
    const int pos_x = b->x_pos;
    const int pos_y = b->y_pos;

    SDL_Log("bomb: pos_x : %d, pos_y : %d", pos_x, pos_y);

    // Left
    if (pos_x - 1 >= 0) {
        if (getBit(map[pos_y], pos_x - 1, 1) == 1 && getBit(map[pos_y], pos_x - 1, 2) == 1) {
            toggleBit(map[pos_y], pos_x - 1, 1);
            toggleBit(map[pos_y], pos_x - 1, 2);
        }
    }
    // Right
    if (pos_x + 1 <= 12) {
        if (getBit(map[pos_y], pos_x + 1, 1) == 1 && getBit(map[pos_y], pos_x + 1, 2) == 1) {
            toggleBit(map[pos_y], pos_x + 1, 1);
            toggleBit(map[pos_y], pos_x + 1, 2);
        }
    }
    // Up
    if (pos_y - 1 >= 0) {
        if (getBit(map[pos_y - 1], pos_x, 1) == 1 && getBit(map[pos_y - 1], pos_x, 2) == 1) {
            toggleBit(map[pos_y - 1], pos_x, 1);
            toggleBit(map[pos_y - 1], pos_x, 2);
        }
    }
    // Down
    if (pos_y + 1 <= 8) {
        if (getBit(map[pos_y + 1], pos_x, 1) == 1 && getBit(map[pos_y + 1], pos_x, 2) == 1) {
            toggleBit(map[pos_y + 1], pos_x, 1);
            toggleBit(map[pos_y + 1], pos_x, 2);
        }
    }
}

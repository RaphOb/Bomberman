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
                case SDLK_ESCAPE :
                    c_emission(DISCONNECT_CODE);
                    res = -1;
                    break;
                case SDLK_b:
                    c_emission(BOMB_CODE);
                    if (game->players[0]->bombPosed == 0 && game->players[0]->bomb->explosion == 0 && canPlayerPlaceBomb(game->players[0]))
                        placeBomb(game->pSDL, game->players[0]);
                    break;
                default :
                    fprintf(stderr,"touche inconnue %d\n", event.key.keysym.sym);
                    break;
            }
        }
    }
    if (game->players[0]->bomb->explosion == 1) {
        checkBombDamage(game->map, game->players[0]->bomb);
    }
    doMove(keystates, game->players[0], game->map);

    return res;
}

void makeExplosion(sdl_t *pSDL, player_t *player)
{
    SDL_Log("x: %d, y: %d", pSDL->dst_bomb.x, pSDL->dst_bomb.y);
    pSDL->dst_explosion2.h = 1;
    pSDL->dst_explosion2.w = 1;
    player->bomb->explosion = 1;
    player->bomb->tickExplosion = SDL_GetTicks();
    playSound(EXPLOSION_SOUND);
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

void placeBomb(sdl_t *pSDL, player_t *player)
{
    const int pSizeBlock = BLOCK_SIZE * SIZE_M;
    int cell_x = START_X_MAP + (player->bomb->x_pos * pSizeBlock) + (pSizeBlock / 2) - (BOMB_PNG_W / 2);
    int cell_y = START_Y_MAP + (player->bomb->y_pos * pSizeBlock) + (pSizeBlock / 2) - (BOMB_PNG_H / 2);

    pSDL->dst_bomb.h = BOMB_PNG_H;
    pSDL->dst_bomb.w = BOMB_PNG_W;
    pSDL->dst_bomb.x = cell_x;
    pSDL->dst_bomb.y = cell_y;
//    SDL_Log("x: %d, y: %d", game->pSDL->dst_bomb.x, game->pSDL->dst_bomb.y);

//    player->bomb->x_pos = player->map_x[0];
//    player->bomb->y_pos = player->map_y[0];
    player->bombPosed = 1;
    player->bomb->tickBombDropped = SDL_GetTicks();

}

void checkBombDamage(map_t map, bomb_t *b)
{
    const int pos_x = b->x_pos;
    const int pos_y = b->y_pos;

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

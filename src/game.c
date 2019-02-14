//
// Created by geoff on 16/01/2019.
//

#include <SDL2/SDL_events.h>
#include <stdio.h>
#include "../header/game.h"
#include "../header/map.h"
#include "../header/map.h"
#include "../header/move.h"

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
                    res = -1;
                    break;
                case SDLK_b:
                    if (game->players[0]->bomb == 0 && game->players[0]->explosion == 0)
                        placeBomb(game);
                    break;
                default :
                    fprintf(stderr,"touche inconnue %d\n", event.key.keysym.sym);
                    break;
            }
        }
    }
    doMove(keystates, game->players[0]);

    return res;
}

void makeExplosion(game_t *game)
{
    SDL_Log("x: %d, y: %d", game->pSDL->dst_bomb.x, game->pSDL->dst_bomb.y);
    game->pSDL->dst_explosion2.h = 1;
    game->pSDL->dst_explosion2.w = 1;
//    game->pSDL->dst_explosion2.x = game->pSDL->dst_bomb.x + ((game->pSDL->dst_bomb.w - game->pSDL->dst_explosion2.w) / 2);
//    game->pSDL->dst_explosion2.y = game->pSDL->dst_bomb.y + ((game->pSDL->dst_bomb.h - game->pSDL->dst_explosion2.h) / 2);
//    SDL_Log("explo : x: %d, y: %d", game->pSDL->dst_explosion2.x, game->pSDL->dst_explosion2.y);

//    game->pSDL->dst_explosion.x = game->pSDL->dst_explosion2.x + 40;
//    game->pSDL->dst_explosion.y = game->pSDL->dst_explosion2.y + 30;
    game->pSDL->dst_explosion.h = 5;
//    game->pSDL->dst_explosion.h = BLOCK_SIZE * SIZE_M * 2;
    game->pSDL->dst_explosion.w = 5;
//    game->pSDL->dst_explosion.w = BLOCK_SIZE * SIZE_M * 2;

    game->players[0]->explosion = 1;
    game->players[0]->tickExplosion = SDL_GetTicks();
}

void placeBomb(game_t *game)
{
    SDL_Log("placeBomb");
    game->pSDL->dst_bomb.h = BOMB_PNG_H;
    game->pSDL->dst_bomb.w = BOMB_PNG_W;
    game->pSDL->dst_bomb.x = game->players[0]->x_pos + 10;
//   game->pSDL->dst_bomb.x = START_X_MAP * 2 + 25; // TODO remplacer le 2 par l'index de la case ou est le joueur + 1;
    game->pSDL->dst_bomb.y = game->players[0]->y_pos + 10;
    SDL_Log("x: %d, y: %d", game->pSDL->dst_bomb.x, game->pSDL->dst_bomb.y);
    game->players[0]->bomb = 1;
//   game->pSDL->dst_bomb.y = START_Y_MAP + 25;
    game->players[0]->tickBombDropped = SDL_GetTicks();

}

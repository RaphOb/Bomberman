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
#include "../header/bomb.h"

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
                    if (p->alive == 'Y') {
                        c_emission(p, BOMB_CODE);
                        int index = getIndexBomb(p);
                        if (p->bombPosed <= p->nbBombe && canPlayerPlaceBomb(p, &p->bomb[index])) { // TODO remplacer le 1 par p->nbBombe quand ce sera update par le serveur
//                        SDL_Log("bomb pos_x: %d, pos_y: %d", p->bomb[p->bombPosed].cell_x, p->bomb[p->bombPosed].cell_y);
                            toggleBit(game->map[p->bomb[index].cell_x], p->bomb[index].cell_y, 3);
                            placeBomb(game->pSDL, p, &p->bomb[index]);
                        }
                    }
                    break;
                default :
//                    SDL_Log("touche inconnue %d\n", event.key.keysym.sym);
                    break;
            }
        }
    }
//    SDL_Log("bombposed: %d", p->bombPosed);
    for (int i = 0; i < MAX_BOMBE; i++) {
        checkExplosion(game, p->bomb[i]);
    }
    if (p->alive == 'Y') {
        doMove(keystates, p, game->map);
    }
    return res;
}

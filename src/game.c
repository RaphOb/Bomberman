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
    game->start = 0;
    if (extractArrayFromFile(game->map) == 0) {
        return (NULL);
    }
    game->pSDL = pSDL;
    for (int i = 0; i < MAX_PLAYER ; i++) {
        game->players[i] = initPlayer();
        game->players[i].number = -1;
    }
    return game;
}


player_t *getMyPlayer(game_t *g)
{
    return &g->players[g->nb_client_serv];
}

int getNbPlayer(player_t players[MAX_PLAYER])
{
    int nb = 0;

    for (int i = 0; i < MAX_PLAYER; i++) {
        if (players[i].number != -1)
            nb++;
    }
    return nb;
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
                    }
                    break;
                default :
//                    SDL_Log("touche inconnue %d\n", event.key.keysym.sym);
                    break;
            }
        }
    }
//    SDL_Log("bombposed: %d", p->bombPosed);
    if (p->alive == 'Y') {
        if (keystates[SDL_SCANCODE_UP]) {
            p->y_pos -= p->speed;
            c_emission(p, UP_CODE);
        }
        else if (keystates[SDL_SCANCODE_DOWN]) {
            p->y_pos += p->speed;
            c_emission(p, DOWN_CODE);
        }
        else if (keystates[SDL_SCANCODE_LEFT]) {
            p->x_pos -= p->speed;
            c_emission(p, LEFT_CODE);
        }
        else if (keystates[SDL_SCANCODE_RIGHT]) {
            p->x_pos += p->speed;
            c_emission(p, RIGHT_CODE);
        } else {
            c_emission(p, 0);
        }
//        doMove(keystates, p, game->map);
    }
    return res;
}
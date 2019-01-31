//
// Created by geoff on 30/01/2019.
//

#include <stdlib.h>
#include <SDL_log.h>
#include "../header/player.h"
#include "../header/map.h"

player_t *initPlayer()
{
    player_t *p = malloc(sizeof(player_t));
    if(!p) {
      SDL_Log("Erreur malloc player");
      return NULL;
    }
    p->alive = 'Y';
    p->bombs_left = 20;
    p->frags = 0;
    p->x_pos = START_X_MAP + (BLOCK_SIZE * SIZE_M);
    p->y_pos = START_Y_MAP + (BLOCK_SIZE / 2 * SIZE_M);
    p->number = 1;

    return p;
}
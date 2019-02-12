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
    p->bomb = 0;
    p->bombs_left = 20;
    p->frags = 0;
    p->x_pos = START_X_MAP;
    p->y_pos = START_Y_MAP;
    p->number = 1;
    p->current_frame = 1;
    p->frame_time = 0;
    p->direction = 2;
    p->still = 1;

    return p;
}
//
// Created by oraph on 18/02/2019.
//

#ifndef BOMBERMAN_MENU_H
#define BOMBERMAN_MENU_H
#include <pthread.h>
#include "../header/SDLEngine.h"

#define IMG_LOGO_W 640
#define IMG_LOGO_H 247
#define IMG_MENU_W 601
#define IMG_MENU_H 478

typedef struct hover_s {

    int hover_off;
    int hover_on;
} hover_t;
int menuEvent(sdl_t *pSDL);
int menuNetworkEvent(sdl_t *pSDL);

#endif //BOMBERMAN_MENU_H

//
// Created by oraph on 18/02/2019.
//

#ifndef BOMBERMAN_MENU_H
#define BOMBERMAN_MENU_H

#include <pthread.h>
#include "SDLEngine.h"
#include "sound.h"

#define IMG_LOGO_W 640
#define IMG_LOGO_H 247
#define IMG_MENU_W 601
#define IMG_MENU_H 478

int menuEvent(sdl_t *pSDL, son_t* son);
int menuNetworkEvent(sdl_t *pSDL, son_t* son);
int menuLobbyEvent(sdl_t *pSDL, son_t* son, int host, int nbPlayer);
int menuGameOverEvent(sdl_t *pSDL);
void makeSoundHover(button_t *button, son_t *son);

#endif //BOMBERMAN_MENU_H

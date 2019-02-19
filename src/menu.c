//
// Created by oraph on 18/02/2019.
//

#include <SDL_quit.h>
#include <SDL_events.h>
#include <stdio.h>
#include "../header/menu.h"


int menuEvent()
{
    int res = 0;
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            res = -1;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_p:
                    res = 1;
                    break;
                case SDLK_q:
                case SDLK_ESCAPE :
                    res = -1;
                    break;
                default :
                    fprintf(stderr,"touche inconnue %d\n", event.key.keysym.sym);
                    break;
            }
        }
    }

    return res;
}

int menuNetworkEvent()
{
    int res = 0;
    SDL_Event event;

    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT)
        res = -1;
    else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_s:
                res = 1;
                break;
            case SDLK_h:
                res = 2;
                break;
            case SDLK_q:
            case SDLK_ESCAPE:
                res = -1;
                break;
            default:break;
        }
    }
    return res;
}

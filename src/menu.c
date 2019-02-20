//
// Created by oraph on 18/02/2019.
//

#include <SDL_quit.h>
#include <SDL_events.h>
#include <stdio.h>
#include <SDL_log.h>
#include "../header/menu.h"
#include "../header/map.h"
#include "../header/SDLEngine.h"


int menuEvent(sdl_t *pSDL)
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
                    fprintf(stderr, "touche inconnue %d\n", event.key.keysym.sym);
                    break;
            }
        }else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.x >((MAP_SIZE_W / 2) - (696 / 4)) && event.button.y > 520 && event.button.y < 520 + 564/2 && event.button.x < ((MAP_SIZE_W / 2) - (696 / 4)) + 696/2) {
                    res = -1;
                }
                if (event.button.x > (MAP_SIZE_W / 2) - (696 / 4) && event.button.x < (MAP_SIZE_W / 2) - (696 / 4) + 696/2 && event.button.y > 280 && event.button.y < 280 + 564/2) {
                    res = 1;
                }
            }
//        } else if (event.type == SDL_MOUSEMOTION) {
//            const SDL_Rect mouse = {event.button.x, event.button.y, 1, 1};
//            const SDL_Rect imagePlay = {(MAP_SIZE_W / 2) - (696 / 4), 280, 696/2, 564/2};
//            if (SDL_HasIntersection(&mouse, &imagePlay)) {
//                SDL_Log("hover play");
//                pSDL->mouseOverPlay = 1;
//                //change image
//            } else {
//                pSDL->mouseOverPlay = 0;
//            }
    //    }
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
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        int size = (MAP_SIZE_W / 2) - (200 / 2);
        if ( event.button.x > size &&  event.button.x < size + 200 &&  event.button.y > 300 &&  event.button.y < 300 + 150) {
            res = 2;
        }
        if ( event.button.x > size &&  event.button.x < size + 200 &&  event.button.y > 600 &&  event.button.y < 600 + 150) {
            res = -1;
        }
        if ( event.button.x > size &&  event.button.x < size + 200 &&  event.button.y > 450 &&  event.button.y < 450 + 150) {
            res = 1;
        }
    }
    return res;
}

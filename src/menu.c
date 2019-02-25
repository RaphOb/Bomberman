//
// Created by oraph on 18/02/2019.
//

#include <SDL2/SDL_quit.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <stdio.h>
#include "../header/menu.h"
#include "../header/map.h"
#include "../header/SDLEngine.h"


int menuEvent(sdl_t *pSDL)
{
    int res = 0;
    SDL_Event event;
    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    pSDL->buttonPlay->hover = 0;
    pSDL->buttonQuit->hover = 0;
    const SDL_Rect mouse = {mouse_x, mouse_y, 1, 1};
    if (SDL_HasIntersection(&mouse, &pSDL->buttonPlay->dstRect)) {
        pSDL->buttonPlay->hover = 1;
    } else if (SDL_HasIntersection(&mouse, &pSDL->buttonQuit->dstRect)) {
        pSDL->buttonQuit->hover = 1;
    }

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
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.x > (MAP_SIZE_W / 2) - (IMG_MENU_W / 4) &&
                event.button.x < (MAP_SIZE_W / 2) - (IMG_MENU_W / 4) + IMG_MENU_W / 2 && event.button.y > 520 &&
                event.button.y < 520 + IMG_MENU_H / 2) {
                res = -1;
            }
            if (event.button.x > (MAP_SIZE_W / 2) - (IMG_MENU_W / 4) &&
                event.button.x < (MAP_SIZE_W / 2) - (IMG_MENU_W / 4) + IMG_MENU_W / 2 && event.button.y > 280 &&
                event.button.y < 280 + IMG_MENU_H / 2) {
                res = 1;
            }
        } else if (event.type == SDL_MOUSEMOTION) {

        }
    }
    return res;
}

int menuNetworkEvent(sdl_t *pSDL)
{
    int res = 0;
    SDL_Event event;

    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    pSDL->buttonConnect->hover = 0;
    pSDL->buttonHost->hover = 0;
    pSDL->buttonQuit->hover = 0;
    const SDL_Rect mouse = {mouse_x, mouse_y, 1, 1};
    if (SDL_HasIntersection(&mouse, &pSDL->buttonConnect->dstRect)) {
        pSDL->buttonConnect->hover = 1;
    } else if (SDL_HasIntersection(&mouse, &pSDL->buttonQuit->dstRect)) {
        pSDL->buttonQuit->hover = 1;
    } else if (SDL_HasIntersection(&mouse, &pSDL->buttonHost->dstRect)) {
        pSDL->buttonHost->hover = 1;
    }

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
        const int size = (MAP_SIZE_W / 2) - (200 / 2);
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

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

int hover_on = 0;
int hover_off = 1;

/**
 * Function : Manage the events from the player in the first menu
 * @param pSDL
 * @param son
 * @return 1 if the player wants to play, 0 if he wants ot quit
 */
int menuEvent(sdl_t *pSDL, son_t* son)
{
    SDL_Event event;
    int res = 0;
    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    const SDL_Rect mouse = {mouse_x, mouse_y, 1, 1};

    pSDL->buttonPlay->hover = 0;
    pSDL->buttonQuit->hover = 0;
    if (SDL_HasIntersection(&mouse, &pSDL->buttonPlay->dstRect)) {
        makeSoundHover(pSDL->buttonPlay, son);
    } else if (SDL_HasIntersection(&mouse, &pSDL->buttonQuit->dstRect)) {
        makeSoundHover(pSDL->buttonQuit, son);
    } else
        hover_off = 1;


    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            res = -1;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_p:
                    res = 1;
                    pSDL->network = 0;
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
                pSDL->network = 0;
            }
        } else if (event.type == SDL_MOUSEMOTION) {

        }
    }
    return res;
}

int menuGameOverEvent(sdl_t *pSDL)
{
    SDL_Event event;
    int res = 0;
    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    const SDL_Rect mouse = {mouse_x, mouse_y, 1, 1};
    SDL_Rect dst_menuQuitter = {550, 650, IMG_MENU_W / 3, IMG_MENU_H / 3};

    pSDL->buttonQuit->hover = 0;
    if (SDL_HasIntersection(&mouse, &dst_menuQuitter)) {
        pSDL->buttonQuit->hover = 1;
        hover_on = 1;
        if (hover_on == 1 && hover_off== 1) {
            hover_off = 0;
        }
    } else
        hover_off = 1;

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            res = -1;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_q:
                case SDLK_ESCAPE :
                    res = -1;
                    break;
                default :
                    fprintf(stderr, "touche inconnue %d\n", event.key.keysym.sym);
                    break;
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.x > 550 && event.button.x < 550 + (IMG_MENU_W / 3) &&
                event.button.y > 650 && event.button.y < 650 + (IMG_MENU_H / 3)) {
                res = -1;
            }
        }
    }
    return res;
}


/**
 * Function : Manage the events from the player in the network menu
 * @param pSDL
 * @param son
 * @return 1 if the player wants to host the game, 2 if he wants to connect to an host and - 1 if he wants to quit
 */
int menuNetworkEvent(sdl_t *pSDL, son_t* son)
{
    SDL_Event event;
    int res = 0;
    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    const SDL_Rect mouse = {mouse_x, mouse_y, 1, 1};

    pSDL->buttonConnect->hover = 0;
    pSDL->buttonHost->hover = 0;
    pSDL->buttonQuit->hover = 0;
    if (SDL_HasIntersection(&mouse, &pSDL->buttonConnect->dstRect)) {
        makeSoundHover(pSDL->buttonConnect, son);
    } else if (SDL_HasIntersection(&mouse, &pSDL->buttonQuit->dstRect)) {
        makeSoundHover(pSDL->buttonQuit, son);
    } else if (SDL_HasIntersection(&mouse, &pSDL->buttonHost->dstRect)) {
        makeSoundHover(pSDL->buttonHost, son);
    } else
        hover_off = 1;

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
            default:
                break;
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        const int size = (MAP_SIZE_W / 2) - (200 / 2);
        if (event.button.x > size &&  event.button.x < size + 200 &&  event.button.y > 300 &&  event.button.y < 300 + 150) {
            res = 2;
        }
        if (event.button.x > size &&  event.button.x < size + 200 &&  event.button.y > 600 &&  event.button.y < 600 + 150) {
            res = -1;
        }
        if (event.button.x > size &&  event.button.x < size + 200 &&  event.button.y > 450 &&  event.button.y < 450 + 150) {
            res = 1;
        }
        if (event.button.x > 20 &&  event.button.x < 20 + 250 &&  event.button.y > 650 &&  event.button.y < 550 + 350) {
            SDL_Log("retour menu");
            pSDL->menu = 0;
            res = 3;
        }
        //Todo :retour menu à faire
    }
    return res;
}

int menuLobbyEvent(sdl_t *pSDL, son_t* son, int host, int nbClient) {
    int res = 0;
    SDL_Event event;

    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    pSDL->buttonLaunch->hover = 0;
    pSDL->buttonQuit->hover = 0;
    const SDL_Rect mouse = {mouse_x, mouse_y, 1, 1};
    const SDL_Rect dst_menuQuitter = {(MAP_SIZE_W / 2) - (IMG_MENU_W / 6) + 400, 600, IMG_MENU_W / 3, IMG_MENU_H / 3};

    if (SDL_HasIntersection(&mouse, &pSDL->buttonLaunch->dstRect)) {
        makeSoundHover(pSDL->buttonLaunch, son);
    } else if (SDL_HasIntersection(&mouse, &dst_menuQuitter)) {
        makeSoundHover(pSDL->buttonQuit, son);
    } else
        hover_off = 1;

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            res = -1;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE :
                    res = -1;
                    break;
                default :
                    fprintf(stderr, "touche inconnue %d\n", event.key.keysym.sym);
                    break;
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            const int size = (MAP_SIZE_W / 2) + 300 ;
            if (event.button.x > size &&  event.button.x < size + 200 &&  event.button.y > 600 &&  event.button.y < 600 + 150) {
                res = -1;
            }
            if (host == 1 && nbClient >= 2) {
                if (event.button.x > size && event.button.x < size + 200 && event.button.y > 450 &&
                    event.button.y < 450 + 150) {
                    res = 1;
                }
            }
        }
    }
    return res;
}

void makeSoundHover(button_t *button, son_t *son)
{
    button->hover = 1;
    hover_on = 1;
    if (hover_on == 1 && hover_off == 1) {
        hover_off = 0;
        playSound(son);
    }
}
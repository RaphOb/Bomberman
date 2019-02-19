//
// Created by geoff on 16/01/2019.
//

#include <math.h>
#include "../header/SDLEngine.h"
#include "../header/map.h"
#include "../header/player.h"

/**
 * function : init SDL map and texture
 * @param game
 * @return
 */
sdl_t *initSDL()
{
    sdl_t *pSDL = malloc(sizeof(sdl_t));
    if (!pSDL) {
        return (NULL);
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        destroySDL(pSDL);
        return NULL;
    }

    if(TTF_Init() == -1) {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        destroySDL(pSDL);
        return NULL;
    }

    pSDL->pWindow = SDL_CreateWindow("Bomberman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAP_SIZE_W,
                                     MAP_SIZE_H, 0);
    if (pSDL->pWindow == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        destroySDL(pSDL);
        return NULL;
    }
    pSDL->pRenderer = SDL_CreateRenderer(pSDL->pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (pSDL->pRenderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        destroySDL(pSDL);
        return NULL;
    }
    
    initPlayerSDL(pSDL);
    initMap(pSDL);
    initBlock(pSDL);
    initBomb(pSDL);
    initExplosion(pSDL);
    initMenu(pSDL);
    return pSDL;
}

/**
 * function: destroy SDL
 * @param pSDL
 * @param game
 */
void destroySDL(sdl_t *pSDL)
{
    if (pSDL->textureMenuJouerOff) {
        SDL_DestroyTexture(pSDL->textureMenuJouerOff);
        pSDL->textureMenuJouerOff = NULL;
    }
    if (pSDL->textureSeconnecter) {
        SDL_DestroyTexture(pSDL->textureSeconnecter);
        pSDL->textureSeconnecter = NULL;
    }
    if (pSDL->textureHeberger) {
        SDL_DestroyTexture(pSDL->textureHeberger);
        pSDL->textureHeberger = NULL;
    }
    if (pSDL->textureMenuJouerOn) {
        SDL_DestroyTexture(pSDL->textureMenuJouerOn);
        pSDL->textureMenuJouerOn = NULL;
    }
    if (pSDL->textureMenuQuitOff) {
        SDL_DestroyTexture(pSDL->textureMenuQuitOff);
        pSDL->textureMenuQuitOff = NULL;
    }
    if (pSDL->textureMenuQuitOn) {
        SDL_DestroyTexture(pSDL->textureMenuQuitOn);
        pSDL->textureMenuQuitOn = NULL;
    }
    if (pSDL->textureMenuLogo) {
        SDL_DestroyTexture(pSDL->textureMenuLogo);
        pSDL->textureMenuLogo = NULL;
    }
    if (pSDL->textureExplosion) {
        SDL_DestroyTexture(pSDL->textureExplosion);
        pSDL->textureExplosion = NULL;
    }
    if (pSDL->textureExplosion2) {
        SDL_DestroyTexture(pSDL->textureExplosion2);
        pSDL->textureExplosion2 = NULL;
    }

    if (pSDL->textureBomb) {
        SDL_DestroyTexture(pSDL->textureBomb);
        pSDL->textureBomb = NULL;
    }
    if (pSDL->textureBlock) {
        SDL_DestroyTexture(pSDL->textureBlock);
        pSDL->textureBlock = NULL;
    }
    if (pSDL->textureMap) {
        SDL_DestroyTexture(pSDL->textureMap);
        pSDL->textureMap = NULL;
    }
    if (pSDL->texturePlayer) {
        SDL_DestroyTexture(pSDL->texturePlayer);
        pSDL->texturePlayer = NULL;
    }
    if (pSDL->pRenderer) {
        SDL_DestroyRenderer(pSDL->pRenderer);
        pSDL->pRenderer = NULL;
    }
    if (pSDL->pWindow) {
        SDL_DestroyWindow(pSDL->pWindow);
        pSDL-> pWindow = NULL;
    }
    TTF_Quit();
    SDL_Quit();
    free(pSDL);
    SDL_Log("Destroy SDL");
}

/** TODO
 * function : comment plz
 * @param sdl_renderer
 */
void clear(SDL_Renderer *sdl_renderer) {
    SDL_RenderClear(sdl_renderer);
    SDL_RenderPresent(sdl_renderer);
}

/**
 * function : Init textures menu
 * @param pSDL
 */
void initMenu(sdl_t *pSDL)
{
    SDL_Surface *menuSelectionJouerOff =IMG_Load("../resources/Menu_Selection_jouerOff.png");
    SDL_Surface *menuSelectionJouerOn =IMG_Load("../resources/Menu_Selection_jouerOn.png");
    SDL_Surface *menuSelectionQuitOff =IMG_Load("../resources/Menu_Selection_quitOff.png");
    SDL_Surface *menuSelectionQuitOn =IMG_Load("../resources/Menu_Selection_quitOn.png");
    SDL_Surface *menuHeberger =IMG_Load("../resources/Menu_hebergerOff.png");
    SDL_Surface *menuSeconnecter =IMG_Load("../resources/Menu_seconnecterOff.png");
    SDL_Surface *menuLogo =IMG_Load("../resources/B_Logo.png");

    if (!(menuSelectionJouerOff || menuSelectionJouerOn || menuSeconnecter
    || menuSelectionQuitOff || menuSelectionQuitOn || menuLogo || menuHeberger)) {
        fprintf(stderr, "impossible d'initialiser l'image :%s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureMenuJouerOff = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuSelectionJouerOff);
        pSDL->textureMenuJouerOn = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuSelectionJouerOn);
        pSDL->textureMenuQuitOff = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuSelectionQuitOff);
        pSDL->textureMenuQuitOn = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuSelectionJouerOn);
        pSDL->textureMenuLogo = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuLogo);
        pSDL->textureSeconnecter = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuSeconnecter);
        pSDL->textureHeberger = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuHeberger);
        if (!(pSDL->textureMenuJouerOff || pSDL->textureMenuJouerOn || pSDL->textureSeconnecter ||
        pSDL->textureMenuQuitOff || pSDL-> textureMenuQuitOn ||pSDL->textureMenuLogo || pSDL->textureSeconnecter)) {
            fprintf(stderr, "impossible d'initialiser la texture :%s\n", IMG_GetError());
            return;
        }
    }
    SDL_FreeSurface(menuSelectionJouerOff);
    SDL_FreeSurface(menuSelectionJouerOn);
    SDL_FreeSurface(menuSelectionQuitOff);
    SDL_FreeSurface(menuSelectionQuitOn);
    SDL_FreeSurface(menuHeberger);
    SDL_FreeSurface(menuSeconnecter);
    SDL_FreeSurface(menuLogo);

}

/**
 * function : init texture Perso
 * @param pSDL
 * @param game
 */
void initPlayerSDL(sdl_t *pSDL)
{
    SDL_Surface *surfaceTrump = IMG_Load("../resources/player3.png");
    if (!surfaceTrump) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->texturePlayer = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfaceTrump);
        if (!pSDL->texturePlayer) {
            fprintf(stderr, "impossible d'intialiser la texture : %s", IMG_GetError());
            destroySDL(pSDL);
            return;
        }

        SDL_Rect d = {START_X_MAP, START_Y_MAP, FRAME_WIDTH, FRAME_HEIGHT};
        pSDL->dst_player = d;
    }
    SDL_FreeSurface(surfaceTrump);
    surfaceTrump = NULL;
}

/**
 * function : init texture bomb
 * @param pSDL
 * @param game
 */
void initBomb(sdl_t *pSDL)
{
    SDL_Surface* surfaceBomb = IMG_Load("../resources/bomb-sprite-png-5.png");
    if (!surfaceBomb) {
        SDL_Log("impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureBomb = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfaceBomb);
        if (!pSDL->textureBomb) {
            SDL_Log("impossible d'initialiser la texture : %s", IMG_GetError());
            destroySDL(pSDL);
            return;
        }
        SDL_Log("Bomb initialised");
    }
    SDL_FreeSurface(surfaceBomb);
    surfaceBomb = NULL;
}

void initExplosion(sdl_t *pSDL)
{
    SDL_Surface *explosion = IMG_Load("../resources/explosion.png");
    SDL_Surface *explosion2 = IMG_Load("../resources/Explosion2.png");
    if (!explosion2 || !explosion) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureExplosion = SDL_CreateTextureFromSurface(pSDL->pRenderer, explosion);
        pSDL->textureExplosion2 = SDL_CreateTextureFromSurface(pSDL->pRenderer, explosion2);
        if (!pSDL->textureExplosion || !pSDL->textureExplosion2) {
            fprintf(stderr, "impossible d'initialiser la texture : %s\n", SDL_GetError());
            destroySDL(pSDL);
            return;
        }
        SDL_Log("Explosion initialised");
    }
    SDL_FreeSurface(explosion2);
    SDL_FreeSurface(explosion);
}

void initBlock(sdl_t *pSDL)
{
    SDL_Surface *block = IMG_Load("../resources/block_map1.png");
    if (!block) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureBlock = SDL_CreateTextureFromSurface(pSDL->pRenderer, block);
        if (!pSDL->textureBlock) {
            fprintf(stderr, "impossible d'initialiser la texture : %s\n", SDL_GetError());
            destroySDL(pSDL);
            return;
        }
    }
    SDL_FreeSurface(block);
    block = NULL;
}

void initMap(sdl_t *pSDL)
{
    SDL_Surface *map = IMG_Load("../resources/maps.png"); // 722 * 482 ; Taille d'une map: 240 * 160
    if (!map) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureMap = SDL_CreateTextureFromSurface(pSDL->pRenderer, map);
        if (!pSDL->textureMap) {
            fprintf(stderr, "impossible d'initialiser la texture : %s\n", SDL_GetError());
            destroySDL(pSDL);
            return;
        }
    }
    SDL_FreeSurface(map);
    map = NULL;
}


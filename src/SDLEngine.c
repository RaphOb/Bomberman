//
// Created by geoff on 16/01/2019.
//

#include <math.h>
#include "../header/SDLEngine.h"
#include "../header/map.h"
#include "../header/player.h"
#include "../header/bit.h"

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
    return pSDL;
}

/** TODO
 * function : comment plz
 * @param pSDL
 * @param x
 * @param y
 */
void renderBlock(sdl_t *pSDL, int x, int y)
{
    SDL_Rect src_block = {0, 0, BLOCK_SIZE, BLOCK_SIZE};
    SDL_Rect dst_block = {START_X_MAP + (BLOCK_SIZE * SIZE_M) + (16 * x * SIZE_M),
                          START_Y_MAP + ((BLOCK_SIZE / 2) * SIZE_M) + (BLOCK_SIZE * y * SIZE_M), BLOCK_SIZE * SIZE_M, BLOCK_SIZE * SIZE_M};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBlock, &src_block, &dst_block);
}

/**
 * function: destroy SDL
 * @param pSDL
 * @param game
 */
void destroySDL(sdl_t *pSDL)
{
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

/** TODO
 * function: comment plz
 * @param game
 * @param pSdl
 */
void renderMap(map_t map, sdl_t *pSdl)
{
    for (int i = 0; i < MAP_X; i++) {
        for (int j = 0; j < MAP_Y; j++) {
            if (getBit(map[i], j, 1) == 1) {
                if (getBit(map[i], j, 2) == 1) {
                    renderBlock(pSdl, j, i);
                }
            }
        }
    }
}

/**
 * function : init texture Perso
 * @param pSDL
 * @param game
 */
void initPlayerSDL(sdl_t *pSDL)
{
    SDL_Surface *surfaceTrump = IMG_Load("../resources/player4.png");
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

        SDL_Rect d = {START_X_MAP + (16 * SIZE_M), START_Y_MAP + (8 * SIZE_M), 30, 70};
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
//        SDL_Log("w: %d, h: %d", surfaceBomb->w, surfaceBomb->h );
        pSDL->textureBomb = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfaceBomb);
        if (!pSDL->textureBomb) {
            SDL_Log("impossible d'initialiser la texture : %s", IMG_GetError());
            destroySDL(pSDL);
            return;
        }
        SDL_Log("Bomb initialised");
        SDL_Rect d = {-30, -30, 30, 32};
        pSDL->dst_bomb = d;
    }
    SDL_FreeSurface(surfaceBomb);
    surfaceBomb = NULL;
}

void initExplosion(sdl_t *pSDL)
{
    SDL_Surface *explosion = IMG_Load("../resources/explosion.png");
    SDL_Surface *explosion2 = IMG_Load("../resources/Explosion2.png");
    if (!(explosion && explosion2) ) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureExplosion = SDL_CreateTextureFromSurface(pSDL->pRenderer, explosion);
        pSDL->textureExplosion2 = SDL_CreateTextureFromSurface(pSDL->pRenderer, explosion2);
        if (!(pSDL->textureExplosion && pSDL->textureExplosion2)) {
            fprintf(stderr, "impossible d'initialiser la texture : %s\n", SDL_GetError());
            destroySDL(pSDL);
            return;
        }
        SDL_Rect e2 = {-40,-40,100,100};
        SDL_Rect e = {-40, -40, 30, 32};
        pSDL->dst_explosion = e;
        pSDL->dst_explosion2 = e2;
        SDL_Log("Explosion initialised");
    }
    SDL_FreeSurface(explosion);
    SDL_FreeSurface(explosion2);
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


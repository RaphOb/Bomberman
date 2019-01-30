//
// Created by geoff on 16/01/2019.
//

#include <math.h>
#include "../header/SDLEngine.h"
#include "../header/game.h"

/**
 * function : init SDL map and texture
 * @param game
 * @return
 */
sdl_t *initSDL(game_t *game)
{
    sdl_t *pSDL = malloc(sizeof(sdl_t));
    if (!pSDL) {
        return (NULL);
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        destroySDL(pSDL, game);
        return NULL;
    }
    pSDL->pWindow = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAP_SIZE_W,
                                     MAP_SIZE_H, SDL_WINDOW_RESIZABLE);
    if (pSDL->pWindow == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        destroySDL(pSDL, game);
        return NULL;
    }
    pSDL->pRenderer = SDL_CreateRenderer(pSDL->pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (pSDL->pRenderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        destroySDL(pSDL, game);
        return NULL;
    }

    SDL_Rect r = {0, 0, 722 / 3, 482 / 3};
    pSDL->src_rect = r;
    SDL_Rect d = {START_X_MAP, START_Y_MAP, MAP_SIZE_W, MAP_SIZE_H};
    pSDL->dst_rect = d;
    SDL_Surface *map = IMG_Load("../resources/maps.png"); // 722 * 482 ; Taille d'une map: 240 * 160
    if (!map) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL, game);
        return NULL;
    }

    pSDL->textureMap = SDL_CreateTextureFromSurface(pSDL->pRenderer, map);
    if (!pSDL->textureMap) {
        fprintf(stderr, "impossible d'initialiser la texture : %s\n", SDL_GetError());
        destroySDL(pSDL, game);
        return NULL;
    }

    SDL_Surface *block = IMG_Load("../resources/block_map1.png");
    if (!block) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL, game);
        return NULL;
    }

    pSDL->textureBlock = SDL_CreateTextureFromSurface(pSDL->pRenderer, block);
    if (!pSDL->textureBlock) {
        fprintf(stderr, "impossible d'initialiser la texture : %s\n", SDL_GetError());
        destroySDL(pSDL, game);
        return NULL;
    }
    
    initTrump(pSDL, game);

    SDL_FreeSurface(map);
    SDL_FreeSurface(block);
    return pSDL;
}

/** TODO
 * function : comment plz
 * @param pSDL
 * @param x
 * @param y
 */
void displayBlock(sdl_t *pSDL, int x, int y)
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
void destroySDL(sdl_t *pSDL, game_t *game)
{
    if (game->textureBomb) {
        SDL_DestroyTexture(game->textureBomb);
    }
    if (pSDL->textureBlock) {
        SDL_DestroyTexture(pSDL->textureBlock);
    }
    if (pSDL->textureMap) {
        SDL_DestroyTexture(pSDL->textureMap);
    }
    if (game->textureTrump) {
        SDL_DestroyTexture(game->textureTrump);
    }
    if (pSDL->pRenderer) {
        SDL_DestroyRenderer(pSDL->pRenderer);
    }
    if (pSDL->pWindow) {
        SDL_DestroyWindow(pSDL->pWindow);
    }
    SDL_Quit();
    free(pSDL);
    free(game);
}

/** TODO
 * function : comment plz
 * @param sdl_renderer
 */
void clear(SDL_Renderer *sdl_renderer) {
    SDL_RenderClear(sdl_renderer);
}

/** TODO
 * function: comment plz
 * @param game
 * @param pSdl
 */
void displayMap(game_t *game, sdl_t *pSdl)
{
    for (int i = 0; i < MAP_X; i++) {
        for (int j = 0; j < MAP_Y; j++) {
            if (getBit(game->map[i], j, 1) == 1) {
                if (getBit(game->map[i], j, 2) == 1) {
                    displayBlock(pSdl, j, i);
                }
            }
        }
    }
}
/** TODO
 * function: whos knows ?!
 * @param c
 * @param indexArray
 * @param indexBit
 * @return
 */
int getBit(const char c[], int indexArray, int indexBit)
{
    return (1 & (c[indexArray] >> indexBit));
}

/**
 * function : init texture Perso
 * @param pSDL
 * @param game
 */
void initTrump(sdl_t *pSDL, game_t *game)
{
    SDL_Surface *surfaceTrump = IMG_Load("../resources/trump.png");
    if (!surfaceTrump) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL, game);
        return;
    } else {
        game->textureTrump = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfaceTrump);
        if (!game->textureTrump) {
            fprintf(stderr, "impossible d'intialiser la texture : %s", IMG_GetError());
            destroySDL(pSDL, game);
            return;
        }
        SDL_FreeSurface(surfaceTrump);
        SDL_Rect d = {START_X_MAP + (16 * SIZE_M), START_Y_MAP + ( 8 * SIZE_M), 30, 70};
        game->dst_trump = d;
    }
}

/**
 * function : init texture bomb
 * @param pSDL
 * @param game
 */
void initBomb(sdl_t *pSDL, game_t *game)
{
    SDL_Surface* surfaceBomb = IMG_Load("../resources/bomb.png");
    if (!surfaceBomb) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL, game);
        return;
    } else {
        game->textureBomb = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfaceBomb);
        if (!game->textureBomb) {
            fprintf(stderr, "impossible d'intialiser la texture : %s", IMG_GetError());
            destroySDL(pSDL, game);
            return;
        }
    }
    SDL_FreeSurface(surfaceBomb);
}

/**
 * function : affiche les textures
 * @param pSDL
 * @param game
 */
void draw_game(sdl_t *pSDL, game_t *game)
{
    SDL_SetRenderDrawColor(pSDL->pRenderer, 0, 0, 0, 255);
    //SDL_RenderClear(pSDL->pRenderer);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMap, &pSDL->src_rect, &pSDL->dst_rect);
    displayMap(game, pSDL);
    SDL_RenderCopy(pSDL->pRenderer, game->textureTrump, NULL, &game->dst_trump);
    SDL_RenderPresent(pSDL->pRenderer);
}
void draw_bomb (sdl_t *pSDL, game_t *game)
{
    SDL_SetRenderDrawColor(pSDL->pRenderer, 0, 0, 0, 255);
    SDL_RenderCopy(pSDL->pRenderer, game->textureBomb, NULL, &game->dst_bomb);
    SDL_RenderPresent(pSDL->pRenderer);
}

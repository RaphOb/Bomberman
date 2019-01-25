//
// Created by geoff on 16/01/2019.
//

#include <math.h>
#include "../header/SDLEngine.h"
#include "../header/game.h"

sdl_t *initSDL(game_t *game)
{
    sdl_t *pSDL = malloc(sizeof(sdl_t));

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return NULL;
    }
    pSDL->pWindow = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAP_SIZE_W, MAP_SIZE_H, SDL_WINDOW_RESIZABLE);
    if (pSDL->pWindow == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return NULL;
    }
    pSDL->pRenderer = SDL_CreateRenderer(pSDL->pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (pSDL->pRenderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Surface *map = IMG_Load("../resources/maps.png"); // 722 * 482 ; Taille d'une map: 240 * 160
    SDL_Rect src_rect = {0, 0, 722 / 3, 482 / 3};
    SDL_Log("map->w: %d, map->h: %d", map->w, map->h);
    SDL_Rect dst_rect = {START_X_MAP, START_Y_MAP, MAP_SIZE_W, MAP_SIZE_H};
    SDL_Texture *texture = SDL_CreateTextureFromSurface(pSDL->pRenderer, map);
    SDL_RenderCopy(pSDL->pRenderer, texture, &src_rect, &dst_rect);
    SDL_RenderPresent(pSDL->pRenderer);

    displayMap(game, pSDL);
    displayTrump(pSDL);

    return pSDL;
}

void displayBlock(sdl_t *pSDL,int x, int y)
{
    SDL_Surface *block = IMG_Load("../resources/block_map1.png");
    SDL_Rect src_block = {0, 0, block->w, block->h};
    SDL_Rect dst_block = {START_X_MAP + (block->w * SIZE_M) + (16 * x * SIZE_M), START_Y_MAP + ((block->h / 2) * SIZE_M) + (16 * y * SIZE_M), block->w * SIZE_M, block->h * SIZE_M};
    SDL_Texture *texture_block = SDL_CreateTextureFromSurface(pSDL->pRenderer, block);
    SDL_RenderCopy(pSDL->pRenderer, texture_block, &src_block, &dst_block);
    SDL_RenderPresent(pSDL->pRenderer);
}

void destroySDL(sdl_t *pSDL)
{
    // On détruit tout ce qui a été créé avec la SDL

    SDL_DestroyRenderer(pSDL->pRenderer);
    SDL_DestroyWindow(pSDL->pWindow);
    SDL_Quit();
    free(pSDL);
}



void clear(SDL_Renderer *sdl_renderer)
{
    SDL_RenderClear(sdl_renderer);
}

void displayMap(game_t *game, sdl_t *pSdl)
{
    for (int i = 0; i < MAP_X; i++) {
        for (int j = 0; j < MAP_Y; j++) {
            if (getBit(game->map[i], j, 1) == 1) {
                if (getBit(game->map[i], j, 2) == 1) {
                    SDL_Log("aaaa");
                    displayBlock(pSdl, j, i);
                }
            }
        }
    }
}

int getBit(const char c[], int indexArray, int indexBit)
{
    return (1 & (c[indexArray] >> indexBit));
}

void displayTrump(sdl_t *pSDL)
{
    SDL_Surface* surfaceTrump = IMG_Load("../resources/trump.png");
    if (!surfaceTrump) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
            //fonction destroy ou free
            return;
    } else {
        SDL_Texture* trumpTexture = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfaceTrump);
            if (!trumpTexture) {
                fprintf(stderr, "impossible d'intialiser la texture : %s", IMG_GetError());
                // destroy
                return;
            }
            SDL_Rect src_trump = {0,0, surfaceTrump->w, surfaceTrump->h};
            SDL_Rect dst_trump = {START_X_MAP + (16 * SIZE_M), START_Y_MAP + (8* SIZE_M), surfaceTrump->w/7, surfaceTrump->h/7};
            SDL_RenderCopy(pSDL->pRenderer, trumpTexture, &src_trump, &dst_trump);
            SDL_RenderPresent(pSDL->pRenderer);
        SDL_FreeSurface(surfaceTrump);
        }
}




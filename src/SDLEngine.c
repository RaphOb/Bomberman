//
// Created by geoff on 16/01/2019.
//

#include <math.h>
#include "../header/SDLEngine.h"
#include "../header/game.h"

sdl_t *initSDL(game_t *game)
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
    pSDL->pWindow = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAP_SIZE_W,
                                     MAP_SIZE_H, SDL_WINDOW_RESIZABLE);
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

    SDL_Rect r = {0, 0, 722 / 3, 482 / 3};
    pSDL->src_rect = r;
    SDL_Rect d = {START_X_MAP, START_Y_MAP, MAP_SIZE_W, MAP_SIZE_H};
    pSDL->dst_rect = d;
    // TODO verif comme dans initTrump
    SDL_Surface *map = IMG_Load("../resources/maps.png"); // 722 * 482 ; Taille d'une map: 240 * 160
    pSDL->textureMap = SDL_CreateTextureFromSurface(pSDL->pRenderer, map);
    SDL_Surface *block = IMG_Load("../resources/block_map1.png");
    pSDL->textureBlock = SDL_CreateTextureFromSurface(pSDL->pRenderer, block);
    initTrump(pSDL, game);

    SDL_FreeSurface(map);
    SDL_FreeSurface(block);
    return pSDL;
}

void displayBlock(sdl_t *pSDL, int x, int y)
{
    SDL_Rect src_block = {0, 0, BLOCK_SIZE, BLOCK_SIZE};
    SDL_Rect dst_block = {START_X_MAP + (BLOCK_SIZE * SIZE_M) + (16 * x * SIZE_M),
                          START_Y_MAP + ((BLOCK_SIZE / 2) * SIZE_M) + (BLOCK_SIZE * y * SIZE_M), BLOCK_SIZE * SIZE_M, BLOCK_SIZE * SIZE_M};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBlock, &src_block, &dst_block);
}

void destroySDL(sdl_t *pSDL)
{
    // On détruit tout ce qui a été créé avec la SDL
    SDL_DestroyRenderer(pSDL->pRenderer);
    SDL_DestroyWindow(pSDL->pWindow);
    SDL_Quit();
    free(pSDL);
}

void destroyGame(game_t *game)
{
    free(game);
}

void clear(SDL_Renderer *sdl_renderer) {
    SDL_RenderClear(sdl_renderer);
    SDL_RenderPresent(sdl_renderer);
}

void displayMap(game_t *game, sdl_t *pSdl, SDL_Surface *block)
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

/**
 *
 * @param c
 * @param indexArray
 * @param indexBit
 * @return
 */
int getBit(const char c[], int indexArray, int indexBit)
{
    return (1 & (c[indexArray] >> indexBit));
}


void initTrump(sdl_t *pSDL, game_t *game)
{
    SDL_Surface *surfaceTrump = IMG_Load("../resources/trump.png");
    if (!surfaceTrump) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        //fonction destroy ou free
        return;
    } else {
        game->textureTrump = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfaceTrump);
//        pSDL->textureTrump = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfaceTrump);
        if (!game->textureTrump) {
            fprintf(stderr, "impossible d'intialiser la texture : %s", IMG_GetError());
            // destroy
            return;
        }
        SDL_FreeSurface(surfaceTrump);
        SDL_Rect d = {START_X_MAP + (16 * SIZE_M), START_Y_MAP + ( 8 * SIZE_M), 30, 70};
        game->dst_trump = d;
    }
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




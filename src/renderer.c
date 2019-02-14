//
// Created by geoff on 14/02/2019.
//

#include "../header/renderer.h"
#include "../header/bit.h"


/**
 * function : affiche les textures
 * @param pSDL
 * @param game
 */
void drawGame(game_t *game)
{
    int currentTick = SDL_GetTicks();
    SDL_RenderClear(game->pSDL->pRenderer);
    SDL_SetRenderDrawColor(game->pSDL->pRenderer, 0, 0, 0, 255);
    renderBackground(game->pSDL);
    renderMap(game->map, game->pSDL);
    if (game->players[0]->bomb == 1) {
        renderBomb(game->pSDL, game);
    }
    if (game->players[0]->explosion == 1) {
        if (currentTick - game->players[0]->tickExplosion > 400) {
            game->players[0]->explosion = 0;
        }
        renderExplosion(game->pSDL);
    }
    renderPlayer(game->pSDL, game->players[0]);
    SDL_RenderPresent(game->pSDL->pRenderer);
}

void renderBomb(sdl_t *pSDL, game_t *game)
{
    static int n = 0;
    const int size_m = 2;
    int currentTick = SDL_GetTicks();
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBomb, NULL, &pSDL->dst_bomb);
    if (currentTick - game->players[0]->tickBombDropped > 1000 && n == 0) {
        pSDL->dst_bomb.x -= BOMB_PNG_W / size_m;
        pSDL->dst_bomb.y -= BOMB_PNG_W / size_m;
        pSDL->dst_bomb.h *= size_m;
        pSDL->dst_bomb.w *= size_m;
        n = 1;
    }
    if (currentTick - game->players[0]->tickBombDropped > 2000) {
        game->players[0]->bomb = 0;
        game->players[0]->tickBombDropped = 0;
        makeExplosion(game);
        n = 0;
    }
}
void renderExplosion(sdl_t *pSDL)
{

    if (pSDL->dst_explosion2.h < BLOCK_SIZE * SIZE_M * 3) {
        pSDL->dst_explosion2.h += 20;
        pSDL->dst_explosion2.w += 20;
    }

//    pSDL->dst_explosion.h += 3;
//    pSDL->dst_explosion.w += 3;
    pSDL->dst_explosion2.x = pSDL->dst_bomb.x + ((pSDL->dst_bomb.w - pSDL->dst_explosion2.w) / 2);
    pSDL->dst_explosion2.y = pSDL->dst_bomb.y + ((pSDL->dst_bomb.h - pSDL->dst_explosion2.h) / 2);

//    pSDL->dst_explosion.x = (int) (pSDL->dst_explosion2.x + pSDL->dst_explosion.w / 3);
//    pSDL->dst_explosion.y = (int) (pSDL->dst_explosion2.y + pSDL->dst_explosion.h / 3);

    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion2, NULL, &pSDL->dst_explosion2);
//    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion, NULL, &pSDL->dst_explosion);
}

void renderBackground(sdl_t *pSDL)
{
    SDL_Rect src_map = {0, 0, 722 / 3, 482 / 3};
    SDL_Rect dst_map = {START_X_BACKGROUND, START_Y_BACKGROUND, MAP_SIZE_W, MAP_SIZE_H};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMap, &src_map, &dst_map);
}

void renderPlayer(sdl_t *pSDL, player_t *player)
{
    if (player->current_frame > 2) {
        player->current_frame = 0;
    }

    SDL_Rect src = {FRAME_WIDTH * player->current_frame, (FRAME_HEIGHT) * player->direction, FRAME_WIDTH, FRAME_HEIGHT};
    SDL_Rect r = {player->x_pos, player->y_pos, PLAYER_WIDTH, PLAYER_HEIGHT};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->texturePlayer, &src, &r);

    if (player->still == 0) {
        player->frame_time++;
        if (FPS / player->frame_time == 4) {
            player->current_frame++;
            player->frame_time = 0;
        }
    }
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
    SDL_Rect dst_block = {START_X_MAP + (16 * x * SIZE_M),
                          START_Y_MAP + (BLOCK_SIZE * y * SIZE_M),
                          BLOCK_SIZE * SIZE_M,
                          BLOCK_SIZE * SIZE_M};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBlock, &src_block, &dst_block);
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

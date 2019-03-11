//
// Created by geoff on 14/02/2019.
//

#include "../header/renderer.h"
#include "../header/bit.h"
#include "../header/menu.h"


/**
 * function : affiche les textures
 * @param game
 */
void drawGame(game_t *game)
{
    SDL_RenderClear(game->pSDL->pRenderer);
    SDL_SetRenderDrawColor(game->pSDL->pRenderer, 0, 0, 0, 255);
    renderBackground(game->pSDL);
    renderMap(game->map, game->pSDL);
    for (int i = 0; i < MAX_PLAYER ; i++) {
        if (game->players[i].number >= 0) {
            //SDL_Log("player : %d\n", game->players[i].number);
            if (game->players[i].bombPosed == 1) {
                renderBomb(game->pSDL, &game->players[i]);
            }
            if (game->players[i].bomb.explosion == 1) {
                int frame = 0;
                int currentTick = SDL_GetTicks();
                if (currentTick - game->players[i].bomb.tickExplosion > 200) frame = 1;
                if (currentTick - game->players[i].bomb.tickExplosion > 400) frame = 2;
                if (currentTick - game->players[i].bomb.tickExplosion > 600) frame = 3;
                if (currentTick - game->players[i].bomb.tickExplosion > 800) frame = 4;
                if (currentTick - game->players[i].bomb.tickExplosion > 1000) {
                    game->players[i].bomb.explosion = 0;
                }
                renderExplosion(game->pSDL, frame, game->map, game->players[i].bomb.range);
            }
            renderPlayer(game->pSDL, &game->players[i]);
        }
    }
    SDL_RenderPresent(game->pSDL->pRenderer);
}

/**
 * Function : speakthemSelf... // TODO : TT
 * @param game
 */
void drawMenu(sdl_t *pSDL)
{
    SDL_RenderClear(pSDL->pRenderer);
    SDL_SetRenderDrawColor(pSDL->pRenderer, 0, 0, 0, 255);
    renderMenu(pSDL);
    SDL_RenderPresent(pSDL->pRenderer);

}

/**
 * Function : set les positions des sprites
 * @param pSDL
 */
void renderMenu(sdl_t *pSDL)
{
    SDL_Rect dst_menuLogo = {(MAP_SIZE_W / 2) - (IMG_LOGO_W / 2), 20, IMG_LOGO_W, IMG_LOGO_H};

    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuLogo, NULL, &dst_menuLogo);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonPlay->textureButton[pSDL->buttonPlay->hover], NULL, &pSDL->buttonPlay->dstRect);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonQuit->textureButton[pSDL->buttonQuit->hover], NULL, &pSDL->buttonQuit->dstRect);

}

void drawMenuNetwork(sdl_t *pSDL)
{
    SDL_RenderClear(pSDL->pRenderer);
    SDL_SetRenderDrawColor(pSDL->pRenderer, 0, 0, 0, 255);
    renderMenuNetwork(pSDL);
    SDL_RenderPresent(pSDL->pRenderer);
}

/**
 *
 * @param pSDL
 */
void renderMenuNetwork(sdl_t *pSDL)
{
    SDL_Rect dst_menuLogo =     {(MAP_SIZE_W / 2) - (IMG_LOGO_W / 2), 20, IMG_LOGO_W, IMG_LOGO_H};
    SDL_Rect dst_menuQuitter = {(MAP_SIZE_W / 2) - (IMG_MENU_W / 6), 600, IMG_MENU_W / 3, IMG_MENU_H / 3};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuLogo, NULL, &dst_menuLogo);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonHost->textureButton[pSDL->buttonHost->hover], NULL, &pSDL->buttonHost->dstRect);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonConnect->textureButton[pSDL->buttonConnect->hover], NULL, &pSDL->buttonConnect->dstRect);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonQuit->textureButton[pSDL->buttonQuit->hover], NULL, &dst_menuQuitter);
}

/**
 * function : render de la bomb/ avec effet d'agrandissement/ timing de la bomb
 * @param pSDL
 * @param game
 */
void renderBomb(sdl_t *pSDL, player_t *player)
{
    static int n = 0;
    const int size_m = 2;
    int currentTick = SDL_GetTicks();
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBomb, NULL, &pSDL->dst_bomb);
    if (currentTick - player->bomb.tickBombDropped > 1000 && n == 0) {
        pSDL->dst_bomb.x -= BOMB_PNG_W / size_m;
        pSDL->dst_bomb.y -= BOMB_PNG_W / size_m;
        pSDL->dst_bomb.h *= size_m;
        pSDL->dst_bomb.w *= size_m;
        n = 1;
    }
    if (currentTick - player->bomb.tickBombDropped > 2000) {
        player->bombPosed = 0;
        player->bomb.tickBombDropped = 0;
        makeExplosion(player, pSDL->son[1]);
        n = 0;
    }
}
void renderExplosion(sdl_t *pSDL, int frame, map_t map, int range)
{
    int isRightBlocked = 0;
    int isLeftBlocked = 0;
    int isUpBlocked = 0;
    int isDownBlocked = 0;
    const int cell_x = (pSDL->dst_bomb.x - REAL_BLOCK_SIZE) / REAL_BLOCK_SIZE;
    const int cell_y = (pSDL->dst_bomb.y - REAL_BLOCK_SIZE / 2) / REAL_BLOCK_SIZE;
    SDL_Rect dst_mid = {pSDL->dst_bomb.x + ((pSDL->dst_bomb.w - REAL_BLOCK_SIZE) / 2), pSDL->dst_bomb.y + ((pSDL->dst_bomb.h - REAL_BLOCK_SIZE) / 2), REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
    SDL_Rect src = {0, 64 - frame * 16, 16, 16};

    for (int i = 0; i < range; i++) {

        SDL_Rect dst_right = {dst_mid.x +  ((i + 1) * REAL_BLOCK_SIZE), dst_mid.y, REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_Rect dst_left = {dst_mid.x - ((i + 1) * REAL_BLOCK_SIZE), dst_mid.y, REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_Rect dst_up = {dst_mid.x, dst_mid.y - ((i + 1) * REAL_BLOCK_SIZE), REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_Rect dst_down = {dst_mid.x, dst_mid.y + ((i + 1) * REAL_BLOCK_SIZE), REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion2[CENTERFLAME], &src, &dst_mid);
        // passe à travers bloc destructible car détruit avant le render

        if (!getBit(map[cell_y], cell_x + 1 + i, 1) && cell_x + 1 + i <= 12 && !isRightBlocked) {
            if (i >= range - 1) SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion2[RIGHTFLAME], &src, &dst_right);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion2[HORIZONTALFLAME], &src, &dst_right);
        } else isRightBlocked = 1;
        if (!getBit(map[cell_y], cell_x - 1 - i, 1) && cell_x - 1 - i >= 0 && !isLeftBlocked) {
            if (i >= range - 1) SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion2[LEFTFLAME], &src, &dst_left);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion2[HORIZONTALFLAME], &src, &dst_left);
        } else isLeftBlocked = 1;
        if (!getBit(map[cell_y - 1 - i], cell_x, 1) && cell_y - 1 - i >= 0 && !isUpBlocked) {
            if (i >= range - 1) SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion2[UPFLAME], &src, &dst_up);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion2[VERTICALFLAME], &src, &dst_up);
        } else isUpBlocked = 1;
        if (!getBit(map[cell_y + 1 + i], cell_x, 1) && cell_y + 1 + i <= 12 && !isDownBlocked) {
            if (i >= range - 1) SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion2[DOWNFLAME], &src, &dst_down);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion2[VERTICALFLAME], &src, &dst_down);
        } else isDownBlocked = 1;

    }
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
//    SDL_Log("player_x : %d, player_y : %d", player->x_pos, player->y_pos);
    SDL_Rect r = {player->x_pos, player->y_pos, PLAYER_WIDTH, PLAYER_HEIGHT};
//    SDL_Log("x_pos: %d, y_pos: %d", player->x_pos, player->y_pos);
//    SDL_Log("player number: %d", player->number);
//    SDL_Log("player texture: %d", pSDL->texturePlayers[player->number] != NULL);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->texturePlayers[0], &src, &r);

    if (player->still == 0) {
        player->frame_time++;
        if (FPS / player->frame_time == 4) {
            player->current_frame++;
            player->frame_time = 0;
        }
    }
}

/**
 * function : Render the block at the coordinates x,y
 * @param pSDL
 * @param x
 * @param y
 */
void renderBlock(sdl_t *pSDL, int x, int y)
{
    SDL_Rect src_block = {0, 0, BLOCK_SIZE_PNG, BLOCK_SIZE_PNG};
    SDL_Rect dst_block = {START_X_MAP + (x * REAL_BLOCK_SIZE),
                          START_Y_MAP + (y * REAL_BLOCK_SIZE),
                          REAL_BLOCK_SIZE,
                          REAL_BLOCK_SIZE};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBlock, &src_block, &dst_block);
}

/** TODO
 * function: Render the map with the blocks
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

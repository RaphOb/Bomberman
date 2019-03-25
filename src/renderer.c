//
// Created by geoff on 14/02/2019.
//

#include "../header/renderer.h"
#include "../header/bit.h"
#include "../header/menu.h"
#include "../header/bonus.h"
#include "../header/bomb.h"


/**
 * function : Display all the textures concerning the game
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
            int currentTick = SDL_GetTicks();
            for (int j = 0; j < game->players[i].nbBombe; j++) {
                if (game->players[i].bomb[j].isPosed) {
                    renderBomb(game->pSDL, &game->players[i].bomb[j]);
                }
                if (game->players[i].bomb[j].explosion == 1) {
                    int frame = 0;
                    for (int k = 1; k <= 4; k++) {
                        if (currentTick - game->players[i].bomb[j].tickExplosion > k * 200) frame = k;
                    }
                    if (currentTick - game->players[i].bomb[j].tickExplosion > 1000) {
                        game->players[i].bomb[j].explosion = 0;
                        game->players[i].bombPosed--;
                        checkBombDamage(game->map, game->players[i].bomb[j]);
                        game->players[i].bomb[j].cell_x = -1;
                        game->players[i].bomb[j].cell_y = -1;
                    } else {
                        renderExplosion(game->pSDL, frame, game->map, game->players[i].bomb[j]);
                    }
                }
            }
            if (game->players[i].alive == 'Y' && game->players[i].co_is_ok != -1) {
                renderPlayer(game->pSDL, &game->players[i]);
            }
        }
    }
    SDL_RenderPresent(game->pSDL->pRenderer);
}

/**
 * function : Draw the first menu in the game
 * @param pSDL
 */
void drawMenu(sdl_t *pSDL)
{
    SDL_RenderClear(pSDL->pRenderer);
    SDL_SetRenderDrawColor(pSDL->pRenderer, 0, 0, 0, 255);
    renderMenu(pSDL);
    SDL_RenderPresent(pSDL->pRenderer);

}

/**
 * Function : set the sprites's positions
 * @param pSDL
 */
void renderMenu(sdl_t *pSDL)
{
    SDL_Rect dst_menuLogo = {(MAP_SIZE_W / 2) - (IMG_LOGO_W / 2), 20, IMG_LOGO_W, IMG_LOGO_H};

    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuLogo, NULL, &dst_menuLogo);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonPlay->textureButton[pSDL->buttonPlay->hover], NULL, &pSDL->buttonPlay->dstRect);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonQuit->textureButton[pSDL->buttonQuit->hover], NULL, &pSDL->buttonQuit->dstRect);

}

/**
 * function : Draw the second menu concerning the network
 * @param pSDL
 */
void drawMenuNetwork(sdl_t *pSDL)
{
    SDL_RenderClear(pSDL->pRenderer);
    SDL_SetRenderDrawColor(pSDL->pRenderer, 0, 0, 0, 255);
    renderMenuNetwork(pSDL);
    SDL_RenderPresent(pSDL->pRenderer);
}

/**
 * function : Render the buttons in the network menu
 * @param pSDL
 */
void renderMenuNetwork(sdl_t *pSDL)
{
    SDL_Rect dst_menu_retour = {20, 550, 350, 350};
    SDL_Rect dst_menuLogo =     {(MAP_SIZE_W / 2) - (IMG_LOGO_W / 2), 20, IMG_LOGO_W, IMG_LOGO_H};
    SDL_Rect dst_menuQuitter = {(MAP_SIZE_W / 2) - (IMG_MENU_W / 6), 600, IMG_MENU_W / 3, IMG_MENU_H / 3};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuRetour, NULL, &dst_menu_retour);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuLogo, NULL, &dst_menuLogo);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonHost->textureButton[pSDL->buttonHost->hover], NULL, &pSDL->buttonHost->dstRect);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonConnect->textureButton[pSDL->buttonConnect->hover], NULL, &pSDL->buttonConnect->dstRect);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonQuit->textureButton[pSDL->buttonQuit->hover], NULL, &dst_menuQuitter);
}

/**
 * function : render de la bomb/ avec effet d'agrandissement/ timing de la bomb
 * @param pSDL
 * @param player
 */
void renderBomb(sdl_t *pSDL, bomb_t *bomb)
{
    static int n = 0;
    const int size_m = 2;
    int currentTick = SDL_GetTicks();
    SDL_Rect dst_bomb = {bomb->pos_x, bomb->pos_y, bomb->width, bomb->height};
//        SDL_Log("pos_x : %d, pos_y; %d, width: %d, height: %d", bomb->pos_x, bomb->pos_x, bomb->width, bomb->height);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBomb, NULL, &dst_bomb);
    if (currentTick - bomb->tickBombDropped > 1000 && n == 0) {
        bomb->pos_x -= BOMB_PNG_W / size_m;
        bomb->pos_y -= BOMB_PNG_H / size_m;
        bomb->height *= size_m;
        bomb->width *= size_m;
        n = 1;
    }
    if (currentTick - bomb->tickBombDropped > 2000) {
        makeExplosion(bomb, pSDL->son[1]);
        n = 0;
    }

//    for (int i = 0; i < MAX_BOMBE; i++) {
//        if (player->bomb[i].isPosed) {
//            SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBomb, NULL, &pSDL->dst_bomb[i]);
//            if (currentTick - player->bomb[i].tickBombDropped > 1000 && n == 0) {
//                pSDL->dst_bomb[i].x -= BOMB_PNG_W / size_m;
//                pSDL->dst_bomb[i].y -= BOMB_PNG_H / size_m;
//                pSDL->dst_bomb[i].h *= size_m;
//                pSDL->dst_bomb[i].w *= size_m;
//                n = 1;
//            }
//            if (currentTick - player->bomb[i].tickBombDropped > 2000) {
//                player->bomb[i].tickBombDropped = 0;
//                player->bomb[i].isPosed = 0;
//                player->bomb[i].explosion = 1;
//                player->bomb[i].tickExplosion = SDL_GetTicks();
//                playSound(pSDL->son[1]);
//                //            makeExplosion(player, pSDL->son[1]);
//                n = 0;
//            }
//        }
//    }
}

/**
 * function : Render the explosion with an animation cell per cell until the range is reached.
 * If a wall is on the way, the explosion's spread stops in that direction
 * @param pSDL
 * @param frame
 * @param map
 * @param range
 */
void renderExplosion(sdl_t *pSDL, int frame, map_t map, bomb_t bomb)
{
    int blocked[4] = {0};
    int pos_x = START_X_MAP + (bomb.cell_x * REAL_BLOCK_SIZE) + (REAL_BLOCK_SIZE / 2) - (BOMB_PNG_W / 2);
    int pos_y = START_Y_MAP + (bomb.cell_y * REAL_BLOCK_SIZE) + (REAL_BLOCK_SIZE / 2) - (BOMB_PNG_H / 2);
//    SDL_Log("pos_x : %d, pos_y : %d", pos_x, pos_y);

    const int cell_x = (pos_x - REAL_BLOCK_SIZE) / REAL_BLOCK_SIZE;
    const int cell_y = (pos_y - REAL_BLOCK_SIZE / 2) / REAL_BLOCK_SIZE;
    SDL_Rect dst_mid = {pos_x + ((BOMB_PNG_W - REAL_BLOCK_SIZE) / 2), pos_y + ((BOMB_PNG_H - REAL_BLOCK_SIZE) / 2), REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
    SDL_Rect src = {0, 64 - frame * 16, 16, 16};

    for (int i = 0; i < bomb.range; i++) {

        SDL_Rect dst_right = {dst_mid.x +  ((i + 1) * REAL_BLOCK_SIZE), dst_mid.y, REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_Rect dst_left = {dst_mid.x - ((i + 1) * REAL_BLOCK_SIZE), dst_mid.y, REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_Rect dst_up = {dst_mid.x, dst_mid.y - ((i + 1) * REAL_BLOCK_SIZE), REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_Rect dst_down = {dst_mid.x, dst_mid.y + ((i + 1) * REAL_BLOCK_SIZE), REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[CENTERFLAME], &src, &dst_mid);

        if (!getBit(map[cell_y], cell_x + 1 + i, 1) && cell_x + 1 + i <= 12 && !blocked[0]) {
            if (i >= bomb.range - 1) SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[RIGHTFLAME], &src, &dst_right);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[HORIZONTALFLAME], &src, &dst_right);
        } else blocked[0] = 1;
        if (!getBit(map[cell_y], cell_x - 1 - i, 1) && cell_x - 1 - i >= 0 && !blocked[1]) {
            if (i >= bomb.range - 1) SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[LEFTFLAME], &src, &dst_left);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[HORIZONTALFLAME], &src, &dst_left);
        } else blocked[1] = 1;
        if (!getBit(map[cell_y - 1 - i], cell_x, 1) && cell_y - 1 - i >= 0 && !blocked[2]) {
            if (i >= bomb.range - 1) SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[UPFLAME], &src, &dst_up);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[VERTICALFLAME], &src, &dst_up);
        } else blocked[2] = 1;
        if (!getBit(map[cell_y + 1 + i], cell_x, 1) && cell_y + 1 + i <= 8 && !blocked[3]) {
            if (i >= bomb.range - 1) SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[DOWNFLAME], &src, &dst_down);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[VERTICALFLAME], &src, &dst_down);
        } else blocked[3] = 1;

    }
}

/**
 * function : Render the background
 * @param pSDL
 */
void renderBackground(sdl_t *pSDL)
{
    SDL_Rect src_map = {0, 0, 722 / 3, 482 / 3};
    SDL_Rect dst_map = {START_X_BACKGROUND, START_Y_BACKGROUND, MAP_SIZE_W, MAP_SIZE_H};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMap, &src_map, &dst_map);
}

/**
 * function : Render the player with an animation
 * @param pSDL
 * @param player
 */
void renderPlayer(sdl_t *pSDL, player_t *player)
{
    if (player->current_frame > 2) {
        player->current_frame = 0;
    }

    SDL_Rect src = {FRAME_WIDTH * player->current_frame, (FRAME_HEIGHT) * player->direction, FRAME_WIDTH, FRAME_HEIGHT};
//    SDL_Log("player_x : %d, player_y : %d", player->cell_x, player->cell_y);
    SDL_Rect r = {player->x_pos, player->y_pos, PLAYER_WIDTH, PLAYER_HEIGHT};
//    SDL_Log("cell_x: %d, cell_y: %d", player->cell_x, player->cell_y);
//    SDL_Log("player number: %d", player->number);
//    SDL_Log("player texture: %d", pSDL->texturePlayers[player->number] != NULL);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->texturePlayers[player->number], &src, &r);

    if (player->still == 0) {
        player->frame_time++;
        if (FPS / player->frame_time == 4) {
            player->current_frame++;
            player->frame_time = 0;
        }
    }
}

void renderBonus(sdl_t *pSDL, typeBonus_e type, int x, int y)
{
    SDL_Rect src = {0, 0, 24, 24};
    SDL_Rect dst = {START_X_MAP + (x * REAL_BLOCK_SIZE),
                    START_Y_MAP + (y * REAL_BLOCK_SIZE),
                    REAL_BLOCK_SIZE,
                    REAL_BLOCK_SIZE};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBonus[type], &src, &dst);
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

/**
 * function: Render the map with the blocks
 * @param game
 * @param pSdl
 */
void renderMap(map_t map, sdl_t *pSdl)
{
    for (int i = 0; i < MAP_X; i++) {
        for (int j = 0; j < MAP_Y; j++) {
            if (getBit(map[i], j, 1) && (getBit(map[i], j, 2))) {
                    renderBlock(pSdl, j, i);
            }
            if (getBit(map[i], j, 4)) {
                renderBonus(pSdl, getBonus(map, j, i), j, i);
            }
//            if (getBit(map[i], j, 3)) {
//                SDL_Log("aled");
//                renderBomb(pSdl, getBomb(j, i, player));
//            }
        }
    }
}

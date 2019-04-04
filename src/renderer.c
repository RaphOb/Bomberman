//
// Created by geoff on 14/02/2019.
//

#include "../header/renderer.h"
#include "../header/bit.h"
#include "../header/menu.h"
#include "../header/bonus.h"
#include "../header/bomb.h"
#include "../header/input.h"
#include "../header/reseau.h"


/**
 * function : Display all the textures concerning the game
 * @param game
 */
void drawGame(game_t *game) {
    pthread_mutex_lock(&game->mutex_map);
    SDL_RenderClear(game->pSDL->pRenderer);
    renderBanner(game->pSDL, game->players, game);
//    renderGameOver(game->pSDL);
    renderBackground(game->pSDL);
    renderMap(game->map, game->pSDL);
    for (int i = 0; i < MAX_PLAYER; i++)  {
        if (game->players[i].number != -1) {
            for (int j = 0; j < MAX_BOMBE; j++) {
                if (game->players[i].bomb[j].isPosed) {
                    renderBomb(game->pSDL, &game->players[i].bomb[j]);
                }
                if (game->players[i].bomb[j].explosion == 1) {
                    renderExplosion(game->pSDL, game->players[i].bomb[j].frame, game->map, game->players[i].bomb[j]);
                }
            }
            if (game->players[i].alive == 'Y' && game->players[i].co_is_ok != -1) {
                renderPlayer(game->pSDL, &game->players[i]);
            } else if (game->players[i].alive == 'N') {
                if (game->players[i].current_frame < 9) {
                    renderBlood(game->pSDL, &game->players[i]);
                }
                game->players[i].current_frame++;
            }
        }
    }
    if (getMyPlayer(game)->alive == 'Y' && isPlayerDead(game->players, getMyPlayer(game)->number) > 0) {
        renderWin(game->pSDL);
        game->leave = menuGameOverEvent(game->pSDL);
        if (game->leave == -1) {
            c_emission(getMyPlayer(game), DISCONNECT_CODE);
        }
    }
    if (getMyPlayer(game)->alive == 'N') {
        renderGameOver(game->pSDL);
        game->leave = menuGameOverEvent(game->pSDL);
        if (game->leave == -1) {
            c_emission(getMyPlayer(game), DISCONNECT_CODE);
        }
    }
    SDL_RenderPresent(game->pSDL->pRenderer);
    pthread_mutex_unlock(&game->mutex_map);
}

/**
 * function : Draw the first menu in the game
 * @param pSDL
 */
void drawMenu(sdl_t *pSDL) {
    SDL_RenderClear(pSDL->pRenderer);
    SDL_SetRenderDrawColor(pSDL->pRenderer, 0, 0, 0, 255);
    renderBackgroundMenu(pSDL, 0);
    renderMenu(pSDL);
    SDL_RenderPresent(pSDL->pRenderer);

}

/**
 * function : draw banner for score and bonus statement
 * @param pSDL
 * @param players
 * @param game
 */
void renderBanner(sdl_t *pSDL, player_t players[MAX_PLAYER], game_t *game) {
    player_t *player = getMyPlayer(game);
    char str[2] = {'\0'};
    const int y = 25;
    SDL_Color color = {0, 0, 0, 255};
    SDL_Rect dst_banner = {0, 0, START_X_BACKGROUND + MAP_SIZE_W, START_Y_BACKGROUND};
    SDL_Rect dst_bonus = {START_X_BACKGROUND + (MAP_SIZE_W / 3), y, 50, 50};
    SDL_Rect dst_text_bonus = {START_X_BACKGROUND + (MAP_SIZE_W / 3) + 60, y, 0, 0};
    SDL_Rect dst_bonus2 = {START_X_BACKGROUND + (MAP_SIZE_W / 4), y, 50, 50};
    SDL_Rect dst_text_bonus2 = {START_X_BACKGROUND + (MAP_SIZE_W / 4) + 60, y, 0, 0};
    SDL_Rect dst_bonus3 = {START_X_BACKGROUND + (MAP_SIZE_W / 6), y, 50, 50};
    SDL_Rect dst_text_bonus3 = {START_X_BACKGROUND + (MAP_SIZE_W / 6) + 60, y, 0, 0};
    sprintf(str, "%d", player->bomb[0].range);
    SDL_Texture *text_bonus = createTextureText(pSDL->pRenderer, pSDL->font, color, str);
    sprintf(str, "%d", player->nbBombe);
    SDL_Texture *text_bonus2 = createTextureText(pSDL->pRenderer, pSDL->font, color, str);
    sprintf(str, "%d", player->speed - 2);
    SDL_Texture *text_bonus3 = createTextureText(pSDL->pRenderer, pSDL->font, color, str);
    // Render rect
    SDL_SetRenderDrawColor(pSDL->pRenderer, 36, 182, 102, 255);
    SDL_RenderFillRect(pSDL->pRenderer, &dst_banner);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBonus[bRangeBombe], NULL, &dst_bonus);
    renderStringText(pSDL->pRenderer, text_bonus, dst_text_bonus);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBonus[bNbBombe], NULL, &dst_bonus2);
    renderStringText(pSDL->pRenderer, text_bonus2, dst_text_bonus2);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBonus[bVitesse], NULL, &dst_bonus3);
    renderStringText(pSDL->pRenderer, text_bonus3, dst_text_bonus3);

    renderTextPlayer(pSDL, players);

    SDL_DestroyTexture(text_bonus);
    SDL_DestroyTexture(text_bonus2);
    SDL_DestroyTexture(text_bonus3);
}

/**
 * function : display speudo
 * @param pSDL
 * @param players
 */
void renderTextPlayer(sdl_t *pSDL, player_t players[MAX_PLAYER]) {
    SDL_Color color = {45, 94, 205, 255};
    SDL_Rect dst = {0, 0, 0, 0};
    SDL_Texture *textureTextPlayer;
    for (int i = 0; i < MAX_PLAYER; i++) {
        if (players[i].number != -1) {
            if (players[i].alive == 'N') {
                color.r = 205;
                color.g = 56;
                color.b = 26;
            }
            int divide = (i % 2) == 0 ? 2 : 4;
            int y = (i < 2) ? 0 : 30;
            dst.x = MAP_SIZE_W - (MAP_SIZE_W / divide);
            dst.y = y;
            textureTextPlayer = createTextureText(pSDL->pRenderer, pSDL->font, color, players[i].name);
            renderStringText(pSDL->pRenderer, textureTextPlayer, dst);
            SDL_DestroyTexture(textureTextPlayer);
        }
    }
}

/**
 * Function : set the sprites's positions
 * @param pSDL
 */
void renderMenu(sdl_t *pSDL) {
    SDL_Rect dst_menuLogo = {(MAP_SIZE_W / 2) - (IMG_LOGO_W / 2), 20, IMG_LOGO_W, IMG_LOGO_H};

    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuLogo, NULL, &dst_menuLogo);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonPlay->textureButton[pSDL->buttonPlay->hover], NULL,
                   &pSDL->buttonPlay->dstRect);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonQuit->textureButton[pSDL->buttonQuit->hover], NULL,
                   &pSDL->buttonQuit->dstRect);

}

/**
 *function : render menu game over when ... game is over :D
 * @param pSDL
 */
void renderGameOver(sdl_t *pSDL) {
    SDL_Rect dst_menuQuitter = {550, 650, IMG_MENU_W / 3, IMG_MENU_H / 3};
    SDL_Rect dst_menugameover = {50, 100, MAP_SIZE_W, MAP_SIZE_W / 2};


    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonQuit->textureButton[pSDL->buttonQuit->hover], NULL, &dst_menuQuitter);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->texturegameover, NULL, &dst_menugameover);
}

/**
 * function : render win logo if player win
 * @param pSDL
 */
void renderWin(sdl_t *pSDL) {
    SDL_Rect dst_youwin = {50, 100, MAP_SIZE_W, MAP_SIZE_W / 2};
    SDL_Rect dst_menuQuitter = {550, 650, IMG_MENU_W / 3, IMG_MENU_H / 3};

    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureyouWin, NULL, &dst_youwin);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonQuit->textureButton[pSDL->buttonQuit->hover], NULL, &dst_menuQuitter);
}

/**
 * function : Draw the second menu concerning the network
 * @param pSDL
 */
void drawMenuNetwork(sdl_t *pSDL) {
    SDL_RenderClear(pSDL->pRenderer);
    SDL_SetRenderDrawColor(pSDL->pRenderer, 0, 0, 0, 255);
    renderBackgroundMenu(pSDL, 0);
    renderMenuNetwork(pSDL);
    SDL_RenderPresent(pSDL->pRenderer);
}

/**
 * function : Render the buttons in the network menu
 * @param pSDL
 */
void renderMenuNetwork(sdl_t *pSDL) {
    SDL_Rect dst_menu_retour = {20, 550, 350, 350};
    SDL_Rect dst_menuLogo = {(MAP_SIZE_W / 2) - (IMG_LOGO_W / 2), 20, IMG_LOGO_W, IMG_LOGO_H};
    SDL_Rect dst_menuQuitter = {(MAP_SIZE_W / 2) - (IMG_MENU_W / 6), 600, IMG_MENU_W / 3, IMG_MENU_H / 3};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuRetour, NULL, &dst_menu_retour);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuLogo, NULL, &dst_menuLogo);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonHost->textureButton[pSDL->buttonHost->hover], NULL,
                   &pSDL->buttonHost->dstRect);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonConnect->textureButton[pSDL->buttonConnect->hover], NULL,
                   &pSDL->buttonConnect->dstRect);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonQuit->textureButton[pSDL->buttonQuit->hover], NULL, &dst_menuQuitter);
}

void drawMenuLobby(sdl_t *pSDL, player_t players[MAX_PLAYER], int host) {
    SDL_RenderClear(pSDL->pRenderer);
    renderBackgroundMenu(pSDL, 1);
    renderMenuLobby(pSDL, players, host);
    SDL_RenderPresent(pSDL->pRenderer);
}


void renderMenuLobby(sdl_t *pSDL, player_t players[MAX_PLAYER], int host) {
    SDL_Rect dst_menuLogo = {(MAP_SIZE_W / 2) - (IMG_LOGO_W / 2), 20, IMG_LOGO_W, IMG_LOGO_H};
    SDL_Rect dst_menuQuitter = {(MAP_SIZE_W / 2) - (IMG_MENU_W / 6) + 400, 600, IMG_MENU_W / 3, IMG_MENU_H / 3};
    renderPlayerConnected(pSDL, players);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuLogo, NULL, &dst_menuLogo);
    if (host == 1) {
        SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonLaunch->textureButton[pSDL->buttonLaunch->hover], NULL,
                       &pSDL->buttonLaunch->dstRect);
    }
    SDL_RenderCopy(pSDL->pRenderer, pSDL->buttonQuit->textureButton[pSDL->buttonQuit->hover], NULL, &dst_menuQuitter);

}

void renderPlayerConnected(sdl_t *pSDL, player_t players[MAX_PLAYER]) {
    TTF_Font *font = TTF_OpenFont("../resources/font/Pixeled.ttf", 20);
    SDL_Color color = {255, 255, 255, 255};
    SDL_Rect dstStringBase = {400, 300, 0, 0};
    SDL_Texture *textureHost = createTextureText(pSDL->pRenderer, font, color, "Joueur(s) connecte(s): ");
    SDL_Rect base_dst = {500, 300, 0, 0};
    renderStringText(pSDL->pRenderer, textureHost, dstStringBase);
    for (int i = 0; i < MAX_PLAYER; i++) {
        if (players[i].number != -1) {
            // Connected
            SDL_Texture *textureTextPlayer = createTextureText(pSDL->pRenderer, font, color, players[i].name);
            base_dst.y += 50;
            renderStringText(pSDL->pRenderer, textureTextPlayer, base_dst);
            SDL_DestroyTexture(textureTextPlayer);
        }
    }
    SDL_DestroyTexture(textureHost);
    TTF_CloseFont(font);

}

/**
 * function : render de la bomb/ avec effet d'agrandissement/ timing de la bomb
 * @param pSDL
 * @param player
 */
void renderBomb(sdl_t *pSDL, bomb_t *bomb) {
    int currentTick = SDL_GetTicks();
    SDL_Rect dst_bomb = {bomb->pos_x, bomb->pos_y, bomb->width, bomb->height};
//        SDL_Log("pos_x : %d, pos_y; %d, width: %d, height: %d", bomb->pos_x, bomb->pos_x, bomb->width, bomb->height);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBomb, NULL, &dst_bomb);
    if (currentTick - bomb->tickBombDropped > 1980) {
        playSound(pSDL->son[1]);
    }
}

/**
 * function : Render the explosion with an animation cell per cell until the range is reached.
 * If a wall is on the way, the explosion's spread stops in that direction
 * @param pSDL
 * @param frame
 * @param map
 * @param range
 */
void renderExplosion(sdl_t *pSDL, int frame, map_t map, bomb_t bomb) {
    int blocked[4] = {0};
    int pos_x = START_X_MAP + (bomb.cell_x * REAL_BLOCK_SIZE) + (REAL_BLOCK_SIZE / 2) - (BOMB_PNG_W / 2);
    int pos_y = START_Y_MAP + (bomb.cell_y * REAL_BLOCK_SIZE) + (REAL_BLOCK_SIZE / 2) - (BOMB_PNG_H / 2);

    const int cell_x = (pos_x - START_X_BACKGROUND - REAL_BLOCK_SIZE) / REAL_BLOCK_SIZE;
    const int cell_y = (pos_y - START_Y_BACKGROUND - REAL_BLOCK_SIZE / 2) / REAL_BLOCK_SIZE;
    SDL_Rect dst_mid = {pos_x + ((BOMB_PNG_W - REAL_BLOCK_SIZE) / 2), pos_y + ((BOMB_PNG_H - REAL_BLOCK_SIZE) / 2),
                        REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
    SDL_Rect src = {0, 64 - frame * 16, 16, 16};

    for (int i = 0; i < bomb.range; i++) {

        SDL_Rect dst_right = {dst_mid.x + ((i + 1) * REAL_BLOCK_SIZE), dst_mid.y, REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_Rect dst_left = {dst_mid.x - ((i + 1) * REAL_BLOCK_SIZE), dst_mid.y, REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_Rect dst_up = {dst_mid.x, dst_mid.y - ((i + 1) * REAL_BLOCK_SIZE), REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_Rect dst_down = {dst_mid.x, dst_mid.y + ((i + 1) * REAL_BLOCK_SIZE), REAL_BLOCK_SIZE, REAL_BLOCK_SIZE};
        SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[CENTERFLAME], &src, &dst_mid);

        if (!getBit(map[cell_y], cell_x + 1 + i, 1) && cell_x + 1 + i <= 12 && !blocked[0]) {
            if (i >= bomb.range - 1)
                SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[RIGHTFLAME], &src, &dst_right);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[HORIZONTALFLAME], &src, &dst_right);
        } else blocked[0] = 1;
        if (!getBit(map[cell_y], cell_x - 1 - i, 1) && cell_x - 1 - i >= 0 && !blocked[1]) {
            if (i >= bomb.range - 1)
                SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[LEFTFLAME], &src, &dst_left);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[HORIZONTALFLAME], &src, &dst_left);
        } else blocked[1] = 1;
        if (!getBit(map[cell_y - 1 - i], cell_x, 1) && cell_y - 1 - i >= 0 && !blocked[2]) {
            if (i >= bomb.range - 1) SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[UPFLAME], &src, &dst_up);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[VERTICALFLAME], &src, &dst_up);
        } else blocked[2] = 1;
        if (!getBit(map[cell_y + 1 + i], cell_x, 1) && cell_y + 1 + i <= 8 && !blocked[3]) {
            if (i >= bomb.range - 1)
                SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[DOWNFLAME], &src, &dst_down);
            else SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[VERTICALFLAME], &src, &dst_down);
        } else blocked[3] = 1;

    }
}

/**
 * function : Render the background
 * @param pSDL
 */
void renderBackground(sdl_t *pSDL) {
    SDL_Rect src_map = {0, 0, 722 / 3, 482 / 3};
    SDL_Rect dst_map = {START_X_BACKGROUND, START_Y_BACKGROUND, MAP_SIZE_W, MAP_SIZE_H};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMap, &src_map, &dst_map);
}

/**
 * function : Render the player with an animation
 * @param pSDL
 * @param player
 */
void renderPlayer(sdl_t *pSDL, player_t *player) {
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

/**
 *
 * @param player
 */
void renderBlood(sdl_t *pSDL, player_t *player) {
    //SDL_Log("current_frame: %d", player->current_frame);
    //SDL_Log("frame_time: %d", player->frame_time);

    SDL_Rect src = {FRAME_WIDTH * player->current_frame, 0, FRAME_WIDTH, FRAME_HEIGHT};
    SDL_Rect dst = {player->x_pos, player->y_pos, PLAYER_WIDTH, PLAYER_HEIGHT};

    SDL_RenderCopy(pSDL->pRenderer, pSDL->texturePlayers[4], &src, &dst);


    player->frame_time++;
    if (FPS / player->frame_time == 6) {
        player->current_frame++;
        player->frame_time = 0;
    }

}

void renderBonus(sdl_t *pSDL, typeBonus_e type, int x, int y) {
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
void renderBlock(sdl_t *pSDL, int x, int y) {
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
void renderMap(map_t map, sdl_t *pSdl) {
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

void renderBackgroundMenu(sdl_t *pSDL, int i) {
    SDL_Rect dst = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBackground[i], NULL, &dst);
}

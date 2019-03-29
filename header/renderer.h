//
// Created by geoff on 14/02/2019.
//

#ifndef BOMBERMAN_RENDERER_H
#define BOMBERMAN_RENDERER_H

#include "../header/game.h"

void drawGame(game_t *game);
void renderBomb(sdl_t *pSDL, bomb_t *bomb);
void renderBackground(sdl_t *pSDL);
void renderPlayer(sdl_t *pSDL, player_t *player);
void renderExplosion(sdl_t *pSDL, int frame, map_t map, bomb_t bomb);
void renderBlock(sdl_t *pSDL, int x, int y);
void renderMap(map_t map, sdl_t *pSdl);
void renderMenu(sdl_t *pSDL);
void drawMenu(sdl_t *pSDL);
void drawMenuNetwork(sdl_t *pSDL);
void renderMenuNetwork(sdl_t *pSDL);
void renderBanner(sdl_t *pSDL, player_t player[MAX_PLAYER], game_t *game);
void renderTextPlayer(sdl_t *pSDL, player_t players[MAX_PLAYER]);
void renderBackgroundMenu(sdl_t *pSDL, int i);

#endif //BOMBERMAN_RENDERER_H

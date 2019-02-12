//
// Created by geoff on 16/01/2019.
//

#include <SDL2/SDL_events.h>
#include <stdio.h>
#include "../header/game.h"
#include "../header/map.h"
#include "../header/map.h"


/**
 * function : init game
 * @return
 */
game_t *initGame(sdl_t *pSDL)
{
    game_t *game = malloc(sizeof(game_t));
    if (!game) {
        return (NULL);
    }

    game->current_map = 0;
    if (extractArrayFromFile(game->map) == 0) {
        return (NULL);
    }
    game->pSDL = pSDL;
    return game;
}

/**
 * function : key press event
 * @param game
 * @return : int
 */
int game_event(game_t *game)
{
    int res = 0;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    int keys[302];
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            res = -1;
        } else if (event.type == SDL_KEYDOWN) {
            //
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE :
                    res = -1;
                    break;
                case SDLK_b:
                    placeBomb(game);
                    break;
                default :
                    fprintf(stderr,"touche inconnue %d\n", event.key.keysym.sym);
            }
        }
    }
    game->players[0]->still = 0;
    if (keystates[SDL_SCANCODE_UP]) {
        if ( game->players[0]->y_pos > 30) {
            game->players[0]->y_pos -= 3;
            game->players[0]->direction = 0;
        }
    }
    else if (keystates[SDL_SCANCODE_RIGHT]) {
        if (game->players[0]->x_pos < (MAP_SIZE_W - (game->pSDL->dst_player.w + 70))) {
            game->players[0]->x_pos += 3;
            game->players[0]->direction = 1;
        }
    }
    else if (keystates[SDL_SCANCODE_DOWN]) {
        if (game->players[0]->y_pos < (MAP_SIZE_H - (game->pSDL->dst_player.h + 30))) {
            game->players[0]->y_pos += 3;
            game->players[0]->direction = 2;
        }
    }
    else if (keystates[SDL_SCANCODE_LEFT]) {
        if (game->players[0]->x_pos > 70 ) {
            game->players[0]->x_pos -= 3;
            game->players[0]->direction = 3;
        }
    } else {
        game->players[0]->still = 1;
    }
    return res;
}

void makeExplosion(game_t *game)
{
    game->pSDL->dst_explosion2.x = game->pSDL->dst_bomb.x - (BLOCK_SIZE * SIZE_M) - 20;
    game->pSDL->dst_explosion2.y = game->pSDL->dst_bomb.y - (BLOCK_SIZE * SIZE_M) - 15;
    game->pSDL->dst_explosion2.h = BLOCK_SIZE * SIZE_M * 3;
    game->pSDL->dst_explosion2.w = BLOCK_SIZE * SIZE_M * 3;
    game->pSDL->dst_explosion.x = game->pSDL->dst_explosion2.x + 40;
    game->pSDL->dst_explosion.y = game->pSDL->dst_explosion2.y + 30;
    game->pSDL->dst_explosion.h = BLOCK_SIZE * SIZE_M * 2;
    game->pSDL->dst_explosion.w = BLOCK_SIZE * SIZE_M * 2;

    game->players[0]->explosion = 1;
    game->players[0]->tickExplosion = SDL_GetTicks();
}

void placeBomb(game_t *game)
{
    SDL_Log("placeBomb");
   game->pSDL->dst_bomb.x = game->players[0]->x_pos + 10;
//   game->pSDL->dst_bomb.x = START_X_MAP * 2 + 25; // TODO remplacer le 2 par l'index de la case ou est le joueur + 1;
   game->pSDL->dst_bomb.y = game->players[0]->y_pos + 10;
//   game->pSDL->dst_bomb.y = START_Y_MAP + 25;
   game->players[0]->bomb = 1;
   game->players[0]->tickBombDropped = SDL_GetTicks();

}

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
        if (currentTick - game->players[0]->tickExplosion > 1000) {
            game->players[0]->explosion = 0;
        }
        renderExplosion(game->pSDL);
    }

    renderPlayer(game->pSDL, game->players[0]);
    SDL_RenderPresent(game->pSDL->pRenderer);
}

void renderBomb(sdl_t *pSDL, game_t *game)
{
    int currentTick = SDL_GetTicks();
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBomb, NULL, &pSDL->dst_bomb);
    if (currentTick - game->players[0]->tickBombDropped > 2000) {
        game->players[0]->bomb = 0;
        game->players[0]->tickBombDropped = 0;
        makeExplosion(game);
    }
}
void renderExplosion(sdl_t *pSDL)
{
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion2, NULL, &pSDL->dst_explosion2);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion, NULL, &pSDL->dst_explosion);
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
    SDL_Rect r = {player->x_pos, player->y_pos, FRAME_WIDTH * 3 , FRAME_HEIGHT * 3};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->texturePlayer, &src, &r);

    if (player->still == 0) {
        player->frame_time++;
        if (FPS / player->frame_time == 4) {
            player->current_frame++;
            player->frame_time = 0;
        }
    }
}



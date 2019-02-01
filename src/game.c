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
                case SDLK_UP:
                case SDLK_DOWN:
                case SDLK_RIGHT:
                case SDLK_LEFT:
                    game_moveT(game, event.key.keysym.sym);
                    break;
                default :
                    fprintf(stderr,"touche inconnue %d\n", event.key.keysym.sym);
            }
        }
        return res;
    }

}
/**
 * function : move perso up, down, right, left
 * @param game
 * @param direction
 */
void game_moveT(game_t *game, SDL_Keycode direction)
{
    if (direction == SDLK_UP) {
        if ( game->players[0]->y_pos > 30) {
            game->players[0]->y_pos -= 10;
            SDL_Log("haut");
        }
    } else if (direction == SDLK_DOWN) {
        if (game->players[0]->y_pos < (MAP_SIZE_H - (game->pSDL->dst_trump.h + 30))) {
            game->players[0]->y_pos += 10;
            SDL_Log("bas");
        }
    } else if (direction == SDLK_LEFT) {
        if (game->players[0]->x_pos > 70 ) {
            game->players[0]->x_pos -= 10;
            SDL_Log("gauche");
        }
    } else if (direction == SDLK_RIGHT) {
        if (game->players[0]->x_pos < (MAP_SIZE_W - (game->pSDL->dst_trump.w + 70))) {
            game->players[0]->x_pos += 10;
            SDL_Log("Droite");
        }
    } else {
        fprintf(stderr, "direction inconnue ");
    }
}

void placeBomb(game_t *game)
{
   game->pSDL->dst_bomb.x = game->players[0]->x_pos + 10;
   game->pSDL->dst_bomb.y = game->players[0]->y_pos + 10;
   game->players[0]->bomb = 1;
}

/**
 * function : affiche les textures
 * @param pSDL
 * @param game
 */
void draw_game(game_t *game)
{
    SDL_RenderClear(game->pSDL->pRenderer);
    SDL_SetRenderDrawColor(game->pSDL->pRenderer, 0, 0, 0, 255);
    renderBackground(game->pSDL);
    renderMap(game->map, game->pSDL);
    if (game->players[0]->bomb == 1) {
        renderBomb(game->pSDL);
    }
    renderPlayer(game->pSDL, game->players[0]);
    SDL_RenderPresent(game->pSDL->pRenderer);
}

void renderBomb(sdl_t *pSDL)
{
    //SDL_SetRenderDrawColor(pSDL->pRenderer, 0, 0, 0, 255);
    //SDL_Log("x : %d, y: %d", pSDL->dst_bomb.x, pSDL->dst_bomb.y);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureBomb, NULL, &pSDL->dst_bomb);
   // SDL_RenderPresent(pSDL->pRenderer);
}

void renderBackground(sdl_t *pSDL)
{
    SDL_Rect src_map = {0, 0, 722 / 3, 482 / 3};
    SDL_Rect dst_map = {START_X_MAP, START_Y_MAP, MAP_SIZE_W, MAP_SIZE_H};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMap, &src_map, &dst_map);
}

void renderPlayer(sdl_t *pSDL, player_t *player)
{
    SDL_Rect r = {player->x_pos, player->y_pos, 30, 70};
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureTrump, NULL, &r);
}



//
// Created by geoff on 16/01/2019.
//

#include <SDL_events.h>
#include <stdio.h>
#include "../header/game.h"
#include "../header/file.h"
#include "../header/SDLEngine.h"

game_t *initGame()
{
    game_t *game = malloc(sizeof(game_t));
    game->dst_trump.x = START_X_MAP + (16 * SIZE_M);
    game->dst_trump.y =  START_Y_MAP + ( 8 * SIZE_M);
    game->dst_trump.h = 70; //need to change that
    game->dst_trump.w = 30; //need to change that shit
    game->src_trump.x = 0;
    game->src_trump.y =  0;
    game->src_trump.h = 100; //need to change that
    game->src_trump.w = 50; //need to change that shit
    if (!game) {
        return (NULL);
    }
    game->current_map = 0;
    if (extractArrayFromFile(game->map) == 0) {
        return (NULL);
    }

    return game;
}

/**
 * function : key press event
 * @param game
 * @return : int
 */
int game_event( game_t *game)
{
    int res = 0;

    SDL_Event evenement;
    if (SDL_PollEvent(&evenement)) {
        if (evenement.type == SDL_QUIT) {
            res = -1;
        } else if (evenement.type == SDL_KEYDOWN) {
            //
            switch (evenement.key.keysym.sym) {
                case SDLK_ESCAPE :
                    res = -1;
                    break;
                case SDLK_UP:
                case SDLK_DOWN:
                case SDLK_RIGHT:
                case SDLK_LEFT:
                    game_moveT(game, evenement.key.keysym.sym );
                    break;
                case SDLK_b:
                    break;

                default :
                    fprintf(stderr,"touche inconnue %d\n", evenement.key.keysym.sym);
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
        if ( game->dst_trump.y > 0){
            game->dst_trump.y -= 10;
            SDL_Log("haut");
        }
    } else if (direction == SDLK_DOWN) {
        if (game->dst_trump.y < (MAP_SIZE_H - game->dst_trump.h)) {
            game->dst_trump.y += 10;
            SDL_Log("bas");
        }
    } else if (direction == SDLK_LEFT) {
        if (game->dst_trump.x > 0 ) {
            game->dst_trump.x -= 10;
            SDL_Log("gauche");
        }
    } else if (direction == SDLK_RIGHT) {
        if (game->dst_trump.x < (MAP_SIZE_W- game->dst_trump.w)) {
            game->dst_trump.x += 10;
            SDL_Log("Droite");
        }
    } else {
        fprintf(stderr, "direction inconnue ");
    }

}


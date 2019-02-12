#include <stdbool.h>
#include "../header/game.h"


int main(int argc, char *argv[])
{
    // Initialisation du jeu
    const unsigned int FPSMeter = 60;
    int tempsActuel = 0;
    int tempsPrecedent = 0;
    Uint32 start;
    sdl_t *pSDL = initSDL();
    player_t *player = initPlayer();
    game_t *game = initGame(pSDL);
    if (!pSDL || !player || !game) {
        return (-1);
    }
    game->players[0] = player;

    int quit = 0;
    while (quit != -1) {

        start = SDL_GetTicks();
        draw_game(game);
        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > 3000 && game->players[0]->bomb == 1) {
            SDL_DestroyTexture(pSDL->textureBomb);
            pSDL->textureBomb = NULL;
            make_explosion(game);
            game->players[0]->bomb = 0;
            game->players[0]->explosion = 0;
            tempsPrecedent = tempsActuel;
            initBomb(pSDL);
            initExplosion(pSDL);
        }

        quit = game_event(game);

        if(1000 / FPSMeter > SDL_GetTicks() - start) {
            SDL_Delay(1000 / FPSMeter - (SDL_GetTicks() - start));
        }
    }

      //le jeu
//    bool terminer = false;
//    SDL_Event evenements;
//    while(!terminer) {
//
//          if (SDL_PollEvent(&evenements)) {
//              if (evenements.type == SDL_QUIT)
//                  terminer = true;
//          }
//          SDL_RenderCopy(pSDL->pRenderer, texture, NULL, &test.pos);
//          SDL_RenderPresent(pSDL->pRenderer);
//    }
//
//
//   TTF_CloseFont(police);
    // On libère la mémoire
    destroySDL(game->pSDL);
    free(game->players[0]);
    free(game);

    return EXIT_SUCCESS;
}
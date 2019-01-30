#include <stdbool.h>
#include "../header/game.h"


int main(int argc, char *argv[])
{
    // Initialisation du jeu
    sdl_t *pSDL = initSDL();
    player_t *player = initPlayer();
    game_t *game = initGame(pSDL);
    game->players[0] = player;
    int quit = 0;
    while (quit != -1) {
        draw_game(game);
        quit = game_event(game);

    }
    /*  //le jeu
      bool terminer = false;
      SDL_Event evenements;
      while(!terminer)
      {
          SDL_WaitEvent(&evenements);

          if(evenements.type == SDL_QUIT)
              terminer = true;
      }*/


    // On libère la mémoire
    destroySDL(pSDL);
    free(game->players[0]);
    free(game);

    return EXIT_SUCCESS;
}
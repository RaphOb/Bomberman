#include <stdbool.h>
#include "../header/SDLEngine.h"
#include "../header/file.h"


int main(int argc, char *argv[])
{
    // Initialisation du jeu
    game_t *game = initGame();
    sdl_t *pSDL = initSDL(game);
    displayTrump(pSDL, game);
    int quit = 0;
    while (quit != -1) {
        draw_game(pSDL, game);
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

    return EXIT_SUCCESS;
}
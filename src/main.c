#include <stdbool.h>
#include "../header/SDLEngine.h"
#include "../header/file.h"


int main(int argc, char *argv[])
{
    // Initialisation du jeu
    game_t *game = initGame();
    sdl_t *pSDL = initSDL(game);

    //le jeu
    bool terminer = false;
    SDL_Event evenements;
    while(!terminer)
    {
        SDL_PollEvent(&evenements);

        clear(pSDL->pRenderer);
        displayGame(game, pSDL);
        SDL_Log("aaa");
        if(evenements.type == SDL_QUIT)
            terminer = true;

    }


    // On libère la mémoire
    destroySDL(pSDL);

    return EXIT_SUCCESS;
}
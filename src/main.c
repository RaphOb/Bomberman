#include <stdbool.h>
#include "../header/game.h"
#include "../header/renderer.h"
#include "../header/menu.h"
#include "../header/input.h"



int main(int argc, char *argv[])
{
    // Initialisation du jeu
    Uint32 start;
    sdl_t *pSDL = initSDL();
    player_t *player = initPlayer();
    game_t *game = initGame(pSDL);
    if (!pSDL || !player || !game) {
        return (-1);
    }
    game->players[0] = player;

    int quit = 0;
    int menu = 0;
    int network = 0;
    int play = 0;
    // First menu
    while(menu == 0) {
        drawMenu(game->pSDL);
        menu = menuEvent();
    }
    // Menu Network
    while (menu != -1 && network == 0) {
        drawMenuNetwork(game->pSDL);
        network = menuNetworkEvent();

        // Input
        SDL_StartTextInput();
        if (network == 1) {
            play = loopInputConnect(game->pSDL);
        } else if (network == 2) {
            play = loopInputHost(game->pSDL);
        }
        SDL_StopTextInput();
    }

    // Game
    while (menu != -1 && quit != -1 && play == 1 && network != -1) {
        drawGame(game);
        start = SDL_GetTicks();
        quit = gameEvent(game);

        if(1000 / FPS > SDL_GetTicks() - start) {
            SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
        }
    }

    // On libère la mémoire
    destroySDL(pSDL);
    free(game->players[0]);
    free(game);

    return EXIT_SUCCESS;
}
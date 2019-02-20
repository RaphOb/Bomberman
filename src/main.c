#include <stdbool.h>
#include "../header/game.h"
#include "../header/renderer.h"
#include "../header/menu.h"
#include "../header/input.h"
#include "../header/serv.h"
#include "../header/client.h"


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
            pthread_t hebergement_thread;
            int ret_thread = pthread_create(&hebergement_thread, NULL, (void *(*)(void *)) app_serv, (void *) NULL);
            if (ret_thread != 0) {
                SDL_Log("thread server fail");
            } else {
                SDL_Log("creation reussie");
                init_client();
                init_co_from_cli_to_serv(NULL, NULL, NULL);
            }
        }
        SDL_StopTextInput();
    }

    // Game
    while (menu != -1 && quit != -1 && play == 1 && network != -1) {
        drawGame(game);
        start = SDL_GetTicks();
        fd_set readfs;
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = (int)start;
        quit = gameEvent(game);
        listen_server(1, timeout, readfs);

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
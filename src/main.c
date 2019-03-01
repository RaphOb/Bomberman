#include <stdbool.h>
#include "../header/game.h"
#include "../header/renderer.h"
#include "../header/menu.h"
#include "../header/input.h"
#include "../header/reseau.h"

static Server serv = { 0 };

int main(int argc, char *argv[])
{
    // Initialisation du jeu
    SDL_Log("argc: %d, argv : %s", argc, argv[0]);
    Uint32 start;
    sdl_t *pSDL = initSDL();
    player_t *player = initPlayer();
    player_t *player2 = initPlayer();
//    player_t *player3 = initPlayer();
//    player_t *player4 = initPlayer();
    game_t *game = initGame(pSDL);
    if (!pSDL || !player || !player2 || !game) {
        return (-1);
    }
    game->players[0] = player;
    game->players[1] = player2;

    int quit = 0;
    int menu = 0;
    int network = 0;
    int play = 0;
    fd_set readfs;
    struct timeval timeout;
    // First menu
    while(menu == 0) {
        drawMenu(game->pSDL);
        menu = menuEvent(game->pSDL);
    }
    // Menu Network
    while (menu != -1 && network == 0) {
        drawMenuNetwork(game->pSDL);
        network = menuNetworkEvent(game->pSDL);

        // Input
        SDL_StartTextInput();
        if (network == 1) {
            play = loopInputConnect(game->pSDL);
        } else if (network == 2) {
            char *port = malloc(sizeof(char) * 10);
            play = loopInputHost(game->pSDL, &port);
            serv.s_port = strdup(port);
            SDL_Log("set port : %s\n", port);
            if (play == 1) {
                pthread_t hebergement_thread;
                int ret_thread = pthread_create(&hebergement_thread, NULL, (void *) app_serv, (void *) serv.s_port);
                if (ret_thread != 0) {
                    SDL_Log("Thread server fail");
                } else {
                    SDL_Log("creation reussie");
                    SDL_Delay(500);
                    init();
                    init_co_from_cli_to_serv(NULL, serv.s_port, NULL);
                }
            }
        }
        SDL_StopTextInput();
    }

    // Game
    while (menu != -1 && quit != -1 && play == 1 && network != -1) {
        drawGame(game);
        start = SDL_GetTicks();
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
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
    player_t player = initPlayer();
    game_t *game = initGame(pSDL);
    if (!pSDL /*|| !player */|| !game) {
        return (-1);
    }
    game->players[0] = player;

    int quit = 0;
    int menu = 0;
    int network = 0;
    int play = 1;
    int song = 0;
    fd_set readfs;
    struct timeval timeout;
    int host = 0;
    // First menu
    while(menu == 0) {
//        if (song == 0) {
           song = playsound(POURLESRELOUXAUXGOUTSDEME_SOUND);
//        }
        drawMenu(game->pSDL);
        menu = menuEvent(game->pSDL, pSDL->son[0]);
    }
    SDL_CloseAudio();
//     Menu Network
    while (menu != -1 && network == 0) {
        drawMenuNetwork(game->pSDL);
        network = menuNetworkEvent(game->pSDL, pSDL->son[0]);

//         Input
        SDL_StartTextInput();
        if (network == 1) {
            play = loopInputConnect(game->pSDL);
            game->nb_client_serv = getNbClientServer();
        } else if (network == 2) {
            host = 1;
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
                    // Attente de la création du serveur
                    SDL_Delay(500);
                    init();
                    init_co_from_cli_to_serv(NULL, serv.s_port, NULL);
                    game->nb_client_serv = getNbClientServer();
                }
            }
        }
        SDL_StopTextInput();
    }

    // Game
    if (host == 1) {
        // Lancer la partie cote serveur
        SDL_Log("[Client] Signal debut de partie au serveur");
        c_emission(&game->players[0], 200);
    }
    pthread_t listen_server_thread;
    int ret_thread = pthread_create(&listen_server_thread, NULL, (void *) listen_server, (void *)(uintptr_t) game);
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
    //free(game->players[0]);
    free(game);

    return EXIT_SUCCESS;
}
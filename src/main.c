#include <stdbool.h>
#include <time.h>
#include "../header/game.h"
#include "../header/renderer.h"
#include "../header/menu.h"
#include "../header/input.h"
#include "../header/reseau.h"


static Server serv = { 0 };

int main(int argc, char *argv[]) {
    // Initialisation du jeu
    SDL_Log("argc: %d, argv : %s", argc, argv[0]);
    srand((unsigned) time(NULL));
    Uint32 start;
    sdl_t *pSDL = initSDL();
    game_t *game = initGame(pSDL);
    player_t player = initPlayer();
    if (!pSDL /*|| !player */|| !game) {
        return (-1);
    }
    //game->players[0] = player;

    int quit = 0;
    int menu = 0;
    int network = 0;
    int play = 0;
    int host = 0;
    pthread_t hebergement_thread;
    // First menu
    while (menu == 0) {

        playsound(TROPSTYLE2_SOUND);

        drawMenu(game->pSDL);
        menu = menuEvent(game->pSDL, pSDL->son[0]);
    }
  // SDL_CloseAudio();
//     Menu Network
    SDL_StartTextInput();
    while (menu != -1 && network == 0) {
        playsound(TROPSTYLE3_SOUND);
        drawMenuNetwork(game->pSDL);
        network = menuNetworkEvent(game->pSDL, pSDL->son[0]);
//         Input
        if (network == 1) {
            play = loopInputConnect(game->pSDL);
            getNbClientServer(game, &player);
        } else if (network == 2) {
            host = 1;
            char *port = malloc(sizeof(char) * 10);
            play = loopInputHost(game->pSDL, &port);
            serv.s_port = strdup(port);
            SDL_Log("set port : %s\n", port);
            if (play == 1) {
                int ret_thread = pthread_create(&hebergement_thread, NULL, (void *) app_serv, (void *) serv.s_port);
                if (ret_thread != 0) {
                    SDL_Log("Thread server fail");
                } else {
                    SDL_Log("creation reussie");
                    // Attente de la création du serveur
                    SDL_Delay(500);
                    init();
                    init_co_from_cli_to_serv(NULL, serv.s_port, NULL);
                    getNbClientServer(game, &player);
                }
            }
        }
    }
    SDL_StopTextInput();

    // Game
    if (host == 1) {
        // Lancer la partie cote serveur
        SDL_Log("[Client] Signal debut de partie au serveur");
        c_emission(&player, 200);
    }
    if (play == 1) {
        int ret_thread = pthread_create(&game->listen_serv_thread, NULL, (void *) listen_server, (void *) (uintptr_t) game);
    }
    while (menu != -1 && quit != -1 && play == 1 && network != -1) {
        playsound(POURLESRELOUXAUXGOUTSDEME_SOUND);
        drawGame(game);
        start = SDL_GetTicks();
        quit = gameEvent(game);

        if(1000 / FPS > SDL_GetTicks() - start) {
            SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
        }
    }

    if (play == 1) {
        SDL_Log("Waiting for listen server thread\n");
        pthread_join(game->listen_serv_thread, NULL);
        SDL_Log("Listen server thread is closed\n");
    }
    if (host == 1) {
        SDL_Log("Waiting for server thread\n");
        pthread_cancel(hebergement_thread);
        SDL_Log("Server thread is closed\n");
    }

    SDL_CloseAudio();

    // On libère la mémoire
    destroySDL(pSDL);
    free(game);

    return EXIT_SUCCESS;
}
#include <stdbool.h>
#include <time.h>
#include "../header/game.h"
#include "../header/renderer.h"
#include "../header/menu.h"
#include "../header/input.h"
#include "../header/reseau.h"

/* initialize state to random bits *//*
static unsigned long state[16];
*//* init should also reset this to 0 *//*
static unsigned int index = 0;
*//* return 32 bit random number *//*
unsigned long WELLRNG512(void)
{
    unsigned long a, b, c, d;
    a = state[index];
    c = state[(index+13)&15];
    b = a^c^(a<<16)^(c<<15);
    c = state[(index+9)&15];
    c ^= (c>>11);
    a = state[index] = b^c;
    d = a^((a<<5)&0xDA442D24UL);
    index = (index + 15)&15;
    a = state[index];
    state[index] = a^b^d^(a<<2)^(b<<18)^(c<<28);
    return state[index];
}*/

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
    pSDL->menu = 0;
    pSDL->network = 3;
    int play = 0;
    int host = 0;
    int lobby = 0;
    pthread_t hebergement_thread;
    // First menu
    while (pSDL->menu == 0) {

        while (pSDL->menu != -1 && pSDL->network == 3) {
            playsound(TROPSTYLE2_SOUND);
            drawMenu(game->pSDL);
            pSDL->menu = menuEvent(game->pSDL, pSDL->son[0]);
        }
        // SDL_CloseAudio();
//     Menu Network
        SDL_StartTextInput();

        while (pSDL->menu != -1 &&  pSDL->network == 0) {
            playsound(TROPSTYLE3_SOUND);
            drawMenuNetwork(game->pSDL);
            pSDL->network = menuNetworkEvent(game->pSDL, pSDL->son[0]);

            //         Input
            if (pSDL->network == 1) {
                play = loopInputConnect(game->pSDL);
                getNbClientServer(game, &player);
            } else if ( pSDL->network == 2) {
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

    while (pSDL->menu != -1 && pSDL->network != -1 && play == 1 && game->start == 0) {
        drawMenuLobby(game->pSDL, game->players, getMyPlayer(game)->host);
        lobby = menuLobbyEvent(game->pSDL, pSDL->son[0], getMyPlayer(game)->host);
        if (lobby == 1) {
            c_emission(&player, START_GAME);
            //Envoyer aux autres joueurs que la partie commence
        }
    }

    while (pSDL->menu != -1 && quit != -1 && play == 1 &&  pSDL->network != -1 && game->start == 1) {
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
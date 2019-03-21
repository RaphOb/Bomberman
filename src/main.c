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
//        if (song == 0) {
//        playsound(POURLESRELOUXAUXGOUTSDEME_SOUND);
//        }
//        int number =  rand() % 100;
//        if ( number < 3) {
//            playSound(pSDL->son[1]);
//            int frame = 0;
//            int dst_x = rand()% 700;
//            int dst_y = rand()% 700;
//            int currentTick = SDL_GetTicks();
//            for (int j = 1; j <= 4; j++) {
//                if (currentTick - game->players[0].bomb.tickExplosion > j * 200) frame = j;
//
//                SDL_Rect dst_mid = {dst_x + ((dst_y - REAL_BLOCK_SIZE) / 2),
//                                    dst_y + ((pSDL->dst_bomb.h - REAL_BLOCK_SIZE) / 2), REAL_BLOCK_SIZE,
//                                    REAL_BLOCK_SIZE};
//                SDL_Rect src = {0, 64 - frame * 16, 16, 16};
//                for (int i = 0; i < 7; i++) {
//
//                    SDL_Rect dst_right = {dst_mid.x + (  REAL_BLOCK_SIZE), dst_mid.y, REAL_BLOCK_SIZE,
//                                          REAL_BLOCK_SIZE};
//                    SDL_Rect dst_left = {dst_mid.x - (REAL_BLOCK_SIZE), dst_mid.y, REAL_BLOCK_SIZE,
//                                         REAL_BLOCK_SIZE};
//                    SDL_Rect dst_up = {dst_mid.x, dst_mid.y - (REAL_BLOCK_SIZE), REAL_BLOCK_SIZE,
//                                       REAL_BLOCK_SIZE};
//                    SDL_Rect dst_down = {dst_mid.x, dst_mid.y + (REAL_BLOCK_SIZE), REAL_BLOCK_SIZE,
//                                         REAL_BLOCK_SIZE};
//                    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[CENTERFLAME], &src, &dst_mid);
//                    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[RIGHTFLAME], &src, &dst_right);
//                    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[HORIZONTALFLAME], &src, &dst_right);
//                    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[VERTICALFLAME], &src, &dst_up);
//                    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[DOWNFLAME], &src, &dst_down);
//                    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureExplosion[LEFTFLAME], &src, &dst_left);
//    SDL_RenderPresent(game->pSDL->pRenderer);
//                }
//            }
//        }
        drawMenu(game->pSDL);
        menu = menuEvent(game->pSDL, pSDL->son[0]);
    }
//    SDL_CloseAudio();
//     Menu Network
    SDL_StartTextInput();
    while (menu != -1 && network == 0) {
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

    SDL_Log("Waiting for listen server thread\n");
    pthread_join(game->listen_serv_thread, NULL);
    SDL_Log("Listen server thread is closed\n");
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
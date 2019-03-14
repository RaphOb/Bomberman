#include <SDL2/SDL_log.h>
#include <math.h>
#include "../header/reseau.h"
#include "../header/game.h"

// ----- INITIALISATION -----
//void init_client(void)
//{
//#ifdef WIN32
//    WSADATA wsa;
//    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
//    if(err < 0)
//    {
//        puts("WSAStartup failed !");
//        exit(EXIT_FAILURE);
//    }
//#endif
//}
//
//static void end(void)
//{
//#ifdef WIN32
//    WSACleanup();
//#endif
//}

static Server serv = { 0 };

void init_co_from_cli_to_serv(char *ip, char *port, char *pseudo)
{
    SOCKET sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
//        SDL_Log("socket()");
        exit(errno);
    }

    SOCKADDR_IN to = { 0 }; /* initialise la structure avec des 0 */
//    int tosize = sizeof to;

    if (ip == NULL) {
        ip = strdup("127.0.0.1");
    }
    if (port == NULL) {
        port = strdup("1234");
    }
    if (pseudo == NULL) {
        pseudo = strdup("Host");
    }

//    SDL_Log("[Client] Connexion sur le port : %s\n", port);

    to.sin_addr.s_addr = inet_addr(ip);
    to.sin_port = htons((u_short) atoi(port)); /* on utilise htons pour le port */
    to.sin_family = AF_INET;

    if(connect(sock,(SOCKADDR *) &to, sizeof(to)) <0)
    {
//        SDL_Log("connect()");
        exit(errno);
    } else {
        serv.sock = sock;
        serv.to = to;
        serv.s_port = port;
    }
}

// ----- DIVERS -----
int getNbClientServer()
{
    char buffer[128] = {'\0'};
    SDL_Log("[Client] NB_CLIENT_SERVER_CODE from server...\n");
    if (recv(serv.sock, buffer, 10, MSG_WAITALL) == -1) {
        SDL_Log("recv()");
    } else {
        SDL_Log("[Client] nb_client_server set to : %d\n", atoi(buffer));
        return atoi(buffer);
    }
}

// ----- COMMUNICATION -----
int c_reception(int code, SOCKET serv_sock)
{
    switch (code) {
        case DISCONNECT_CODE:
//            SDL_Log("Disconnected by the server.\n");
            closesocket(serv_sock);
            return 0;
        default:
            return -1;
    }
}

void write_to_serv(t_client_request c_request)
{
    if(send(serv.sock, (char*)&c_request, sizeof(c_request), 0) < 0)
    {
//        SDL_Log("send()");
    }
}

void c_emission(player_t *player, int code)
{
    t_client_request c_request;
    c_request.x_pos = player->x_pos;
    c_request.y_pos = player->y_pos;
    c_request.dir = player->direction;
    switch (code) {
        case DISCONNECT_CODE:
            c_request.code_reseau = DISCONNECT_CODE;
            break;
        case UP_CODE:
            c_request.code_reseau = UP_CODE;
            break;
        case DOWN_CODE:
            c_request.code_reseau = DOWN_CODE;
            break;
        case LEFT_CODE:
            c_request.code_reseau = LEFT_CODE;
            break;
        case RIGHT_CODE:
            c_request.code_reseau = RIGHT_CODE;
            break;
        case 200:
            c_request.code_reseau = 200;
            break;
        default:
            c_request.code_reseau = 0;
            break;
    }
    write_to_serv(c_request);
}

void listen_server(void* g_param)
{
    SDL_Log("[Client] Listen server is ON");
    int n = 0;
    game_t *game = (game_t*)(uintptr_t)g_param;

    while(1) {
        FD_ZERO(&serv.readfs);
        FD_SET(serv.sock, &serv.readfs);

        game_t g = {0};

        select((int)serv.sock+1, &serv.readfs, NULL, NULL, NULL);

        if (FD_ISSET(serv.sock, &serv.readfs)) {
            if((n = recv((SOCKET)serv.sock, (char *)&g, sizeof(g), 0)) < 0)
            {
                SDL_Log("recv()");
            } else {
//                SDL_Log("[Client] Reception de données serveur...\n");
                // On s'assure que le joueur de ce client se trouve bien dans game.players[0]
                for (int i = 0; i < MAX_PLAYER ; i++) {
                    if (g.players[i].number > 0) {
                        //SDL_Log("g.players[i].number : %d", g.players[i].number);
                        if (game->nb_client_serv == g.players[i].number) {
                            maj_player(game, 0, &g.players[i]);
                        } else {
                            maj_player(game, i + 1, &g.players[i]);
                        }
                    }
                }
                c_reception(game->players[0].code_reseau, serv.sock);
            }
        }
    }
}

void maj_player(game_t *g, int indice, player_t *p)
{
    pthread_mutex_lock(&g->players[indice].mutex_player);
    //SDL_Log("g->players[indice].x_pos : %d", g->players[indice].x_pos);
    //SDL_Log("p->x_pos : %d", p->x_pos);
    if (g->players[indice].x_pos == 0) {
        g->players[indice].x_pos = p->x_pos;
    } else {
        double x = (p->x_pos * 100 / g->players[indice].x_pos) - 100;
        if (fabs(x) > 3 ) {
            g->players[indice].x_pos = p->x_pos;
        }
    }
    if (g->players[indice].y_pos == 0) {
        g->players[indice].y_pos = p->y_pos;
    } else {
        double y = (p->y_pos * 100 / g->players[indice].y_pos) - 100;
        if (fabs(y) > 3 ) {
            g->players[indice].y_pos = p->y_pos;
        }
    }
    g->players[indice].still = p->still;
    g->players[indice].alive = p->alive;
    g->players[indice].code_reseau = p->code_reseau;
    g->players[indice].direction = p->direction;
    g->players[indice].speed = p->speed;
    g->players[indice].number = p->number;
    pthread_mutex_unlock(&g->players[indice].mutex_player);
}
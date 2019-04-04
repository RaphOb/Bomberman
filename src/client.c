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
void getNbClientServer(game_t *g, player_t *p)
{
    char *buffer;
    int n;
    buffer = malloc(sizeof(char) * 2);
    SDL_Log("[Client] NB_CLIENT_SERVER_CODE from server...\n");
    if ((n = recv(serv.sock, buffer, sizeof(buffer), MSG_WAITALL)) == -1) {
        SDL_Log("recv()");
    } else {
        if (buffer != NULL) {
            g->nb_client_serv = atoi(buffer);
            player_t *myPlayer = getMyPlayer(g);
            myPlayer->alive = p->alive;
            myPlayer->code_reseau = p->code_reseau;
            myPlayer->co_is_ok = 1;
            myPlayer->number = g->nb_client_serv;
            myPlayer->checksum = p->checksum;
            myPlayer->speed = p->speed;
            myPlayer->bombPosed = p->bombPosed;
            myPlayer->nbBombe = p->nbBombe;
            myPlayer->frags = p->frags;
            myPlayer->current_frame = p->current_frame;
            myPlayer->frame_time = p->frame_time;
            myPlayer->still = p->still;
            myPlayer->mutex_player = p->mutex_player;
            setPlayerXYDir(myPlayer);
            strcpy(myPlayer->name, g->name);
            c_emission(myPlayer, 201);
        }
    }
}

// ----- COMMUNICATION -----
int c_reception(int code, SOCKET serv_sock)
{
    switch (code) {
        case DISCONNECT_CODE:
            SDL_Log("Disconnected by the server.\n");
            closesocket(serv_sock);
            return 0;
        default:
            return 1;
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
    //c_request.dir = player->direction;
//    c_request.still = player->still;
    c_request.speed = player->speed;
    strcpy(c_request.name, player->name);
    //c_request.nbBombe = player->nbBombe;
    c_request.alive = player->alive;
    c_request.co_is_ok = player->co_is_ok;
    switch (code) {
        case DISCONNECT_CODE:
            player->co_is_ok = 0;
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
        case BOMB_CODE:
            c_request.code_reseau = BOMB_CODE;
            c_request.command = 1;
            break;
        case CO_IS_OK:
            c_request.code_reseau = CO_IS_OK;
            break;
        case START_GAME :
            c_request.code_reseau = START_GAME;
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
    player_t *p = getMyPlayer(game);
    int run = 1;

    while(run) {
        FD_ZERO(&serv.readfs);
        FD_SET(serv.sock, &serv.readfs);

        game_t g;

        select((int)serv.sock+1, &serv.readfs, NULL, NULL, NULL);

        if (FD_ISSET(serv.sock, &serv.readfs)) {
            if((n = recv((SOCKET)serv.sock, (char *)&g, sizeof(g), 0)) < 0)
            {
                SDL_Log("[Client - listen_server] recv()");
                run =  c_reception(DISCONNECT_CODE, serv.sock);
            } else {
                // On s'assure que le joueur de ce client se trouve bien dans game.players[0]
                game->checksum_map = 0;
                for (int x = 0; x < 9; x++) {
                    for (int y = 0; y < 13; y++) {
                        game->checksum_map += g.map[x][y];
                    }
                }
                for (int i = 0; i < MAX_PLAYER ; i++) {
                    if (g.players[i].number >= 0 && g.players[i].checksum == sizeof(g.players[i])) {
                        game->start = g.start;
                        maj_player(game, g.players[i].number, &g.players[i]);
                        pthread_mutex_lock(&game->mutex_map);
                        if (game->checksum_map == g.checksum_map) {
                            for (int x = 0; x < 9; x++) {
                                for (int y = 0; y < 13; y++) {
                                    game->map[x][y] = g.map[x][y];
                                }
                            }
                        }
                        pthread_mutex_unlock(&game->mutex_map);
                    }
                }
                run =  c_reception(p->code_reseau, serv.sock);
            }
        }
    }
    pthread_cancel(game->listen_serv_thread);
}

void maj_player(game_t *g, int indice, player_t *p)
{
    pthread_mutex_lock(&g->players[indice].mutex_player);
    g->players[indice].x_pos = p->x_pos;
    g->players[indice].y_pos = p->y_pos;
    //SDL_Log("direction: %d", p->direction);
    g->players[indice].still = p->still;
    g->players[indice].code_reseau = p->code_reseau;
    g->players[indice].direction = p->direction;
    g->players[indice].speed = p->speed;
    g->players[indice].number = p->number;
    g->players[indice].alive = p->alive;
    g->players[indice].co_is_ok = p->co_is_ok;
    strcpy(g->players[indice].name, p->name);
    g->players[indice].host = p->host;
    // Bombe
    g->players[indice].bombPosed = p->bombPosed;
    g->players[indice].nbBombe = p->nbBombe;
    for (int j = 0; j < MAX_BOMBE; j++) {
        g->players[indice].bomb[j].pos_x = p->bomb[j].pos_x;
        g->players[indice].bomb[j].pos_y = p->bomb[j].pos_y;
        g->players[indice].bomb[j].cell_x = p->bomb[j].cell_x;
        g->players[indice].bomb[j].cell_y = p->bomb[j].cell_y;
        g->players[indice].bomb[j].range = p->bomb[j].range;
        g->players[indice].bomb[j].width = p->bomb[j].width;
        g->players[indice].bomb[j].height = p->bomb[j].height;
        g->players[indice].bomb[j].isPosed = p->bomb[j].isPosed;
        g->players[indice].bomb[j].tickBombDropped = p->bomb[j].tickBombDropped;
        g->players[indice].bomb[j].tickExplosion = p->bomb[j].tickExplosion;
        g->players[indice].bomb[j].explosion = p->bomb[j].explosion;
        g->players[indice].bomb[j].frame = p->bomb[j].frame;
    }
    //SDL_Log("g->players[indice].bomb[0].tickExplosion = %d\n", g->players[indice].bomb[0].tickExplosion);
    pthread_mutex_unlock(&g->players[indice].mutex_player);
}
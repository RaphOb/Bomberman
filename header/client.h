#ifndef BOMBERMAN_CLIENT_H
#define BOMBERMAN_CLIENT_H

#ifdef WIN32 /* si vous êtes sous Windows */

#include <winsock2.h>
#include <sys/types.h>
#include <unistd.h> /* close */

#elif defined (linux) /* si vous êtes sous Linux */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else /* sinon vous êtes sur une plateforme non supportée */

#error not defined for this platform

#endif

#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "player.h"
#include "game.h"

typedef struct {
    SOCKET sock;
    SOCKADDR_IN to;
    char *s_port;
    struct timeval timeout;
    fd_set readfs;
} Server;

typedef struct s_client_request
{
    int x_pos; /* La position x souhaitée par le client */
    int y_pos; /* La position y souhaitée par le client */
    int dir; /* La direction souhaitée par le client */
    int command; /* Une commande du client (0 : Ne rien faire / 1 : Poser une bombe) */
    int speed; /* La vitesse du joueur */
    int ckecksum; /* Un checksum simple */
    int code_reseau;
    int still;
    int range;
    char alive;
    int co_is_ok;
} t_client_request;

// ----- INITIALISATION -----
//void init_client(void);
//void end(void);
void init_co_from_cli_to_serv(char *ip, char *port, char *pseudo);
// ----- DIVERS -----
void getNbClientServer(game_t *g, player_t *p);
void maj_player(game_t *g, int indice, player_t *p);
// ----- COMMUNICATION -----
int c_reception(int code, SOCKET serv_sock);
void write_to_serv(t_client_request c_request);
void c_emission(player_t *player, int code);
void listen_server(void* game);

#endif //BOMBERMAN_CLIENT_H

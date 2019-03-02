#ifndef BOMBERMAN_SERV_H
#define BOMBERMAN_SERV_H

#ifdef WIN32 /* si vous êtes sous Windows */

#include <winsock2.h>
#include <sys/types.h>
#include <unistd.h> /* close */

#elif defined (linux) /* si vous êtes sous Linux */

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
#include <pthread.h>
#include <inttypes.h>
#include <stdlib.h>
#include "game.h"

// Structure concernant un client (spécifique au serveur)
typedef struct
{
    int num_client;
    pthread_t c_thread;
    pthread_mutex_t mutex_client;
    char name[1024];
    SOCKADDR_IN csin;
    player_t p;
}Client;

// ----- INITIALISATION -----
void init(void);
void end(void);
int init_co(char *port);
// ----- THREAD -----
void delete_one_thread(Client *c);
void delete_all_threads();
void wait_end_of_threads();
int into_thread(void* fd_client);
int game_thread();
// ----- SOCKET -----
void close_socket_client(Client *c);
void close_all_socket_clients();
// ----- CLIENTS -----
void disconnect_client(Client *c);
void disconnect_all_clients();
void delete_client(Client *c);
void delete_all_clients();
Client* get_client(int c);
int add_client(int s, SOCKADDR_IN csin);
void display_clients_co();
// ----- DIVERS -----
void set_pseudo(Client *c);
// ----- COMMUNICATION -----
void write_to_client(Client *c, int code);
void write_to_all_clients(int code);
//player_t *fillPlayerInfo(Client *c, int code);
void s_emission(Client *c, int code);
int s_reception(Client *c, t_client_request *c_request);
// ----- MAIN -----
int app_serv(void* struct_serv);

game_t init_game_server_side();

#define MAX_CLIENT 4

#endif //BOMBERMAN_SERV_H

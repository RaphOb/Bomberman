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

typedef struct
{
    int num_client;
    pthread_t c_thread;
    pthread_mutex_t mutex_client;
    char name[1024];
    SOCKADDR_IN csin;
}Client;

// ----- INITIALISATION -----
static void init(void);
static void end(void);
static int init_co(char *port);
// ----- THREAD -----
static void delete_one_thread(Client *c);
static void delete_all_threads();
static void wait_end_of_threads();
static void into_thread(void* fd_client);
// ----- SOCKET -----
static void close_socket_client(Client *c);
static void close_all_socket_clients();
// ----- CLIENTS -----
static void disconnect_client(Client *c);
static void disconnect_all_clients();
static void delete_client(Client *c);
static void delete_all_clients();
static Client* get_client(int c);
static int add_client(int s, SOCKADDR_IN csin);
static void display_clients_co();
// ----- DIVERS -----
static void set_pseudo(Client *c);
// ----- COMMUNICATION -----
static void write_code_to_client(Client *c, int code);
static void write_code_to_all_clients(int code);
static void s_emission(Client *c, int code);
static int s_reception(Client *c, char *buffer);
// ----- MAIN -----
int app_serv(void* struct_serv);

#define MAX_CLIENT 4

static Client clients[MAX_CLIENT] = { 0 };
static SOCKET sock;

#endif //BOMBERMAN_SERV_H

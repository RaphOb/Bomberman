#ifndef BOMBERMAN_CLIENT_H
#define BOMBERMAN_CLIENT_H

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
#include <inttypes.h>
#include <stdlib.h>

typedef struct {
    SOCKET sock;
    SOCKADDR_IN to;
    char *c_pseudo;
} Server;

// ----- INITIALISATION -----
void init_client(void);
static void end(void);
void init_co_from_cli_to_serv(char *ip, char *port, char *pseudo);
// ----- DIVERS -----
static void hello_cli_serv();
// ----- COMMUNICATION -----
static int c_reception(int code, SOCKET serv_sock);
static void write_to_serv(char *buffer, int from_keyboard);
static void write_code_to_server(int code);
void c_emission(int code);
int listen_server(int run, struct timeval timeout, fd_set readfs);
// ----- MAIN -----
int app_client();

#define DISCONNECT_CODE 10
#define OK_CODE 11
#define PSEUDO_CODE 20
#define UP_CODE 30
#define DOWN_CODE 31
#define LEFT_CODE 32
#define RIGHT_CODE 33
#define BOMB_CODE 34
#define CODE_SIZE 2+1

// Keyboard typing -> 1 else 0
#define KEYBOARD_TYPING_MODE 0

static Server serv = { 0 };

#endif //BOMBERMAN_CLIENT_H

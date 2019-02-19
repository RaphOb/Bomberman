#include <SDL_log.h>
#include "../header/client.h"

// ----- INITIALISATION -----
void init_client(void)
{
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

static void end(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}

void init_co_from_cli_to_serv(char *ip, char *port, char *pseudo)
{
    SOCKET sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        SDL_Log("socket()");
        exit(errno);
    }

    SOCKADDR_IN to = { 0 }; /* initialise la structure avec des 0 */
    int tosize = sizeof to;

    if (ip == NULL) {
        ip = strdup("127.0.0.1");
    }
    if (port == NULL) {
        port = strdup("1234");
    }
    if (pseudo == NULL) {
        pseudo = strdup("Host");
    }

    to.sin_addr.s_addr = inet_addr(ip);
    to.sin_port = htons((u_short) atoi(port)); /* on utilise htons pour le port */
    to.sin_family = AF_INET;

    if(connect(sock,(SOCKADDR *) &to, sizeof(to)) <0)
    {
        SDL_Log("connect()");
        exit(errno);
    } else {
        serv.sock = sock;
        serv.to = to;
        serv.c_pseudo = pseudo;
    }
    //hello_cli_serv();
    if (serv.c_pseudo[0] != '\0') {
        //c_emission(PSEUDO_CODE);
    }
}

// ----- DIVERS -----
static void hello_cli_serv()
{
    // On attend que le serveur envoie le code OK_CODE
    char buffer[128];
    SDL_Log("Waiting OK_CODE from server...\n");
    if (recv(serv.sock, buffer, 2, MSG_WAITALL) == -1) {
        SDL_Log("recv()");
    } else {
        c_reception((int)strtoimax(buffer, NULL, 10), serv.sock);
    }
}

// ----- COMMUNICATION -----
static int c_reception(int code, SOCKET serv_sock)
{
    switch (code) {
        case DISCONNECT_CODE:
            SDL_Log("Disconnected by the server.\n");
            closesocket(serv_sock);
            return 0;
            break;
        case OK_CODE:
            SDL_Log("OK\n");
            return 1;
            break;
        default:
            break;
    }
}

static void write_to_serv(char *buffer, int from_keyboard)
{
    if(send(serv.sock, buffer, (int) strlen(buffer)-from_keyboard, 0) < 0)
    {
        SDL_Log("send()");
    }
}

static void write_code_to_server(int code)
{
    char buffer[CODE_SIZE] = {'\0'};
    sprintf(buffer, "%d", code);
    write_to_serv(buffer, 0);
}

void c_emission(int code)
{
    switch (code) {
        case PSEUDO_CODE:
            write_code_to_server(PSEUDO_CODE);
            write_to_serv(serv.c_pseudo, 0);
            break;
        case UP_CODE:
            SDL_Log("Send up to server\n");
            write_code_to_server(UP_CODE);
            break;
        case DOWN_CODE:
            SDL_Log("Send down to server\n");
            write_code_to_server(DOWN_CODE);
            break;
        case LEFT_CODE:
            SDL_Log("Send left to server\n");
            write_code_to_server(LEFT_CODE);
            break;
        case RIGHT_CODE:
            SDL_Log("Send right to server\n");
            write_code_to_server(RIGHT_CODE);
            break;
        case BOMB_CODE:
            SDL_Log("Send bomb to server\n");
            write_code_to_server(BOMB_CODE);
            break;
        default:
            break;
    }
}

int listen_server(int run, struct timeval timeout, fd_set readfs)
{
    char buffer[128] = { '\0' };
    int n = 0;
    timeout.tv_sec = 0;
    timeout.tv_usec = 200;
    FD_ZERO(&readfs);
    FD_SET(serv.sock, &readfs);
    memset(buffer, '\0', 128);

    // Keyboard typing
    if (KEYBOARD_TYPING_MODE) {
        SDL_Log("type smt : \n");
        fgets(buffer, 128, stdin);
        write_to_serv(buffer, 1);
    }

    select((int)serv.sock+1, &readfs, NULL, NULL, &timeout);

    memset(buffer, '\0', 128);
    if (FD_ISSET(serv.sock, &readfs)) {
        if((n = recv((SOCKET)serv.sock, buffer, 128, 0)) < 0)
        {
            SDL_Log("recv()");
        } else {
            buffer[n] = 0;
            if (strlen(buffer) == 2) {
                return c_reception((int)strtoimax(buffer, NULL, 10), serv.sock);
            } else {
                SDL_Log("Reception d'un autre message : %s\n", buffer);
            }
        }
    }
}

int app_client()
{
    //init();
    char buffer[128];
    int n = 0;
    int run = 1;
    struct timeval timeout;

    char *pseudo = "test";

    //init_co_from_cli_to_serv(pseudo);

    fd_set readfs;
    int tosize = sizeof serv.to;

    while (run) {
        timeout.tv_sec = 0;
        timeout.tv_usec = 200;
        FD_ZERO(&readfs);
        FD_SET(serv.sock, &readfs);
        memset(buffer, '\0', 128);

        // Keyboard typing
        if (KEYBOARD_TYPING_MODE) {
            SDL_Log("type smt : \n");
            fgets(buffer, 128, stdin);
            write_to_serv(buffer, 1);
        }

        select((int)serv.sock+1, &readfs, NULL, NULL, &timeout);

        memset(buffer, '\0', 128);
        if (FD_ISSET(serv.sock, &readfs)) {
            if((n = recv((SOCKET)serv.sock, buffer, 128, 0)) < 0)
            {
                SDL_Log("recv()");
            } else {
                buffer[n] = 0;
                if (strlen(buffer) == 2) {
                    run = c_reception((int)strtoimax(buffer, NULL, 10), serv.sock);
                } else {
                    SDL_Log("Reception d'un autre message : %s\n", buffer);
                }
            }
        }
    }

    buffer[n] = '\0';
    closesocket(serv.sock);
    end();
    return 0;

}
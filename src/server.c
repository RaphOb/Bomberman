#include <SDL_log.h>
#include "../header/serv.h"


// ----- INITIALISATION -----
static void init(void)
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

static int init_co()
{
    SOCKADDR_IN sin = { 0 };
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        SDL_Log("socket()");
        exit(errno);
    }

    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(1234);

    if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
    {
        SDL_Log("bind()");
        exit(errno);
    }

    if(listen(sock, MAX_CLIENT) == SOCKET_ERROR)
    {
        SDL_Log("listen()");
        exit(errno);
    }

    return (int)sock;
}

// ----- THREAD -----
static void wait_end_of_threads()
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        pthread_join(clients[i].c_thread, NULL);
    }
}

static void delete_one_thread(Client *c)
{
    SDL_Log("Suppression du thread du client %d\n", c->num_client);
    pthread_cancel(c->c_thread);
}

static void delete_all_threads()
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        delete_one_thread(&clients[i]);
    }
}

// ----- SOCKET -----
static void close_all_socket_clients()
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        if (clients[i].num_client != 0) {
            close_socket_client(&clients[i]);
        }
    }
    display_clients_co();
}

static void close_socket_client(Client *c)
{
    if (closesocket((SOCKET)c->num_client) != 0) {
        SDL_Log("closesocket()");
    } else {
        SDL_Log("Socket fermee pour %d\n", c->num_client);
    }
}

// ----- CLIENTS -----
static void disconnect_all_clients()
{
    write_code_to_all_clients(DISCONNECT_CODE);
    close_all_socket_clients();
    delete_all_threads();
    delete_all_clients();
}

static void disconnect_client(Client *c)
{
    write_code_to_client(c, DISCONNECT_CODE);
    close_socket_client(c);
    delete_client(c);
}

static void delete_all_clients()
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        if (clients[i].num_client != 0) {
            delete_client(&clients[i]);
        }
    }
    display_clients_co();
}

static void delete_client(Client *c)
{
    SDL_Log("Client supprime pour %d\n", c->num_client);
    c->num_client = 0;
    strcpy(c->name, "\0");
}

static int add_client(int s, SOCKADDR_IN csin)
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        if (clients[i].num_client == 0) {
            clients[i].num_client = s;
            clients[i].csin = csin;
            clients[i].mutex_client = PTHREAD_MUTEX_INITIALIZER;
            display_clients_co();
            return 1;
        }
    }
    display_clients_co();
    return 0;
}

static Client* get_client(int c)
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        if (clients[i].num_client == c) {
            return (&clients[i]);
        }
    }
    return NULL;
}

static void display_clients_co()
{
    for (int i = 0 ; i < 4 ; i++) {
        if (clients[i].name[0] != '\0') {
            SDL_Log("client (%s) %d : %d\t", clients[i].name, i, clients[i].num_client);
        } else {
            SDL_Log("client %d : %d\t", i, clients[i].num_client);
        }
    }
    SDL_Log("\n");
}

// ----- DIVERS -----
static void set_pseudo(Client *c)
{
    char buffer[1024];
    memset(buffer, '\0', 1024);
    int n = 0;
    fd_set readfs;

    FD_ZERO(&readfs);
    FD_SET((SOCKET)c->num_client, &readfs);

    select(c->num_client+1, &readfs, NULL, NULL, NULL);

    if (FD_ISSET((SOCKET)c->num_client, &readfs)) {
        if((n = recv((SOCKET)c->num_client, buffer, 1024, 0)) < 0)
        {
            SDL_Log("recv()");
            //return -1;
        } else {
            buffer[n] = 0;
            strcpy(c->name, buffer);
        }
    }
}

// ----- COMMUNICATION -----
static void write_code_to_client(Client *c, int code)
{
    char buffer[CODE_SIZE] = {'\0'};
    sprintf(buffer, "%d", code);
    if(sendto((SOCKET)c->num_client, buffer, (int) strlen(buffer), 0, (SOCKADDR *) & c->csin, sizeof(c->csin)) < 0)
    {
        SDL_Log("sendto()");
    }
}

static void write_code_to_all_clients(int code)
{
    char buffer[CODE_SIZE];
    itoa(code, buffer, 10);
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        write_code_to_client(&clients[i], code);
    }
}

static void s_emission(Client *c, int code)
{
    switch (code) {
        case DISCONNECT_CODE:
            if (c == NULL) {
                disconnect_all_clients();
            } else {
                disconnect_client(c);
            }
            break;
        case OK_CODE:
            if (c == NULL) {
                write_code_to_all_clients(OK_CODE);
            } else {
                write_code_to_client(c, OK_CODE);
            }
            break;
        default:
            SDL_Log("Case doesnt exist for this code, aborted.\n");
            break;
    }
}

static int s_reception(Client *c, char *buffer)
{
    int code;

    SDL_Log("Client dit : %s\n", buffer);
    if (strlen(buffer) == 2) {
        code = (int)strtoimax(buffer, NULL, 10);
        switch (code) {
            case DISCONNECT_CODE:
                if (c == NULL) {
                    disconnect_all_clients();
                } else {
                    disconnect_client(c);
                    return 0;
                }
                display_clients_co();
                break;
            case PSEUDO_CODE:
                set_pseudo(c);
                display_clients_co();
                break;
            case UP_CODE:
                SDL_Log("Client %d : UP\n", c->num_client);
                break;
            case DOWN_CODE:
                SDL_Log("Client %d : DOWN\n", c->num_client);
                break;
            case LEFT_CODE:
                SDL_Log("Client %d : LEFT\n", c->num_client);
                break;
            case RIGHT_CODE:
                SDL_Log("Client %d : RIGHT\n", c->num_client);
                break;
            case BOMB_CODE:
                SDL_Log("Client %d : BOMB\n", c->num_client);
                break;
            default:
                SDL_Log("Case doesnt exist for this code, aborted.\n");
                break;
        }
    }
    s_emission(c, OK_CODE);
    return 1;
}

// ----- THREAD -----
static void into_thread(void* fd_client)
{
    fd_set readfs;
    int run = 1;
    int fd_client_int = (int) fd_client;

    pthread_mutex_lock(&clients->mutex_client);
    Client *c = get_client(fd_client_int);
    pthread_mutex_unlock(&clients->mutex_client);

    while(run) {
        char buffer[1024];
        memset(buffer, '\0', 1024);
        int n = 0;
        FD_ZERO(&readfs);
        FD_SET((SOCKET)c->num_client, &readfs);

        select(c->num_client+1, &readfs, NULL, NULL, NULL);

        if (FD_ISSET((SOCKET)c->num_client, &readfs)) {
            if((n = recv((SOCKET)c->num_client, buffer, 1024, 0)) < 0)
            {
                //SDL_Log("recv()");
                SDL_Log("Impossible de joindre le client\n");
                close_socket_client(c);
                delete_client(c);
                return;
            } else {
                buffer[n] = 0;
                pthread_mutex_lock(&clients->mutex_client);
                run = s_reception(c, buffer);
                pthread_mutex_unlock(&clients->mutex_client);
            }
        }
        SDL_Log("run = %d\n", run);
    }
}

// ----- MAIN -----
int app_serv(void)
{
    init();

    sock = (SOCKET)init_co();

    while (1) {
        SOCKADDR_IN csin = { 0 };
        int sinsize = sizeof(csin);
        SOCKET client = accept(sock, (struct sockaddr *)&csin, &sinsize);
        if (client < 0) {
            SDL_Log("accept()");
            return -1;
        }

        if (add_client((int)client, csin) == 0) {
            SDL_Log("Server is full.\n");
            closesocket(client);
        } else {
            SDL_Log("Creation du thread client.\n");
            int ret_thread = pthread_create(&get_client((int)client)->c_thread, NULL, (void *(*)(void *)) into_thread, (void *) client);
            if (ret_thread != 0) {
                SDL_Log("Echec de la création du thread, suppression du client.\n");
                delete_client(get_client((int)client));
            }
        }

        SDL_Log("loop\n");
    }
    wait_end_of_threads();
    close_all_socket_clients();
    closesocket(sock);
    end();
}
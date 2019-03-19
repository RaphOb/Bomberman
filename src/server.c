#include <SDL2/SDL_log.h>
#include <SDL_timer.h>
#include "../header/reseau.h"
#include "../header/game.h"

static Client clients[MAX_CLIENT] = { 0 };
static SOCKET sock;
game_serv_side g_serv_info;

// ----- INITIALISATION -----
void init(void)
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

void end(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}

int init_co(char *port)
{
    SOCKADDR_IN sin = { 0 };
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
//        SDL_Log("[Server] socket()");
        exit(errno);
    }
//    SDL_Log("[Server] Ecoute sur le port : %s\n", port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons((u_short) atoi(port));

    if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
    {
//        SDL_Log("[Server] bind()");
        exit(errno);
    }

    if(listen(sock, MAX_CLIENT) == SOCKET_ERROR)
    {
//        SDL_Log("[Server] listen()");
        exit(errno);
    }

    return (int)sock;
}

// ----- THREAD -----
void wait_end_of_threads()
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        pthread_join(clients[i].c_thread, NULL);
    }
}

void delete_one_thread(Client *c)
{
    if (c->num_client != -1) {
        SDL_Log("[Server (%d)] Suppression du thread du client %d\n", c->num_client, c->num_client);
        pthread_cancel(c->c_thread);
    }
}

void delete_all_threads()
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        delete_one_thread(&clients[i]);
    }
}

// ----- SOCKET -----
void close_all_socket_clients()
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        if (clients[i].num_client != -1) {
            close_socket_client(&clients[i]);
        }
    }
    display_clients_co();
}

void close_socket_client(Client *c)
{
    c->p.co_is_ok = 0;
    if (closesocket((SOCKET)c->num_client) != 0) {
        SDL_Log("[Server (%d)] closesocket()", c->num_client);
    } else {
        SDL_Log("[Server (%d)] Socket fermee pour %d\n", c->num_client, c->num_client);
    }
}

void close_game_thread()
{
    pthread_cancel(g_serv_info.g_thread);
}

// ----- CLIENTS -----

void disconnect_all_clients()
{
    write_to_all_clients(DISCONNECT_CODE);
    close_all_socket_clients();
    delete_all_threads();
    delete_all_clients();
}

void disconnect_client(Client *c)
{
    if (c->is_host == 1) {
        disconnect_all_clients();
    } else {
        write_to_client(c, DISCONNECT_CODE);
        close_socket_client(c);
        delete_client(c);
    }
}

void delete_all_clients()
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        if (clients[i].num_client != 0) {
            delete_client(&clients[i]);
        }
    }
    display_clients_co();
}

void delete_client(Client *c)
{
    SDL_Log("[Server (%d)] Client supprime pour %d\n", c->num_client, c->num_client);
    pthread_mutex_lock(&c->mutex_client);
    c->num_client = -1;
    c->p.number = -1;
    c->p.co_is_ok = 0;
    pthread_mutex_unlock(&c->mutex_client);
}

void init_all_clients()
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        clients[i].num_client = -1;
        clients[i].p.number = -1;
    }
}

int add_client(int s, SOCKADDR_IN csin)
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        if (clients[i].num_client == -1) {
            clients[i].num_client = s;
            clients[i].csin = csin;
            clients[i].p.number = i;
            clients[i].is_host = 0;
            clients[i].p.alive = 'Y';
            clients[i].p.co_is_ok = 0;
            clients[i].p.still = 1;
            clients[i].p.bomb.range = 1;
            switch (i) {
                case 0:
                    clients[i].p.x_pos = START_X_MAP;
                    clients[i].p.y_pos = START_Y_MAP;
                    clients[i].p.direction = 2;
                    break;
                case 1:
                    clients[i].p.x_pos = START_X_MAP;
                    clients[i].p.y_pos = START_Y_MAP;
                    clients[i].p.direction = 0;
                    break;
                case 2:
                    clients[i].p.x_pos = START_X_MAP;
                    clients[i].p.y_pos = START_Y_MAP;
                    clients[i].p.direction = 1;
                    break;
                case 3:
                    clients[i].p.x_pos = START_X_MAP;
                    clients[i].p.y_pos = START_Y_MAP;
                    clients[i].p.direction = 3;
                    break;
                default:
                    break;
            }
            pthread_mutex_init(&clients[i].mutex_client, NULL);
            display_clients_co();
            return 1;
        }
    }
    display_clients_co();
    return 0;
}

Client* get_client(int c)
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        if (clients[i].num_client == c) {
            return (&clients[i]);
        }
    }
    return NULL;
}

void display_clients_co()
{
    for (int i = 0 ; i < 4 ; i++) {
        if (clients[i].p.name != NULL) {
            SDL_Log("[Server (%d)] client (%s) %d : %d\t", clients[i].num_client, clients[i].p.name, i, clients[i].num_client);
        } else {
            SDL_Log("[Server (%d)] client %d : %d\t", i, clients[i].num_client, clients[i].num_client);
        }
    }
}

// ----- DIVERS -----

// ----- COMMUNICATION -----
void write_to_client(Client *c, int code)
{
    if (code == DISCONNECT_CODE) {
        c->p.alive = 'N';
        SDL_Delay(50);
    }
    if (c->num_client != -1 && c->p.co_is_ok == 1) {
        game_t g = init_game_server_side(code);
        if (sendto((SOCKET) c->num_client, (char *) &g, sizeof(g), 0, (SOCKADDR *) &c->csin, sizeof(c->csin)) < 0) {
            SDL_Log("[Server (%d)] sendto()", c->num_client);
        }
    }
}

void write_to_all_clients(int code)
{
    for (int i=0 ; i<MAX_CLIENT ; i++) {
        if (clients[i].num_client != -1 && clients[i].p.co_is_ok == 1) {
            write_to_client(&clients[i], code);
        }
    }
}

void s_emission(Client *c, int code)
{
    char *buffer;
    switch (code) {
        case DISCONNECT_CODE:
            if (c == NULL) {
                disconnect_all_clients();
            } else {
                disconnect_client(c);
            }
            break;
        case NB_CLIENT_SERV_CODE:
            // On envoi sa place dans le tableau - ce sera le numero commun entre le client et le serveur
            buffer = malloc(sizeof(char) * 2);
            sprintf(buffer, "%d", c->p.number);
            buffer[1]= '\0';
            //SDL_Log("c->num_client = %d\n", c->num_client);
            if(sendto((SOCKET)c->num_client, buffer, sizeof(buffer), 0, (SOCKADDR *) & c->csin, sizeof(c->csin)) < 0)
            {
                SDL_Log("[Server (%d)] NB_CLIENT_SERV_CODE : sendto()", c->num_client);
            }
            return;
            break;
        default:
            //SDL_Log("[Server] Emission : Case doesnt exist for this code, aborted.\n");
            break;
    }
    if (c == NULL) {
        write_to_all_clients(code);
    } else {
        write_to_client(c, code);
    }
}

game_t init_game_server_side(int code)
{
    game_t g;
    //useless for server side
    g.nb_client_serv = 0;

    for (int i = 0; i < MAX_CLIENT ; i++) {
        Client c = clients[i];
        g.players[i].x_pos = c.p.x_pos;
        g.players[i].y_pos = c.p.y_pos;
        g.players[i].direction = c.p.direction;
        g.players[i].number = c.p.number;
        g.players[i].alive = c.p.alive;
        g.players[i].co_is_ok = c.p.co_is_ok;
        g.players[i].speed = 1;
        g.players[i].code_reseau = code;
        g.players[i].checksum = sizeof(g.players[i]);
        g.players[i].still = c.p.still;
        g.players[i].bomb.range = c.p.bomb.range;
    }

    return g;
}

int s_reception(Client *c, t_client_request *c_request)
{
    int ret_thread;
    // Le code permet d'interpreter les donnees de la structure de façons différentes et d'en faire ce qu'on veut
    //SDL_Log("[Server] Client code : %d\n", c_request->code_reseau);
    c->p.x_pos = c_request->x_pos;
    c->p.y_pos = c_request->y_pos;
    c->p.direction = c_request->dir;
    c->p.still = c_request->still;
    c->p.alive = c_request->alive;
    switch (c_request->code_reseau) {
        case DISCONNECT_CODE:
            if (c == NULL) {
                //disconnect_all_clients();
            } else {
                disconnect_client(c);
                return 0;
            }
            display_clients_co();
            break;
        case UP_CODE:
            //SDL_Log("[Server] Client %d : UP\n", c->num_client);
            break;
        case DOWN_CODE:
            //SDL_Log("[Server] Client %d : DOWN\n", c->num_client);
            break;
        case LEFT_CODE:
            //SDL_Log("[Server] Client %d : LEFT\n", c->num_client);
            break;
        case RIGHT_CODE:
            //SDL_Log("[Server] Client %d : RIGHT\n", c->num_client);
            break;
        case BOMB_CODE:
            //SDL_Log("[Server] Client %d : BOMB\n", c->num_client);
            break;
        case CO_IS_OK:
            c->p.co_is_ok = 1;
            break;
        case 200:
            c->is_host = 1;
            ret_thread = pthread_create(&g_serv_info.g_thread, NULL, (void *) game_thread, NULL);
            break;
        default:
            //SDL_Log("[Server (%d)] Reception : Case doesnt exist for this code, aborted. Client num : %d\tCode : %d\n", c->num_client, c->num_client, c_request->code_reseau);
            break;
    }
    return 1;
}

// ----- THREAD -----
int game_thread()
{
    SDL_Log("[Server] Lancement de game thread\n");
    // Bloquer une variable -> change var avec un client qui balance un code
    while(1) {
        // NULL -> tous les clients ; 0 Pas de code particulier
        SDL_Delay(2);
        s_emission(NULL, 0);
    }
}

int into_thread(void* fd_client)
{
    fd_set readfs;
    int run = 1;
    int fd_client_int = (int) (uintptr_t)fd_client;

    pthread_mutex_lock(&clients->mutex_client);
    Client *c = get_client(fd_client_int);
    pthread_mutex_unlock(&clients->mutex_client);
    s_emission(c, NB_CLIENT_SERV_CODE);

    while(run) {
        int n = 0;
        FD_ZERO(&readfs);
        FD_SET((SOCKET)c->num_client, &readfs);
        t_client_request c_request = {0};

        select(c->num_client+1, &readfs, NULL, NULL, NULL);

        if (FD_ISSET((SOCKET)c->num_client, &readfs)) {
            if((n = recv((SOCKET)c->num_client, (char *)&c_request, sizeof(c_request), 0)) < 0)
            {
                SDL_Log("[Server (%d)] Impossible de joindre le client\n", c->num_client);
                close_socket_client(c);
                delete_client(c);
                delete_one_thread(c);
            } else {
                pthread_mutex_lock(&clients->mutex_client);
                run = s_reception(c, &c_request);
                pthread_mutex_unlock(&clients->mutex_client);
            }
        }
        //SDL_Log("[Server] run = %d\n", run);
    }
    delete_one_thread(c);
}

// ----- MAIN -----
int app_serv(void* serv_port)
{
    init();
    init_all_clients();

    char *port = strdup((char *)serv_port);

    sock = (SOCKET)init_co(port);

    while (1) {
        SOCKADDR_IN csin = { 0 };
        int sinsize = sizeof(csin);
//        SDL_Log("[Server] Attente d'un client...\n");
        SOCKET client = accept(sock, (struct sockaddr *)&csin, &sinsize);
        if (client == INVALID_SOCKET) {
//            SDL_Log("[Server] accept()");
            return -1;
        }

        if (add_client((int)client, csin) == 0) {
            SDL_Log("[Server] Server is full.\n");
            closesocket(client);
        } else {
            SDL_Log("[Server (%d)] Creation du thread client.\n", (int)client);
            int ret_thread = pthread_create(&get_client((int)client)->c_thread, NULL, (void *) into_thread, (void *) (uintptr_t) client);
            if (ret_thread != 0) {
                SDL_Log("[Server (%d)] Echec de la création du thread, suppression du client.\n", (int)client);
                delete_client(get_client((int)client));
            }
        }
    }
    wait_end_of_threads();
    close_all_socket_clients();
    closesocket(sock);
    end();
}
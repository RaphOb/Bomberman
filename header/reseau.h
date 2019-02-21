#ifndef BOMBERMAN_RESEAU_H
#define BOMBERMAN_RESEAU_H

#include "client.h"
#include "serv.h"

#define DISCONNECT_CODE 10
#define OK_CODE 11
#define PSEUDO_CODE 20
#define UP_CODE 30
#define DOWN_CODE 31
#define LEFT_CODE 32
#define RIGHT_CODE 33
#define BOMB_CODE 34
#define CODE_SIZE 2+1

static Server serv = { 0 };

#endif //BOMBERMAN_RESEAU_H

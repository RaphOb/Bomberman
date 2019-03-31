#ifndef BOMBERMAN_RESEAU_H
#define BOMBERMAN_RESEAU_H

#include "client.h"
#include "serv.h"
#include "bomb.h"
#include "bit.h"

#define DISCONNECT_CODE 10
#define OK_CODE 11
#define PSEUDO_CODE 20
#define UP_CODE 30
#define DOWN_CODE 31
#define LEFT_CODE 32
#define RIGHT_CODE 33
#define BOMB_CODE 34
#define CO_IS_OK 201
#define START_GAME 404
#define NB_CLIENT_SERV_CODE 111
#define CODE_SIZE (2+1)

#endif //BOMBERMAN_RESEAU_H

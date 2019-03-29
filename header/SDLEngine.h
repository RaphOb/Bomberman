//
// Created by geoff on 16/01/2019.
//

#ifndef BOMBERMAN_SDLENGINE_H
#define BOMBERMAN_SDLENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "../header/map.h"

#define BOMB_PNG_W 30
#define BOMB_PNG_H 32
#define WINDOW_WIDTH (START_X_BACKGROUND + MAP_SIZE_W)
#define WINDOW_HEIGHT (START_Y_BACKGROUND + MAP_SIZE_H)

//Path wav sound
#define EXPLOSION_SOUND     "../resources/sound/explosion.wav"
#define HOVER_SOUND         "../resources/sound/menu_hover.wav"
#define TROPSTYLE1_SOUND    "../resources/sound/Hotline-Miami-OST-Musikk-per-automatikk-_Elliott-Berlin_.wav"
#define TROPSTYLE2_SOUND    "../resources/sound/lastman-tv-series-original-soundtrack-preview.wav"
#define TROPSTYLE3_SOUND    "../resources/sound/M.O.O.N.-_Hydrogen_-_Hotline-Miami-Soundtrack_.wav"
#define POURLESRELOUXAUXGOUTSDEME_SOUND    "../resources/sound/Waterflame - Glorious morning.wav"

// variable declarations
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the music we have to play

// CENTERFLAME = 0, UPFLAME = 1, ...
enum Flame { CENTERFLAME, UPFLAME, DOWNFLAME, RIGHTFLAME, LEFTFLAME, HORIZONTALFLAME, VERTICALFLAME};

typedef struct son_s {
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_AudioDeviceID deviceId;

}son_t;

typedef struct button_s {
    SDL_Texture *textureButton[2];
    int hover;
    SDL_Rect dstRect;
} button_t;

struct sdl_s {
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    SDL_Texture *textureBackground[2];
    SDL_Texture *textureMap;
    SDL_Texture *textureBlock;
    SDL_Texture *texturePlayers[4];
    SDL_Rect dst_bomb[5];
    SDL_Texture *textureBomb;
    SDL_Texture *textureExplosion[7];
    button_t *buttonPlay;
    button_t *buttonQuit;
    button_t *buttonConnect;
    button_t *buttonHost;
    SDL_Texture *textureMenuLogo;
    SDL_Texture *textureMenuRetour;
    son_t* son[2];
    SDL_Texture *textureBonus[6];
    TTF_Font *font;
    int menu;
    int network;
};
typedef struct sdl_s sdl_t;

struct text_s {
    SDL_Rect pos;
    int length;
    char *text;
    SDL_Color color;
};
typedef struct text_s text_t;

sdl_t *initSDL();
void destroySDL(sdl_t *pSDL);
void initPlayerSDL(sdl_t *pSDL);
void initMap(sdl_t *pSDL);
void initBlock(sdl_t *pSDL);
void initBomb(sdl_t *pSDL);
void initExplosion(sdl_t *pSDL);
void initMenu(sdl_t *pSDL);
void initBonus(sdl_t *pSDL);
void initBackground(sdl_t *pSDL);
button_t *initButton(SDL_Rect rect, SDL_Texture *textureOn, SDL_Texture *textureOff);
void playSound(son_t* son);
son_t* initAudio(char* path);
void closeAudio(son_t* son);
int playsound(char* path);
void my_audio_callback(void *userdata, Uint8 *stream, int len);


#endif //BOMBERMAN_SDLENGINE_H
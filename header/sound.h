//
// Created by geoff on 03/04/2019.
//

#ifndef BOMBERMAN_SOUND_H
#define BOMBERMAN_SOUND_H

#include <SDL2/SDL.h>

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

typedef struct son_s {
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_AudioDeviceID deviceId;
} son_t;

void playSound(son_t* son);
son_t* initAudio(char* path);
void closeAudio(son_t* son);
int playMusic(char *path);
void my_audio_callback(void *userdata, Uint8 *stream, int len);

#endif //BOMBERMAN_SOUND_H

//
// Created by geoff on 03/04/2019.
//

#include "../header/sound.h"

void closeAudio(son_t* son)
{
    SDL_CloseAudioDevice(son->deviceId);
}

son_t* initAudio(char* path)
{
    son_t* son = malloc(sizeof(son_t));
    if (!son) {
        return NULL ;
    }
    SDL_LoadWAV(path, &son->wavSpec, &son->wavBuffer, &son->wavLength);
    son->deviceId = SDL_OpenAudioDevice(NULL, 0, &son->wavSpec, NULL, 0);
    return son;
}
/**
 * functin :Play sound
 * @param path
 */
void playSound(son_t* son)
{


    SDL_QueueAudio(son->deviceId, son->wavBuffer, son->wavLength);
    SDL_PauseAudioDevice(son->deviceId, 0);

    if (SDL_GetQueuedAudioSize(son->deviceId) == 0) {
        SDL_FreeWAV(son->wavBuffer);
    }
}

/**
 * function : play music
 * @param path
 * @return
 */
int playMusic(char *path)
{

    static Uint32 wav_length = 0;
    static Uint8 *wav_buffer;
    static SDL_AudioSpec wav_spec;

//    if (audio_len == 0) {
//        return 1;
//    }
    if (audio_len == 0) {
        SDL_CloseAudio();
//        SDL_FreeWAV(wav_buffer);
        if (SDL_LoadWAV(path, &wav_spec, &wav_buffer, &wav_length) == NULL) {
            return 1;
        }

        wav_spec.callback = my_audio_callback;
        wav_spec.userdata = NULL;

        audio_pos = wav_buffer;
        audio_len = wav_length;

        if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
            SDL_Log("Couldn't open audio: %s\n", SDL_GetError());
            exit(-1);
        }
        SDL_PauseAudio(0);
    }

    return 0;
}

/**
 * function : callback function
 * @param userdata
 * @param stream
 * @param len
 */
void my_audio_callback(void *userdata, Uint8 *stream, int len) {

//    if (audio_len ==0)
//        return;
    if (audio_len != 0) {
        len = ((Uint32) len > audio_len ? audio_len : (Uint32) len);
        SDL_memcpy(stream, audio_pos, (Uint32) len);
        SDL_MixAudio(stream, audio_pos, (Uint32) len, SDL_MIX_MAXVOLUME / 4);

        audio_pos += len;
        audio_len -= len;
    }
}

//
// Created by geoff on 18/02/2019.
//

#ifndef BOMBERMAN_INPUT_H
#define BOMBERMAN_INPUT_H

#include "../header/SDLEngine.h"
#include "../header/renderer.h"

typedef struct input_s {
    char *str;
    size_t len;
    SDL_Color color;
    TTF_Font *font;
    SDL_Texture *textureMsgDisplayed;
    SDL_Surface *inputSurface;
    SDL_Texture *inputTexture;
} input_t;

int loopInput(sdl_t *pSDL);
void renderStringIp(SDL_Renderer *pRenderer, SDL_Texture *textureIp);
void renderStringPort(SDL_Renderer *pRenderer, SDL_Texture *texturePort);
SDL_Texture *createTexturePort(SDL_Renderer *pRenderer, TTF_Font *font, SDL_Color color);
SDL_Texture *createTextureIp(SDL_Renderer *pRenderer, TTF_Font *font, SDL_Color color);
void renderInput(SDL_Rect rect, sdl_t *pSDL, input_t *input);
input_t *initInput(TTF_Font *font, SDL_Color color, SDL_Texture *msgDisplayed);
void destroyInput(input_t *input);
int manageInput(input_t *input, sdl_t *pSDL);

#endif //BOMBERMAN_INPUT_H

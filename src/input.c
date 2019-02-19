//
// Created by geoff on 18/02/2019.
//

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <SDL_events.h>
#include <SDL_log.h>
#include <SDL_ttf.h>
#include "../header/input.h"
#define LEN_MAX 80

int loopInput(sdl_t *pSDL)
{
    SDL_Rect textRectIp = {500, 400, 0, 0};
    SDL_Rect textRectPort = {580, 450, 0, 0};
    TTF_Font *font = TTF_OpenFont("../resources/font/Pixeled.ttf", 20);
    SDL_Color color = {255, 255, 255, 255};
    SDL_Texture *textureIp = NULL;
    textureIp = createTextureIp(pSDL->pRenderer, font, color);
    SDL_Texture *texturePort = NULL;
    texturePort = createTexturePort(pSDL->pRenderer, font, color);

    input_t *ip = initInput(font, color, textureIp);
    input_t *port = initInput(font, color, texturePort);

    int quit = 0;

    while (quit != -1 && quit != 2) {
        SDL_RenderClear(pSDL->pRenderer);
        renderMenuIp(pSDL);
        renderStringIp(pSDL->pRenderer, ip->textureMsgDisplayed);
        renderStringPort(pSDL->pRenderer, port->textureMsgDisplayed);
        renderInput(textRectPort, pSDL, port);
        renderInput(textRectIp, pSDL, ip);
        if (quit == 1) {
            quit = manageInput(port, pSDL);
        } else {
            quit = manageInput(ip, pSDL);
        }
        SDL_RenderPresent(pSDL->pRenderer);
    }

    destroyInput(ip);
    destroyInput(port);
    TTF_CloseFont(font);
    return (quit == -1) ? 0 : 1;
}

int manageInput(input_t *input, sdl_t *pSDL)
{
    SDL_Event event;
    static int quit = 0;
    int renderText = 0;

    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT)
        quit = -1;
    else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && input->len) {
            input->str[input->len - 1] = '\0';
            input->len--;
            renderText = 1;
        } else if (event.key.keysym.sym == SDLK_RETURN) {
            quit += 1;
        } else if (event.key.keysym.sym == SDLK_ESCAPE) {
            quit = -1;
        }
    } else if (event.type == SDL_TEXTINPUT) {
        input->len += strlen(event.text.text);;
        strcat(input->str, event.text.text);
        renderText = 1;
    }

    if (renderText) {
    }
    return quit;
}


void renderStringIp(SDL_Renderer *pRenderer, SDL_Texture *textureIp)
{
    SDL_Rect dst_ip = {300, 400, 0, 0};
    SDL_QueryTexture(textureIp, NULL, NULL, &dst_ip.w, &dst_ip.h);
    SDL_RenderCopy(pRenderer, textureIp, NULL, &dst_ip);
}

void renderStringPort(SDL_Renderer *pRenderer, SDL_Texture *texturePort)
{
    SDL_Rect dst_port = {300, 450, 0, 0};
    SDL_QueryTexture(texturePort, NULL, NULL, &dst_port.w, &dst_port.h);
    SDL_RenderCopy(pRenderer, texturePort, NULL, &dst_port);
}

void renderInput(SDL_Rect textRect, sdl_t *pSDL, input_t *input)
{
    SDL_Rect dst_menuLogo = {(MAP_SIZE_W / 2) - (750 / 2), 20, 750, 250};

    input->inputSurface = TTF_RenderText_Solid(input->font, input->str, input->color);
    input->inputTexture = SDL_CreateTextureFromSurface(pSDL->pRenderer, input->inputSurface);
    SDL_QueryTexture(input->inputTexture, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(pSDL->pRenderer, input->inputTexture, NULL, &textRect);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuLogo, NULL, &dst_menuLogo);
}


SDL_Texture *createTexturePort(SDL_Renderer *pRenderer, TTF_Font *font, SDL_Color color)
{
    SDL_Surface *surfaceStringPort = TTF_RenderText_Solid(font, "ENTREZ LE PORT: ", color);
    SDL_Texture *textureStringPort = SDL_CreateTextureFromSurface(pRenderer, surfaceStringPort);
    SDL_FreeSurface(surfaceStringPort);
    return textureStringPort;
}

SDL_Texture *createTextureIp(SDL_Renderer *pRenderer, TTF_Font *font, SDL_Color color)
{
    SDL_Surface *surfaceStringIp = TTF_RenderText_Solid(font, "ENTREZ L'IP: ", color);
    SDL_Texture *textureStringIp = SDL_CreateTextureFromSurface(pRenderer, surfaceStringIp);
    SDL_FreeSurface(surfaceStringIp);
    return textureStringIp;
}

input_t *initInput(TTF_Font *font, SDL_Color color, SDL_Texture *msgDisplayed)
{
    input_t *input = malloc(sizeof(input_t));
    if (!input)
        return NULL;

    input->font = font;
    input->color = color;
    input->textureMsgDisplayed = msgDisplayed;
    input->str = malloc(sizeof(char) * 30);
    input->len = 0;

}

void destroyInput(input_t *input)
{
    SDL_FreeSurface(input->inputSurface);
    SDL_DestroyTexture(input->textureMsgDisplayed);
    SDL_DestroyTexture(input->inputTexture);
    free(input->str);
    free(input);
}
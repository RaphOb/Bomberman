//
// Created by geoff on 18/02/2019.
//

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_ttf.h>
#include "../header/input.h"
#include "../header/reseau.h"
#include "../header/menu.h"
#define LEN_MAX 80

/**
 *
 * @param pSDL
 * @return
 */
int loopInputConnect(game_t *game)
{
    sdl_t *pSDL = game->pSDL;
    SDL_Rect textRectPseudo = {440, 500, 0, 0};
    SDL_Rect textRectIp = {510, 400, 0, 0};
    SDL_Rect textRectPort = {580, 450, 0, 0};
    SDL_Rect dstStringIp = {300, 400, 0, 0};
    SDL_Rect dstStringPort = {300, 450, 0, 0};
    SDL_Rect dstStringPseudo = {300, 500, 0, 0};
    TTF_Font *font = TTF_OpenFont("../resources/font/Pixeled.ttf", 20);
    SDL_Color color = {255, 255, 255, 255};
    SDL_Texture *textureIp = createTextureText(pSDL->pRenderer, font, color, "ENTREZ L'IP: ");
    SDL_Texture *texturePort = createTextureText(pSDL->pRenderer, font, color, "ENTREZ LE PORT: ");
    SDL_Texture *texturePseudo = createTextureText(pSDL->pRenderer, font, color, "PSEUDO: ");

    input_t *ip = initInput(font, color, textureIp);
    input_t *port = initInput(font, color, texturePort);
    input_t *pseudo = initInput(font, color, texturePseudo);

    int quit = 0;
    while (quit != -1 && quit != 3) {
        SDL_RenderClear(pSDL->pRenderer);
        renderBackgroundMenu(pSDL, 1);
        renderStringText(pSDL->pRenderer, ip->textureMsgDisplayed, dstStringIp);
        renderStringText(pSDL->pRenderer, port->textureMsgDisplayed, dstStringPort);
        renderStringText(pSDL->pRenderer, pseudo->textureMsgDisplayed, dstStringPseudo);
        renderInput(textRectPort, pSDL, port);
        renderInput(textRectIp, pSDL, ip);
        renderInput(textRectPseudo, pSDL, pseudo);
        SDL_Rect dst_menu_retour = {20, 550, 350, 350};
        SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuRetour, NULL, &dst_menu_retour);

        if (quit == 0) {
            quit = manageInput(ip, pSDL);
        } else if (quit == 1) {
            quit = manageInput(port, pSDL);
        } else {
            quit = manageInput(pseudo, pSDL);
        }
        SDL_RenderPresent(pSDL->pRenderer);
    }
    if (quit == 3) {
        init();
        strcpy(game->name, pseudo->str);
        init_co_from_cli_to_serv(ip->str, port->str, pseudo->str);
    }
    destroyInput(ip);
    destroyInput(port);
    destroyInput(pseudo);
    TTF_CloseFont(font);
    return (quit == -1) ? 0 : 1;
}


/**
 *
 * @param pSDL
 * @param p
 * @return
 */
int loopInputHost(game_t *game, char **p)
{
    sdl_t *pSDL = game->pSDL;
    TTF_Font *font = TTF_OpenFont("../resources/font/Pixeled.ttf", 20);
    SDL_Color color = {255, 255, 255, 255};
    SDL_Rect dstStringPseudo = {400, 450, 0, 0};
    SDL_Rect textRectPseudo = {540, 450, 0, 0};
    SDL_Rect textRectPort = {680, 400, 0, 0};
    SDL_Rect dstStringPort = {400, 400, 0, 0};
    SDL_Texture *texturePort = createTextureText(pSDL->pRenderer, font, color, "ENTREZ LE PORT: ");
    SDL_Texture *texturePseudo = createTextureText(pSDL->pRenderer, font, color, "PSEUDO: ");


    input_t *pseudo = initInput(font, color, texturePseudo);
    input_t *port = initInput(font, color, texturePort);


    int quit = 0;
    while (quit != -1 && quit != 2) {
        SDL_RenderClear(pSDL->pRenderer);
        renderBackgroundMenu(pSDL, 1);
        renderStringText(pSDL->pRenderer, pseudo->textureMsgDisplayed, dstStringPseudo);
        renderStringText(pSDL->pRenderer, port->textureMsgDisplayed, dstStringPort);
        renderInput(textRectPort, pSDL, port);
        renderInput(textRectPseudo, pSDL, pseudo);


        if (quit == 0) {
            quit = manageInput(port, pSDL);
        } else if (quit == 1) {
            quit = manageInput(pseudo, pSDL);
        }
        SDL_Rect dst_menu_retour = {20, 550, 350, 350};
        SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuRetour, NULL, &dst_menu_retour);
        SDL_RenderPresent(pSDL->pRenderer);
    }

    if (quit == 2) {
        *p = strdup(port->str);
        strcpy(game->name, pseudo->str);
    }

    destroyInput(pseudo);
    destroyInput(port);
    TTF_CloseFont(font);
    return (quit == -1) ? 0 : 1;

}

/**
 * function : Manage the events from the player on an input.
 * @param input
 * @return
 */
int manageInput(input_t *input, sdl_t* pSDL)
{
    SDL_Event event;
    static int quit = 0;

    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT)
        quit = -1;
    else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && input->len) {
            input->str[input->len - 1] = '\0';
            input->len--;
        } else if (event.key.keysym.sym == SDLK_RETURN) {
            quit += 1;
        } else if (event.key.keysym.sym == SDLK_ESCAPE) {
            quit = -1;
        }
    } else if (event.type == SDL_TEXTINPUT) {
        input->len += strlen(event.text.text);
        strcat(input->str, event.text.text);
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.x > 20 &&  event.button.x < 20 + 250 &&  event.button.y > 650 &&  event.button.y < 550 + 350) {
            pSDL->network = 0;
            quit = -1;
            SDL_Log("Petit bug ici encore");
        }
    }

    return quit;
}

/**
 * function : Render a text
 * @param pRenderer
 * @param textureText
 * @param dst
 */
void renderStringText(SDL_Renderer *pRenderer, SDL_Texture *textureText, SDL_Rect dst)
{
    SDL_QueryTexture(textureText, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(pRenderer, textureText, NULL, &dst);
}

/**
 * function : Render an input
 * @param textRect
 * @param pSDL
 * @param input
 */
void renderInput(SDL_Rect textRect, sdl_t *pSDL, input_t *input)
{
    SDL_Rect dst_menuLogo = {(MAP_SIZE_W / 2) - (IMG_LOGO_W / 2), 20, IMG_LOGO_W, IMG_LOGO_H};

    input->inputSurface = TTF_RenderText_Solid(input->font, input->str, input->color);
    input->inputTexture = SDL_CreateTextureFromSurface(pSDL->pRenderer, input->inputSurface);
    SDL_QueryTexture(input->inputTexture, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(pSDL->pRenderer, input->inputTexture, NULL, &textRect);
    SDL_RenderCopy(pSDL->pRenderer, pSDL->textureMenuLogo, NULL, &dst_menuLogo);
}

/**
 * function : Create a texture for a text
 * @param pRenderer
 * @param font
 * @param color
 * @param str
 * @return
 */
SDL_Texture *createTextureText(SDL_Renderer *pRenderer, TTF_Font *font, SDL_Color color, char *str)
{
    SDL_Surface *surfaceText = TTF_RenderText_Solid(font, str, color);
    SDL_Texture *textureText = SDL_CreateTextureFromSurface(pRenderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    return textureText;
}

/**
 * function : Create a input_t variable
 * @param font
 * @param color
 * @param msgDisplayed
 * @return
 */
input_t *initInput(TTF_Font *font, SDL_Color color, SDL_Texture *msgDisplayed)
{
    input_t *input = malloc(sizeof(input_t));
    if (!input)
        return NULL;

    input->font = font;
    input->color = color;
    input->textureMsgDisplayed = msgDisplayed;
    input->str = malloc(sizeof(char) * 30);
    input->str[0] = '\0';
    input->len = 0;

    return input;
}

/**
 * function : Free all the resources used for the input
 * @param input
 */
void destroyInput(input_t *input)
{
    SDL_FreeSurface(input->inputSurface);
    SDL_DestroyTexture(input->textureMsgDisplayed);
    SDL_DestroyTexture(input->inputTexture);
    free(input->str);
    free(input);
}
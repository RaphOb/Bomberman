//
// Created by geoff on 16/01/2019.
//

#include <math.h>
#include "../header/SDLEngine.h"
#include "../header/map.h"
#include "../header/player.h"
#include "../header/menu.h"

/**
 * function : init SDL map and texture
 * @param game
 * @return
 */
sdl_t *initSDL()
{
    sdl_t *pSDL = malloc(sizeof(sdl_t));
    if (!pSDL) {
        return (NULL);
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        destroySDL(pSDL);
        return NULL;
    }

    if(TTF_Init() == -1) {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        destroySDL(pSDL);
        return NULL;
    }

    pSDL->pWindow = SDL_CreateWindow("Bomberman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAP_SIZE_W,
                                     MAP_SIZE_H, 0);
    if (pSDL->pWindow == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        destroySDL(pSDL);
        return NULL;
    }
    pSDL->pRenderer = SDL_CreateRenderer(pSDL->pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (pSDL->pRenderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        destroySDL(pSDL);
        return NULL;
    }

    initPlayerSDL(pSDL);
    initMap(pSDL);
    initBlock(pSDL);
    initBomb(pSDL);
    initExplosion(pSDL);
    initMenu(pSDL);
    return pSDL;
}

/**
 * functin :Play sound
 * @param path
 */
void playSound(char* path)
{
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;

    SDL_LoadWAV(path, &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);

    if (SDL_GetQueuedAudioSize(deviceId) == 0) {
        SDL_Log("hello");
       // SDL_CloseAudioDevice(deviceId);
        SDL_FreeWAV(wavBuffer);
    }
}

/**
 * function: destroy SDL
 * @param pSDL
 * @param game
 */
void destroySDL(sdl_t *pSDL)
{
    if (pSDL->buttonConnect->textureButton[0]) {
        SDL_DestroyTexture(pSDL->buttonConnect->textureButton[0]);
        pSDL->buttonConnect->textureButton[0] = NULL;
    }
    if (pSDL->buttonHost->textureButton[0]) {
        SDL_DestroyTexture(pSDL->buttonHost->textureButton[0]);
        pSDL->buttonHost->textureButton[0] = NULL;
    }
    if (pSDL->buttonQuit->textureButton[0]) {
        SDL_DestroyTexture(pSDL->buttonQuit->textureButton[0]);
        pSDL->buttonQuit->textureButton[0] = NULL;
    }
    if (pSDL->buttonPlay->textureButton[0]) {
        SDL_DestroyTexture(pSDL->buttonPlay->textureButton[0]);
        pSDL->buttonPlay->textureButton[0] = NULL;
    }
    if (pSDL->textureMenuLogo) {
        SDL_DestroyTexture(pSDL->textureMenuLogo);
        pSDL->textureMenuLogo = NULL;
    }
    if (pSDL->buttonPlay->textureButton[1]) {
        SDL_DestroyTexture(pSDL->buttonPlay->textureButton[1]);
        pSDL->buttonPlay->textureButton[1] = NULL;
    }
    if (pSDL->buttonQuit->textureButton[1]) {
        SDL_DestroyTexture(pSDL->buttonQuit->textureButton[1]);
        pSDL->buttonQuit->textureButton[1] = NULL;
    }
    if (pSDL->buttonConnect->textureButton[1]) {
        SDL_DestroyTexture(pSDL->buttonConnect->textureButton[1]);
        pSDL->buttonConnect->textureButton[1] = NULL;
    }
    if (pSDL->buttonHost->textureButton[1]) {
        SDL_DestroyTexture(pSDL->buttonHost->textureButton[1]);
        pSDL->buttonHost->textureButton[1] = NULL;
    }
    for (int i = 0; i < 7; i++) {
        if (pSDL->textureExplosion2[i]) {
            SDL_DestroyTexture(pSDL->textureExplosion2[i]);
            pSDL->textureExplosion2[i] = NULL;
        }
    }
    if (pSDL->textureBomb) {
        SDL_DestroyTexture(pSDL->textureBomb);
        pSDL->textureBomb = NULL;
    }
    if (pSDL->textureBlock) {
        SDL_DestroyTexture(pSDL->textureBlock);
        pSDL->textureBlock = NULL;
    }
    if (pSDL->textureMap) {
        SDL_DestroyTexture(pSDL->textureMap);
        pSDL->textureMap = NULL;
    }
    if (pSDL->texturePlayers[0]) {
        SDL_DestroyTexture(pSDL->texturePlayers[0]);
        pSDL->texturePlayers[0] = NULL;
    }
    if (pSDL->texturePlayers[1]) {
        SDL_DestroyTexture(pSDL->texturePlayers[1]);
        pSDL->texturePlayers[1] = NULL;
    }
    if (pSDL->pRenderer) {
        SDL_DestroyRenderer(pSDL->pRenderer);
        pSDL->pRenderer = NULL;
    }
    if (pSDL->pWindow) {
        SDL_DestroyWindow(pSDL->pWindow);
        pSDL-> pWindow = NULL;
    }

    TTF_Quit();
    SDL_Quit();
    free(pSDL->buttonPlay);
    free(pSDL->buttonQuit);
    free(pSDL->buttonHost);
    free(pSDL->buttonConnect);
    free(pSDL);
    SDL_Log("Destroy SDL");
}

/**
 * function : Init textures menu
 * @param pSDL
 */
void initMenu(sdl_t *pSDL)
{
    SDL_Surface *menuSelectionJouerOff =IMG_Load("../resources/img/Menu_Selection_jouerOff.png");
    SDL_Surface *menuSelectionJouerOn =IMG_Load("../resources/img/Menu_Selection_jouerOn.png");
    SDL_Surface *menuSelectionQuitOff =IMG_Load("../resources/img/Menu_Selection_quitOff.png");
    SDL_Surface *menuSelectionQuitOn =IMG_Load("../resources/img/Menu_Selection_quitOn.png");
    SDL_Surface *menuHeberger =IMG_Load("../resources/img/Menu_hebergerOff.png");
    SDL_Surface *menuHebergerOn =IMG_Load("../resources/img/Menu_hebergerOn.png");
    SDL_Surface *menuSeconnecter =IMG_Load("../resources/img/Menu_seconnecterOff.png");
    SDL_Surface *menuSeconnecterOn =IMG_Load("../resources/img/Menu_seconnecterOn.png");
    SDL_Surface *menuLogo =IMG_Load("../resources/img/B_Logo.png");

    if (!(menuSelectionJouerOff || menuSelectionJouerOn || menuSeconnecter
    || menuSelectionQuitOff || menuSelectionQuitOn || menuLogo || menuHeberger
    || menuSeconnecterOn || menuHebergerOn)) {
        fprintf(stderr, "impossible d'initialiser l'image :%s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureMenuLogo = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuLogo);
        SDL_Texture *textureMenuJouerOff = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuSelectionJouerOff);
        SDL_Texture *textureMenuJouerOn = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuSelectionJouerOn);
        SDL_Texture *textureMenuQuitOff = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuSelectionQuitOff);
        SDL_Texture *textureMenuQuitOn = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuSelectionQuitOn);
        SDL_Texture *textureSeconnecter = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuSeconnecter);
        SDL_Texture *textureSeconnecterOn = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuSeconnecterOn);
        SDL_Texture *textureHeberger = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuHeberger);
        SDL_Texture *textureHebergerOn = SDL_CreateTextureFromSurface(pSDL->pRenderer, menuHebergerOn);
        if (!(textureMenuJouerOff || textureMenuJouerOn || textureSeconnecter ||
        textureMenuQuitOff || textureMenuQuitOn || pSDL->textureMenuLogo ||
         textureHeberger || textureHebergerOn || textureSeconnecterOn)) {
            fprintf(stderr, "impossible d'initialiser la texture :%s\n", IMG_GetError());
            return;
        }


        SDL_Rect dst_menuJouerOff = {(MAP_SIZE_W / 2) - (IMG_MENU_W / 4), 280, IMG_MENU_W / 2, IMG_MENU_H / 2};
        SDL_Rect dst_menuQuitOff =  {(MAP_SIZE_W / 2) - (IMG_MENU_W / 4), 520, IMG_MENU_W / 2, IMG_MENU_H / 2};
        SDL_Rect dst_menuHeberger = {(MAP_SIZE_W / 2) - (IMG_MENU_W / 6), 300, IMG_MENU_W / 3, IMG_MENU_H / 3};
        SDL_Rect dst_menuSeconnecter = {(MAP_SIZE_W / 2) - (IMG_MENU_W / 6), 450, IMG_MENU_W / 3, IMG_MENU_H / 3};

        pSDL->buttonPlay = initButton(dst_menuJouerOff, textureMenuJouerOff, textureMenuJouerOn);
        pSDL->buttonQuit = initButton(dst_menuQuitOff, textureMenuQuitOff, textureMenuQuitOn);
        pSDL->buttonHost = initButton(dst_menuHeberger, textureHeberger, textureHebergerOn);
        pSDL->buttonConnect = initButton(dst_menuSeconnecter, textureSeconnecter, textureSeconnecterOn);

    }
    SDL_FreeSurface(menuSelectionJouerOff);
    SDL_FreeSurface(menuSelectionJouerOn);
    SDL_FreeSurface(menuSelectionQuitOff);
    SDL_FreeSurface(menuSelectionQuitOn);
    SDL_FreeSurface(menuHeberger);
    SDL_FreeSurface(menuHebergerOn);
    SDL_FreeSurface(menuSeconnecter);
    SDL_FreeSurface(menuSeconnecterOn);
    SDL_FreeSurface(menuLogo);

}


button_t *initButton(SDL_Rect rect, SDL_Texture *textureOn, SDL_Texture *textureOff)
{
    button_t *b = malloc(sizeof(button_t));

    if (!b) {
        SDL_Log("erreur malloc bouton");
        return NULL;
    }

    b->hover = 0;
    b->textureButton[0] = textureOn;
    b->textureButton[1] = textureOff;
    b->dstRect = rect;
    return b;
}

/**
 * function : init texture Perso
 * @param pSDL
 * @param game
 */
void initPlayerSDL(sdl_t *pSDL)
{
    SDL_Surface *surfacePlayer = IMG_Load("../resources/sprite/perso1.png");
    SDL_Surface *surfacePlayer2 = IMG_Load("../resources/sprite/perso2.png");
    if (!surfacePlayer || !surfacePlayer2) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->texturePlayers[0] = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfacePlayer);
        pSDL->texturePlayers[1] = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfacePlayer2);
        if (!pSDL->texturePlayers[0] || !pSDL->texturePlayers[1]) {
            fprintf(stderr, "impossible d'intialiser la texture : %s", IMG_GetError());
            destroySDL(pSDL);
            return;
        }

    }
    SDL_FreeSurface(surfacePlayer);
    SDL_FreeSurface(surfacePlayer2);
    surfacePlayer = NULL;
    surfacePlayer2 = NULL;
}

/**
 * function : init texture bomb
 * @param pSDL
 * @param game
 */
void initBomb(sdl_t *pSDL)
{
    SDL_Surface* surfaceBomb = IMG_Load("../resources/img/bomb.png");
    if (!surfaceBomb) {
        SDL_Log("impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureBomb = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfaceBomb);
        if (!pSDL->textureBomb) {
            SDL_Log("impossible d'initialiser la texture : %s", IMG_GetError());
            destroySDL(pSDL);
            return;
        }
        SDL_Log("Bomb initialised");
    }
    SDL_FreeSurface(surfaceBomb);
    surfaceBomb = NULL;
}

void initExplosion(sdl_t *pSDL)
{
    SDL_Surface *center = IMG_Load("../resources/sprite/flamecenter.png");
    SDL_Surface *down = IMG_Load("../resources/sprite/flamedown.png");
    SDL_Surface *horizontal = IMG_Load("../resources/sprite/flamehorizontal.png");
    SDL_Surface *left = IMG_Load("../resources/sprite/flameleft.png");
    SDL_Surface *right = IMG_Load("../resources/sprite/flameright.png");
    SDL_Surface *up = IMG_Load("../resources/sprite/flameup.png");
    SDL_Surface *vertical = IMG_Load("../resources/sprite/flamevertical.png");
    if (!center || !down || !horizontal || !left || !right || !up || !vertical) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureExplosion2[CENTERFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, center);
        pSDL->textureExplosion2[DOWNFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, down);
        pSDL->textureExplosion2[HORIZONTALFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, horizontal);
        pSDL->textureExplosion2[LEFTFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, left);
        pSDL->textureExplosion2[RIGHTFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, right);
        pSDL->textureExplosion2[UPFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, up);
        pSDL->textureExplosion2[VERTICALFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, vertical);
        if (!pSDL->textureExplosion2[CENTERFLAME] || !pSDL->textureExplosion2[DOWNFLAME] || !pSDL->textureExplosion2[HORIZONTALFLAME] ||
            !pSDL->textureExplosion2[LEFTFLAME] || !pSDL->textureExplosion2[RIGHTFLAME] || !pSDL->textureExplosion2[UPFLAME] || !pSDL->textureExplosion2[VERTICALFLAME]) {
            fprintf(stderr, "impossible d'initialiser la texture : %s\n", SDL_GetError());
            destroySDL(pSDL);
            return;
        }
        SDL_Log("Explosion initialised");
    }
    SDL_FreeSurface(center);
    SDL_FreeSurface(down);
    SDL_FreeSurface(horizontal);
    SDL_FreeSurface(left);
    SDL_FreeSurface(right);
    SDL_FreeSurface(up);
    SDL_FreeSurface(vertical);
}

void initBlock(sdl_t *pSDL)
{
    SDL_Surface *block = IMG_Load("../resources/img/block_map1.png");
    if (!block) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureBlock = SDL_CreateTextureFromSurface(pSDL->pRenderer, block);
        if (!pSDL->textureBlock) {
            fprintf(stderr, "impossible d'initialiser la texture : %s\n", SDL_GetError());
            destroySDL(pSDL);
            return;
        }
    }
    SDL_FreeSurface(block);
    block = NULL;
}

void initMap(sdl_t *pSDL)
{
    SDL_Surface *map = IMG_Load("../resources/sprite/maps.png"); // 722 * 482 ; Taille d'une map: 240 * 160
    if (!map) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureMap = SDL_CreateTextureFromSurface(pSDL->pRenderer, map);
        if (!pSDL->textureMap) {
            fprintf(stderr, "impossible d'initialiser la texture : %s\n", SDL_GetError());
            destroySDL(pSDL);
            return;
        }
    }
    SDL_FreeSurface(map);
    map = NULL;
}


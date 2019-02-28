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
    if (pSDL->textureExplosion) {
        SDL_DestroyTexture(pSDL->textureExplosion);
        pSDL->textureExplosion = NULL;
    }
    if (pSDL->textureExplosion2) {
        SDL_DestroyTexture(pSDL->textureExplosion2);
        pSDL->textureExplosion2 = NULL;
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
    if (pSDL->texturePlayer) {
        SDL_DestroyTexture(pSDL->texturePlayer);
        pSDL->texturePlayer = NULL;
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

/** TODO
 * function : comment plz
 * @param sdl_renderer
 */
void clear(SDL_Renderer *sdl_renderer) {
    SDL_RenderClear(sdl_renderer);
    SDL_RenderPresent(sdl_renderer);
}

/**
 * function : Init textures menu
 * @param pSDL
 */
void initMenu(sdl_t *pSDL)
{
    SDL_Surface *menuSelectionJouerOff =IMG_Load("../resources/Menu_Selection_jouerOff.png");
    SDL_Surface *menuSelectionJouerOn =IMG_Load("../resources/Menu_Selection_jouerOn.png");
    SDL_Surface *menuSelectionQuitOff =IMG_Load("../resources/Menu_Selection_quitOff.png");
    SDL_Surface *menuSelectionQuitOn =IMG_Load("../resources/Menu_Selection_quitOn.png");
    SDL_Surface *menuHeberger =IMG_Load("../resources/Menu_hebergerOff.png");
    SDL_Surface *menuHebergerOn =IMG_Load("../resources/Menu_hebergerOn.png");
    SDL_Surface *menuSeconnecter =IMG_Load("../resources/Menu_seconnecterOff.png");
    SDL_Surface *menuSeconnecterOn =IMG_Load("../resources/Menu_seconnecterOn.png");
    SDL_Surface *menuLogo =IMG_Load("../resources/B_Logo.png");

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
    SDL_Surface *surfaceTrump = IMG_Load("../resources/perso3.png");
    if (!surfaceTrump) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->texturePlayer = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfaceTrump);
        if (!pSDL->texturePlayer) {
            fprintf(stderr, "impossible d'intialiser la texture : %s", IMG_GetError());
            destroySDL(pSDL);
            return;
        }

        SDL_Rect d = {START_X_MAP, START_Y_MAP, FRAME_WIDTH, FRAME_HEIGHT};
        pSDL->dst_player = d;
    }
    SDL_FreeSurface(surfaceTrump);
    surfaceTrump = NULL;
}

/**
 * function : init texture bomb
 * @param pSDL
 * @param game
 */
void initBomb(sdl_t *pSDL)
{
    SDL_Surface* surfaceBomb = IMG_Load("../resources/bomb-sprite-png-5.png");
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
    SDL_Surface *explosion = IMG_Load("../resources/explosion.png");
    SDL_Surface *explosion2 = IMG_Load("../resources/Explosion2.png");
    if (!explosion2 || !explosion) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->textureExplosion = SDL_CreateTextureFromSurface(pSDL->pRenderer, explosion);
        pSDL->textureExplosion2 = SDL_CreateTextureFromSurface(pSDL->pRenderer, explosion2);
        if (!pSDL->textureExplosion || !pSDL->textureExplosion2) {
            fprintf(stderr, "impossible d'initialiser la texture : %s\n", SDL_GetError());
            destroySDL(pSDL);
            return;
        }
        SDL_Log("Explosion initialised");
    }
    SDL_FreeSurface(explosion2);
    SDL_FreeSurface(explosion);
}

void initBlock(sdl_t *pSDL)
{
    SDL_Surface *block = IMG_Load("../resources/block_map1.png");
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
    SDL_Surface *map = IMG_Load("../resources/maps.png"); // 722 * 482 ; Taille d'une map: 240 * 160
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


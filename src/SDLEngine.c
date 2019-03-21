//
// Created by geoff on 16/01/2019.
//

#include <math.h>
#include "../header/SDLEngine.h"
#include "../header/map.h"
#include "../header/player.h"
#include "../header/menu.h"
#include "../header/bonus.h"

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

    pSDL->son[0] = initAudio(HOVER_SOUND);
    pSDL->son[1] = initAudio(EXPLOSION_SOUND);
    initPlayerSDL(pSDL);
    initMap(pSDL);
    initBlock(pSDL);
    initBomb(pSDL);
    initExplosion(pSDL);
    initMenu(pSDL);
    initBonus(pSDL);
    return pSDL;
}


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
int playsound(char* path)
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
        SDL_MixAudio(stream, audio_pos, (Uint32) len, SDL_MIX_MAXVOLUME / 3);

        audio_pos += len;
        audio_len -= len;
    }
}

/**
 * function: destroy SDL
 * @param pSDL
 * @param game
 */
void destroySDL(sdl_t *pSDL)
{
    for (int i = 0; i < 6; i++) {
        if (pSDL->textureBonus[i]) {
            SDL_DestroyTexture(pSDL->textureBonus[i]);
            pSDL->textureBonus[i] = NULL;
        }
    }
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
        if (pSDL->textureExplosion[i]) {
            SDL_DestroyTexture(pSDL->textureExplosion[i]);
            pSDL->textureExplosion[i] = NULL;
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
    for (int i = 0; i < 4; i++) {
        if (pSDL->texturePlayers[i]) {
            SDL_DestroyTexture(pSDL->texturePlayers[i]);
            pSDL->texturePlayers[i] = NULL;
        }
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
    closeAudio(pSDL->son[0]);
    closeAudio(pSDL->son[1]);
    free(pSDL->buttonPlay);
    free(pSDL->buttonQuit);
    free(pSDL->buttonHost);
    free(pSDL->buttonConnect);
    free(pSDL->son[0]);
    free(pSDL->son[1]);
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
    SDL_Surface *menu_retour = IMG_Load("../resources/img/Retour_menu.png");

    if (!(menuSelectionJouerOff || menuSelectionJouerOn || menuSeconnecter
    || menuSelectionQuitOff || menuSelectionQuitOn || menuLogo || menuHeberger
    || menuSeconnecterOn || menuHebergerOn || menu_retour)) {
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
        pSDL->textureMenuRetour = SDL_CreateTextureFromSurface(pSDL->pRenderer, menu_retour);
        if (!(textureMenuJouerOff || textureMenuJouerOn || textureSeconnecter ||
        textureMenuQuitOff || textureMenuQuitOn || pSDL->textureMenuLogo ||
         textureHeberger || textureHebergerOn || textureSeconnecterOn || pSDL->textureMenuRetour)) {
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
    SDL_FreeSurface(menu_retour);

}
/**
 * function : Create a struct button_t
 * @param rect
 * @param textureOn
 * @param textureOff
 * @return a struct button_t which contains the normal texture and the hover one, a boolean to know if the button is hovered and
 * a SDL_Rect for the position in the window.
 */
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
    SDL_Surface *surfacePlayer3 = IMG_Load("../resources/sprite/perso3.png");
    SDL_Surface *surfacePlayer4 = IMG_Load("../resources/sprite/perso4.png");
    if (!surfacePlayer || !surfacePlayer2) {
        fprintf(stderr, "impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return;
    } else {
        pSDL->texturePlayers[0] = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfacePlayer);
        pSDL->texturePlayers[1] = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfacePlayer2);
        pSDL->texturePlayers[2] = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfacePlayer3);
        pSDL->texturePlayers[3] = SDL_CreateTextureFromSurface(pSDL->pRenderer, surfacePlayer4);
        if (!pSDL->texturePlayers[0] || !pSDL->texturePlayers[1] || !pSDL->texturePlayers[2] || !pSDL->texturePlayers[3]) {
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
        pSDL->textureExplosion[CENTERFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, center);
        pSDL->textureExplosion[DOWNFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, down);
        pSDL->textureExplosion[HORIZONTALFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, horizontal);
        pSDL->textureExplosion[LEFTFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, left);
        pSDL->textureExplosion[RIGHTFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, right);
        pSDL->textureExplosion[UPFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, up);
        pSDL->textureExplosion[VERTICALFLAME] = SDL_CreateTextureFromSurface(pSDL->pRenderer, vertical);
        if (!pSDL->textureExplosion[CENTERFLAME] || !pSDL->textureExplosion[DOWNFLAME] || !pSDL->textureExplosion[HORIZONTALFLAME] ||
            !pSDL->textureExplosion[LEFTFLAME] || !pSDL->textureExplosion[RIGHTFLAME] || !pSDL->textureExplosion[UPFLAME] || !pSDL->textureExplosion[VERTICALFLAME]) {
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
//    map = NULL;
}

void initBonus(sdl_t *pSDL)
{
    SDL_Surface *bRangeBomb = IMG_Load("../resources/img/bRangeBombe.png");
    SDL_Surface *mRangeBomb = IMG_Load("../resources/img/mRangeBombe.png");
    SDL_Surface *bNbBomb = IMG_Load("../resources/img/bNbBombe.png");
    SDL_Surface *mNbBomb = IMG_Load("../resources/img/mNbBombe.png");
    SDL_Surface *bSpeed = IMG_Load("../resources/img/bVitesse.png");
    SDL_Surface *mSpeed = IMG_Load("../resources/img/mVitesse.png");
    if (!bRangeBomb || !mRangeBomb || !bNbBomb || !mNbBomb || !bSpeed || !mSpeed) {
        SDL_Log("impossible d'initialiser l'image : %s\n", SDL_GetError());
        destroySDL(pSDL);
        return ;
    } else {
        pSDL->textureBonus[bRangeBombe] = SDL_CreateTextureFromSurface(pSDL->pRenderer, bRangeBomb);
        pSDL->textureBonus[mRangeBombe] = SDL_CreateTextureFromSurface(pSDL->pRenderer, mRangeBomb);
        pSDL->textureBonus[bNbBombe] = SDL_CreateTextureFromSurface(pSDL->pRenderer, bNbBomb);
        pSDL->textureBonus[mNbBombe] = SDL_CreateTextureFromSurface(pSDL->pRenderer, mNbBomb);
        pSDL->textureBonus[bVitesse] = SDL_CreateTextureFromSurface(pSDL->pRenderer, bSpeed);
        pSDL->textureBonus[mVitesse] = SDL_CreateTextureFromSurface(pSDL->pRenderer, mSpeed);
        if (!pSDL->textureBonus[bRangeBombe] || !pSDL->textureBonus[mRangeBombe] || !pSDL->textureBonus[bNbBombe]
        || !pSDL->textureBonus[mNbBombe] || !pSDL->textureBonus[bVitesse] || !pSDL->textureBonus[mVitesse]) {
            SDL_Log("impossible d'initialiser la texture : %s\n", SDL_GetError());
            destroySDL(pSDL);
            return ;
        }
    }
    SDL_FreeSurface(bRangeBomb);
    SDL_FreeSurface(mRangeBomb);
    SDL_FreeSurface(bNbBomb);
    SDL_FreeSurface(mNbBomb);
    SDL_FreeSurface(bSpeed);
    SDL_FreeSurface(mSpeed);
}


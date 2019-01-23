//
// Created by geoff on 16/01/2019.
//

#include "../header/game.h"
#include "../header/file.h"

game_t *initGame()
{
    game_t *game = malloc(sizeof(game_t));

    if (!game) {
        return (NULL);
    }
    game->current_map = 0;
    if (extractArrayFromFile(game->map) == 0) {
        return (NULL);
    }

    return game;
}




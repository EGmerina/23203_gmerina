#pragma once
#include <vector>
#include "player.h"

void play_game(std::vector<Player *> players)
{
    for (Player *u : players)
    {
        u->play();
    }
}
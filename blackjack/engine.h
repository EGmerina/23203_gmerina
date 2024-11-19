#pragma once
#include <vector>
#include "player.h"

void play_game(std::vector<std::unique_ptr<Player>> players)
{
    for (auto &u : players) // что это такое???? но работает...наверное
    {
        u->get_new_card();
        // распечатать лицевую карту
        u->get_new_card();
    }
}

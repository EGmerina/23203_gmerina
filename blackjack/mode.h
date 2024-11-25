#pragma once
#include <vector>
#include <memory>
#include "player.h"
#include "deck.h"

class Mode
{
public:
    virtual void play_game(std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Deck> deck) = 0;
};

#pragma once
#include <vector>
#include <memory>
#include "../deckes/deck.h"
#include "../players/player.h"

class Mode
{
public:
    Mode &operator=(const Mode &b) = delete;
    virtual ~Mode() = default;

    virtual void play_game(std::vector<std::unique_ptr<Player>> &&players, std::unique_ptr<Deck> deck) = 0;
};

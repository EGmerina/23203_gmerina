#pragma once
#include "mode.h"

class DetailedMode : public Mode
{
public:
    virtual void play_game(std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Deck> deck) override ;
};

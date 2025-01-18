#pragma once
#include "mode.h"

class DetailedMode : public Mode
{
public:
    virtual void play_game(std::vector<std::unique_ptr<Player>> players, std::unique_ptr<Deck> deck) override;
};

#pragma once

#include "player.h"

class Masha : public Player
{
public:
    virtual void hit() override;
    virtual void stand() override;
    virtual void play(Card opponents_card, Hand its_hand) override;
};

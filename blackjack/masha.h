#pragma once

#include "player.h"

class Masha : public Player
{
    virtual Player *create_player() override;
    virtual void hit(Card opponents_card, Hand my_hand) override;
    virtual void stand(Card opponents_card, Hand my_hand) override;
    virtual void play() override;
};
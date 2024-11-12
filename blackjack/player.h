#pragma once
#include "cards.h"

class Player
{
public:
    virtual Player *create_player() = 0;
    virtual void hit(Card opponents_card, Hand my_hand) = 0;
    virtual void stand(Card opponents_card, Hand my_hand) = 0;
    virtual void play() = 0;
};

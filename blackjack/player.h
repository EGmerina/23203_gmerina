#pragma once
#include "hand.h"

static unsigned char get_random_num(const unsigned char beg, const unsigned char end)
{
    srand(time(0));
    return rand() % (end - beg + 1) + beg;
}

class Player
{
public:
    virtual void hit() = 0;
    virtual void stand() = 0;
    virtual void play() = 0;
    void get_new_card()
    {
        Card new_card = get_random_num(1, 10);
        my_hand.add_card(new_card);
    }

private:
    Hand my_hand;
    Card opponents_card;
};

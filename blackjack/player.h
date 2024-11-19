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
    virtual void play(Card opponents_card) = 0;
    Card get_new_card()
    {
        Card new_card = get_random_num(1, 10);
        my_hand.add_card(new_card);
        return new_card;
    }

    Card &get_face_card() //???? надо ли? может как-то обойти проблему доступа?
    {
        return my_hand.get_face_card();
    }

    unsigned char get_points()
    {
        return my_hand.get_points();
    }

private:
    Hand my_hand;
};

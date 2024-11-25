#pragma once
#include "hand.h"
/*
static unsigned char get_random_num(const unsigned char beg, const unsigned char end)
{
    srand(time(0));
    return rand() % (end - beg + 1) + beg;
}*/

class Player
{
public:
    virtual bool make_move(Card opponents_card, Hand &its_hand) = 0; // возвращает true если hit, если stand то false
};

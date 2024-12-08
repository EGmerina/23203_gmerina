#pragma once
#include "../hand.h"
#include <string>

class Player
{
public:
    virtual bool make_move(Card opponents_card, Hand &its_hand) = 0; // возвращает true если hit, если stand то false
    virtual std::string get_name() = 0;
};

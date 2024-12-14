#pragma once
#include <string>
#include "../hand.h"

class Player
{
public:
    virtual bool make_move(const Card opponents_card, const Hand &its_hand) = 0; // возвращает true если hit, если stand то false
    virtual std::string get_name() = 0;
};

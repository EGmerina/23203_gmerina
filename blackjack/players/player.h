#pragma once
#include <string>
#include "../hand.h"

enum Move : bool
{
    hit = true,
    stand = false
};

class Player
{
public:
    virtual ~Player() = default;
    virtual Move make_move(Card opponents_card, const Hand &its_hand) = 0;
    virtual std::string get_name() = 0;
};

#pragma once

#include "../hand.h"

class Deck
{
public:
    Deck &operator=(const Deck &b) = delete;
    virtual ~Deck() = default;
    virtual Card get_new_card() = 0;
};

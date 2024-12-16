#pragma once
#include <time.h>
#include "../hand.h"

class Deck
{
public:
    Deck()
    {
        srand(time(0));
    }
    Deck &operator=(const Deck &b) = delete;
    virtual ~Deck() = default;
    virtual Card get_new_card() = 0;
};

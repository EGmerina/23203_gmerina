#pragma once

#include "../hand.h"

class Deck
{
public:
    virtual Card get_new_card() = 0;
};

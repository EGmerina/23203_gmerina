#pragma once
#include "deck.h"

class SimpleDeck : public Deck
{
public:
    virtual Card get_new_card() override;
};
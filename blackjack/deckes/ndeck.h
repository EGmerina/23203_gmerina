#pragma once
#include "deck.h"

class NDeck : public Deck
{
public:
    NDeck(unsigned char N = 1);
    virtual Card get_new_card() override;

private:
    unsigned char N;
    std::vector<Card> deck;
};

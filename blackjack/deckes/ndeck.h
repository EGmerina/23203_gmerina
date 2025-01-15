#pragma once
#include "deck.h"

class NDeck : public Deck
{
public:
    NDeck(int N = 4);
    virtual Card get_new_card() override;

private:
    unsigned char N;
    std::vector<Card> deck;
};

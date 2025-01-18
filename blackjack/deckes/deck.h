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
    Deck(const Deck & copy) = delete;
    Deck &operator=(const Deck &b) = delete;
    virtual ~Deck() = default;
    virtual Card get_new_card() = 0;

protected:
    size_t get_random_num(size_t beg, size_t end)
    {
        return rand() % (end - beg + 1) + beg;
    }
};

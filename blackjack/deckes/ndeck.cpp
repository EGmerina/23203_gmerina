#include "ndeck.h"

#include <assert.h>

#include "deck.cpp"
#include "../register_creator.h"

NDeck::NDeck(unsigned char N ) : N(N)
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            for (auto u : standart_deck)
            {
                deck.push_back(u);
            }
        }
    }
}
Card NDeck::get_new_card()
{
    size_t random_index = get_random_num(0, 13 * 4 * N - 1);
    assert(!deck.empty());
    deck.erase(deck.begin() + random_index);
    return deck[random_index];
}

namespace
{
    RegisterCreator<Deck, NDeck> a("deck=N");
}

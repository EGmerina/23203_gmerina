#include "ndeck.h"

#include <assert.h>

#include "../register_creator.h"

#define Jack 10
#define Queen 10
#define King 10

constexpr static Card standart_deck[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, 11};

NDeck::NDeck(int N = 4) : N(N)
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
    RegisterCreator<Deck, NDeck> a("N");
}

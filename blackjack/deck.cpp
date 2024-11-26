#include "deck.h"
#include "factory.h"
#include <string>

#define Jack 10
#define Queen 10
#define King 10

static Card standart_deck[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, 11};

static size_t get_random_num(const size_t beg, const size_t end)
{
    srand(time(0));
    return rand() % (end - beg + 1) + beg;
}

class SimpleDeck : public Deck
{
public:
    virtual Card get_new_card() override
    {
        return get_random_num(1, 10);
    }
};

class NDeck : public Deck
{
public:
    NDeck(unsigned char N = 1) : N(N)
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
    virtual Card get_new_card() override
    {
        size_t random_index = get_random_num(0, 4 * N - 1);
        return deck[random_index];
    }

private:
    unsigned char N;
    std::vector<Card> deck;
};

Deck *createSimpleDeck()
{
    return new SimpleDeck();
}

Deck *createNDeck()
{
    return new NDeck();
}

namespace
{
    bool f = Factory<std::string, Deck>::getInstance()->registerCreator("deck=simple", createSimpleDeck);

    bool k = Factory<std::string, Deck>::getInstance()->registerCreator("deck=N", createNDeck); // как вместо N поставить любую цифру?
}

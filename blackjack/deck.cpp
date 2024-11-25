#include "deck.h"
#include "factory.h"
#include <string>

static unsigned char get_random_num(const unsigned char beg, const unsigned char end)
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
    virtual Card get_new_card() override
    {
    }
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

    bool k = Factory<std::string, Deck>::getInstance()->registerCreator("deck=N", createNDeck);
}

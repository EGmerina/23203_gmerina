#include "deck.h"
#include "factory.h"
#include <string>

class SimpleDeck : public Deck
{
public:
    virtual Card get_new_card() override
    {
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

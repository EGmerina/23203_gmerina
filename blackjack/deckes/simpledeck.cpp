#include "simpledeck.h"
#include "deck.cpp"

class SimpleDeck : public Deck
{
public:
    virtual Card get_new_card() override
    {
        return get_random_num(1, 10);
    }
};

Deck *createSimpleDeck()
{
    return new SimpleDeck();
}

namespace
{
    bool f = Factory<std::string, Deck>::getInstance()->registerCreator("deck=simple", createSimpleDeck);

}

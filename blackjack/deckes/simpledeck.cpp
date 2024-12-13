#include "simpledeck.h"
#include "deck.cpp"
#include "../register_creator.h"

Card SimpleDeck::get_new_card()
{
    return get_random_num(1, 10);
}

namespace
{
    RegisterCreator<Deck, SimpleDeck> b("deck=simple");
}
//TODO написать сравнения для 1 и 11
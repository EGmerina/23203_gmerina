#include "simpledeck.h"
#include "../register_creator.h"

Card SimpleDeck::get_new_card()
{
    return get_random_num(2, 11);
}

namespace
{
    RegisterCreator<Deck, SimpleDeck> b("simple");
}

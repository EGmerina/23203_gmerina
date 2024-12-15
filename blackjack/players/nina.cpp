#include "nina.h"

#include <string>
#include "../register_creator.h"

Move Nina ::make_move(const Card opponents_card, const Hand &its_hand)
{
    if ((opponents_card == 10 || opponents_card == 11 || opponents_card == 1) && its_hand.get_points() < 20)
    {
        return hit;
    }
    if (its_hand.get_cards_number() <= 3 && its_hand.get_points() < 14)
    {
        return hit;
    }
    return stand;
}

std::string Nina ::get_name()
{
    return "nina";
}

namespace
{
    RegisterCreator<Player, Nina> b("nina");
}

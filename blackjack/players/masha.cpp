#include "masha.h"

#include <string>
#include "../register_creator.h"

Move Masha ::make_move(const Card opponents_card, const Hand &its_hand)
{
    if (its_hand.get_points() <= 13 || (its_hand.get_points() < 19 && opponents_card == 10))
    {
        return hit;
    }
    return stand;
}

std::string Masha ::get_name()
{
    return "masha";
}

namespace
{
    RegisterCreator<Player, Masha> b("masha");
}

#include "vasya.h"

#include <string>
#include "../register_creator.h"

Move Vasya ::make_move(const Card opponents_card, const Hand &its_hand)
{
    if (opponents_card < 7 && its_hand.get_points() <= 15)
    {
        return hit;
    }
    return stand;
}

std::string Vasya ::get_name()
{
    return "vasya";
}

namespace
{
    RegisterCreator<Player, Vasya> b("vasya");
}

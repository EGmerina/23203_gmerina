
#include "dealer.h"

#include <string>
#include "../register_creator.h"

Move Dealer ::make_move(const Card opponents_card, const Hand &its_hand)
{
    if (its_hand.get_points() < 17)
    {
        return hit;
    }
    return stand;
}

std::string Dealer ::get_name()
{
    return "dealer";
}

namespace
{
    RegisterCreator<Player, Dealer> b("dealer");
}

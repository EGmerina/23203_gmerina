#include "yan.h"

#include <string>
#include "../register_creator.h"

Move Yan ::make_move(const Card opponents_card, const Hand &its_hand)
{
    if (its_hand.get_cards_number() == 4 || its_hand.get_points() >= 21)
    {
        return stand;
    }
    return hit;
}

std::string Yan ::get_name()
{
    return "yan";
}

namespace
{
    RegisterCreator<Player, Yan> b("yan");
}

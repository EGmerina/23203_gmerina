#include "yan.h"

#include <string>
#include "../register_creator.h"

static bool hit()
{
    return true;
}

static bool stand()
{
    return false;
}

bool Yan ::make_move(const Card opponents_card, const Hand &its_hand)
{
    if (its_hand.get_cards_number() == 4 || its_hand.get_points() >= 21)
    {
        return stand();
    }
    return hit();
}

std::string Yan ::get_name()
{
    std::string name = "yan";
    return name;
}

namespace
{
    RegisterCreator<Player, Yan> b("yan");
}

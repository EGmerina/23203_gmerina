#include "masha.h"

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

bool Masha ::make_move(const Card opponents_card, const Hand &its_hand)
{
    if (its_hand.get_points() <= 13 && opponents_card == 10)
    {
        return hit();
    }
    return stand();
}

std::string Masha ::get_name()
{
    std::string name = "masha";
    return name;
}

namespace
{
    RegisterCreator<Player, Masha> b("masha");
}

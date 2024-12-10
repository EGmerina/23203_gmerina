#include "vasya.h"

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

bool Vasya ::make_move(const Card opponents_card, const Hand &its_hand)
{
    if (opponents_card < 7 && its_hand.get_points() <= 15)
    {
        return hit();
    }
    return stand();
}

std::string Vasya ::get_name()
{
    std::string name = "vasya";
    return name;
}

namespace
{
    RegisterCreator<Player, Vasya> b("vasya");
}

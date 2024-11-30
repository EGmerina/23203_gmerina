#include "factory.h"
#include "masha.h"

#include <string>

static bool hit()
{
    return true;
}

static bool stand()
{
    return false;
}

bool Masha ::make_move(Card opponents_card, Hand &its_hand)
{
    if (its_hand.get_points() <= 13)
    {
        return hit();
    }
    if (opponents_card == 10)
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

Player *createMasha()
{
    return new Masha();
}
namespace
{
    bool b = Factory<std::string, Player>::getInstance()->registerCreator("masha", createMasha);
}

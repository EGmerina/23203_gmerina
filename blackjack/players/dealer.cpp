#include "factory.h"
#include "dealer.h"

#include <string>

static bool hit()
{
    return true;
}

static bool stand()
{
    return false;
}

bool Dealer ::make_move(Card opponents_card, Hand &its_hand)
{
    if (its_hand.get_points() < 17)
    {
        return hit();
    }
    return stand();
}

std::string Dealer ::get_name()
{
    std::string name = "dealer";
    return name;
}

Player *createDealer()
{
    return new Dealer();
}
namespace
{
    bool b = Factory<std::string, Player>::getInstance()->registerCreator("dealer", createDealer);
}

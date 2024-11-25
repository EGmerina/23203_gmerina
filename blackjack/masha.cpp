#include "factory.h"
#include "masha.h"

#include <string>

/*void Masha::hit()
{
    printf("hit\n");
    printf("new card : %d\n", get_new_card());
    printf("current number of points : %d\n", get_points());
}
void Masha::stand()
{
    printf("stand\n");
    printf("current number of points : %d\n", get_points()); // это вообще задача user interface
}
void Masha::play(Card opponents_card, Hand its_hand)
{
    printf("masha's turn\n");
    printf("current number of points : %d\n", get_points());
    while (get_points() < 17)
    {
        hit();
    }
    stand();
}*/

bool Masha ::make_move(Card opponents_card, Hand &its_hand)
{
    
}

std::string Masha ::get_name()
{
}

Player *createMasha()
{
    return new Masha();
}
namespace
{
    bool b = Factory<std::string, Player>::getInstance()->registerCreator("masha", createMasha);
}

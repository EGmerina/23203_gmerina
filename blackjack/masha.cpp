#include "factory.h"
#include "masha.h"

#include <string>

void Masha::hit()
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
void Masha::play(Card opponents_card)
{
    printf("masha's turn\n");
    printf("current number of points : %d\n", get_points());
    while (get_points() < 17)
    {
        hit();
    }
    stand();
}

Player *createMasha()
{
    return new Masha();
}
namespace
{
    bool b = Factory<std::string, Player>::getInstance()->registerCreator("masha", createMasha);
}

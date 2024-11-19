#include "factory.h"
#include "masha.h"

#include <string>

void Masha::hit()
{
}
void Masha::stand()
{
}
void Masha::play()
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

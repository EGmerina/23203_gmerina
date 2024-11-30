#include "detailedmode.h"
#include "factory.h"

void DetailedMode::play_game(std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Deck> deck)
{
}

Mode *createDetailedMode()
{
    return new DetailedMode();
}
namespace
{
    bool a = Factory<std::string, Mode>::getInstance()->registerCreator("mode=detailed", createDetailedMode);
}

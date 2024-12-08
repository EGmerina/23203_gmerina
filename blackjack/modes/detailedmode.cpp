#include "detailedmode.h"
#include "../register_creator.h"

void DetailedMode::play_game(std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Deck> deck)
{
    // TODO write play_game detailed
}
namespace
{
    RegisterCreator<Mode, DetailedMode> b("mode=detailed");
}

#pragma once
#include <vector>
#include <memory>
#include "../deckes/deck.h"
#include "../players/player.h"

class Mode
{
public:
    Mode &operator=(const Mode &b) = delete;
    virtual ~Mode() = default;

    virtual void play_game(std::vector<std::unique_ptr<Player>> players, std::unique_ptr<Deck> deck) = 0;

protected:
    std::unique_ptr<Player>::pointer get_winner(const std::unique_ptr<Player> &player1, Hand &p1_hand, const std::unique_ptr<Player> &player2, Hand &p2_hand);
};

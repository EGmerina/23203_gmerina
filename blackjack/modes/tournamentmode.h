#pragma once
#include "mode.h"
#include <map>
#include <string>
#include "../console_interface.h"

class TournamentMode : public Mode
{
public:
    virtual void play_game(std::vector<std::unique_ptr<Player>> &&players_arr, std::unique_ptr<Deck> my_deck) override;

private:
    std::vector<std::unique_ptr<Player>> players;
    std::map<std::string, size_t> victories_num;
    std::unique_ptr<Deck> deck = nullptr;
    ConsoleInterface interface;

    void play_tour(std::unique_ptr<Player> player1, std::unique_ptr<Player> player2);
    void announce_winners_by_victories();
};


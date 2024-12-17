#include "detailedmode.h"

#include <iostream>
#include <string>

#include "mode.cpp"
#include "../register_creator.h"
#include "../console_interface.h"

#define INITIAL_NUMBER_OF_CARDS 2

static bool stop_game(std::istream &cin, std::string &str)
{
    std::getline(cin, str);
    if (str == "quit")
    {
        return true;
    }
    return false;
}

void DetailedMode::play_game(std::vector<std::unique_ptr<Player>> &&players, std::unique_ptr<Deck> deck)
{
    std::string command_str;

    std::unique_ptr<Player> player1 = std::move(players[0]);
    std::unique_ptr<Player> player2 = std::move(players[1]);
    Hand p1_hand;
    Hand p2_hand;
    ConsoleInterface interface;
    interface.announce_new_round(player1, player2);

    for (size_t i = 0; i < INITIAL_NUMBER_OF_CARDS; i++)
    {
        p1_hand.add_card(deck->get_new_card());
        p2_hand.add_card(deck->get_new_card());
    }

    bool p1_play = true;
    bool p2_play = true;

    interface.output_points(player1, p1_hand);
    interface.output_points(player2, p2_hand);

    while (p1_play || p2_play)
    {
        if (p1_play)
        {
            if (stop_game(std::cin, command_str))
                return;
            interface.announce_whos_turn(player1);
            if (stop_game(std::cin, command_str))
                return;
            p1_play = player1->make_move(p2_hand.get_face_card(), p1_hand);
            interface.announce_move(p1_play);
            if (p1_play)
            {
                if (stop_game(std::cin, command_str))
                    return;
                p1_hand.add_card(deck->get_new_card());
                interface.output_points(player1, p1_hand);
            }
        }
        if (p2_play)
        {
            if (stop_game(std::cin, command_str))
                return;
            interface.announce_whos_turn(player2);
            if (stop_game(std::cin, command_str))
                return;
            p2_play = player2->make_move(p1_hand.get_face_card(), p2_hand);
            interface.announce_move(p2_play);
            if (p2_play)
            {
                if (stop_game(std::cin, command_str))
                    return;
                p2_hand.add_card(deck->get_new_card());
                interface.output_points(player2, p2_hand);
            }
        }
    }

    auto winner = get_winner(&player1, p1_hand, &player2, p2_hand);

    interface.announce_winner(winner);
}
namespace
{
    RegisterCreator<Mode, DetailedMode> b("detailed");
}

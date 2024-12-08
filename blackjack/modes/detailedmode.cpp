#include "detailedmode.h"
#include <string>
#include <iostream>
#include "../register_creator.h"
#include "../console_interface.h"
#include "mode.cpp"

#define INITIAL_NUMBER_OF_CARDS 2

void DetailedMode::play_game(std::vector<std::shared_ptr<Player>> &players, std::shared_ptr<Deck> deck)
{
    std::string command_str;

    std::shared_ptr<Player> player1 = players[0];
    std::shared_ptr<Player> player2 = players[1];
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

    std::cin >> command_str;
    if (command_str == "quit")
        return;

    while (p1_play || p2_play)
    {
        if (p1_play)
        {

            p1_play = player1->make_move(p2_hand.get_face_card(), p1_hand);
            if (p1_play)
            {
                // TODO write play_game detailed
            }
        }
        if (p2_play)
        {
            p2_play = player2->make_move(p1_hand.get_face_card(), p2_hand);
        }
    }

    interface.output_points(player1, p1_hand);
    interface.output_points(player2, p2_hand);

    std::shared_ptr<Player> winner = get_winner(player1, p1_hand, player2, p2_hand);

    interface.announce_winner(winner);
}
namespace
{
    RegisterCreator<Mode, DetailedMode> b("mode=detailed");
}

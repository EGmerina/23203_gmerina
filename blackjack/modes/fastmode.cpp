#include "fastmode.h"

#include <string>

#include "mode.cpp"
#include "../console_interface.h"
#include "../register_creator.h"

void FastMode::play_game(std::vector<std::shared_ptr<Player>> &players, std::shared_ptr<Deck> deck)
{
    std::shared_ptr<Player> player1 = players[0];
    std::shared_ptr<Player> player2 = players[1];
    Hand p1_hand;
    Hand p2_hand;

    p1_hand.add_card(deck->get_new_card());
    p2_hand.add_card(deck->get_new_card());

    bool p1_play = true;
    bool p2_play = true;

    while (p1_play || p2_play)
    {
        if (p1_play)
        {
            p1_hand.add_card(deck->get_new_card());
            p1_play = player1->make_move(p2_hand.get_face_card(), p1_hand);
        }
        if (p2_play)
        {
            p2_hand.add_card(deck->get_new_card());
            p2_play = player2->make_move(p1_hand.get_face_card(), p2_hand);
        }
    }

    ConsoleInterface interface;
    interface.output_points(player1, p1_hand);
    interface.output_points(player2, p2_hand);

    std::shared_ptr<Player> winner = get_winner(player1, p1_hand, player2, p2_hand);

    interface.announce_winner(winner);
}

namespace
{
    RegisterCreator<Mode, FastMode> b("mode=fast");
}

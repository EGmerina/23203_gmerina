#include "tournamentmode.h"
#include "../register_creator.h"

void TournamentMode::play_tour(const std::unique_ptr<Player> &player1, const std::unique_ptr<Player> &player2)
{
    interface.announce_new_round(player1, player2);
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

    interface.output_points(player1, p1_hand);
    interface.output_points(player2, p2_hand);

    const std::unique_ptr<Player>::pointer winner = get_winner(player1, p1_hand, player2, p2_hand);
    interface.announce_winner(winner);
    if (winner != nullptr)
    {
        victories_num[winner->get_name()] += 1;
    }
}

void TournamentMode::announce_winners_by_victories()
{
    size_t first_vict_num = victories_num[players[0]->get_name()];
    bool draw_flag = 1;
    size_t max_vict_num = 0;
    for (auto &u : players)
    {
        size_t cur_vict_num = victories_num[u->get_name()];
        if (first_vict_num != cur_vict_num)
        {
            draw_flag = 0;
        }
        if (max_vict_num < cur_vict_num)
        {
            max_vict_num = cur_vict_num;
        }
    }
    if (draw_flag)
    {
        std::unique_ptr<Player> nobody = nullptr;
        interface.announce_winner(nobody.get());
        return;
    }
    for (auto &u : players)
    {
        if (victories_num[u->get_name()] == max_vict_num)
        {
            interface.announce_winner(u.get());
        }
    }
}

void TournamentMode::play_game(std::vector<std::unique_ptr<Player>> &&players_arr, std::unique_ptr<Deck> my_deck)
{
    players = std::move(players_arr);
    deck = std::move(my_deck);

    for (size_t first = 0; first < players.size() - 1; first++)
    {
        for (size_t second = first + 1; second < players.size(); second++)
        {
            play_tour(players[first], players[second]);
        }
    }

    interface.announce_total_protocol();
    for (auto &u : players)
    {
        interface.output_vict_num(u, victories_num[u->get_name()]);
    }
    announce_winners_by_victories();
}

namespace
{
    RegisterCreator<Mode, TournamentMode> b("tournament");
}
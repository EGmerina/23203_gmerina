#include "tournamentmode.h"
#include <map>
#include <string>
#include "../console_interface.h"
#include "../register_creator.h"
#include "mode.cpp"

static void generate_opponents(std::vector<std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>>> &pairs, std::vector<std::shared_ptr<Player>> &players)
{
    for (size_t first = 0; first < players.size() - 1; first++)
    {
        for (size_t second = first + 1; second < players.size(); second++)
        {
            std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>> new_pair{players[first], players[second]};
            pairs.push_back(new_pair);
        }
    }
}

static void play_tour(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, std::shared_ptr<Deck> deck, std::map<std::string, size_t> &victories_num)
{
    // считать количество побед каждого игрока
    ConsoleInterface interface;
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

    std::shared_ptr<Player> winner = get_winner(player1, p1_hand, player2, p2_hand);
    interface.announce_winner(winner);
    if (winner != nullptr)
    {
        victories_num[winner->get_name()] += 1;
    }
}

static std::shared_ptr<Player> get_winner_by_victories(std::vector<std::shared_ptr<Player>> &players, std::map<std::string, size_t> &victories_num)
{
    size_t max_vict_num = 0;
    std::shared_ptr<Player> winner = nullptr;
    for (auto &u : players)
    {
        size_t cur_vict_num = victories_num[u->get_name()];
        if (max_vict_num < cur_vict_num)
        {
            max_vict_num = cur_vict_num;
            winner = u;
        }
    }
    return winner;
}

void TournamentMode::play_game(std::vector<std::shared_ptr<Player>> &players, std::shared_ptr<Deck> deck)
{
    std::vector<std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>>> pairs;
    generate_opponents(pairs, players);
    std::map<std::string, size_t> victories_num;
    for (auto &u : pairs)
    {
        play_tour(u.first, u.second, deck, victories_num);
    }
    ConsoleInterface interface;
    interface.announce_total_protocol();
    for (auto &u : players)
    {
        interface.output_vict_num(u, victories_num[u->get_name()]);
    }
    std::shared_ptr<Player> winner = get_winner_by_victories(players, victories_num);
    interface.announce_winner(winner);
}

namespace
{
    RegisterCreator<Mode, TournamentMode> b("mode=tournament");
}

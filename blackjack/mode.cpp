#include "factory.h"
#include "mode.h"
#include "deck.h"
#include "player.h"
#include "console_interface.h"
#include <string>

static std::shared_ptr<Player> get_winner(std::shared_ptr<Player> player1, Hand &p1_hand, std::shared_ptr<Player> player2, Hand &p2_hand)
{
    if (p1_hand.get_points() > 21)
    {
        return player2;
    }
    if (p2_hand.get_points() > 21)
    {
        return player1;
    }
    if (p1_hand.get_points() >= p2_hand.get_points()) // дописать нормально, с ничьей и проверкой карт на блэкджек
    {
        return player1;
    }
    return player2;
}

static void generate_opponents(std::vector<std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>>> pairs, std::vector<std::shared_ptr<Player>> players)
{
    for (size_t first = 0; first < players.size() - 1; first++)
    {
        for (size_t second = first; second < players.size(); second++)
        {
            std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>> new_pair{players[first], players[second]};
            pairs.push_back(new_pair);
        }
    }
}

static void play_tour(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2)
{
    // дописать
    unsigned char stand_cnt = 0;
    while (stand_cnt < 2)
    {
        Hand p1;
        Hand p2;
    }
}

class DetailedMode : public Mode
{
public:
    virtual void play_game(std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Deck> deck) override
    {
    }
};

class FastMode : public Mode
{
public:
    virtual void play_game(std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Deck> deck) override
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
};

class TournamentMode : public Mode
{
public:
    virtual void play_game(std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Deck> deck) override
    {
        std::vector<std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>>> pairs;
        generate_opponents(pairs, players);
        for (auto &u : pairs)
        {
            play_tour(u.first, u.second); // дописать
        }
    }
};

Mode *createDetailedMode()
{
    return new DetailedMode();
}

Mode *createFastMode()
{
    return new FastMode();
}

Mode *createTournamentMode()
{
    return new TournamentMode();
}
namespace
{
    bool a = Factory<std::string, Mode>::getInstance()->registerCreator("mode=detailed", createDetailedMode);

    bool b = Factory<std::string, Mode>::getInstance()->registerCreator("mode=fast", createFastMode);

    bool c = Factory<std::string, Mode>::getInstance()->registerCreator("mode=tournament", createTournamentMode);
}
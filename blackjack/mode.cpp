#include "factory.h"
#include "mode.h"
#include "deck.h"
#include "player.h"
#include "console_interface.h"
#include <string>

#define INITIAL_CARDS_NUMBER 2

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
        for (size_t i = 0; i < INITIAL_CARDS_NUMBER; i++)
        {
            p1_hand.add_card(deck->get_new_card());
            p2_hand.add_card(deck->get_new_card());
        }

        bool p1_play = true;
        bool p2_play = true;

        while (p1_play || p2_play)
        {
            if (p1_play)
            {
                p1_play = player1->make_move(p2_hand.get_face_card(), p1_hand);
            }
            if (p2_play)
            {
                p2_play = player2->make_move(p1_hand.get_face_card(), p2_hand);
            }
        }

        output_points(player1, p1_hand);
        output_points(player2, p2_hand);

        std::shared_ptr<Player> winner = get_winner(player1, p1_hand, player2, p2_hand);

        announce_winner(winner);
    }
};

class TournamentMode : public Mode
{
public:
    virtual void play_game(std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Deck> deck) override
    {
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
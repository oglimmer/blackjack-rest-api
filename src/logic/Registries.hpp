

#ifndef BLACKJACK_REGISTRIES_HPP
#define BLACKJACK_REGISTRIES_HPP

#include "util.h"
#include "GameLogic.hpp"

class DrawDeckRegistry {
SINGLETON(DrawDeckRegistry)

private:
    std::map<int, std::shared_ptr<DrawDeck>> drawDecks;

    DrawDeckRegistry() {}

public:
    int CreateDrawDeck();

    std::shared_ptr<DrawDeck> GetDrawDeck(int id);
};

class GameRegistry {
SINGLETON(GameRegistry)

private:
    std::map<int, std::shared_ptr<Game>> games;

    GameRegistry() {}

public:
    int CreateGame(std::shared_ptr<Player> player, std::shared_ptr<DrawDeck> drawDeck);

    std::shared_ptr<Game> GetGame(int id);

    void DeleteGame(std::shared_ptr<Game> game);

    void DeleteGame(int id);
};

class PlayerRegistry {
SINGLETON(PlayerRegistry)

private:
    std::map<int, std::shared_ptr<Player>> players;

    PlayerRegistry() {}

public:
    int CreatePlayer();

    std::shared_ptr<Player> GetPlayer(int id);
};

#endif //BLACKJACK_REGISTRIES_HPP

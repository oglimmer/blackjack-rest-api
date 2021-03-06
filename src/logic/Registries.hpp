
#pragma  once


#include <thread>
#include "util.h"
#include "GameLogic.hpp"

class DrawDeckRegistry {
SINGLETON(DrawDeckRegistry)

private:
    std::map<int, std::shared_ptr<DrawDeck>> drawDecks;
    std::mutex drawDecks_mutex;

    DrawDeckRegistry() {}

public:
    int CreateDrawDeck();

    std::shared_ptr<DrawDeck> GetDrawDeck(int id);

    void ClearTimedout();
};

class GameRegistry {
SINGLETON(GameRegistry)

private:
    std::map<int, std::shared_ptr<Game>> games;
    std::mutex games_mutex;

    GameRegistry() {}

public:
    int CreateGame(std::shared_ptr<DrawDeck> drawDeck);

    std::shared_ptr<Game> GetGame(int id);

    void DeleteGame(std::shared_ptr<Game> game);

    void DeleteGame(int id);

    void ClearTimedout();
};

class PlayerRegistry {
SINGLETON(PlayerRegistry)

private:
    std::map<int, std::shared_ptr<Player>> players;
    std::mutex players_mutex;

    PlayerRegistry() {}

public:
    void CreatePlayer(std::shared_ptr<Player> player);

    std::shared_ptr<Player> GetPlayer(int id);

    void ClearTimedout();
};

void runWatchThread();

class TimeoutCleaner {
SINGLETON(TimeoutCleaner)

private:
    std::unique_ptr<std::thread> watchThread = std::unique_ptr<std::thread>(new std::thread(runWatchThread));

    TimeoutCleaner() {}

public:


};

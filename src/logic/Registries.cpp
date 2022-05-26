
#include "Registries.hpp"

/* ***************************************** DrawDeckRegistry ******************************************************* */

int DrawDeckRegistry::CreateDrawDeck() {
    auto drawDeck = Package::GetInstance().CreateDrawDeck();
    drawDeck->shuffle();
    int id = Rnd::GetInstance().GetEngine()();
    const std::lock_guard<std::mutex> lockGuard(drawDecks_mutex);
    drawDecks.insert_or_assign(id, drawDeck);
    return id;
}

std::shared_ptr<DrawDeck> DrawDeckRegistry::GetDrawDeck(int id) {
    const std::lock_guard<std::mutex> lockGuard(drawDecks_mutex);
    try {
        auto ret = drawDecks.at(id);
        ret->Use();
        return ret;
    } catch (std::out_of_range) {
        return nullptr;
    }
}

void DrawDeckRegistry::ClearTimedout() {
    const std::lock_guard<std::mutex> lockGuard(drawDecks_mutex);
    for (auto it = drawDecks.begin(); it != drawDecks.end();) {
        if (it->second->IsOutDated()) {
            OATPP_LOGD("DrawDeckRegistry", "[ClearTimedout] erasing %d", it->first);
            it = drawDecks.erase(it);
        } else {
            ++it;
        }
    }
    OATPP_LOGD("DrawDeckRegistry", "[ClearTimedout] Still %d drawDecks in registry.", drawDecks.size());
}

/* ***************************************** GameRegistry ******************************************************* */

int GameRegistry::CreateGame(std::shared_ptr<DrawDeck> drawDeck) {
    drawDeck->ReshuffleIfNeeded();
    auto game = std::shared_ptr<Game>(new Game(drawDeck));
    int id = Rnd::GetInstance().GetEngine()();
    const std::lock_guard<std::mutex> lockGuard(games_mutex);
    games.insert_or_assign(id, game);
    return id;
}

std::shared_ptr<Game> GameRegistry::GetGame(int id) {
    const std::lock_guard<std::mutex> lockGuard(games_mutex);
    try {
        auto ret = games.at(id);
        ret->Use();
        return ret;
    } catch (std::out_of_range) {
        return nullptr;
    }
}

void GameRegistry::DeleteGame(std::shared_ptr<Game> game) {
    const std::lock_guard<std::mutex> lockGuard(games_mutex);
    for (auto it = games.begin(); it != games.end();) {
        if (it->second == game) {
            it = games.erase(it);
        } else {
            ++it;
        }
    }
}

void GameRegistry::DeleteGame(int id) {
    const std::lock_guard<std::mutex> lockGuard(games_mutex);
    games.erase(id);
    OATPP_LOGD("GameRegistry", "[DeleteGame] id=%d", id);
}

void GameRegistry::ClearTimedout() {
    const std::lock_guard<std::mutex> lockGuard(games_mutex);
    for (auto it = games.begin(); it != games.end();) {
        if (it->second->IsOutDated()) {
            OATPP_LOGD("GameRegistry", "[ClearTimedout] erasing %d", it->first);
            it = games.erase(it);
        } else {
            ++it;
        }
    }
    OATPP_LOGD("GameRegistry", "[ClearTimedout] Still %d games in registry.", games.size());
}

/* ***************************************** PlayerRegistry ******************************************************* */

void PlayerRegistry::CreatePlayer(std::shared_ptr<Player> player) {
    int id = player->GetId();
    const std::lock_guard<std::mutex> lockGuard(players_mutex);
    players.insert_or_assign(id, player);
}

std::shared_ptr<Player> PlayerRegistry::GetPlayer(int id) {
    const std::lock_guard<std::mutex> lockGuard(players_mutex);
    try {
        auto ret = players.at(id);
        ret->Use();
        return ret;
    } catch (std::out_of_range) {
        return nullptr;
    }
}

void PlayerRegistry::ClearTimedout() {
    const std::lock_guard<std::mutex> lockGuard(players_mutex);
    for (auto it = players.begin(); it != players.end();) {
        if (it->second->IsOutDated()) {
            OATPP_LOGD("PlayerRegistry", "[ClearTimedout] erasing %d", it->first);
            it = players.erase(it);
        } else {
            ++it;
        }
    }
    OATPP_LOGD("PlayerRegistry", "[ClearTimedout] Still %d players in registry.", players.size());
}

/* ***************************************** misc ******************************************************* */

void runWatchThread() {
    OATPP_LOGD("runWatchThread", "Started");
    while (true) {
        PlayerRegistry::GetInstance().ClearTimedout();
        DrawDeckRegistry::GetInstance().ClearTimedout();
        GameRegistry::GetInstance().ClearTimedout();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 60 * 15));
    }
    OATPP_LOGD("runWatchThread", "Finished");
}

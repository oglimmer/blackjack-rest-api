
#include "Registries.hpp"

/* ***************************************** DrawDeckRegistry ******************************************************* */

int DrawDeckRegistry::CreateDrawDeck() {
    auto drawDeck = Package::GetInstance().CreateDrawDeck();
    drawDeck->shuffle();
    int id = Rnd::GetInstance().GetEngine()();
    auto lockGuard = std::lock_guard(mutex);
    drawDecks.insert_or_assign(id, drawDeck);
    return id;
}

std::shared_ptr<DrawDeck> DrawDeckRegistry::GetDrawDeck(int id) {
    auto lockGuard = std::lock_guard(mutex);
    auto ret = drawDecks[id];
    if (ret) {
        ret->Use();
    }
    return ret;
}

void DrawDeckRegistry::ClearTimedout() {
    auto lockGuard = std::lock_guard(mutex);
    for (auto it = drawDecks.begin(); it != drawDecks.end();) {
        OATPP_LOGD("DrawDeckRegistry", "[ClearTimedout] checking %d", it);
        if (it->second->IsOutDated()) {
            OATPP_LOGI("DrawDeckRegistry", "[ClearTimedout] erasing %d", it);
            it = drawDecks.erase(it);
        } else {
            ++it;
        }
    }
}

/* ***************************************** GameRegistry ******************************************************* */

int GameRegistry::CreateGame(std::shared_ptr<DrawDeck> drawDeck) {
    drawDeck->ReshuffleIfNeeded();
    auto game = std::shared_ptr<Game>(new Game(drawDeck));
    int id = Rnd::GetInstance().GetEngine()();
    auto lockGuard = std::lock_guard(mutex);
    games.insert_or_assign(id, game);
    return id;
}

std::shared_ptr<Game> GameRegistry::GetGame(int id) {
    auto lockGuard = std::lock_guard(mutex);
    auto ret = games[id];
    if (ret) {
        ret->Use();
    }
    return ret;
}

void GameRegistry::DeleteGame(std::shared_ptr<Game> game) {
    auto lockGuard = std::lock_guard(mutex);
    for (auto it = games.begin(); it != games.end();) {
        if (it->second == game) {
            it = games.erase(it);
        } else {
            ++it;
        }
    }
}

void GameRegistry::DeleteGame(int id) {
    auto lockGuard = std::lock_guard(mutex);
    games.erase(id);
    OATPP_LOGI("GameRegistry", "[DeleteGame] id=%d", id);
}

void GameRegistry::ClearTimedout() {
    auto lockGuard = std::lock_guard(mutex);
    for (auto it = games.begin(); it != games.end();) {
        OATPP_LOGD("GameRegistry", "[ClearTimedout] checking %d", it);
        if (it->second->IsOutDated()) {
            OATPP_LOGI("GameRegistry", "[ClearTimedout] erasing %d", it);
            it = games.erase(it);
        } else {
            ++it;
        }
    }
}

/* ***************************************** PlayerRegistry ******************************************************* */

void PlayerRegistry::CreatePlayer(std::shared_ptr<Player> player) {
    int id = player->GetId();
    auto lockGuard = std::lock_guard(mutex);
    players.insert_or_assign(id, player);
}

std::shared_ptr<Player> PlayerRegistry::GetPlayer(int id) {
    auto lockGuard = std::lock_guard(mutex);
    auto ret = players[id];
    if (ret) {
        ret->Use();
    }
    return ret;
}

void PlayerRegistry::ClearTimedout() {
    auto lockGuard = std::lock_guard(mutex);
    for (auto it = players.begin(); it != players.end();) {
        OATPP_LOGD("PlayerRegistry", "[ClearTimedout] checking %d", it);
        if (it->second->IsOutDated()) {
            OATPP_LOGI("PlayerRegistry", "[ClearTimedout] erasing %d", it);
            it = players.erase(it);
        } else {
            ++it;
        }
    }
}

/* ***************************************** misc ******************************************************* */

void runWatchThread() {
    OATPP_LOGD("runWatchThread", "Started");
    while (true) {
        OATPP_LOGD("runWatchThread", "run....");
        PlayerRegistry::GetInstance().ClearTimedout();
        DrawDeckRegistry::GetInstance().ClearTimedout();
        GameRegistry::GetInstance().ClearTimedout();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    OATPP_LOGD("runWatchThread", "Finished");
}

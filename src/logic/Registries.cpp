
#include "Registries.hpp"

/* ***************************************** DrawDeckRegistry ******************************************************* */

int DrawDeckRegistry::CreateDrawDeck() {
    auto drawDeck = Package::CreateDrawDeck();
    drawDeck->shuffle();
    int id = Rnd::GetInstance().GetEngine()();
    mutex.lock();
    drawDecks.insert_or_assign(id, drawDeck);
    mutex.unlock();
    return id;
}

std::shared_ptr<DrawDeck> DrawDeckRegistry::GetDrawDeck(int id) {
    mutex.lock();
    auto ret = drawDecks[id];
    ret->Use();
    mutex.unlock();
    return ret;
}

void DrawDeckRegistry::ClearTimedout() {
    mutex.lock();
    for (auto it = drawDecks.begin(); it != drawDecks.end();) {
        OATPP_LOGD("DrawDeckRegistry", "[ClearTimedout] checking %d", it);
        if (it->second->IsOutDated()) {
            OATPP_LOGI("DrawDeckRegistry", "[ClearTimedout] erasing %d", it);
            it = drawDecks.erase(it);
        } else {
            ++it;
        }
    }
    mutex.unlock();
}

/* ***************************************** GameRegistry ******************************************************* */

int GameRegistry::CreateGame(std::shared_ptr<Player> player, std::shared_ptr<DrawDeck> drawDeck) {
    drawDeck->ReshuffleIfNeeded();
    auto game = std::shared_ptr<Game>(new Game(player, drawDeck));
    int id = Rnd::GetInstance().GetEngine()();
    mutex.lock();
    games.insert_or_assign(id, game);
    mutex.unlock();
    return id;
}

std::shared_ptr<Game> GameRegistry::GetGame(int id) {
    mutex.lock();
    auto ret = games[id];
    ret->Use();
    mutex.unlock();
    return ret;
}

void GameRegistry::DeleteGame(std::shared_ptr<Game> game) {
    mutex.lock();
    for (auto it = games.begin(); it != games.end();) {
        if (it->second == game) {
            it = games.erase(it);
        } else {
            ++it;
        }
    }
    mutex.unlock();
}

void GameRegistry::DeleteGame(int id) {
    mutex.lock();
    games.erase(id);
    mutex.unlock();
}

void GameRegistry::ClearTimedout() {
    mutex.lock();
    for (auto it = games.begin(); it != games.end();) {
        OATPP_LOGD("GameRegistry", "[ClearTimedout] checking %d", it);
        if (it->second->IsOutDated()) {
            OATPP_LOGI("GameRegistry", "[ClearTimedout] erasing %d", it);
            it = games.erase(it);
        } else {
            ++it;
        }
    }
    mutex.unlock();
}

/* ***************************************** PlayerRegistry ******************************************************* */

int PlayerRegistry::CreatePlayer() {
    auto player = std::shared_ptr<Player>(new Player());
    int id = Rnd::GetInstance().GetEngine()();
    mutex.lock();
    players.insert_or_assign(id, player);
    mutex.unlock();
    return id;
}

std::shared_ptr<Player> PlayerRegistry::GetPlayer(int id) {
    mutex.lock();
    auto ret = players[id];
    ret->Use();
    mutex.unlock();
    return ret;
}

void PlayerRegistry::ClearTimedout() {
    mutex.lock();
    for (auto it = players.begin(); it != players.end();) {
        OATPP_LOGD("PlayerRegistry", "[ClearTimedout] checking %d", it);
        if (it->second->IsOutDated()) {
            OATPP_LOGI("PlayerRegistry", "[ClearTimedout] erasing %d", it);
            it = players.erase(it);
        } else {
            ++it;
        }
    }
    mutex.unlock();
}

/* ***************************************** misc ******************************************************* */

void runWatchThread() {
    OATPP_LOGD("runWatchThread", "Started");
    while(true) {
        OATPP_LOGD("runWatchThread", "run....");
        PlayerRegistry::GetInstance().ClearTimedout();
        DrawDeckRegistry::GetInstance().ClearTimedout();
        GameRegistry::GetInstance().ClearTimedout();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    OATPP_LOGD("runWatchThread", "Finished");
}

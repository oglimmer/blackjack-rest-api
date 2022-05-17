
#include "Registries.hpp"


int DrawDeckRegistry::CreateDrawDeck() {
    auto drawDeck = Package::CreateDrawDeck();
    drawDeck->shuffle();
    int id = Rnd::GetInstance().GetEngine()();
    drawDecks.insert_or_assign(id, drawDeck);
    return id;
}

std::shared_ptr<DrawDeck> DrawDeckRegistry::GetDrawDeck(int id) {
    return drawDecks[id];
}

int GameRegistry::CreateGame(std::shared_ptr<Player> player, std::shared_ptr<DrawDeck> drawDeck) {
    drawDeck->ReshuffleIfNeeded();
    auto game = std::shared_ptr<Game>(new Game(player, drawDeck));
    int id = Rnd::GetInstance().GetEngine()();
    games.insert_or_assign(id, game);
    return id;
}

std::shared_ptr<Game> GameRegistry::GetGame(int id) {
    return games[id];
}

void GameRegistry::DeleteGame(std::shared_ptr<Game> game) {
    for (auto it = games.begin(); it != games.end(); ++it) {
        if (it->second == game) {
            games.erase(it);
            break;
        }
    }
}

void GameRegistry::DeleteGame(int id) {
    games.erase(id);
}

int PlayerRegistry::CreatePlayer() {
    auto player = std::shared_ptr<Player>(new Player());
    int id = Rnd::GetInstance().GetEngine()();
    players.insert_or_assign(id, player);
    return id;
}

std::shared_ptr<Player> PlayerRegistry::GetPlayer(int id) {
    return players[id];
}
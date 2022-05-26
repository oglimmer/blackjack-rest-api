
#ifndef BLACKJACK_DBHIGHSCORE_HPP
#define BLACKJACK_DBHIGHSCORE_HPP

#include "controller/GameController.hpp"


#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"

#include "DTOs.hpp"
#include "CouchdbHighscoreClient.hpp"

using namespace oatpp::network;
using namespace oatpp::web;
using namespace oatpp::parser;

class DBHighscore {
SINGLETON(DBHighscore)

private:
    std::shared_ptr<CouchdbHighscoreClient> client;
    std::shared_ptr<json::mapping::ObjectMapper> objectMapper;
    std::string doc_id;
    std::string user;
    std::string password;
    std::string host;
    v_uint16 port;

    DBHighscore();

public:
    std::shared_ptr<DBHighscoreDto> Get() const;

    void Store(const std::vector<Highscore> &highscoreList);

};


#endif //BLACKJACK_DBHIGHSCORE_HPP

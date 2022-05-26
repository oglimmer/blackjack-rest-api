
#include "DBHighscore.hpp"

std::string EnvToString(const std::string &env, std::string defaultVal) {
    const char *raw = std::getenv(env.c_str());
    if (raw) {
        return std::string(raw);
    }
    return defaultVal;
}

v_uint16 EnvToInt(const std::string &env, int defaultVal) {
    const char *raw = std::getenv(env.c_str());
    if (raw) {
        return atoi(raw);
    }
    return defaultVal;
}

DBHighscore::DBHighscore() {
    doc_id = EnvToString("DOC_ID", {});
    host = EnvToString("HOST", "localhost");
    port = EnvToInt("PORT", 5984);
    user = EnvToString("USER", {});
    password = EnvToString("PASSWORD", {});
    if (!doc_id.empty()) {
        OATPP_LOGI("DBHighscore", "DB mode active. Using %s with user %s and doc_id %s", host.c_str(), user.c_str(), doc_id.c_str());
        auto connectionProvider = tcp::client::ConnectionProvider::createShared(
                {host, port, oatpp::network::Address::IP_4});
        auto requestExecutor = client::HttpRequestExecutor::createShared(connectionProvider);
        objectMapper = json::mapping::ObjectMapper::createShared();
        client = CouchdbHighscoreClient::createShared(requestExecutor, objectMapper);
    } else {
        OATPP_LOGI("DBHighscore", "DB mode deactivated.");
    }
}

std::shared_ptr<DBHighscoreDto> DBHighscore::Get() const {
    if (doc_id.empty()) {
        auto res = std::make_shared<DBHighscoreDto>();
        res->highscore = {};
        return res;
    }
    std::string Authorization(user + ":" + password);
    auto resp = client->getResult(doc_id, Authorization);
    if (resp->getStatusCode() > 299) {
        OATPP_LOGE("DBHighscore", "Failed to load highscore, %d", resp->getStatusCode())
        auto res = std::make_shared<DBHighscoreDto>();
        res->highscore = {};
        return res;
    }
    auto respDto = resp->readBodyToDto<oatpp::Object<DBHighscoreDto >>(
            objectMapper.get());
    return respDto.getPtr();
}

void DBHighscore::Store(const std::vector<Highscore> &highscoreList) {
    if (doc_id.empty()) {
        return;
    }
    std::string Authorization(user + ":" + password);
    auto loadedDto = client->getResult(doc_id, Authorization)->readBodyToDto<oatpp::Object<DBHighscoreDto >>(
            objectMapper.get());
    loadedDto->highscore->clear();
    std::for_each(highscoreList.begin(), highscoreList.end(),
                  [&](const Highscore &ele) {
                      auto newEle = DBHighscoreElementDto::Wrapper::createShared();
                      newEle->name = ele.GetName();
                      newEle->money = ele.GetMaxMoney();
                      newEle->id = ele.GetId();
                      loadedDto->highscore->push_back(newEle);
                  });

    auto resp = client->storeHighscore(doc_id, loadedDto, Authorization);

    if (resp->getStatusCode() > 299) {
        OATPP_LOGE("DBHighscore", "Failed to save highscore, %d", resp->getStatusCode())
    }
}

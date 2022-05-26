#include "GameController.hpp"

int GameController::CreatePlayer(const std::shared_ptr<IncomingRequest> &request) {
    auto &reg = PlayerRegistry::GetInstance();
    int id = Rnd::GetInstance().GetEngine()();
    try {
        auto requestDto = request->readBodyToDto<oatpp::Object<CreatePlayerRequest>>(getDefaultObjectMapper());
        reg.CreatePlayer(std::shared_ptr<Player>(new Player(id, requestDto->name)));
    } catch (...) {
        reg.CreatePlayer(std::shared_ptr<Player>(new Player(id)));
    }
    return id;
}

void GameController::LogAccess(const std::shared_ptr<IncomingRequest> &request, const std::string &endpoint) {
    std::string ip("no X-Forwarded-For");
    std::string ua("no user-agent");
    for (const auto &pair: request->getHeaders().getAll()) {
//        OATPP_LOGD("GameController", ("[" + endpoint + "] '%s' = '%s'").c_str(), pair.first.getData(), pair.second.getData());
        if (std::string(static_cast<const char *>(pair.first.getData())) == "X-Forwarded-For") {
            ip = static_cast<const char *>(pair.second.getData());
        }
        if (std::string(static_cast<const char *>(pair.first.getData())) == "User-Agent") {
            ua = static_cast<const char *>(pair.second.getData());
        }
    }
    OATPP_LOGI("GameController", ("[" + endpoint + "] '%s' '%s'").c_str(), ip.c_str(), ua.c_str());
}
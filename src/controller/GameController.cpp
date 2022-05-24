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
    auto &context = request->getConnection()->getInputStreamContext();
    for (const auto &pair: context.getProperties().getAll()) {
        if (std::string(static_cast<const char *>(pair.first.getData())) == "peer_address") {
            OATPP_LOGI("GameController", ("[" + endpoint + "] '%s'").c_str(), pair.second.getData());
        }
    }
}
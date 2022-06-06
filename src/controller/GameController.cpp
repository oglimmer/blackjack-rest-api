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

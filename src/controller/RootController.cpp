
#include "RootController.hpp"

void RootController::LogAccess(const std::shared_ptr<IncomingRequest> &request, const std::string &endpoint) {
    std::string ip("no X-Forwarded-For");
    for (const auto &pair: request->getHeaders().getAll()) {
        OATPP_LOGD("GameController", ("[" + endpoint + "] '%s' = '%s'").c_str(), pair.first.getData(), pair.second.getData());
        if (std::string(static_cast<const char *>(pair.first.getData())) == "X-Forwarded-For") {
            ip = static_cast<const char *>(pair.second.getData());
        }
    }
    OATPP_LOGI("GameController", ("[" + endpoint + "] '%s'").c_str(), ip.c_str());
}
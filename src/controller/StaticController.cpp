

#include "StaticController.hpp"


bool StaticController::hasEnding(std::string fullString, std::string const &ending) {
    std::transform(fullString.begin(), fullString.end(), fullString.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

std::string StaticController::getMimeType(const std::string &filename) {
    if (hasEnding(filename, ".html")) return "text/html";
    if (hasEnding(filename, ".jpg")) return "image/jpeg";
    if (hasEnding(filename, ".jpeg")) return "image/jpeg";
    if (hasEnding(filename, ".png")) return "image/png";
    if (hasEnding(filename, ".gif")) return "image/gif";
    if (hasEnding(filename, ".css")) return "text/css";
    if (hasEnding(filename, ".js")) return "text/javascript";
    if (hasEnding(filename, ".xml")) return "text/xml";
    return "text/plain";
}

std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> StaticController::serviceFile(const String &filename) {
    const char *rootDir = std::getenv("STATIC_ROOT");
    const String &fullFilepath = rootDir + filename;
    std::ifstream t(fullFilepath);
    if (t.good()) {
        std::stringstream buffer;
        buffer << t.rdbuf();
        auto resp = createResponse(Status::CODE_200, buffer.str().c_str());
        resp->putHeader("Content-Type", getMimeType(filename));
        return resp;
    } else {
        return createResponse(Status::CODE_404, "");
    }
}
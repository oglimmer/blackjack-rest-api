#pragma once

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/incoming/Request.hpp"
#include <sstream>
#include <fstream>

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen


class StaticController : public oatpp::web::server::api::ApiController {
private:
    std::string getMimeType(const std::string &filename);

    bool hasEnding(std::string fullString, std::string const &ending);

    std::shared_ptr<oatpp::web::protocol::http::outgoing::Response> serviceFile(const String &filename);

public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    explicit StaticController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            : oatpp::web::server::api::ApiController(objectMapper) {
    }

    ENDPOINT("GET", "/{filename}", getUIResource, PATH(String, filename)) {
        return serviceFile(filename);
    }

    ENDPOINT("GET", "/ace/{filename}", getUIResourceAce, PATH(String, filename)) {
        return serviceFile("ace/" + filename);
    }

    ENDPOINT("GET", "/ace/snippets/{filename}", getUIResourceAceSnippets, PATH(String, filename)) {
        return serviceFile("ace/snippets/" + filename);
    }

};


#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen



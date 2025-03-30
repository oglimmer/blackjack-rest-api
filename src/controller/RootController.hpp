#pragma  once


#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/incoming/Request.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

class RootController : public oatpp::web::server::api::ApiController {
public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    explicit RootController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            : oatpp::web::server::api::ApiController(objectMapper) {
    }


    ENDPOINT("GET", "/", rootRedirect, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        auto resp = createResponse(Status::CODE_301, "");
        resp->putHeader("Location", "/index.html");
        return resp;
    }


    ENDPOINT("GET", "/v2/", root, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        auto resp = createResponse(Status::CODE_301, "");
        resp->putHeader("Location", "/swagger/ui");
        return resp;
    }

    ENDPOINT("GET", "/health", info) {
        return createResponse(Status::CODE_200, "");
    }

};


#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen


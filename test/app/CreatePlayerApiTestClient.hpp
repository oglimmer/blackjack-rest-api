
#ifndef CreatePlayerApiTestClient_hpp
#define CreatePlayerApiTestClient_hpp

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

/* Begin Api Client code generation */
#include OATPP_CODEGEN_BEGIN(ApiClient)

/**
 * CreatePlayer API client.
 * Use this client to call application APIs.
 */
class CreatePlayerApiTestClient : public oatpp::web::client::ApiClient {

API_CLIENT_INIT(CreatePlayerApiTestClient)

    API_CALL("POST", "/player", createPlayer)

    // TODO - add more client API calls here

};

/* End Api Client code generation */
#include OATPP_CODEGEN_END(ApiClient)

#endif // CreatePlayerApiTestClient_hpp

#pragma  once


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

    API_CALL("POST", "/v2/player", createPlayer)

    API_CALL("POST", "/v2/deck", createDeck)

    API_CALL("POST", "/v2/game", createGame, BODY_DTO(Object < CreateGameRequest > , createGameDto))

    API_CALL("POST", "/v2/game/{gameId}/bet", createBet,
             BODY_DTO(Object < BetRequest > , betRequest),
             PATH(Int32, gameId))

    API_CALL("POST", "/v2/game/{gameId}/bet/{betId}/stand", stand,
             PATH(Int32, betId),
             PATH(Int32, gameId))

    API_CALL("POST", "/v2/game/{gameId}/bet/{betId}/insurance", insurance,
             PATH(Int32, betId),
             PATH(Int32, gameId),
             BODY_DTO(Object < InsuranceRequest > , insuranceRequest))

    API_CALL("POST", "/v2/game/{gameId}/bet/{betId}/split", split,
             PATH(Int32, betId),
             PATH(Int32, gameId))

    API_CALL("GET", "/v2/game/{gameId}/bet/{betId}", getResult,
             PATH(Int32, betId),
             PATH(Int32, gameId))

};

/* End Api Client code generation */
#include OATPP_CODEGEN_END(ApiClient)

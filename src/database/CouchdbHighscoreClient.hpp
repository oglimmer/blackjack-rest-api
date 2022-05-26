

#ifndef BLACKJACK_COUCHDBHIGHSCORECLIENT_HPP
#define BLACKJACK_COUCHDBHIGHSCORECLIENT_HPP


#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "DTOs.hpp"

#include OATPP_CODEGEN_BEGIN(ApiClient)

class CouchdbHighscoreClient : public oatpp::web::client::ApiClient {

API_CLIENT_INIT(CouchdbHighscoreClient)

    API_CALL("PUT", "/blackjack/{_id}", storeHighscore,
             PATH(String, _id),
             BODY_DTO(Object < DBHighscoreDto > , dbEntity),
             AUTHORIZATION_BASIC(String, authString))

    API_CALL("GET", "/blackjack/{_id}", getResult,
             PATH(String, _id),
             AUTHORIZATION_BASIC(String, authString))

};

#include OATPP_CODEGEN_END(ApiClient)


#endif //BLACKJACK_COUCHDBHIGHSCORECLIENT_HPP

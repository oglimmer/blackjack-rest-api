#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class EndResponse : public oatpp::DTO {

    DTO_INIT(EndResponse, DTO)

    DTO_FIELD(String, dealersSecondCard);
    DTO_FIELD(Int32, dealerTotal);
    DTO_FIELD(Vector<String>, dealersAdditionalCard);

    DTO_FIELD(String, result);
};

class BetRequest : public oatpp::DTO {
    DTO_INIT(BetRequest, DTO)

    DTO_FIELD(Int32, playerId);
    DTO_FIELD(Int32, bet);
};

class BetResponse : public EndResponse {

    DTO_INIT(BetResponse, EndResponse)

    DTO_FIELD(String, card1);
    DTO_FIELD(String, card2);
    DTO_FIELD(String, dealersCard);
    DTO_FIELD(Int32, yourTotal);

    DTO_FIELD(Int32, betId);
};

class HitResponse : public EndResponse {

    DTO_INIT(HitResponse, EndResponse)

    DTO_FIELD(String, drawnCard);
    DTO_FIELD(Int32, yourTotal);

};

class StandResponse : public EndResponse {

    DTO_INIT(StandResponse, EndResponse)

};


class GetPlayerResponse : public oatpp::DTO {

    DTO_INIT(GetPlayerResponse, DTO)

    DTO_FIELD(Int32, cash);
};



class CreateGameDto : public oatpp::DTO {
    DTO_INIT(CreateGameDto, DTO)
    DTO_FIELD(Int32, deckId);
};

class CreateGameResponse : public oatpp::DTO {
    DTO_INIT(CreateGameResponse, DTO)
    DTO_FIELD(Int32, gameId);
};

class CreatePlayerResponse : public oatpp::DTO {
    DTO_INIT(CreatePlayerResponse, DTO)
    DTO_FIELD(Int32, playerId);
};

class CreateDeckResponse : public oatpp::DTO {
    DTO_INIT(CreateDeckResponse, DTO)
    DTO_FIELD(Int32, deckId);
};


#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */

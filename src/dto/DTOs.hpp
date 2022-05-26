#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

class GameException : public std::exception {
public:
    explicit GameException(const char *message) : msg_(message) {}

    explicit GameException(const std::string &message) : msg_(message) {}

    virtual ~GameException() noexcept {}

    virtual const char *what() const noexcept {
        return msg_.c_str();
    }

protected:
    std::string msg_;
};

#include OATPP_CODEGEN_BEGIN(DTO)

class BetGetResponse : public oatpp::DTO {

    DTO_INIT(BetGetResponse, DTO)

    DTO_FIELD(String, dealersSecondCard);

    DTO_FIELD(Int32, dealerTotal);

    DTO_FIELD(Vector < String >, dealersAdditionalCard);

    DTO_FIELD(String, result);
};

class BetRequest : public oatpp::DTO {
    DTO_INIT(BetRequest, DTO)

    DTO_FIELD(Int32, playerId);

    DTO_FIELD(Int32, bet);
};

class BetResponse : public oatpp::DTO {

    DTO_INIT(BetResponse, oatpp::DTO)

    DTO_FIELD(String, card1);

    DTO_FIELD(String, card2);

    DTO_FIELD(String, dealersCard);

    DTO_FIELD(Int32, yourTotal);

    DTO_FIELD(Vector < String >, followActions);

    DTO_FIELD(Int32, betId);
};

class SplitResponse : public oatpp::DTO {

    DTO_INIT(SplitResponse, oatpp::DTO)

    DTO_FIELD(String, firstBetCard1);

    DTO_FIELD(String, firstBetCard2);

    DTO_FIELD(Int32, firstBetTotal);

    DTO_FIELD(String, secondBetCard1);

    DTO_FIELD(String, secondBetCard2);

    DTO_FIELD(Int32, secondBetTotal);

    DTO_FIELD(Vector < String >, followActions);

    DTO_FIELD(Vector < String >, secondBetFollowAction);

    DTO_FIELD(Int32, secondBetId);
};

class HitResponse : public oatpp::DTO {

    DTO_INIT(HitResponse, oatpp::DTO)

    DTO_FIELD(String, drawnCard);

    DTO_FIELD(Int32, yourTotal);

    DTO_FIELD(Vector < String >, followActions);
};

class StandResponse : public oatpp::DTO {

    DTO_INIT(StandResponse, oatpp::DTO)

    DTO_FIELD(Vector < String >, followActions); // this is always [] which makes the evaluation in bash easier
};


class GetPlayerResponse : public oatpp::DTO {

    DTO_INIT(GetPlayerResponse, DTO)

    DTO_FIELD(Int32, cash);
};


class CreateGameRequest : public oatpp::DTO {
    DTO_INIT(CreateGameRequest, DTO)

    DTO_FIELD(Int32, deckId);
};

class CreateGameResponse : public oatpp::DTO {
    DTO_INIT(CreateGameResponse, DTO)

    DTO_FIELD(Int32, gameId);
};

class CreatePlayerRequest : public oatpp::DTO {
    DTO_INIT(CreatePlayerRequest, DTO)

    DTO_FIELD(String, name);
};

class CreatePlayerResponse : public oatpp::DTO {
    DTO_INIT(CreatePlayerResponse, DTO)

    DTO_FIELD(Int32, playerId);
};

class CreateDeckResponse : public oatpp::DTO {
    DTO_INIT(CreateDeckResponse, DTO)

    DTO_FIELD(Int32, deckId);
};

class InsuranceRequest : public oatpp::DTO {
    DTO_INIT(InsuranceRequest, DTO)

    DTO_FIELD(String, insurance);
};

class HighscoreElementResponse : public oatpp::DTO {
    DTO_INIT(HighscoreElementResponse, DTO)

    DTO_FIELD(Int32, pos);

    DTO_FIELD(String, name);

    DTO_FIELD(Int32, money);
};

class HighscoreResponse : public oatpp::DTO {
    DTO_INIT(HighscoreResponse, DTO)

    DTO_FIELD(Vector < Object < HighscoreElementResponse >>, highscores);
};


#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */

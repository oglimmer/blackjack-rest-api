#ifndef GameController_hpp
#define GameController_hpp

#include "dto/DTOs.hpp"
#include "logic/GameLogic.hpp"
#include "logic/Registries.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/incoming/Request.hpp"

class CustomErrorHandler : public oatpp::base::Countable, public oatpp::web::server::handler::ErrorHandler {
public:
    CustomErrorHandler() = default;

public:
    static std::shared_ptr<CustomErrorHandler> createShared() {
        return std::make_shared<CustomErrorHandler>();
    }

    std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
    handleError(const std::exception_ptr &error) override {
        try {
            std::rethrow_exception(error);
        } catch (const oatpp::web::protocol::CommunicationError &e) {
            return oatpp::web::protocol::http::outgoing::ResponseFactory::createResponse(
                    oatpp::web::protocol::http::Status::CODE_400, e.what());
        } catch (const GameException &e) {
            return oatpp::web::protocol::http::outgoing::ResponseFactory::createResponse(
                    oatpp::web::protocol::http::Status::CODE_400, e.what());
        } catch (const std::runtime_error &e) {
            return oatpp::web::protocol::http::outgoing::ResponseFactory::createResponse(
                    oatpp::web::protocol::http::Status::CODE_500, e.what());
        } catch (...) {
            throw;
        }
    }

    std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
    handleError(const oatpp::web::protocol::http::Status &status, const oatpp::String &message,
                const Headers &headers) override {
        throw std::logic_error("Function not implemented");
    }

};

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen
#include "logic/HighscoreList.hpp"

class GameController : public oatpp::web::server::api::ApiController {
public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    explicit GameController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            : oatpp::web::server::api::ApiController(objectMapper) {
        setErrorHandler(CustomErrorHandler::createShared());
    }

private:
    int CreatePlayer(const std::shared_ptr<IncomingRequest> &request);

    void LogAccess(const std::shared_ptr<IncomingRequest> &request, const std::string &endpoint);

public:


    ENDPOINT_INFO(createPlayer) {
        info->summary = "Creates a new Player object";
        info->addConsumes < Object < CreatePlayerRequest >> ("application/json");
        info->addResponse < Object < CreatePlayerResponse >> (Status::CODE_200, "application/json");
    }

    ENDPOINT("POST", "/v2/player", createPlayer, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/player");
        auto id = CreatePlayer(request);
        auto dto = CreatePlayerResponse::createShared();
        dto->playerId = id;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(getPlayer) {
        info->summary = "Returns the cash a player owns";
        info->addResponse < Object < GetPlayerResponse >> (Status::CODE_200, "application/json");
        info->pathParams["playerId"].description = "Player ID, retrieved by POST /v2/player";
    }

    ENDPOINT("GET", "/v2/player/{playerId}", getPlayer,
             PATH(Int32, playerId),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "GET /v2/player/" + std::to_string(playerId));
        auto &reg = PlayerRegistry::GetInstance();
        auto player = reg.GetPlayer(playerId);
        if (!player) {
            return createResponse(Status::CODE_404, "Unable to find player");
        }

        auto dto = GetPlayerResponse::createShared();
        dto->cash = player->GetCash();
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(createDrawDeck) {
        info->summary = "Creates a new Deck object";
        info->addResponse < Object < CreateDeckResponse >> (Status::CODE_200, "application/json");
    }

    ENDPOINT("POST", "/v2/deck", createDrawDeck, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/deck");
        auto &reg = DrawDeckRegistry::GetInstance();
        auto id = reg.CreateDrawDeck();

        auto dto = CreateDeckResponse::createShared();
        dto->deckId = id;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(createGame) {
        info->summary = "Creates a new Game.";
        info->addConsumes < Object < CreateGameRequest >> ("application/json");
        info->addResponse < Object < CreateDeckResponse >> (Status::CODE_200, "application/json");
    }

    ENDPOINT("POST", "/v2/game", createGame,
             BODY_DTO(Object < CreateGameRequest > , createGameDto),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/game");
        auto &drawDeckReg = DrawDeckRegistry::GetInstance();
        auto &gameReg = GameRegistry::GetInstance();
        auto drawDeck = drawDeckReg.GetDrawDeck(createGameDto->deckId.getValue(-1));

        if (!drawDeck) {
            return createResponse(Status::CODE_404, "Unable to find deck");
        }

        auto id = gameReg.CreateGame(drawDeck);

        auto dto = CreateGameResponse::createShared();
        dto->gameId = id;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(placeBet) {
        info->summary = "Places the initial bet on a game.";
        info->addConsumes < Object < BetRequest >> ("application/json");
        info->addResponse < Object < BetResponse >> (Status::CODE_200, "application/json");
        info->pathParams["gameId"].description = "Game ID, retrieved by POST /v2/game";
    }

    ENDPOINT("POST", "/v2/game/{gameId}/bet", placeBet,
             BODY_DTO(Object < BetRequest > , betRequest),
             PATH(Int32, gameId),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/game/" + std::to_string(gameId) + "/bet");
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }
        auto &playerReg = PlayerRegistry::GetInstance();
        auto player = playerReg.GetPlayer(betRequest->playerId.getValue(-1));
        if (!player) {
            return createResponse(Status::CODE_404, "Unable to find player");
        }

        auto dto = BetResponse::createShared();
        game->PlaceBet(betRequest->bet, player, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(doubleBet) {
        info->summary = "Doubles the bet and takes another card. This option is only available when the first two cards are 9,10,11 in total.";
        info->addResponse < Object < HitResponse >> (Status::CODE_200, "application/json");
        info->pathParams["gameId"].description = "Game ID, retrieved by POST /v2/game";
        info->pathParams["betId"].description = "Bet ID, retrieved by POST /v2/game/<GAMEID>/bet";
    }

    ENDPOINT("POST", "/v2/game/{gameId}/bet/{betId}/double", doubleBet,
             PATH(Int32, gameId),
             PATH(Int32, betId),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/game/" + std::to_string(gameId) + "/bet/" + std::to_string(betId) + "/double");
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }
        auto bet = game->GetBet(betId);
        if (!bet) {
            return createResponse(Status::CODE_404, "Unable to find bet");
        }
        if (bet->GetStand()) {
            return createResponse(Status::CODE_404, "Bet already finished");
        }

        auto dto = HitResponse::createShared();
        game->DoubleBet(bet, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(split) {
        info->summary = "Split your hand into two hands. Also draws 2 additional cards for each hand. This option is only available when the first two cards are of the same rank.";
        info->addResponse < Object < SplitResponse >> (Status::CODE_200, "application/json");
        info->pathParams["gameId"].description = "Game ID, retrieved by POST /v2/game";
        info->pathParams["betId"].description = "Bet ID, retrieved by POST /v2/game/<GAMEID>/bet";
    }

    ENDPOINT("POST", "/v2/game/{gameId}/bet/{betId}/split", split,
             PATH(Int32, gameId),
             PATH(Int32, betId),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/game/" + std::to_string(gameId) + "/bet/" + std::to_string(betId) + "/split");
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }
        auto bet = game->GetBet(betId);
        if (!bet) {
            return createResponse(Status::CODE_404, "Unable to find bet");
        }
        if (bet->GetStand()) {
            return createResponse(Status::CODE_404, "Bet already finished");
        }

        auto dto = SplitResponse::createShared();
        game->Split(bet, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(hit) {
        info->summary = "Hit - takes another card.";
        info->addResponse < Object < HitResponse >> (Status::CODE_200, "application/json");
        info->pathParams["gameId"].description = "Game ID, retrieved by POST /v2/game";
        info->pathParams["betId"].description = "Bet ID, retrieved by POST /v2/game/<GAMEID>/bet";
    }

    ENDPOINT("POST", "/v2/game/{gameId}/bet/{betId}/hit", hit,
             PATH(Int32, gameId),
             PATH(Int32, betId),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/game/" + std::to_string(gameId) + "/bet/" + std::to_string(betId) + "/hit");
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }
        auto bet = game->GetBet(betId);
        if (!bet) {
            return createResponse(Status::CODE_404, "Unable to find bet");
        }
        if (bet->GetStand()) {
            return createResponse(Status::CODE_404, "Bet already finished");
        }

        auto dto = HitResponse::createShared();
        game->Hit(bet, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(stand) {
        info->summary = "Stand - Finishes the game, thus let's the dealer take cards and finally pays the winner.";
        info->addResponse < Object < StandResponse >> (Status::CODE_200, "application/json");
        info->pathParams["gameId"].description = "Game ID, retrieved by POST /v2/game";
        info->pathParams["betId"].description = "Bet ID, retrieved by POST /v2/game/<GAMEID>/bet";
    }

    ENDPOINT("POST", "/v2/game/{gameId}/bet/{betId}/stand", stand,
             PATH(Int32, gameId),
             PATH(Int32, betId),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/game/" + std::to_string(gameId) + "/bet/" + std::to_string(betId) + "/stand");
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }
        auto bet = game->GetBet(betId);
        if (!bet) {
            return createResponse(Status::CODE_404, "Unable to find bet");
        }
        if (bet->GetStand()) {
            return createResponse(Status::CODE_404, "Bet already finished");
        }
        auto dto = StandResponse::createShared();
        game->Stand(bet, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(insurance) {
        info->summary = "Answers the dealer's question for an insurance. This option is only available when the dealer's first card as an Ace.";
        info->addResponse < Object < StandResponse >> (Status::CODE_200, "application/json");
        info->addConsumes < Object < InsuranceRequest >> ("application/json");
        info->pathParams["gameId"].description = "Game ID, retrieved by POST /v2/game";
        info->pathParams["betId"].description = "Bet ID, retrieved by POST /v2/game/<GAMEID>/bet";
    }

    ENDPOINT("POST", "/v2/game/{gameId}/bet/{betId}/insurance", insurance,
             PATH(Int32, gameId),
             PATH(Int32, betId),
             BODY_DTO(Object < InsuranceRequest > , insuranceRequest),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/game/" + std::to_string(gameId) + "/bet/" + std::to_string(betId) + "/insurance");
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }
        auto bet = game->GetBet(betId);
        if (!bet) {
            return createResponse(Status::CODE_404, "Unable to find bet");
        }
        std::string buyInsurance = insuranceRequest->insurance;
        std::transform(buyInsurance.begin(), buyInsurance.end(), buyInsurance.begin(), ::tolower);
        bool bInsurance = buyInsurance == "true" || buyInsurance == "yes" || buyInsurance == "y";
        auto dto = StandResponse::createShared();
        game->Insurance(bInsurance, bet, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(getBet) {
        info->summary = "Returns information about the dealer's cards, the dealer's total Value and who won the game.";
        info->addResponse < Object < BetGetResponse >> (Status::CODE_200, "application/json");
        info->pathParams["gameId"].description = "Game ID, retrieved by POST /v2/game";
        info->pathParams["betId"].description = "Bet ID, retrieved by POST /v2/game/<GAMEID>/bet";
    }

    ENDPOINT("GET", "/v2/game/{gameId}/bet/{betId}", getBet,
             PATH(Int32, gameId),
             PATH(Int32, betId),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "GET /v2/game/" + std::to_string(gameId) + "/bet/" + std::to_string(betId) + "");
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }
        auto bet = game->GetBet(betId);
        if (!bet) {
            return createResponse(Status::CODE_404, "Unable to find bet");
        }
        auto dto = BetGetResponse::createShared();
        if (game->AddResponse(bet, dto)) {
            GameRegistry::GetInstance().DeleteGame(gameId);
        }
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(highscore) {
        info->summary = "Returns the highscore.";
        info->addResponse<String>(Status::CODE_200, "application/json");
    }

    ENDPOINT("GET", "/v2/highscore", highscore,
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "GET /v2/highscore");
        auto list = HighscoreList::GetInstance().GetHighscores();
        std::stringstream s;
        std::copy(list->begin(), list->end(), std::ostream_iterator<std::string>(s, "\r\n"));
        return createResponse(Status::CODE_200, s.str());
    }

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* GameController_hpp */

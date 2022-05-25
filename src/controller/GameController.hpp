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

    ENDPOINT("GET", "/", rootRedirect, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "GET /");
        auto resp = createResponse(Status::CODE_301);
        resp->putHeader("Location", "/v2/");
        return resp;
    }

    ENDPOINT("GET", "/v2/", root, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
//        LogAccess(request, "GET /v2/");
        return createResponse(Status::CODE_200,
                              "## Create your player (do once)\ncurl https://bj.oglimmer.de/v2/player -X POST\n\n## Create a stack of cards (called deck, do this once):\ncurl https://bj.oglimmer.de/v2/deck -X POST\n\n## Check your cash:\ncurl https://bj.oglimmer.de/v2/player/${PLAYER_ID}\n\n## Start a single game (re-use player and deck)\ncurl https://bj.oglimmer.de/v2/game -d '{\"deckId\": '${DECK_ID}'}'\n\n## Put your bet on a newly started game (once per game)\ncurl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet -d '{\"playerId\": '${PLAYER_ID}', \"bet\": '${BET}'}'\n\n## Either Hit (take a card, do this as long as you want more cards)\ncurl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}/hit -X POST\n## or Stand (finish and let the dealer draw cards):\ncurl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}/stand -X POST\n## or Double (if your initial cards are 9,10,11 in total)\ncurl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}/double -X POST\n## or Split (if your initial cards are of the same rank)\ncurl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}/split -X POST\n## when the dealer has an Ace on the first card, you are asked for an insurance:\ncurl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}/insurance -d '{\"insurance\": \"${yes|no}\"}'\n\n## When the 'followActions' are [] use this to get the end result\ncurl https://bj.oglimmer.de/v2/game/${GAME_ID}/bet/${BET_ID}\n\nUse: https://raw.githubusercontent.com/oglimmer/blackjack-client-server/master/play.sh to play via bash! (needs curl and jq installed)\n");
    }

    ENDPOINT("POST", "/v2/player", createPlayer, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/player");
        auto id = CreatePlayer(request);
        auto dto = CreatePlayerResponse::createShared();
        dto->playerId = id;
        return createDtoResponse(Status::CODE_200, dto);
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

    ENDPOINT("POST", "/v2/deck", createDrawDeck, REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/deck");
        auto &reg = DrawDeckRegistry::GetInstance();
        auto id = reg.CreateDrawDeck();

        auto dto = CreateDeckResponse::createShared();
        dto->deckId = id;
        return createDtoResponse(Status::CODE_200, dto);
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

    ENDPOINT("POST", "/v2/game/{gameId}/bet/{bet}", deprecatedBet,
             PATH(Int32, gameId),
             PATH(Int32, bet),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/game/" + std::to_string(gameId) + "/bet/" + std::to_string(bet));
        return createResponse(Status::CODE_304, "Endpoint deprecated. Use /game/{gameId}/bet with '{playerId, bet}'");
    }

    ENDPOINT("POST", "/v2/game/{gameId}/hit", deprecatedHit,
             PATH(Int32, gameId),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/game/" + std::to_string(gameId) + "/hit");
        return createResponse(Status::CODE_304, "Endpoint deprecated. Use /game/{gameId}/bet/{betId}/hit");
    }

    ENDPOINT("POST", "/v2/game/{gameId}/stand", deprecatedStand,
             PATH(Int32, gameId),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        LogAccess(request, "POST /v2/game/" + std::to_string(gameId) + "/stand");
        return createResponse(Status::CODE_304, "Endpoint deprecated. Use /game/{gameId}/bet/{betId}/stand");
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
            //GameRegistry::GetInstance().DeleteGame(gameId); this needs to give each bet a chance to GET
        }
        return createDtoResponse(Status::CODE_200, dto);
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

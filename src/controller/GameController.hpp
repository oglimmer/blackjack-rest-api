#ifndef GameController_hpp
#define GameController_hpp

#include "dto/DTOs.hpp"
#include "logic/GameLogic.hpp"
#include "logic/Registries.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

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

public:

    ENDPOINT("GET", "/v2/", root) {
        return createResponse(Status::CODE_200);
    }

    ENDPOINT("POST", "/v2/player", createPlayer) {
        auto &reg = PlayerRegistry::GetInstance();
        auto id = reg.CreatePlayer();

        auto dto = CreatePlayerResponse::createShared();
        dto->playerId = id;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("GET", "/v2/player/{playerId}", getPlayer, PATH(Int32, playerId)) {
        auto &reg = PlayerRegistry::GetInstance();
        auto player = reg.GetPlayer(playerId);
        if (!player) {
            return createResponse(Status::CODE_404, "Unable to find player");
        }

        auto dto = GetPlayerResponse::createShared();
        dto->cash = player->GetCash();
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/v2/deck", createDrawDeck) {
        auto &reg = DrawDeckRegistry::GetInstance();
        auto id = reg.CreateDrawDeck();

        auto dto = CreateDeckResponse::createShared();
        dto->deckId = id;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/v2/game", createGame, BODY_DTO(Object<CreateGameRequest>, createGameDto)) {
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
             PATH(Int32, gameId)) {
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
             PATH(Int32, bet)) {
        return createResponse(Status::CODE_304, "Endpoint deprecated. Use /game/{gameId}/bet with '{playerId, bet}'");
    }

    ENDPOINT("POST", "/v2/game/{gameId}/hit", deprecatedHit,
             PATH(Int32, gameId)) {
        return createResponse(Status::CODE_304, "Endpoint deprecated. Use /game/{gameId}/bet/{betId}/hit");
    }

    ENDPOINT("POST", "/v2/game/{gameId}/stand", deprecatedStand,
             PATH(Int32, gameId)) {
        return createResponse(Status::CODE_304, "Endpoint deprecated. Use /game/{gameId}/bet/{betId}/stand");
    }

    ENDPOINT("POST", "/v2/game/{gameId}/bet/{betId}/double", doubleBet,
             PATH(Int32, gameId),
             PATH(Int32, betId)) {
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }
        auto bet = game->GetBet(betId);
        if (!bet) {
            return createResponse(Status::CODE_404, "Unable to find bet");
        }

        auto dto = HitResponse::createShared();
        game->DoubleBet(bet, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/v2/game/{gameId}/bet/{betId}/split", split,
             PATH(Int32, gameId),
             PATH(Int32, betId)) {
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }
        auto bet = game->GetBet(betId);
        if (!bet) {
            return createResponse(Status::CODE_404, "Unable to find bet");
        }

        auto dto = SplitResponse::createShared();
        game->Split(bet, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/v2/game/{gameId}/bet/{betId}/hit", hit,
             PATH(Int32, gameId),
             PATH(Int32, betId)) {
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }
        auto bet = game->GetBet(betId);
        if (!bet) {
            return createResponse(Status::CODE_404, "Unable to find bet");
        }

        auto dto = HitResponse::createShared();
        game->Hit(bet, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/v2/game/{gameId}/bet/{betId}/stand", stand,
             PATH(Int32, gameId),
             PATH(Int32, betId)) {
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }
        auto bet = game->GetBet(betId);
        if (!bet) {
            return createResponse(Status::CODE_404, "Unable to find bet");
        }
        auto dto = StandResponse::createShared();
        game->Stand(bet, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("GET", "/v2/game/{gameId}/bet/{betId}", getBet,
             PATH(Int32, gameId),
             PATH(Int32, betId)) {
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

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* GameController_hpp */

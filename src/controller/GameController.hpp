#ifndef GameController_hpp
#define GameController_hpp

#include "dto/DTOs.hpp"
#include "logic/GameLogic.hpp"
#include "logic/Registries.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen


class GameController : public oatpp::web::server::api::ApiController {
public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    GameController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            : oatpp::web::server::api::ApiController(objectMapper) {}

public:

    ENDPOINT("POST", "/player", createPlayer) {
        auto &reg = PlayerRegistry::GetInstance();
        auto id = reg.CreatePlayer();

        auto dto = CreatePlayerResponse::createShared();
        dto->playerId = id;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("GET", "/player/{playerId}", getPlayer, PATH(Int32, playerId)) {
        auto &reg = PlayerRegistry::GetInstance();
        auto player = reg.GetPlayer(playerId);
        if (!player) {
            return createResponse(Status::CODE_404, "Unable to find player");
        }

        auto dto = GetPlayerResponse::createShared();
        dto->cash = player->GetCash();
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/deck", createDrawDeck) {
        auto &reg = DrawDeckRegistry::GetInstance();
        auto id = reg.CreateDrawDeck();

        auto dto = CreateDeckResponse::createShared();
        dto->deckId = id;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/game", createGame, BODY_DTO(Object < CreateGameDto > , createGameDto)) {
        auto &playerReg = PlayerRegistry::GetInstance();
        auto &drawDeckReg = DrawDeckRegistry::GetInstance();
        auto &gameReg = GameRegistry::GetInstance();
        auto player = playerReg.GetPlayer(createGameDto->playerId.getValue(-1));
        auto drawDeck = drawDeckReg.GetDrawDeck(createGameDto->deckId.getValue(-1));

        if (!player) {
            return createResponse(Status::CODE_404, "Unable to find player");
        }
        if (!drawDeck) {
            return createResponse(Status::CODE_404, "Unable to find deck");
        }

        auto id = gameReg.CreateGame(player, drawDeck);

        auto dto = CreateGameResponse::createShared();
        dto->gameId = id;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/game/{gameId}/bet/{bet}", placeBet,
             PATH(Int32, gameId),
             PATH(Int32, bet)) {
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }

        auto dto = BetResponse::createShared();
        game->Bet(bet, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/game/{gameId}/hit", hit,
             PATH(Int32, gameId)) {
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }

        auto dto = HitResponse::createShared();
        if (game->Hit(dto)) {
            GameRegistry::GetInstance().DeleteGame(gameId);
        }
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/game/{gameId}/stand", stand,
             PATH(Int32, gameId)) {
        auto game = GameRegistry::GetInstance().GetGame(gameId);
        if (!game) {
            return createResponse(Status::CODE_404, "Unable to find game");
        }

        auto dto = StandResponse::createShared();
        if (game->Stand(dto)) {
            GameRegistry::GetInstance().DeleteGame(gameId);
        }
        return createDtoResponse(Status::CODE_200, dto);
    }

    // TODO Insert Your endpoints here !!!

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* GameController_hpp */

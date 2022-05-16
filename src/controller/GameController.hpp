#ifndef GameController_hpp
#define GameController_hpp

#include "dto/DTOs.hpp"
#include "logic/GameLogic.hpp"

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
        auto id = gameReg.CreateGame(player, drawDeck);

        auto dto = CreateGameResponse::createShared();
        dto->gameId = id;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/game/{gameId}/bet/{bet}", placeBet,
             PATH(Int32, gameId),
             PATH(Int32, bet)) {
        auto game = GameRegistry::GetInstance().GetGame(gameId);

        auto dto = BetResponse::createShared();
        game->Bet(bet, dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/game/{gameId}/hit", hit,
             PATH(Int32, gameId)) {
        auto game = GameRegistry::GetInstance().GetGame(gameId);

        auto dto = HitResponse::createShared();
        game->Hit(dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/game/{gameId}/stand", stand,
             PATH(Int32, gameId)) {
        auto game = GameRegistry::GetInstance().GetGame(gameId);

        auto dto = StandResponse::createShared();
        game->Stand(dto);
        return createDtoResponse(Status::CODE_200, dto);
    }

    // TODO Insert Your endpoints here !!!

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* GameController_hpp */

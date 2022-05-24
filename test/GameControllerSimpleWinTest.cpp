#include "GameControllerSimpleWinTest.hpp"

#include "controller/GameController.hpp"

#include "app/CreatePlayerApiTestClient.hpp"
#include "app/TestComponent.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"

void GameControllerSimpleWinTest::onRun() {

    TestComponent component;

    oatpp::test::web::ClientServerTestRunner runner;

    runner.addController(std::make_shared<GameController>());

    Package::GetInstance().Cheat(4, 10, 9, 10, 10);

    runner.run([this, &runner] {

        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

        auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);

        auto client = CreatePlayerApiTestClient::createShared(requestExecutor, objectMapper);

        auto responseCreatePlayer = client->createPlayer();
        OATPP_ASSERT(responseCreatePlayer->getStatusCode() == 200);
        auto messageCreatePlayer = responseCreatePlayer->readBodyToDto<oatpp::Object<CreatePlayerResponse >>(
                objectMapper.get());
        OATPP_ASSERT(messageCreatePlayer);
        OATPP_ASSERT(messageCreatePlayer->playerId != -1);
        int playerId = messageCreatePlayer->playerId;

        auto responseCreateDeck = client->createDeck();
        OATPP_ASSERT(responseCreateDeck->getStatusCode() == 200);
        auto messageCreateDeck = responseCreateDeck->readBodyToDto<oatpp::Object<CreateDeckResponse >>(
                objectMapper.get());
        OATPP_ASSERT(messageCreateDeck);
        OATPP_ASSERT(messageCreateDeck->deckId != -1);
        int deckId = messageCreateDeck->deckId;

        auto createGameRequest = CreateGameRequest::createShared();
        createGameRequest->deckId = deckId;
        auto responseCreateGame = client->createGame(createGameRequest);
        OATPP_ASSERT(responseCreateGame->getStatusCode() == 200);
        auto messageCreateGame = responseCreateGame->readBodyToDto<oatpp::Object<CreateGameResponse >>(
                objectMapper.get());
        OATPP_ASSERT(messageCreateGame);
        OATPP_ASSERT(messageCreateGame->gameId != -1);
        int gameId = messageCreateGame->gameId;

        auto betRequest = BetRequest::createShared();
        betRequest->playerId = playerId;
        betRequest->bet = 10;
        auto responseBet = client->createBet(betRequest, gameId);
        OATPP_ASSERT(responseBet->getStatusCode() == 200);
        auto messageBet = responseBet->readBodyToDto<oatpp::Object<BetResponse >>(objectMapper.get());
        OATPP_ASSERT(messageBet);
        OATPP_ASSERT(messageBet->betId != -1);
        OATPP_ASSERT(messageBet->yourTotal == 20);
        int betId = messageBet->betId;

        auto responseStand = client->stand(betId, gameId);
        OATPP_ASSERT(responseStand->getStatusCode() == 200);
        auto messageStand = responseStand->readBodyToDto<oatpp::Object<StandResponse >>(objectMapper.get());
        OATPP_ASSERT(messageStand);
        OATPP_ASSERT(messageStand->followActions->empty());

        auto responseResult = client->getResult(betId, gameId);
        OATPP_ASSERT(responseResult->getStatusCode() == 200);
        auto messageResult = responseResult->readBodyToDto<oatpp::Object<BetGetResponse >>(objectMapper.get());
        OATPP_ASSERT(messageResult);
        OATPP_ASSERT(!messageResult->result->empty());
        OATPP_ASSERT(messageResult->dealerTotal == 19);
        OATPP_ASSERT(messageResult->result == "You won!!");


    }, std::chrono::minutes(10) /* test timeout */);

    std::this_thread::sleep_for(std::chrono::seconds(1));

}

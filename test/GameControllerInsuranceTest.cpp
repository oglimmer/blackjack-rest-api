#include "GameControllerInsuranceTest.hpp"

#include "controller/GameController.hpp"

#include "app/CreatePlayerApiTestClient.hpp"
#include "app/TestComponent.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"


void GameControllerInsuranceTest::onRun() {

    TestComponent component;

    oatpp::test::web::ClientServerTestRunner runner;

    runner.addController(std::make_shared<GameController>());

    Package::GetInstance().Cheat(8, 9 /*closeDealer*/, 11 /*openDealer*/, 7, 7, 6, 11, 5, 4);

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
        OATPP_ASSERT(messageBet->yourTotal == 14);
        OATPP_ASSERT(messageBet->dealersCard == "11-of-Hearts");
        OATPP_ASSERT(messageBet->followActions->size() == 1);
        OATPP_ASSERT(messageBet->followActions->at(0) == "insurance");
        int betId = messageBet->betId;

        auto insuranceRequest = InsuranceRequest::createShared();
        insuranceRequest->insurance = "no";
        auto responseInsurance = client->insurance(betId, gameId, insuranceRequest);
        OATPP_ASSERT(responseInsurance->getStatusCode() == 200);
        auto messageInsurance = responseInsurance->readBodyToDto<oatpp::Object<StandResponse >>(objectMapper.get());
        OATPP_ASSERT(messageInsurance);
        OATPP_ASSERT(messageInsurance->followActions->size() == 3);
        OATPP_ASSERT(messageInsurance->followActions->at(0) == "hit");
        OATPP_ASSERT(messageInsurance->followActions->at(1) == "stand");
        OATPP_ASSERT(messageInsurance->followActions->at(2) == "split");


        auto responseSplit = client->split(betId, gameId);
        OATPP_ASSERT(responseSplit->getStatusCode() == 200);
        auto messageSplit = responseSplit->readBodyToDto<oatpp::Object<SplitResponse >>(objectMapper.get());
        OATPP_ASSERT(messageSplit);
        OATPP_ASSERT(messageSplit->followActions->size() == 2);
        OATPP_ASSERT(messageSplit->followActions->at(0) == "hit");
        OATPP_ASSERT(messageSplit->followActions->at(1) == "stand");
        OATPP_ASSERT(messageSplit->firstBetCard1 == "7-of-Hearts");
        OATPP_ASSERT(messageSplit->firstBetCard2 == "6-of-Hearts");
        OATPP_ASSERT(messageSplit->firstBetTotal == 13);
        OATPP_ASSERT(messageSplit->secondBetCard1 == "7-of-Hearts");
        OATPP_ASSERT(messageSplit->secondBetCard2 == "11-of-Hearts");
        OATPP_ASSERT(messageSplit->secondBetTotal == 18);
        OATPP_ASSERT(messageSplit->secondBetFollowAction->size() == 2);
        OATPP_ASSERT(messageSplit->secondBetFollowAction->at(0) == "hit");
        OATPP_ASSERT(messageSplit->secondBetFollowAction->at(1) == "stand");
        int betId2nd = messageSplit->secondBetId;

        auto responseStand = client->stand(betId, gameId);
        OATPP_ASSERT(responseStand->getStatusCode() == 200);
        auto messageStand = responseStand->readBodyToDto<oatpp::Object<StandResponse >>(objectMapper.get());
        OATPP_ASSERT(messageStand);
        OATPP_ASSERT(messageStand->followActions->empty());


        auto responseStand2nd = client->stand(betId2nd, gameId);
        OATPP_ASSERT(responseStand2nd->getStatusCode() == 200);
        auto messageStand2nd = responseStand2nd->readBodyToDto<oatpp::Object<StandResponse >>(objectMapper.get());
        OATPP_ASSERT(messageStand2nd);
        OATPP_ASSERT(messageStand2nd->followActions->empty());


        auto responseResult = client->getResult(betId, gameId);
        OATPP_ASSERT(responseResult->getStatusCode() == 200);
        auto messageResult = responseResult->readBodyToDto<oatpp::Object<BetGetResponse >>(objectMapper.get());
        OATPP_ASSERT(messageResult);
        OATPP_ASSERT(!messageResult->result->empty());
        OATPP_ASSERT(messageResult->dealerTotal == 20);
        OATPP_ASSERT(messageResult->result == "You lost!!");

        auto responseResult2nd = client->getResult(betId2nd, gameId);
        OATPP_ASSERT(responseResult2nd->getStatusCode() == 200);
        auto messageResultnd = responseResult2nd->readBodyToDto<oatpp::Object<BetGetResponse >>(objectMapper.get());
        OATPP_ASSERT(messageResultnd);
        OATPP_ASSERT(!messageResultnd->result->empty());
        OATPP_ASSERT(messageResultnd->dealerTotal == 20);
        OATPP_ASSERT(messageResultnd->result == "You lost!!");


    }, std::chrono::minutes(10) /* test timeout */);

    std::this_thread::sleep_for(std::chrono::seconds(1));

}

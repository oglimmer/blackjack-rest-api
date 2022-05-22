
#include "BlackJackUnitTest.hpp"


#include "app/TestComponent.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"

#include "logic/GameLogic.hpp"



void BlackJackUnitTest::testSimple() {
    auto drawDeck = Package::CreateDrawDeck();

    auto c1 = drawDeck->DrawCard();
    auto c2 = drawDeck->DrawCard();
    auto c3 = drawDeck->DrawCard();
    auto c4 = drawDeck->DrawCard();

    auto drawnDeck = std::unique_ptr<DrawnCards>(new DrawnCards());

    drawnDeck->AddCard(c1);
    OATPP_ASSERT(drawnDeck->GetValue() == 11);
    drawnDeck->AddCard(c2);
    OATPP_ASSERT(drawnDeck->GetValue() == 21);
    drawnDeck->AddCard(c3);
    OATPP_ASSERT(drawnDeck->GetValue() == 21);
    drawnDeck->AddCard(c4);
    OATPP_ASSERT(drawnDeck->GetValue() == 31);
}

void BlackJackUnitTest::testDoubleAce() {
    auto drawDeck = Package::CreateDrawDeck();

    auto ace1 = drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    auto ace2 = drawDeck->DrawCard();
    auto king = drawDeck->DrawCard();
    auto queen = drawDeck->DrawCard();
    auto jack = drawDeck->DrawCard();

    auto drawnDeck = std::unique_ptr<DrawnCards>(new DrawnCards());

    drawnDeck->AddCard(ace1);
    OATPP_ASSERT(drawnDeck->GetValue() == 11);
    drawnDeck->AddCard(king);
    OATPP_ASSERT(drawnDeck->GetValue() == 21);
    drawnDeck->AddCard(queen);
    OATPP_ASSERT(drawnDeck->GetValue() == 21);
    drawnDeck->AddCard(jack);
    OATPP_ASSERT(drawnDeck->GetValue() == 31);
    drawnDeck->AddCard(ace2);
    OATPP_ASSERT(drawnDeck->GetValue() == 32);
}

void BlackJackUnitTest::testDoubleAce2() {
    auto drawDeck = Package::CreateDrawDeck();

    auto ace1 = drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    auto ace2 = drawDeck->DrawCard();
    auto king = drawDeck->DrawCard();
    auto queen = drawDeck->DrawCard();
    auto jack = drawDeck->DrawCard();

    auto drawnDeck = std::unique_ptr<DrawnCards>(new DrawnCards());

    drawnDeck->AddCard(ace1);
    OATPP_ASSERT(drawnDeck->GetValue() == 11);
    drawnDeck->AddCard(ace2);
    OATPP_ASSERT(drawnDeck->GetValue() == 12);
    drawnDeck->AddCard(king);
    OATPP_ASSERT(drawnDeck->GetValue() == 12);
    drawnDeck->AddCard(queen);
    OATPP_ASSERT(drawnDeck->GetValue() == 22);
    drawnDeck->AddCard(jack);
    OATPP_ASSERT(drawnDeck->GetValue() == 32);
}

void BlackJackUnitTest::testDoubleAce3() {
    auto drawDeck = Package::CreateDrawDeck();

    auto ace1 = drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    drawDeck->DrawCard();
    auto ace2 = drawDeck->DrawCard();
    auto king = drawDeck->DrawCard();
    auto queen = drawDeck->DrawCard();
    auto jack = drawDeck->DrawCard();

    auto drawnDeck = std::unique_ptr<DrawnCards>(new DrawnCards());

    drawnDeck->AddCard(king);
    OATPP_ASSERT(drawnDeck->GetValue() == 10);
    drawnDeck->AddCard(ace1);
    OATPP_ASSERT(drawnDeck->GetValue() == 21);
    drawnDeck->AddCard(ace2);
    OATPP_ASSERT(drawnDeck->GetValue() == 12);
    drawnDeck->AddCard(queen);
    OATPP_ASSERT(drawnDeck->GetValue() == 22);
    drawnDeck->AddCard(jack);
    OATPP_ASSERT(drawnDeck->GetValue() == 32);
}


void BlackJackUnitTest::onRun() {

    TestComponent component;

    oatpp::test::web::ClientServerTestRunner runner;

    runner.run([this, &runner] {

        testSimple();
        testDoubleAce();
        testDoubleAce2();
        testDoubleAce3();


    }, std::chrono::minutes(10) /* test timeout */);

    std::this_thread::sleep_for(std::chrono::seconds(1));

}




#include "UnitTest.hpp"
#include "logic/GameLogic.hpp"


void testSimple() {
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

void testDoubleAce() {
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

void testDoubleAce2() {
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

void testDoubleAce3() {
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

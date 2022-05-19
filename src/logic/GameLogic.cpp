
#include "GameLogic.hpp"

#define LIFETIME_GAME 3600
#define LIFETIME_PLAYER 3600
#define LIFETIME_DRAWDECK 3600

/* ***************************************** Card ******************************************************* */

Card::Card(const std::string &desc) : desc(desc) {
}

const std::string &Card::GetDesc() const {
    return this->desc;
}

/* ***************************************** RegularCard ******************************************************* */

RegularCard::RegularCard(int value, const std::string &desc) : Card(desc), value(value) {
}

void RegularCard::GetValue(std::vector<int> &result) const {
    std::for_each(result.begin(), result.end(), [&](int& i) { i += this->value; });
}

/* ***************************************** AceCard ******************************************************* */

AceCard::AceCard(const std::string &desc) : Card(desc) {
}

void AceCard::GetValue(std::vector<int> &result) const {
    std::vector<int> vect1(result);
    std::vector<int> vect2(result);
    std::for_each(vect1.begin(), vect1.end(), [&](int& i) { i += 1; });
    std::for_each(vect2.begin(), vect2.end(), [&](int& i) { i += 11; });
    result.clear();
    result.insert(result.begin(), vect1.begin(), vect1.end());
    result.insert(result.begin(), vect2.begin(), vect2.end());
}

/* ***************************************** Deck ******************************************************* */

void Deck::AddCard(const std::shared_ptr<Card> card) {
    this->cards.push_back(card);
}

const std::vector<std::shared_ptr<Card>> &Deck::GetCards() const {
    return this->cards;
}

/* ***************************************** DrawnCards ******************************************************* */

int DrawnCards::GetValue() const {
    if(oatpp::base::Environment::getLogger()->isLogPriorityEnabled(oatpp::base::Logger::PRIORITY_D)) {
        OATPP_LOGD("DrawnCards", "[GetValue] cards.size=%d", cards.size());
        std::for_each(cards.begin(), cards.end(), [&](const std::shared_ptr<Card> &card) {
            OATPP_LOGD("DrawnCards", "[GetValue] card=%s", card->GetDesc().c_str());
        });
    }
    std::vector<int> result = {0};
    std::for_each(cards.begin(), cards.end(), [&](const std::shared_ptr<Card> &card) { card->GetValue(result); });
    OATPP_LOGD("DrawnCards", "[GetValue] result.size=%d", result.size());
    int largestValueLess22 = 0;
    int smallestValueLarger21 = 99999;
    std::for_each(result.begin(), result.end(), [&](int i) {
        OATPP_LOGD("DrawnCards", "[GetValue] found sub-value=%d", i);
        if (i <= 21 && i > largestValueLess22) {
            largestValueLess22 = i;
        }
        if (i > 21 && i < smallestValueLarger21) {
            smallestValueLarger21 = i;
        }
    });
    OATPP_LOGD("DrawnCards", "[GetValue] largestValueLess22=%d, smallestValueLarger21=%d", largestValueLess22, smallestValueLarger21);
    return largestValueLess22 > 0 ? largestValueLess22 : smallestValueLarger21;
}
int DrawnCards::Size() const {
    return cards.size();
}

/* ***************************************** DrawDeck ******************************************************* */

void DrawDeck::AddCard(const std::shared_ptr<Card> card) {
    Deck::AddCard(card);
    this->allCards.push_back(card);
}

void DrawDeck::shuffle() {
    std::shuffle(begin(cards), end(cards), Rnd::GetInstance().GetEngine());
}

const std::shared_ptr<Card> DrawDeck::DrawCard() {
    std::shared_ptr<Card> topCard = cards.back();
    cards.pop_back();
    return topCard;
}

void DrawDeck::ReshuffleIfNeeded() {
    if (cards.size() < 52) {
        cards.clear();
        cards.insert(cards.end(), allCards.begin(), allCards.end());
        std::shuffle(begin(cards), end(cards), Rnd::GetInstance().GetEngine());
    }
}

bool DrawDeck::IsOutDated() const {
    if(oatpp::base::Environment::getLogger()->isLogPriorityEnabled(oatpp::base::Logger::PRIORITY_D)) {
        OATPP_LOGD("DrawDeck", "[IsOutDated] Created at %s", toString(this->lastUsed).c_str());
    }
    std::chrono::seconds later{LIFETIME_DRAWDECK};
    auto now = std::chrono::system_clock::now();
    auto t1 = now - later;
    return this->lastUsed < t1;
}

void DrawDeck::Use() {
    this->lastUsed = std::chrono::system_clock::now();
}

/* ***************************************** Package ******************************************************* */

void Package::Add52Cards(std::shared_ptr<DrawDeck> drawDeck) {
    const std::string suits[] = {"Hearts", "Spades", "Diamonds", "Clubs"};
    for (int j = 0; j < 4; j++) {
        for (int i = 2; i < 11; i++) {
            drawDeck->AddCard(std::shared_ptr<Card>(new RegularCard(i, std::to_string(i) + "-of-" + suits[j])));
        }
        drawDeck->AddCard(std::shared_ptr<Card>(new RegularCard(10, "Jack-of-" + suits[j])));
        drawDeck->AddCard(std::shared_ptr<Card>(new RegularCard(10, "Queen-of-" + suits[j])));
        drawDeck->AddCard(std::shared_ptr<Card>(new RegularCard(10, "King-of-" + suits[j])));
        drawDeck->AddCard(std::shared_ptr<Card>(new AceCard("Ace-of-" + suits[j])));
    }
}

std::shared_ptr<DrawDeck> Package::CreateDrawDeck() {
    auto newDrawDeck = std::shared_ptr<DrawDeck>(new DrawDeck());
    for (int i = 0; i < 6; i++) {
        Add52Cards(newDrawDeck);
    }
    return newDrawDeck;
}

/* ***************************************** Game ******************************************************* */

Game::Game(std::shared_ptr<Player> player, std::shared_ptr<DrawDeck> drawDeck) : drawDeck(drawDeck), player(player) {
}

bool Game::Hit(HitResponse::Wrapper hitResponse) {
    const auto card = drawDeck->DrawCard();
    drawnCards->AddCard(card);

    hitResponse->drawnCard = card->GetDesc();
    hitResponse->yourTotal = drawnCards->GetValue();

    return CheckEnd(false, hitResponse);
}


bool Game::Stand(StandResponse::Wrapper standResponse) {
    return CheckEnd(true, standResponse);
}

bool Game::Bet(int _bet, BetResponse::Wrapper betResponse) {
    if (_bet > this->player->GetCash() || _bet < 1) {
        throw std::exception();
    }
    this->bet = _bet;
    this->player->SubCash(bet);

    drawnCardsDealer = std::unique_ptr<DrawnCards>(new DrawnCards());

    auto dealerCardOpen = drawDeck->DrawCard();
    dealerCardClosed = drawDeck->DrawCard();
    betResponse->dealersCard = dealerCardOpen->GetDesc();

    drawnCardsDealer->AddCard(dealerCardOpen);
    drawnCardsDealer->AddCard(dealerCardClosed);

    drawnCards = std::unique_ptr<DrawnCards>(new DrawnCards());

    const auto c1 = drawDeck->DrawCard();
    const auto c2 = drawDeck->DrawCard();

    betResponse->card1 = c1->GetDesc();
    betResponse->card2 = c2->GetDesc();

    drawnCards->AddCard(c1);
    drawnCards->AddCard(c2);

    betResponse->yourTotal = drawnCards->GetValue();

    return CheckEnd(false, betResponse);
}

bool Game::CheckEnd(bool done, EndResponse::Wrapper endResponse) {
    const int playerTotalValue = drawnCards->GetValue();
    if (playerTotalValue > 21) {
        endResponse->result = "You busted!!!";
        return true;
    } else if (done || playerTotalValue == 21) {
        endResponse->dealersSecondCard = dealerCardClosed->GetDesc();
        int totalValueDealer = drawnCardsDealer->GetValue();
        endResponse->dealersAdditionalCard = {};
        while (totalValueDealer < 17 && !(playerTotalValue == 21 && drawnCards->Size() == 2)) {
            const auto card = drawDeck->DrawCard();
            drawnCardsDealer->AddCard(card);
            totalValueDealer = drawnCardsDealer->GetValue();
            endResponse->dealersAdditionalCard->push_back(card->GetDesc());
        }
        endResponse->dealerTotal = totalValueDealer;
        if (playerTotalValue == 21 && drawnCards->Size() == 2 && totalValueDealer == 21 && drawnCardsDealer->Size() == 2) {
            endResponse->result = "You and the dealer have Blackjack!!";
            player->AddCash(1.5 * bet);
        } else if (playerTotalValue == 21 && drawnCards->Size() == 2) {
            endResponse->result = "You have Blackjack!!";
            player->AddCash(2.5 * bet);
        } else if (totalValueDealer == 21 && drawnCardsDealer->Size() == 2) {
            endResponse->result = "The dealer has Blackjack!!";
        } else if (totalValueDealer > 21) {
            endResponse->result = "You won!!";
            player->AddCash(2 * bet);
        } else if (playerTotalValue > totalValueDealer) {
            endResponse->result = "You won!!";
            player->AddCash(2 * bet);
        } else if (playerTotalValue == totalValueDealer) {
            endResponse->result = "Tie!!";
            player->AddCash(bet);
        } else {
            endResponse->result = "You lost!!";
        }
        return true;
    } else if (drawnCardsDealer->GetValue() == 21 && drawnCardsDealer->Size() == 2) {
        endResponse->result = "The dealer has Blackjack!!";
        return true;
    }
    return false;
}

bool Game::IsOutDated() const {
    if(oatpp::base::Environment::getLogger()->isLogPriorityEnabled(oatpp::base::Logger::PRIORITY_D)) {
        OATPP_LOGD("Game", "[IsOutDated] Created at %s", toString(this->lastUsed).c_str());
    }
    std::chrono::seconds later{LIFETIME_GAME};
    auto now = std::chrono::system_clock::now();
    auto t1 = now - later;
    return this->lastUsed < t1;
}

void Game::Use() {
    this->lastUsed = std::chrono::system_clock::now();
}

/* ***************************************** Player ******************************************************* */

Player::Player() : cash(1000) {
}

int Player::GetCash() const {
    return cash;
}

void Player::SubCash(int bet) {
    cash -= bet;
}

void Player::AddCash(int bet) {
    cash += bet;
}

bool Player::IsOutDated() const {
    if(oatpp::base::Environment::getLogger()->isLogPriorityEnabled(oatpp::base::Logger::PRIORITY_D)) {
        OATPP_LOGD("Player", "[IsOutDated] Created at %s", toString(this->lastUsed).c_str());
    }
    std::chrono::seconds later{LIFETIME_PLAYER};
    auto now = std::chrono::system_clock::now();
    auto t1 = now - later;
    return this->lastUsed < t1;
}

void Player::Use() {
    this->lastUsed = std::chrono::system_clock::now();
}
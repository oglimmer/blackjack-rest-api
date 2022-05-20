
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
bool DrawnCards::Val9_10_11() const {
    int val = GetValue();
    return val >= 9 && val <= 11;
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

/* ***************************************** Bet ******************************************************* */

Bet::Bet(std::shared_ptr<Player> player, int bet) :
    player(player),
    bet(bet),
    drawnCards( std::unique_ptr<DrawnCards>(new DrawnCards())),
    betId(Rnd::GetInstance().GetEngine()()){
}

std::unique_ptr<DrawnCards>& Bet::GetDrawnCards() {
    return this->drawnCards;
}

std::shared_ptr<Player> Bet::GetPlayer() const {
    return this->player;
}

int Bet::GetBet() const {
    return this->bet;
}

int Bet::GetBetId() const {
    return this->betId;
}

void Bet::IncBet(int additionalBet) {
    this->bet += additionalBet;
}

/* ***************************************** Game ******************************************************* */

Game::Game(std::shared_ptr<DrawDeck> drawDeck) :
        drawDeck(drawDeck) {
}

bool Game::Hit(std::shared_ptr<Bet> bet, HitResponse::Wrapper hitResponse) {
    const auto card = drawDeck->DrawCard();
    bet->GetDrawnCards()->AddCard(card);

    hitResponse->drawnCard = card->GetDesc();
    hitResponse->yourTotal = bet->GetDrawnCards()->GetValue();

    return CheckEnd(false, bet, hitResponse);
}

bool Game::DoubleBet(std::shared_ptr<Bet> bet, HitResponse::Wrapper hitResponse) {
    if (bet->GetBet() > bet->GetPlayer()->GetCash()) {
        throw std::exception();
    }
    if (!bet->GetDrawnCards()->Val9_10_11()) {
        throw std::exception();
    }
    bet->GetPlayer()->SubCash(bet->GetBet());
    bet->IncBet(bet->GetBet());
    const auto card = drawDeck->DrawCard();
    bet->GetDrawnCards()->AddCard(card);

    hitResponse->drawnCard = card->GetDesc();
    hitResponse->yourTotal = bet->GetDrawnCards()->GetValue();

    return CheckEnd(true, bet, hitResponse);
}


bool Game::Stand(std::shared_ptr<Bet> bet, StandResponse::Wrapper standResponse) {
    return CheckEnd(true, bet, standResponse);
}

bool Game::PlaceBet(int bet, std::shared_ptr<Player> player, BetResponse::Wrapper betResponse) {
    if (bet > player->GetCash() || bet < 1) {
        throw std::exception();
    }
    auto newBet = std::shared_ptr<Bet>(new Bet(player, bet));
    this->bets.push_back(newBet);
    player->SubCash(bet);

    drawnCardsDealer = std::unique_ptr<DrawnCards>(new DrawnCards());

    auto dealerCardOpen = drawDeck->DrawCard();
    dealerCardClosed = drawDeck->DrawCard();
    betResponse->dealersCard = dealerCardOpen->GetDesc();

    drawnCardsDealer->AddCard(dealerCardOpen);
    drawnCardsDealer->AddCard(dealerCardClosed);

    const auto c1 = drawDeck->DrawCard();
    const auto c2 = drawDeck->DrawCard();

    betResponse->card1 = c1->GetDesc();
    betResponse->card2 = c2->GetDesc();

    newBet->GetDrawnCards()->AddCard(c1);
    newBet->GetDrawnCards()->AddCard(c2);

    betResponse->yourTotal = newBet->GetDrawnCards()->GetValue();
    betResponse->betId = newBet->GetBetId();

    return CheckEnd(false, newBet, betResponse);
}

bool Game::CheckEnd(bool done, std::shared_ptr<Bet> bet, EndResponse::Wrapper endResponse) {
    const int playerTotalValue = bet->GetDrawnCards()->GetValue();
    if (playerTotalValue > 21) {
        endResponse->result = "You busted!!!";
        return true;
    } else if (done || playerTotalValue == 21) {
        endResponse->dealersSecondCard = dealerCardClosed->GetDesc();
        int totalValueDealer = drawnCardsDealer->GetValue();
        endResponse->dealersAdditionalCard = {};
        while (totalValueDealer < 17 && !(playerTotalValue == 21 && bet->GetDrawnCards()->Size() == 2)) {
            const auto card = drawDeck->DrawCard();
            drawnCardsDealer->AddCard(card);
            totalValueDealer = drawnCardsDealer->GetValue();
            endResponse->dealersAdditionalCard->push_back(card->GetDesc());
        }
        endResponse->dealerTotal = totalValueDealer;
        if (playerTotalValue == 21 && bet->GetDrawnCards()->Size() == 2 && totalValueDealer == 21 && drawnCardsDealer->Size() == 2) {
            endResponse->result = "You and the dealer have Blackjack!!";
            bet->GetPlayer()->AddCash(1.5 * bet->GetBet());
        } else if (playerTotalValue == 21 && bet->GetDrawnCards()->Size() == 2) {
            endResponse->result = "You have Blackjack!!";
            bet->GetPlayer()->AddCash(2.5 * bet->GetBet());
        } else if (totalValueDealer == 21 && drawnCardsDealer->Size() == 2) {
            endResponse->result = "The dealer has Blackjack!!";
        } else if (totalValueDealer > 21) {
            endResponse->result = "You won!!";
            bet->GetPlayer()->AddCash(2 * bet->GetBet());
        } else if (playerTotalValue > totalValueDealer) {
            endResponse->result = "You won!!";
            bet->GetPlayer()->AddCash(2 * bet->GetBet());
        } else if (playerTotalValue == totalValueDealer) {
            endResponse->result = "Tie!!";
            bet->GetPlayer()->AddCash(bet->GetBet());
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

std::shared_ptr<Bet> Game::GetBet(int betId) {
    return *std::find_if(bets.begin(), bets.end(),
                            [&](const std::shared_ptr<Bet> &m) -> bool { return m->GetBetId() == betId; });
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
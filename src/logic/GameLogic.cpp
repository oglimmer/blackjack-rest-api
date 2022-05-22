
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
    std::for_each(result.begin(), result.end(), [&](int &i) { i += this->value; });
}

/* ***************************************** AceCard ******************************************************* */

AceCard::AceCard(const std::string &desc) : Card(desc) {
}

void AceCard::GetValue(std::vector<int> &result) const {
    std::vector<int> vect1(result);
    std::vector<int> vect2(result);
    std::for_each(vect1.begin(), vect1.end(), [&](int &i) { i += 1; });
    std::for_each(vect2.begin(), vect2.end(), [&](int &i) { i += 11; });
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

const std::shared_ptr<Card> Deck::DrawCard() {
    std::shared_ptr<Card> topCard = cards.back();
    cards.pop_back();
    return topCard;
}

/* ***************************************** DrawnCards ******************************************************* */

int DrawnCards::GetValue() const {
    if (oatpp::base::Environment::getLogger()->isLogPriorityEnabled(oatpp::base::Logger::PRIORITY_D)) {
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
    OATPP_LOGD("DrawnCards", "[GetValue] largestValueLess22=%d, smallestValueLarger21=%d", largestValueLess22,
               smallestValueLarger21);
    return largestValueLess22 > 0 ? largestValueLess22 : smallestValueLarger21;
}

int DrawnCards::Size() const {
    return cards.size();
}

bool DrawnCards::Simple9_10_11() const {
    int val = GetValue();
    return val >= 9 && val <= 11 && cards.size() == 2;
}

bool DrawnCards::IsBlackJack() const {
    return cards.size() == 2 && GetValue() == 21;
}

bool DrawnCards::IsSimplePair() const {
    if (cards.size() != 2) {
        return false;
    }
    std::vector<int> card0Val, card1Val;
    card0Val.push_back(0);
    card1Val.push_back(0);
    cards[0]->GetValue(card0Val);
    cards[1]->GetValue(card1Val);
    return card0Val[0] == card1Val[0];
}

/* ***************************************** DrawDeck ******************************************************* */

void DrawDeck::AddCard(const std::shared_ptr<Card> card) {
    Deck::AddCard(card);
    this->allCards.push_back(card);
}

void DrawDeck::shuffle() {
    std::shuffle(begin(cards), end(cards), Rnd::GetInstance().GetEngine());
}

void DrawDeck::ReshuffleIfNeeded() {
    if (cards.size() < 52) {
        cards.clear();
        cards.insert(cards.end(), allCards.begin(), allCards.end());
        std::shuffle(begin(cards), end(cards), Rnd::GetInstance().GetEngine());
    }
}

bool DrawDeck::IsOutDated() const {
    if (oatpp::base::Environment::getLogger()->isLogPriorityEnabled(oatpp::base::Logger::PRIORITY_D)) {
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
        drawnCards(std::unique_ptr<DrawnCards>(new DrawnCards())),
        betId(Rnd::GetInstance().GetEngine()()),
        stand(false) {
}

std::unique_ptr<DrawnCards> &Bet::GetDrawnCards() {
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

bool Bet::GetStand() const {
    return this->stand;
}

void Bet::SetStand(bool stand) {
    this->stand = stand;
}

bool Bet::IsDone() const {
    return this->stand || drawnCards->GetValue() >= 21;
}


/* ***************************************** Game ******************************************************* */

Game::Game(std::shared_ptr<DrawDeck> drawDeck) :
        drawDeck(drawDeck) {
}

void Game::Hit(std::shared_ptr<Bet> bet, HitResponse::Wrapper &hitResponse) {
    const auto card = drawDeck->DrawCard();
    bet->GetDrawnCards()->AddCard(card);

    hitResponse->drawnCard = card->GetDesc();
    hitResponse->yourTotal = bet->GetDrawnCards()->GetValue();

    if (bet->GetDrawnCards()->GetValue() > 21) {
        bet->SetStand(true);
    }

    auto actions = AddFollowActions(bet);
    if (!actions->empty()) {
        hitResponse->followActions = {};
        hitResponse->followActions->insert(hitResponse->followActions->begin(), actions->begin(), actions->end());
    }

    WrapUp();
}

void Game::DoubleBet(std::shared_ptr<Bet> bet, HitResponse::Wrapper &hitResponse) {
    if (bet->GetBet() > bet->GetPlayer()->GetCash()) {
        throw std::exception();
    }
    if (!bet->GetDrawnCards()->Simple9_10_11()) {
        throw std::exception();
    }
    bet->GetPlayer()->SubCash(bet->GetBet());
    bet->IncBet(bet->GetBet());
    const auto card = drawDeck->DrawCard();
    bet->GetDrawnCards()->AddCard(card);

    hitResponse->drawnCard = card->GetDesc();
    hitResponse->yourTotal = bet->GetDrawnCards()->GetValue();

    bet->SetStand(true);

    auto actions = AddFollowActions(bet);
    if (!actions->empty()) {
        hitResponse->followActions = {};
        hitResponse->followActions->insert(hitResponse->followActions->begin(), actions->begin(), actions->end());
    }

    WrapUp();
}


void Game::Stand(std::shared_ptr<Bet> bet, StandResponse::Wrapper &standResponse) {
    bet->SetStand(true);
    WrapUp();
}

void Game::PlaceBet(int betVal, std::shared_ptr<Player> player, BetResponse::Wrapper &betResponse) {
    if (betVal > player->GetCash() || betVal < 1) {
        throw std::exception();
    }

    auto bet = std::shared_ptr<Bet>(new Bet(player, betVal));
    this->bets.push_back(bet);
    player->SubCash(betVal);

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

    bet->GetDrawnCards()->AddCard(c1);
    bet->GetDrawnCards()->AddCard(c2);

    betResponse->yourTotal = bet->GetDrawnCards()->GetValue();
    betResponse->betId = bet->GetBetId();

    auto actions = AddFollowActions(bet);
    if (!actions->empty()) {
        betResponse->followActions = {};
        betResponse->followActions->insert(betResponse->followActions->begin(), actions->begin(), actions->end());
    }

    WrapUp();
}

void Game::Split(std::shared_ptr<Bet> bet, SplitResponse::Wrapper &splitResponse) {
    if (bet->GetBet() > bet->GetPlayer()->GetCash()) {
        throw std::exception();
    }
    if (!bet->GetDrawnCards()->IsSimplePair() || bets.size() != 1) {
        throw std::exception();
    }

    auto bet2nd = std::shared_ptr<Bet>(new Bet(bet->GetPlayer(), bet->GetBet()));
    this->bets.push_back(bet2nd);
    bet->GetPlayer()->SubCash(bet->GetBet());
    splitResponse->secondBetId = bet2nd->GetBetId();

    // transfer 2nd card from 1st bet into 2nd bet
    auto removedCard = bet->GetDrawnCards()->DrawCard();
    bet2nd->GetDrawnCards()->AddCard(removedCard);

    // add a card to 1st bet
    const auto c1 = drawDeck->DrawCard();
    bet->GetDrawnCards()->AddCard(c1);
    splitResponse->firstBetCard1 = bet->GetDrawnCards()->GetCards()[0]->GetDesc();
    splitResponse->firstBetCard2 = bet->GetDrawnCards()->GetCards()[1]->GetDesc();
    splitResponse->firstBetTotal = bet->GetDrawnCards()->GetValue();

    // add a card 2nd bet
    const auto c2 = drawDeck->DrawCard();
    bet2nd->GetDrawnCards()->AddCard(c2);
    splitResponse->secondBetCard1 = bet2nd->GetDrawnCards()->GetCards()[0]->GetDesc();
    splitResponse->secondBetCard2 = bet2nd->GetDrawnCards()->GetCards()[1]->GetDesc();
    splitResponse->secondBetTotal = bet2nd->GetDrawnCards()->GetValue();

    auto actions = AddFollowActions(bet);
    if (!actions->empty()) {
        splitResponse->followActions = {};
        splitResponse->followActions->insert(splitResponse->followActions->begin(), actions->begin(), actions->end());
    }

    auto actions2ndBet = AddFollowActions(bet2nd);
    if (!actions2ndBet->empty()) {
        splitResponse->secondBetFollowAction = {};
        splitResponse->secondBetFollowAction->insert(splitResponse->secondBetFollowAction->begin(),
                                                     actions2ndBet->begin(), actions2ndBet->end());
    }

    WrapUp();
}

std::unique_ptr<std::vector<std::string>> Game::AddFollowActions(std::shared_ptr<Bet> bet) {
    auto actions = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>());
    if (bet->GetDrawnCards()->GetValue() < 21 && !bet->GetStand()) {
        actions->push_back("hit");
        actions->push_back("stand");
        if (bet->GetDrawnCards()->Simple9_10_11() && bet->GetPlayer()->GetCash() >= bet->GetBet()) {
            actions->push_back("double");
        }
        if (bet->GetDrawnCards()->IsSimplePair() && bets.size() == 1) {
            actions->push_back("split");
        }
    }
    return actions;
}


void Game::WrapUp() {
    if (IsDone()) {
        AdvanceDealer();
        Payout();
    }
}

void Game::AdvanceDealer() {
    int totalValueDealer = drawnCardsDealer->GetValue();
    while (totalValueDealer < 17) {
        const auto card = drawDeck->DrawCard();
        drawnCardsDealer->AddCard(card);
        totalValueDealer = drawnCardsDealer->GetValue();
    }
}

bool Game::AddResponse(std::shared_ptr<Bet> bet, BetGetResponse::Wrapper &response) const {
    const int playerTotalValue = bet->GetDrawnCards()->GetValue();
    bool isDone = IsDone();
    if (playerTotalValue > 21) {
        response->result = "You busted!!!";
    } else if (isDone) {
        int totalValueDealer = drawnCardsDealer->GetValue();
        if (bet->GetDrawnCards()->IsBlackJack() && drawnCardsDealer->IsBlackJack()) {
            response->result = "You and the dealer have Blackjack!!";
        } else if (bet->GetDrawnCards()->IsBlackJack()) {
            response->result = "You have Blackjack!!";
        } else if (drawnCardsDealer->IsBlackJack()) {
            response->result = "The dealer has Blackjack!!";
        } else if (totalValueDealer > 21 || playerTotalValue > totalValueDealer) {
            response->result = "You won!!";
        } else if (playerTotalValue == totalValueDealer) {
            response->result = "Tie!!";
        } else {
            response->result = "You lost!!";
        }
    }
    if (isDone) {
        response->dealersSecondCard = dealerCardClosed->GetDesc();
        response->dealerTotal = drawnCardsDealer->GetValue();
        response->dealersAdditionalCard = {};
        std::vector<std::shared_ptr<Card>, std::allocator<std::shared_ptr<Card>>>::const_iterator first = this->drawnCardsDealer->GetCards().begin();
        first += 2;
        std::for_each(first, this->drawnCardsDealer->GetCards().end(), [&](const std::shared_ptr<Card> &card) {
            response->dealersAdditionalCard->push_back(card->GetDesc());
        });
    }
    return isDone;
}

void Game::Payout() const {
    if (IsDone()) {
        std::for_each(bets.begin(), bets.end(), [&](const std::shared_ptr<Bet> &bet) { Payout(bet); });
    }
}

void Game::Payout(std::shared_ptr<Bet> bet) const {
    int playerTotalValue = bet->GetDrawnCards()->GetValue();
    if (playerTotalValue <= 21) {
        int totalValueDealer = drawnCardsDealer->GetValue();
        if (bet->GetDrawnCards()->IsBlackJack() && drawnCardsDealer->IsBlackJack()) {
            bet->GetPlayer()->AddCash(1.5 * bet->GetBet());
        } else if (bet->GetDrawnCards()->IsBlackJack()) {
            bet->GetPlayer()->AddCash(2.5 * bet->GetBet());
        } else if (totalValueDealer > 21 || playerTotalValue > totalValueDealer) {
            bet->GetPlayer()->AddCash(2 * bet->GetBet());
        } else if (playerTotalValue == totalValueDealer) {
            bet->GetPlayer()->AddCash(bet->GetBet());
        }
    }
}

bool Game::IsDone() const {
    auto it = std::find_if(bets.begin(), bets.end(),
                           [&](const std::shared_ptr<Bet> &bet) -> bool { return !IsBetDone(bet); });
    OATPP_LOGI("Game", "[IsDone] ret = %d", it == bets.end());
    return it == bets.end();
}

bool Game::IsOutDated() const {
    if (oatpp::base::Environment::getLogger()->isLogPriorityEnabled(oatpp::base::Logger::PRIORITY_D)) {
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

bool Game::IsBetDone(std::shared_ptr<Bet> bet) const {
    OATPP_LOGI("Game", "[IsBetDone] bet->IsDone() = %d", bet->IsDone());
    OATPP_LOGI("Game", "[IsBetDone] drawnCardsDealer->IsBlackJack() = %d", drawnCardsDealer->IsBlackJack());
    return bet->IsDone() || drawnCardsDealer->IsBlackJack();
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
    if (oatpp::base::Environment::getLogger()->isLogPriorityEnabled(oatpp::base::Logger::PRIORITY_D)) {
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
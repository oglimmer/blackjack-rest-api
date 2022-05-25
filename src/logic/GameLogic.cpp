
#include "GameLogic.hpp"
#include "HighscoreList.hpp"

#define LIFETIME_GAME 3600
#define LIFETIME_PLAYER 3600

/* ***************************************** Bet ******************************************************* */

Bet::Bet(std::shared_ptr<Player> player, int bet) :
        player(player),
        bet(bet),
        drawnCards(std::unique_ptr<DrawnCards>(new DrawnCards())),
        betId(Rnd::GetInstance().GetEngine()()),
        stand(false),
        isAskedForInsurance(false),
        isInsuranceBought(false) {
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

bool Bet::IsAskedForInsurance() const {
    return this->isAskedForInsurance;
}

void Bet::SetAskedForInsurance(bool b) {
    this->isAskedForInsurance = b;
}

bool Bet::IsInsuranceBought() const {
    return this->isInsuranceBought;
}

void Bet::SetInsuranceBought(bool b) {
    this->isInsuranceBought = b;
}

/* ***************************************** Game ******************************************************* */

Game::Game(std::shared_ptr<DrawDeck> drawDeck) :
        drawDeck(drawDeck),
        drawnCardsDealer(std::unique_ptr<DrawnCards>(new DrawnCards())),
        dealerCardOpen(drawDeck->DrawCard()),
        dealerCardClosed(drawDeck->DrawCard()) {
    drawnCardsDealer->AddCard(dealerCardOpen);
    drawnCardsDealer->AddCard(dealerCardClosed);
}

void Game::Hit(std::shared_ptr<Bet> bet, HitResponse::Wrapper &hitResponse) {
    if (!IsFollowActionsAllowed(bet, "hit")) {
        throw GameException("Action `hit` not allowed.");
    }

    const auto card = drawDeck->DrawCard();
    bet->GetDrawnCards()->AddCard(card);

    hitResponse->drawnCard = card->GetDesc();
    hitResponse->yourTotal = bet->GetDrawnCards()->GetValue();

    if (bet->GetDrawnCards()->GetValue() > 21) {
        bet->SetStand(true);
    }

    WrapUp();

    auto actions = AddFollowActions(bet);
    hitResponse->followActions = {};
    if (!actions->empty()) {
        hitResponse->followActions->insert(hitResponse->followActions->begin(), actions->begin(), actions->end());
    }
}

void Game::DoubleBet(std::shared_ptr<Bet> bet, HitResponse::Wrapper &hitResponse) {
    if (bet->GetBet() > bet->GetPlayer()->GetCash()) {
        throw GameException("Not enough money to double.");
    }
    if (!bet->GetDrawnCards()->Simple9_10_11()) {
        throw GameException("Double is only allowed when the first two cards are 9,10,11 in total.");
    }
    if (!IsFollowActionsAllowed(bet, "double")) {
        throw GameException("Action `double` not allowed.");
    }

    bet->GetPlayer()->SubCash(bet->GetBet());
    bet->IncBet(bet->GetBet());
    const auto card = drawDeck->DrawCard();
    bet->GetDrawnCards()->AddCard(card);

    hitResponse->drawnCard = card->GetDesc();
    hitResponse->yourTotal = bet->GetDrawnCards()->GetValue();

    bet->SetStand(true);

    WrapUp();

    auto actions = AddFollowActions(bet);
    hitResponse->followActions = {};
    if (!actions->empty()) {
        hitResponse->followActions->insert(hitResponse->followActions->begin(), actions->begin(), actions->end());
    }
}


void Game::Stand(std::shared_ptr<Bet> bet, StandResponse::Wrapper &standResponse) {
    if (!IsFollowActionsAllowed(bet, "stand")) {
        throw GameException("Action `stand` not allowed.");
    }
    bet->SetStand(true);
    WrapUp();
    standResponse->followActions = {};
}

void Game::PlaceBet(int betVal, std::shared_ptr<Player> player, BetResponse::Wrapper &betResponse) {
    if (betVal > player->GetCash() || betVal < 1) {
        throw GameException("Not enough money.");
    }

    auto bet = std::shared_ptr<Bet>(new Bet(player, betVal));
    this->bets.push_back(bet);
    player->SubCash(betVal);

    betResponse->dealersCard = dealerCardOpen->GetDesc();

    const auto c1 = drawDeck->DrawCard();
    const auto c2 = drawDeck->DrawCard();

    betResponse->card1 = c1->GetDesc();
    betResponse->card2 = c2->GetDesc();

    bet->GetDrawnCards()->AddCard(c1);
    bet->GetDrawnCards()->AddCard(c2);

    betResponse->yourTotal = bet->GetDrawnCards()->GetValue();
    betResponse->betId = bet->GetBetId();

    WrapUp();

    auto actions = AddFollowActions(bet);
    betResponse->followActions = {};
    if (!actions->empty()) {
        betResponse->followActions->insert(betResponse->followActions->begin(), actions->begin(), actions->end());
    }
}

void Game::Split(std::shared_ptr<Bet> bet, SplitResponse::Wrapper &splitResponse) {
    if (bet->GetBet() > bet->GetPlayer()->GetCash()) {
        throw GameException("Not enough money to split.");
    }
    if (!bet->GetDrawnCards()->IsSimplePair() || bets.size() != 1) {
        throw GameException("Split is only allowed when the first two cards are of the same rank.");
    }
    if (!IsFollowActionsAllowed(bet, "split")) {
        throw GameException("Action `split` not allowed.");
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

    WrapUp();

    auto actions = AddFollowActions(bet);
    splitResponse->followActions = {};
    if (!actions->empty()) {
        splitResponse->followActions->insert(splitResponse->followActions->begin(), actions->begin(), actions->end());
    }

    auto actions2ndBet = AddFollowActions(bet2nd);
    splitResponse->secondBetFollowAction = {};
    if (!actions2ndBet->empty()) {
        splitResponse->secondBetFollowAction->insert(splitResponse->secondBetFollowAction->begin(),
                                                     actions2ndBet->begin(), actions2ndBet->end());
    }
}

void Game::Insurance(bool insurance, std::shared_ptr<Bet> bet, StandResponse::Wrapper &standResponse) {
    if (!IsFollowActionsAllowed(bet, "insurance")) {
        throw GameException("Action `insurance` not allowed.");
    }
    if (insurance && bet->GetBet() * 0.5 > bet->GetPlayer()->GetCash()) {
        throw GameException("Not enough money to buy an insurance.");
    }

    bet->SetAskedForInsurance(true);
    bet->SetInsuranceBought(insurance);
    if (insurance) {
        bet->GetPlayer()->SubCash(bet->GetBet() * 0.5);
    }

    WrapUp();

    auto actions = AddFollowActions(bet);
    standResponse->followActions = {};
    if (!actions->empty()) {
        standResponse->followActions->insert(standResponse->followActions->begin(), actions->begin(), actions->end());
    }
}

bool Game::IsFollowActionsAllowed(std::shared_ptr<Bet> bet, std::string action) {
    auto allowedActions = AddFollowActions(bet);
    return std::find(allowedActions->begin(), allowedActions->end(), action) != allowedActions->end();
}

std::unique_ptr<std::vector<std::string>> Game::AddFollowActions(std::shared_ptr<Bet> bet) {
    auto actions = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>());
    if (bet->GetDrawnCards()->GetValue() < 21 && !bet->GetStand()) {
        if (dealerCardOpen->GetRank() == 11 && !bet->IsAskedForInsurance() &&
            bet->GetPlayer()->GetCash() >= bet->GetBet() * 0.5) {
            actions->push_back("insurance");
        } else {
            actions->push_back("hit");
            actions->push_back("stand");
            if (bet->GetDrawnCards()->Simple9_10_11() && bet->GetPlayer()->GetCash() >= bet->GetBet()) {
                actions->push_back("double");
            }
            if (bet->GetDrawnCards()->IsSimplePair() && bets.size() == 1 &&
                bet->GetPlayer()->GetCash() >= bet->GetBet()) {
                actions->push_back("split");
            }
        }
    }
    return actions;
}


void Game::WrapUp() {
    if (drawnCardsDealer->IsBlackJack()) {
        if (dealerCardOpen->GetRank() == 11 && bets[0]->IsAskedForInsurance()) {
            bets[0]->SetStand(true);
        }
        if (dealerCardClosed->GetRank() == 11) {
            bets[0]->SetStand(true);
        }
    }
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
    response->dealersAdditionalCard = {};
    if (isDone) {
        response->dealersSecondCard = dealerCardClosed->GetDesc();
        response->dealerTotal = drawnCardsDealer->GetValue();
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
            bet->GetPlayer()->AddCash(bet->GetBet());
        } else if (bet->GetDrawnCards()->IsBlackJack()) {
            bet->GetPlayer()->AddCash(2.5 * bet->GetBet());
        } else if (totalValueDealer > 21 || playerTotalValue > totalValueDealer) {
            bet->GetPlayer()->AddCash(2 * bet->GetBet());
        } else if (playerTotalValue == totalValueDealer) {
            bet->GetPlayer()->AddCash(bet->GetBet());
        }
        // side bet
        if (drawnCardsDealer->IsBlackJack() && bet->IsInsuranceBought()) {
            bet->GetPlayer()->AddCash(1.5 * bet->GetBet());
        }
    }
}

bool Game::IsDone() const {
    auto it = std::find_if(bets.begin(), bets.end(),
                           [&](const std::shared_ptr<Bet> &bet) -> bool { return !bet->IsDone(); });
    OATPP_LOGD("Game", "[IsDone] ret = %d", it == bets.end());
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


/* ***************************************** Player ******************************************************* */

#define START_CASH 1000

Player::Player(int id) : cash(START_CASH), id(id) {
}

Player::Player(int id, const std::string &name) : Player(id) {
    this->name = name;
}

int Player::GetId() const {
    return id;
}

int Player::GetCash() const {
    return cash;
}

void Player::SubCash(int bet) {
    cash -= bet;
}

void Player::AddCash(int bet) {
    OATPP_LOGD("Player", "[AddCash] From %d add %d = %d", cash, bet, cash + bet);
    cash += bet;
    HighscoreList::GetInstance().CheckHighScore(id, cash, name);
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

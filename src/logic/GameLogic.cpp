
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
        isInsuranceBought(false),
        isResultChecked(false) {
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

bool Bet::ResultChecked() const {
    return this->isResultChecked;
}

void Bet::SetResultChecked(bool b) {
    this->isResultChecked = b;
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
    const std::lock_guard<std::mutex> lockGuard(game_mutex);
    if (!IsFollowActionsAllowed(bet, "hit")) {
        throw GameException("Action `hit` not allowed.");
    }

    const auto card = drawDeck->DrawCard();
    bet->GetDrawnCards()->AddCard(card);

    hitResponse->drawnCard = card->GetDesc();
    hitResponse->yourTotal = bet->GetDrawnCards()->GetValue();

    WrapUp(bet);

    auto actions = AddFollowActions(bet);
    hitResponse->followActions = {};
    if (!actions->empty()) {
        hitResponse->followActions->insert(hitResponse->followActions->begin(), actions->begin(), actions->end());
    }
}

void Game::DoubleBet(std::shared_ptr<Bet> bet, HitResponse::Wrapper &hitResponse) {
    const std::lock_guard<std::mutex> lockGuard(game_mutex);
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

    WrapUp(bet);

    auto actions = AddFollowActions(bet);
    hitResponse->followActions = {};
    if (!actions->empty()) {
        hitResponse->followActions->insert(hitResponse->followActions->begin(), actions->begin(), actions->end());
    }
}


void Game::Stand(std::shared_ptr<Bet> bet, StandResponse::Wrapper &standResponse) {
    const std::lock_guard<std::mutex> lockGuard(game_mutex);
    if (!IsFollowActionsAllowed(bet, "stand")) {
        throw GameException("Action `stand` not allowed.");
    }
    bet->SetStand(true);
    WrapUp(bet);
    standResponse->followActions = {};
}

void Game::PlaceBet(int betVal, std::shared_ptr<Player> player, BetResponse::Wrapper &betResponse) {
    const std::lock_guard<std::mutex> lockGuard(game_mutex);
    if (betVal > player->GetCash() || betVal < 1) {
        throw GameException("Not enough money.");
    }
    if (betVal > 1000) {
        throw GameException("Max bet is 1000.");
    }

    auto bet = std::make_shared<Bet>(player, betVal);
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

    if (bet->GetDrawnCards()->IsBlackJack()) {
        bet->SetStand(true);
    }

    WrapUp(bet);

    auto actions = AddFollowActions(bet);
    betResponse->followActions = {};
    if (!actions->empty()) {
        betResponse->followActions->insert(betResponse->followActions->begin(), actions->begin(), actions->end());
    }
}

void Game::Split(std::shared_ptr<Bet> bet, SplitResponse::Wrapper &splitResponse) {
    const std::lock_guard<std::mutex> lockGuard(game_mutex);
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
    bet2nd->SetAskedForInsurance(true);
    this->bets.push_back(bet2nd);
    bet->GetPlayer()->SubCash(bet->GetBet());
    splitResponse->secondBetId = bet2nd->GetBetId();

    // transfer 2nd card from 1st bet into 2nd bet
    auto removedCard = bet->GetDrawnCards()->DrawCard();
    bet2nd->GetDrawnCards()->AddCard(removedCard);

    // add a card to 1st bet
    const auto c1 = drawDeck->DrawCard();
    bet->GetDrawnCards()->AddCard(c1);
    splitResponse->firstBetCard1 = bet->GetDrawnCards()->GetCard(0)->GetDesc();
    splitResponse->firstBetCard2 = bet->GetDrawnCards()->GetCard(1)->GetDesc();
    splitResponse->firstBetTotal = bet->GetDrawnCards()->GetValue();

    // add a card 2nd bet
    const auto c2 = drawDeck->DrawCard();
    bet2nd->GetDrawnCards()->AddCard(c2);
    splitResponse->secondBetCard1 = bet2nd->GetDrawnCards()->GetCard(0)->GetDesc();
    splitResponse->secondBetCard2 = bet2nd->GetDrawnCards()->GetCard(1)->GetDesc();
    splitResponse->secondBetTotal = bet2nd->GetDrawnCards()->GetValue();

    WrapUp(bet, bet2nd);

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
    const std::lock_guard<std::mutex> lockGuard(game_mutex);
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

    WrapUp(bet);

    auto actions = AddFollowActions(bet);
    standResponse->followActions = {};
    if (!actions->empty()) {
        standResponse->followActions->insert(standResponse->followActions->begin(), actions->begin(), actions->end());
    }
}


bool Game::AddResponse(std::shared_ptr<Bet> bet, BetGetResponse::Wrapper &response) const {
    const std::lock_guard<std::mutex> lockGuard(game_mutex);
    const int playerTotalValue = bet->GetDrawnCards()->GetValue();
    bool isDone = IsDone();
    response->payout = 0;
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
        response->payout = GetPayoutValue(bet);
    } else if (bet->IsDone()) {
        response->result = "Other bets still in progress";
    } else {
        response->result = "This bet is still in progress";
    }
    response->dealersAdditionalCard = {};
    if (isDone) {
        response->dealersSecondCard = dealerCardClosed->GetDesc();
        response->dealerTotal = drawnCardsDealer->GetValue();
        int pos = 0;
        this->drawnCardsDealer->ForEachCard([&](const std::shared_ptr<Card> &card) {
            if (pos > 1) {
                response->dealersAdditionalCard->push_back(card->GetDesc());
            }
            pos++;
        });
    }
    if (isDone) {
        bet->SetResultChecked(true);
    }
    return isDone && allResultsChecked();
}

bool Game::IsOutDated() const {
    const std::lock_guard<std::mutex> lockGuard(lastUsed_mutex);
    if (oatpp::base::Environment::getLogger()->isLogPriorityEnabled(oatpp::base::Logger::PRIORITY_D)) {
        OATPP_LOGD("Game", "[IsOutDated] Created at %s", toString(this->lastUsed).c_str());
    }
    std::chrono::seconds later{LIFETIME_GAME};
    auto now = std::chrono::system_clock::now();
    auto t1 = now - later;
    return this->lastUsed < t1;
}

void Game::Use() {
    const std::lock_guard<std::mutex> lockGuard(lastUsed_mutex);
    this->lastUsed = std::chrono::system_clock::now();
}

std::shared_ptr<Bet> Game::GetBet(int betId) {
    const std::lock_guard<std::mutex> lockGuard(game_mutex);
    return *std::find_if(bets.begin(), bets.end(),
                         [&](const std::shared_ptr<Bet> &m) -> bool { return m->GetBetId() == betId; });
}



// private

bool Game::IsFollowActionsAllowed(std::shared_ptr<Bet> bet, std::string action) const {
    auto allowedActions = AddFollowActions(bet);
    return std::find(allowedActions->begin(), allowedActions->end(), action) != allowedActions->end();
}

std::unique_ptr<std::vector<std::string>> Game::AddFollowActions(std::shared_ptr<Bet> bet) const {
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


void Game::WrapUp(std::shared_ptr<Bet> bet, std::shared_ptr<Bet> bet2nd) {
    if (drawnCardsDealer->IsBlackJack()) {
        if (dealerCardOpen->GetRank() == 11 && bets[0]->IsAskedForInsurance()) {
            bets[0]->SetStand(true);
        }
        if (dealerCardClosed->GetRank() == 11) {
            bets[0]->SetStand(true);
        }
    }
    if (bet->GetDrawnCards()->GetValue() >= 21) {
        bet->SetStand(true);
    }
    if (bet2nd && bet2nd->GetDrawnCards()->GetValue() >= 21) {
        bet2nd->SetStand(true);
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


bool Game::allResultsChecked() const {
    // all bets are checked if none is not-checked
    auto it = std::find_if(bets.begin(), bets.end(),
                           [](const std::shared_ptr<Bet> &bet) -> bool { return !bet->ResultChecked(); });
    return it == bets.end();
}

void Game::Payout() const {
    if (IsDone()) {
        std::for_each(bets.begin(), bets.end(),
                      [&](const std::shared_ptr<Bet> &bet) { bet->GetPlayer()->AddCash(GetPayoutValue(bet)); });
    }
}

int Game::GetPayoutValue(std::shared_ptr<Bet> bet) const {
    int payout = 0;
    int playerTotalValue = bet->GetDrawnCards()->GetValue();
    if (playerTotalValue <= 21) {
        int totalValueDealer = drawnCardsDealer->GetValue();
        if (bet->GetDrawnCards()->IsBlackJack() && drawnCardsDealer->IsBlackJack()) {
            payout += bet->GetBet();
        } else if (bet->GetDrawnCards()->IsBlackJack()) {
            payout += 2.5 * bet->GetBet();
        } else if (totalValueDealer > 21 || playerTotalValue > totalValueDealer) {
            payout += 2 * bet->GetBet();
        } else if (playerTotalValue == totalValueDealer) {
            payout += bet->GetBet();
        }
        // side bet
        if (drawnCardsDealer->IsBlackJack() && bet->IsInsuranceBought()) {
            payout += 1.5 * bet->GetBet();
        }
    }
    return payout;
}

bool Game::IsDone() const {
    auto it = std::find_if(bets.begin(), bets.end(),
                           [&](const std::shared_ptr<Bet> &bet) -> bool { return !bet->IsDone(); });
    OATPP_LOGD("Game", "[IsDone] ret = %d", it == bets.end());
    return it == bets.end();
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
    const std::lock_guard<std::mutex> lockGuard(cash_mutex);
    return cash;
}

void Player::SubCash(int bet) {
    const std::lock_guard<std::mutex> lockGuard(cash_mutex);
    cash -= bet;
}

void Player::AddCash(int bet) {
    const std::lock_guard<std::mutex> lockGuard(cash_mutex);
    OATPP_LOGD("Player", "[AddCash] From %d add %d = %d", cash, bet, cash + bet);
    cash += bet;
    HighscoreList::GetInstance().CheckHighScore(id, cash, name);
}

bool Player::IsOutDated() const {
    const std::lock_guard<std::mutex> lockGuard(lastUsed_mutex);
    if (oatpp::base::Environment::getLogger()->isLogPriorityEnabled(oatpp::base::Logger::PRIORITY_D)) {
        OATPP_LOGD("Player", "[IsOutDated] Created at %s", toString(this->lastUsed).c_str());
    }
    std::chrono::seconds later{LIFETIME_PLAYER};
    auto now = std::chrono::system_clock::now();
    auto t1 = now - later;
    return this->lastUsed < t1;
}

void Player::Use() {
    const std::lock_guard<std::mutex> lockGuard(lastUsed_mutex);
    this->lastUsed = std::chrono::system_clock::now();
}

const std::string &Player::GetName() const {
    return this->name;
}

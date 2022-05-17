
#include "GameLogic.hpp"

Card::Card(const std::string &desc) : desc(desc) {
}

const std::string &Card::GetDesc() const {
    return this->desc;
}


RegularCard::RegularCard(int value, const std::string &desc) : Card(desc), value(value) {
}

int RegularCard::GetValue([[maybe_unused]] const std::vector<std::shared_ptr<Card>> &BunchOfCards) const {
    return this->value;
}


AceCard::AceCard(const std::string &desc) : Card(desc) {
}

int AceCard::GetValue(const std::vector<std::shared_ptr<Card>> &BunchOfCards) const {
    int total = accumulate(BunchOfCards.begin(), BunchOfCards.end(), 0, [&](int start, std::shared_ptr<Card> card) {
        if (card.get() != this) {
            return start + card->GetValue(BunchOfCards);
        }
        return start;
    });
    return total < 11 ? 11 : 1;
}


void Deck::AddCard(const std::shared_ptr<Card> card) {
    this->cards.push_back(card);
}

const std::vector<std::shared_ptr<Card>> &Deck::GetCards() const {
    return this->cards;
}


int DrawnCards::GetValue() const {
    return accumulate(cards.begin(), cards.end(), 0, [&](int start, std::shared_ptr<Card> card) {
        return start + card->GetValue(cards);
    });
}

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

void Game::Bet(int _bet, BetResponse::Wrapper betResponse) {
    if (_bet > this->player->GetCash()) {
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
}

bool Game::CheckEnd(bool done, EndResponse::Wrapper endResponse) {
    const int playerTotalValue = drawnCards->GetValue();
    if (playerTotalValue > 21) {
        endResponse->result = "You busted!!!";
        return true;
    } else if (done) {
        endResponse->dealersSecondCard = dealerCardClosed->GetDesc();
        int totalValueDealer = drawnCardsDealer->GetValue();
        endResponse->dealersAdditionalCard = {};
        while (totalValueDealer < 17) {
            const auto card = drawDeck->DrawCard();
            drawnCardsDealer->AddCard(card);
            totalValueDealer = drawnCardsDealer->GetValue();
            endResponse->dealersAdditionalCard->push_back(card->GetDesc());
        }
        endResponse->dealerTotal = totalValueDealer;
        if (totalValueDealer > 21) {
            endResponse->result = "You won!!";
            player->AddCash(2 * bet);
        } else if (playerTotalValue > totalValueDealer) {
            endResponse->result = "You won!!";
            player->AddCash(2 * bet);
        } else {
            endResponse->result = "You lost!!";
        }
        return true;
    }
    return false;
}


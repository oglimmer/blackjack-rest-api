

#include "Deck.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

#include <cstdarg>

#define LIFETIME_DRAWDECK 3600


/* ***************************************** Deck ******************************************************* */

void Deck::AddCard(const std::shared_ptr<Card> card) {
    const std::lock_guard<std::mutex> lock_guard(cards_mutex);
    this->cards.push_back(card);
}

void Deck::ForEachCard(std::function<void(const std::shared_ptr<Card>&)> function) {
    const std::lock_guard<std::mutex> lock_guard(cards_mutex);
    std::for_each(this->cards.begin(), this->cards.end(), function);
}

std::shared_ptr<Card> Deck::GetCard(int pos) const {
    const std::lock_guard<std::mutex> lock_guard(cards_mutex);
    return cards.at(pos);
}

const std::shared_ptr<Card> Deck::DrawCard() {
    const std::lock_guard<std::mutex> lock_guard(cards_mutex);
    std::shared_ptr<Card> topCard = cards.back();
    OATPP_LOGD("Deck", "[DrawCard] Drew = %s", topCard->GetDesc().c_str());
    cards.pop_back();
    return topCard;
}

/* ***************************************** DrawnCards ******************************************************* */

int DrawnCards::GetValue() const {
    const std::lock_guard<std::mutex> lock_guard(cards_mutex);
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

bool DrawnCards::Simple9_10_11() const {
    int val = GetValue();
    const std::lock_guard<std::mutex> lock_guard(cards_mutex);
    return val >= 9 && val <= 11 && cards.size() == 2;
}

bool DrawnCards::IsBlackJack() const {
    int val = GetValue();
    const std::lock_guard<std::mutex> lock_guard(cards_mutex);
    return cards.size() == 2 && val == 21;
}

bool DrawnCards::IsSimplePair() const {
    const std::lock_guard<std::mutex> lock_guard(cards_mutex);
    return cards.size() == 2 && cards[0]->GetRank() == cards[1]->GetRank();
}

/* ***************************************** DrawDeck ******************************************************* */

void DrawDeck::AddCard(const std::shared_ptr<Card> card) {
    Deck::AddCard(card);
    const std::lock_guard<std::mutex> lock_guard(cards_mutex);
    this->allCards.push_back(card);
}

void DrawDeck::shuffle() {
    const std::lock_guard<std::mutex> lock_guard(cards_mutex);
    if (!Package::GetInstance().IsCheat()) {
        std::shuffle(begin(cards), end(cards), Rnd::GetInstance().GetEngine());
    }
}

void DrawDeck::ReshuffleIfNeeded() {
    const std::lock_guard<std::mutex> lock_guard(cards_mutex);
    if (cards.size() < 52 && !Package::GetInstance().IsCheat()) {
        cards.clear();
        cards.insert(cards.end(), allCards.begin(), allCards.end());
        std::shuffle(begin(cards), end(cards), Rnd::GetInstance().GetEngine());
    }
}

bool DrawDeck::IsOutDated() const {
    const std::lock_guard<std::mutex> lock_guard(lastUsed_mutex);
    if (oatpp::base::Environment::getLogger()->isLogPriorityEnabled(oatpp::base::Logger::PRIORITY_D)) {
        OATPP_LOGD("DrawDeck", "[IsOutDated] Created at %s", toString(this->lastUsed).c_str());
    }
    std::chrono::seconds later{LIFETIME_DRAWDECK};
    auto now = std::chrono::system_clock::now();
    auto t1 = now - later;
    return this->lastUsed < t1;
}

void DrawDeck::Use() {
    const std::lock_guard<std::mutex> lock_guard(lastUsed_mutex);
    this->lastUsed = std::chrono::system_clock::now();
}

/* ***************************************** Package ******************************************************* */

void Package::Add52Cards(std::shared_ptr<DrawDeck> drawDeck) {
    for (int j = 0; j < 4; j++) {
        for (int i = 2; i < 11; i++) {
            drawDeck->AddCard(std::shared_ptr<Card>(new RegularCard(i, std::to_string(i) + "-of-" + SUITES[j])));
        }
        drawDeck->AddCard(std::shared_ptr<Card>(new RegularCard(10, "Jack-of-" + SUITES[j])));
        drawDeck->AddCard(std::shared_ptr<Card>(new RegularCard(10, "Queen-of-" + SUITES[j])));
        drawDeck->AddCard(std::shared_ptr<Card>(new RegularCard(10, "King-of-" + SUITES[j])));
        drawDeck->AddCard(std::shared_ptr<Card>(new AceCard("Ace-of-" + SUITES[j])));
    }
}

std::shared_ptr<DrawDeck> Package::CreateDrawDeck() {
    auto newDrawDeck = std::shared_ptr<DrawDeck>(new DrawDeck());
    if (IsCheat()) {
        std::for_each(deckDefintion.begin(), deckDefintion.end(), [&](int rank) {
            OATPP_LOGI("Package", "[CreateDrawDeck] Cheat Add Card = %d", rank);
            std::shared_ptr<Card> card;
            if (rank == 11) {
                card = std::shared_ptr<Card>(new AceCard("11-of-" + SUITES[0]));
            } else {
                card = std::shared_ptr<Card>(new RegularCard(rank, std::to_string(rank) + "-of-" + SUITES[0]));
            }
            newDrawDeck->AddCard(card);
        });
    } else {
        for (int i = 0; i < 6; i++) {
            Add52Cards(newDrawDeck);
        }
    }
    return newDrawDeck;
}

void Package::Cheat(int ranksCount, ...) {
    OATPP_LOGI("Package", "[Cheat] Called with %d parameters", ranksCount);
    this->deckDefintion.clear();
    va_list vl;
    va_start(vl, ranksCount);
    for (int i = 0; i < ranksCount; i++) {
        this->deckDefintion.insert(this->deckDefintion.begin(), va_arg(vl, int));
    }
    va_end(vl);
}

bool Package::IsCheat() const {
    return !deckDefintion.empty();
}
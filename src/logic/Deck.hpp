
#ifndef BLACKJACK_DECK_HPP
#define BLACKJACK_DECK_HPP

#include <memory>
#include <vector>

#include "./util.h"
#include "Card.hpp"

class Deck {
protected:
    std::vector<std::shared_ptr<Card>> cards;
public:
    void AddCard(const std::shared_ptr<Card> card);

    const std::vector<std::shared_ptr<Card>> &GetCards() const;

    const std::shared_ptr<Card> DrawCard();
};


class DrawnCards : public Deck {
public:
    int GetValue() const;

    int Size() const;

    bool Simple9_10_11() const;

    bool IsBlackJack() const;

    bool IsSimplePair() const;
};

class DrawDeck : public Deck {
private:
    std::chrono::time_point<std::chrono::system_clock> lastUsed = std::chrono::system_clock::now();
protected:
    std::vector<std::shared_ptr<Card>> allCards;
public:
    void AddCard(const std::shared_ptr<Card> card);

    void shuffle();

    void ReshuffleIfNeeded();

    bool IsOutDated() const;

    void Use();
};


class Package {
    SINGLETON(Package)

private:
    const std::string SUITES[4];
    std::vector<int> deckDefintion;

    Package() : SUITES{"Hearts", "Spades", "Diamonds", "Clubs"} {}

    void Add52Cards(std::shared_ptr<DrawDeck> drawDeck);

public:
    std::shared_ptr<DrawDeck> CreateDrawDeck();

    void Cheat(int ranks, ...);

    bool IsCheat() const;
};

#endif //BLACKJACK_DECK_HPP

#ifndef GameLogic_hpp
#define GameLogic_hpp

#define OATPP_DISABLE_LOGD

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <random>

#include "oatpp/web/server/api/ApiController.hpp"
#include "../dto/DTOs.hpp"
#include "./util.h"

class Card {
protected:
    Card(const std::string &desc);

private:
    const std::string desc;
public:
    virtual int GetValue(const std::vector<std::shared_ptr<Card>> &BunchOfCards) const = 0;

    const std::string &GetDesc() const;
};

class RegularCard : public Card {
public:
    RegularCard(int value, const std::string &desc);

private:
    const int value;
public:
    virtual int GetValue([[maybe_unused]] const std::vector<std::shared_ptr<Card>> &BunchOfCards) const override;
};

class AceCard : public Card {
public:
    explicit AceCard(const std::string &desc);

    virtual int GetValue(const std::vector<std::shared_ptr<Card>> &BunchOfCards) const override;
};

class Deck {
protected:
    std::vector<std::shared_ptr<Card>> cards;
public:
    void AddCard(const std::shared_ptr<Card> card);

    const std::vector<std::shared_ptr<Card>> &GetCards() const;
};


class DrawnCards : public Deck {
public:
    int GetValue() const;
    int Size() const;
};

class DrawDeck : public Deck {
private:
    std::chrono::time_point<std::chrono::system_clock> lastUsed = std::chrono::system_clock::now();
protected:
    std::vector<std::shared_ptr<Card>> allCards;
public:
    void AddCard(const std::shared_ptr<Card> card);

    void shuffle();

    const std::shared_ptr<Card> DrawCard();

    void ReshuffleIfNeeded();

    bool IsOutDated() const;

    void Use();
};


class Package {
private:
    Package() {}

    static void Add52Cards(std::shared_ptr<DrawDeck> drawDeck);

public:
    static std::shared_ptr<DrawDeck> CreateDrawDeck();
};

class Player {
public:
    Player() : cash(1000) {
    }

private:
    int cash;
    std::chrono::time_point<std::chrono::system_clock> lastUsed = std::chrono::system_clock::now();
public:
    int GetCash() const {
        return cash;
    }

    void SubCash(int bet) {
        cash -= bet;
    }

    void AddCash(int bet) {
        cash += bet;
    }

    bool IsOutDated() const;

    void Use();
};

class Game {
public:
    Game(std::shared_ptr<Player> player, std::shared_ptr<DrawDeck> drawDeck);

private:
    std::shared_ptr<DrawDeck> drawDeck;
    std::unique_ptr<DrawnCards> drawnCards;
    std::unique_ptr<DrawnCards> drawnCardsDealer;
    std::shared_ptr<Card> dealerCardClosed;
    std::shared_ptr<Player> player;
    int bet = 0;
    std::chrono::time_point<std::chrono::system_clock> lastUsed = std::chrono::system_clock::now();
public:
    bool Hit(HitResponse::Wrapper hitResponse);

    bool Stand(StandResponse::Wrapper standResponse);

    bool Bet(int _bet, BetResponse::Wrapper betResponse);

    bool IsOutDated() const;

    void Use();
private:
    bool CheckEnd(bool done, EndResponse::Wrapper endResponse);
};


#endif

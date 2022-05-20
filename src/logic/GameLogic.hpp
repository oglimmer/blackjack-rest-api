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
    /**
     * A card must add its value to all elements of `result`, a card may add new elements if it has multiple values
     * @param result will have at least 1 element.
     */
    virtual void GetValue(std::vector<int> &result) const = 0;

    const std::string &GetDesc() const;
};

class RegularCard : public Card {
public:
    RegularCard(int value, const std::string &desc);

private:
    const int value;
public:
    /**
     * A regular card will just add its value to all elements
     * @param result number of elements will not be changed
     */
    virtual void GetValue(std::vector<int> &result) const override;
};

class AceCard : public Card {
public:
    explicit AceCard(const std::string &desc);

    /**
     * An Ace will duplicate the elements in `result` and then add 1 to the first half and 11 to the second half.
     * @param result number of elements will be doubled
     */
    virtual void GetValue(std::vector<int> &result) const override;
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
    bool Val9_10_11() const;
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
    Player();

private:
    int cash;
    std::chrono::time_point<std::chrono::system_clock> lastUsed = std::chrono::system_clock::now();
public:
    int GetCash() const;

    void SubCash(int bet);

    void AddCash(int bet);

    bool IsOutDated() const;

    void Use();
};

class Bet {
private:
    int betId;
    std::unique_ptr<DrawnCards> drawnCards;
    int bet;
    std::shared_ptr<Player> player;
public:
    Bet(std::shared_ptr<Player> player, int bet);

    std::unique_ptr<DrawnCards>& GetDrawnCards();
    std::shared_ptr<Player> GetPlayer() const;
    int GetBetId() const;
    int GetBet() const;
    void IncBet(int additionalBet);
};

class Game {
public:
    Game(std::shared_ptr<DrawDeck> drawDeck);

private:
    std::shared_ptr<DrawDeck> drawDeck;
    std::unique_ptr<DrawnCards> drawnCardsDealer;
    std::shared_ptr<Card> dealerCardClosed;
    std::vector<std::shared_ptr<Bet>> bets;
    std::chrono::time_point<std::chrono::system_clock> lastUsed = std::chrono::system_clock::now();
public:
    bool PlaceBet(int _bet, std::shared_ptr<Player> player, BetResponse::Wrapper betResponse);
    std::shared_ptr<Bet> GetBet(int betId);

    bool Hit(std::shared_ptr<Bet> bet, HitResponse::Wrapper hitResponse);
    bool Stand(std::shared_ptr<Bet> bet, StandResponse::Wrapper standResponse);
    bool DoubleBet(std::shared_ptr<Bet> bet, HitResponse::Wrapper hitResponse);

    bool IsOutDated() const;
    void Use();
private:
    bool CheckEnd(bool done, std::shared_ptr<Bet> bet, EndResponse::Wrapper endResponse);
};


#endif

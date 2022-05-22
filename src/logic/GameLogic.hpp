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
    bool stand;
public:
    Bet(std::shared_ptr<Player> player, int bet);

    std::unique_ptr<DrawnCards> &GetDrawnCards();

    std::shared_ptr<Player> GetPlayer() const;

    int GetBetId() const;

    int GetBet() const;

    void IncBet(int additionalBet);

    bool GetStand() const;

    void SetStand(bool stand);

    bool IsDone() const;

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
    void PlaceBet(int betVal, std::shared_ptr<Player> player, BetResponse::Wrapper &betResponse);

    std::shared_ptr<Bet> GetBet(int betId);

    void Hit(std::shared_ptr<Bet> bet, HitResponse::Wrapper &hitResponse);

    void Stand(std::shared_ptr<Bet> bet, StandResponse::Wrapper &standResponse);

    void DoubleBet(std::shared_ptr<Bet> bet, HitResponse::Wrapper &hitResponse);

    void Split(std::shared_ptr<Bet> bet, SplitResponse::Wrapper &splitResponse);

    bool IsOutDated() const;

    void Use();

    bool IsBetDone(std::shared_ptr<Bet> bet) const;

    bool AddResponse(std::shared_ptr<Bet> bet, BetGetResponse::Wrapper &response) const;

private:
    std::unique_ptr<std::vector<std::string>> AddFollowActions(std::shared_ptr<Bet> bet);

    void WrapUp();

    void AdvanceDealer();

    void Payout() const;

    void Payout(std::shared_ptr<Bet> bet) const;

    bool IsDone() const;
};


#endif

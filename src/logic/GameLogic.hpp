#ifndef GameLogic_hpp
#define GameLogic_hpp

#define OATPP_DISABLE_LOGD

#include <map>
#include <algorithm>

#include "oatpp/web/server/api/ApiController.hpp"
#include "../dto/DTOs.hpp"
#include "./util.h"
#include "Card.hpp"
#include "Deck.hpp"


class Player {
public:
    Player(int id);

    Player(int id, const std::string &name);

private:
    int id;
    std::string name;
    int cash;
    std::chrono::time_point<std::chrono::system_clock> lastUsed = std::chrono::system_clock::now();
public:
    int GetId() const;

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
    bool isAskedForInsurance;
    bool isInsuranceBought;
    bool isResultChecked;
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

    bool IsAskedForInsurance() const;

    void SetAskedForInsurance(bool b);

    bool IsInsuranceBought() const;

    void SetInsuranceBought(bool b);

    bool ResultChecked() const;

    void SetResultChecked(bool b);
};

class Game {
public:
    Game(std::shared_ptr<DrawDeck> drawDeck);

private:
    std::shared_ptr<DrawDeck> drawDeck;
    std::unique_ptr<DrawnCards> drawnCardsDealer;
    std::shared_ptr<Card> dealerCardClosed;
    std::shared_ptr<Card> dealerCardOpen;
    std::vector<std::shared_ptr<Bet>> bets;
    std::chrono::time_point<std::chrono::system_clock> lastUsed = std::chrono::system_clock::now();
public:
    void PlaceBet(int betVal, std::shared_ptr<Player> player, BetResponse::Wrapper &betResponse);

    std::shared_ptr<Bet> GetBet(int betId);

    void Hit(std::shared_ptr<Bet> bet, HitResponse::Wrapper &hitResponse);

    void Stand(std::shared_ptr<Bet> bet, StandResponse::Wrapper &standResponse);

    void DoubleBet(std::shared_ptr<Bet> bet, HitResponse::Wrapper &hitResponse);

    void Split(std::shared_ptr<Bet> bet, SplitResponse::Wrapper &splitResponse);

    void Insurance(bool insurance, std::shared_ptr<Bet> bet, StandResponse::Wrapper &standResponse);

    bool IsOutDated() const;

    void Use();

    bool AddResponse(std::shared_ptr<Bet> bet, BetGetResponse::Wrapper &response) const;

private:
    std::unique_ptr<std::vector<std::string>> AddFollowActions(std::shared_ptr<Bet> bet);

    bool IsFollowActionsAllowed(std::shared_ptr<Bet> bet, std::string action);

    void WrapUp();

    void AdvanceDealer();

    void Payout() const;

    void Payout(std::shared_ptr<Bet> bet) const;

    bool IsDone() const;

    bool allResultsChecked() const;

};


#endif

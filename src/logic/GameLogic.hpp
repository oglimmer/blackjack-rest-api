#ifndef GameLogic_hpp
#define GameLogic_hpp


#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <random>

#include "oatpp/web/server/api/ApiController.hpp"
#include "../dto/DTOs.hpp"

using namespace std;

#define SINGLETON(clazz) \
    public: \
        clazz(const clazz&) = delete; \
        clazz& operator=(const clazz &) = delete; \
        clazz(clazz &&) = delete; \
        clazz & operator=(clazz &&) = delete; \
        static auto& GetInstance(){ \
            static clazz instance; \
            return instance; \
        }

class Card
{
    protected:
        Card(const string& desc) : desc(desc)
        {  
        }
    private:
        const string desc;
    public:
        virtual int GetValue(const vector<shared_ptr<Card>>& BunchOfCards) const = 0;
        const string& GetDesc() const
        {
            return this->desc;
        }
};

class RegularCard : public Card
{
    public:
        RegularCard(int value, const string& desc) : Card(desc), value(value)
        {            
        }
    private:
        const int value;
    public:
        virtual int GetValue([[maybe_unused]] const vector<shared_ptr<Card>>& BunchOfCards) const override
        {
            return this->value;
        }
};

class AceCard : public Card
{
    public:
        explicit AceCard(const string& desc) : Card(desc)
        {            
        }        

        virtual int GetValue(const vector<shared_ptr<Card>>& BunchOfCards) const override
        {
            int total = accumulate(BunchOfCards.begin(), BunchOfCards.end(), 0, [&](int start, shared_ptr<Card> card) {
                if (card.get() != this) {
                    return start + card->GetValue(BunchOfCards);
                }          
                return start;      
            });
            return total < 11 ? 11 : 1;
        }
};

class Deck
{
    protected:
        vector<shared_ptr<Card>> cards;
    public:
        void AddCard(const shared_ptr<Card> card)
        {
            this->cards.push_back(card);
        }
        const vector<shared_ptr<Card>>& GetCards() const
        {
            return this->cards;
        }
        void debug() const
        {
            for_each(cards.begin(), cards.end(), [](shared_ptr<Card> const& card)
            {
                cout << card->GetDesc() << endl;
            });            
        }
};

class Rnd
{
    SINGLETON(Rnd)
    private:
        default_random_engine engine;
        Rnd() 
        {
            engine.seed(chrono::system_clock::now().time_since_epoch().count()); 
        }
    public:
        default_random_engine& GetEngine()
        {
            return engine;
        }
};

class DrawnCards : public Deck
{
    public:
        int GetValue() const
        {
            return accumulate(cards.begin(), cards.end(), 0, [&](int start, shared_ptr<Card> card) {
                return start + card->GetValue(cards);
            });
        }
};

class DrawDeck : public Deck
{
    protected:
        vector<shared_ptr<Card>> allCards;
    public:
        void AddCard(const shared_ptr<Card> card)
        {
            Deck::AddCard(card);
            this->allCards.push_back(card);
        }
        void shuffle()
        {                        
            std::shuffle(begin(cards), end(cards), Rnd::GetInstance().GetEngine());
        }
        const shared_ptr<Card> DrawCard()
        {
            shared_ptr<Card> topCard = cards.back();
            cards.pop_back();
            return topCard;
        }        
        void ReshuffleIfNeeded()
        {
            if (cards.size() < 52) {
                cards.clear();
                cards.insert(cards.end(), allCards.begin(), allCards.end());
                std::shuffle(begin(cards), end(cards), Rnd::GetInstance().GetEngine());
                cout << "Deck reshuffled" << endl;
            }
        }
};



class Package
{
    private:
        Package() {}
        static void Add52Cards(shared_ptr<DrawDeck> drawDeck)
        {
            const string suits[] = {"Hearts", "Spades", "Diamonds", "Clubs"};
            for(int j = 0; j < 4; j++) {
                for(int i = 2; i < 11; i++) {
                    drawDeck->AddCard(shared_ptr<Card>(new RegularCard(i, to_string(i) + "-of-" + suits[j])));
                }
                drawDeck->AddCard(shared_ptr<Card>(new RegularCard(10, "Jack-of-" + suits[j])));
                drawDeck->AddCard(shared_ptr<Card>(new RegularCard(10, "Queen-of-" + suits[j])));
                drawDeck->AddCard(shared_ptr<Card>(new RegularCard(10, "King-of-" + suits[j])));
                drawDeck->AddCard(shared_ptr<Card>(new AceCard("Ace-of-" + suits[j])));
            }
        }
    public:
        static shared_ptr<DrawDeck> CreateDrawDeck()
        {
            auto newDrawDeck = shared_ptr<DrawDeck>(new DrawDeck());
            for(int i = 0; i < 6; i++) {
                Add52Cards(newDrawDeck);
            }
            return newDrawDeck;
        }
};

class Player
{
    public:
        Player() : cash(1000) {
        }
    private:
        int cash;
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
};

class Game
{
    public:
        Game(shared_ptr<Player> player, shared_ptr<DrawDeck> drawDeck) : drawDeck(drawDeck), player(player) {
        }
    private:
        shared_ptr<DrawDeck> drawDeck;
        unique_ptr<DrawnCards> drawnCards;
        unique_ptr<DrawnCards> drawnCardsDealer;
        shared_ptr<Card> dealerCardClosed;
        shared_ptr<Player> player;
        int bet = 0;
    public:

        void Hit(HitResponse::Wrapper hitResponse)
        {
            const auto card = drawDeck->DrawCard();
            drawnCards->AddCard(card);

            hitResponse->drawnCard = card->GetDesc();
            hitResponse->yourTotal = drawnCards->GetValue();

            CheckEnd(false, hitResponse);
        }


        void Stand(StandResponse::Wrapper standResponse)
        {
            CheckEnd(true, standResponse);
        }

        void Bet(int _bet, BetResponse::Wrapper betResponse)
        {
            if (_bet > this->player->GetCash()) {
                throw std::exception();
            }
            this->bet = _bet;
            this->player->SubCash(bet);

            drawnCardsDealer = unique_ptr<DrawnCards>(new DrawnCards());

            auto dealerCardOpen = drawDeck->DrawCard();
            dealerCardClosed = drawDeck->DrawCard();
            betResponse->dealersCard = dealerCardOpen->GetDesc();

            drawnCardsDealer->AddCard(dealerCardOpen);
            drawnCardsDealer->AddCard(dealerCardClosed);

            drawnCards = unique_ptr<DrawnCards>(new DrawnCards());

            const auto c1 = drawDeck->DrawCard();
            const auto c2 = drawDeck->DrawCard();

            betResponse->card1 = c1->GetDesc();
            betResponse->card2 = c2->GetDesc();

            drawnCards->AddCard(c1);
            drawnCards->AddCard(c2);

            betResponse->yourTotal = drawnCards->GetValue();
        }
    private:
        void CheckEnd(bool done, EndResponse::Wrapper endResponse)
        {
            const int playerTotalValue = drawnCards->GetValue();
            if (playerTotalValue > 21) {
                endResponse->result = "You busted!!!";
            }
            else if (done) {
                endResponse->dealersSecondCard = dealerCardClosed->GetDesc();
                int totalValueDealer = drawnCardsDealer->GetValue();
                endResponse->dealersAdditionalCard = "";
                while (totalValueDealer < 17) {
                    const auto card = drawDeck->DrawCard();
                    drawnCardsDealer->AddCard(card);
                    totalValueDealer = drawnCardsDealer->GetValue();
                    endResponse->dealersAdditionalCard = endResponse->dealersAdditionalCard + ", " + card->GetDesc();
                }
                endResponse->dealerTotal = totalValueDealer;
                if (totalValueDealer > 21) {
                    endResponse->result = "You won!!";
                    player->AddCash(2*bet);
                } else if (playerTotalValue > totalValueDealer) {
                    endResponse->result = "You won!!";
                    player->AddCash(2*bet);
                } else {
                    endResponse->result = "You lost!!";
                }
            }            
        }

};


class DrawDeckRegistry
{
    SINGLETON(DrawDeckRegistry)
    private:
        map<int, shared_ptr<DrawDeck>> drawDecks;
        DrawDeckRegistry() {}
    public:
        int CreateDrawDeck()
        {
            auto drawDeck = Package::CreateDrawDeck();
            drawDeck->shuffle();
            int id = Rnd::GetInstance().GetEngine()();
            drawDecks.insert_or_assign(id, drawDeck);
            return id;
        }
        shared_ptr<DrawDeck> GetDrawDeck(int id)
        {
            return drawDecks[id];
        }
};

class GameRegistry
{
    SINGLETON(GameRegistry)
    private:
        map<int, shared_ptr<Game>> games;
        GameRegistry() {}
    public:
        int CreateGame(shared_ptr<Player> player, shared_ptr<DrawDeck> drawDeck)
        {
            drawDeck->ReshuffleIfNeeded();
            auto game = shared_ptr<Game>(new Game(player, drawDeck));
            int id = Rnd::GetInstance().GetEngine()();
            games.insert_or_assign(id, game);
            return id;
        }
        shared_ptr<Game> GetGame(int id)
        {
            return games[id];
        }
};

class PlayerRegistry
{
    SINGLETON(PlayerRegistry)
    private:
        map<int, shared_ptr<Player>> players;
        PlayerRegistry() {}
    public:
        int CreatePlayer()
        {
            auto player = shared_ptr<Player>(new Player());            
            int id = Rnd::GetInstance().GetEngine()();
            players.insert_or_assign(id, player);
            return id;
        }  
        shared_ptr<Player> GetPlayer(int id)
        {
            return players[id];
        }     
};



#endif

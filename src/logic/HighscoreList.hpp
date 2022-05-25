
#ifndef BLACKJACK_HIGHSCORELIST_HPP
#define BLACKJACK_HIGHSCORELIST_HPP

#include "util.h"

#include <string>
#include <vector>
#include <mutex>

class Highscore {
private:
    int id;
    std::string name;
    int maxMoney;

public:
    Highscore(int id, const std::string &name, int maxMoney);

    int GetMaxMoney() const;

    void SetMaxMoney(int money);

    const std::string &GetName() const;

    int GetId() const;
};

class HighscoreList {
SINGLETON(HighscoreList)

private:
    HighscoreList();

    std::vector<Highscore> highscore;
    std::mutex mutex;
public:
    void CheckHighScore(int id, int money, const std::string &name);

    const std::unique_ptr<std::vector<std::string>> GetHighscores();
};


#endif //BLACKJACK_HIGHSCORELIST_HPP
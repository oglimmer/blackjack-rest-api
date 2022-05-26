

#include "HighscoreList.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "database/DBHighscore.hpp"

Highscore::Highscore(int id, const std::string &name, int maxMoney) : maxMoney(maxMoney), name(name), id(id) {

}

int Highscore::GetId() const {
    return id;
}

int Highscore::GetMaxMoney() const {
    return maxMoney;
}

void Highscore::SetMaxMoney(int money) {
    maxMoney = money;
}

const std::string &Highscore::GetName() const {
    return name;
}

HighscoreList::HighscoreList() {
    auto db = DBHighscore::GetInstance().Get();
    std::for_each(db->highscore->begin(), db->highscore->end(), [&](const oatpp::Object<DBHighscoreElementDto> &ele) {
        CheckHighScore(ele->id, ele->money, ele->name);
    });
}

void HighscoreList::CheckHighScore(int id, int money, const std::string &name) {
    if (name.empty()) {
        return;
    }
    auto lg = std::lock_guard(mutex);
    OATPP_LOGD("HighscoreList", "[CheckHighScore] in %d - %d - %s", id, money, name.c_str());
    if (highscore.empty()) {
        Highscore hs(id, name, money);
        highscore.push_back(hs);
        OATPP_LOGD("HighscoreList", "[CheckHighScore] Added first item. %d", highscore.size());
    } else {
        std::sort(highscore.begin(), highscore.end(),
                  [](const Highscore &o1, const Highscore &o2) -> bool { return o1.GetMaxMoney() > o2.GetMaxMoney(); });
        if (highscore.size() < 10 || money > highscore.back().GetMaxMoney()) {
            OATPP_LOGD("HighscoreList", "[CheckHighScore] Considered! %d > %d", money, highscore.back().GetMaxMoney());
            auto it = std::find_if(highscore.begin(), highscore.end(),
                                   [&](const Highscore &hs) -> bool { return hs.GetId() == id; });
            if (it == highscore.end()) {
                OATPP_LOGD("HighscoreList", "[CheckHighScore] A new entry.");
                Highscore hs(id, name, money);
                highscore.push_back(hs);
            } else {
                OATPP_LOGD("HighscoreList", "[CheckHighScore] Update existing entry.");
                (*it).SetMaxMoney(money);
            }
            std::sort(highscore.begin(), highscore.end(),
                      [](const Highscore &o1, const Highscore &o2) -> bool {
                          return o1.GetMaxMoney() > o2.GetMaxMoney();
                      });

            if (highscore.size() > 10) {
                OATPP_LOGD("HighscoreList", "[CheckHighScore] highscore.size() = %d", highscore.size());
                highscore.pop_back();
                OATPP_LOGD("HighscoreList", "[CheckHighScore] highscore.size() = %d", highscore.size());
            }

            DBHighscore::GetInstance().Store(highscore);
        } else {
            OATPP_LOGD("HighscoreList", "[CheckHighScore] Too less to be considered. %d", highscore.size());
        }
    }
}

const std::unique_ptr<std::vector<std::string>> HighscoreList::GetHighscores() {
    auto lg = std::lock_guard(mutex);
    auto result = std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>());
    std::transform(highscore.begin(), highscore.end(), std::back_inserter(*result),
                   [](const Highscore &hs) -> std::string {
                       return hs.GetName() + " " + std::to_string(hs.GetMaxMoney());
                   });
    return result;
}
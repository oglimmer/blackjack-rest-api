
#include "Card.hpp"

#include <algorithm>

/* ***************************************** Card ******************************************************* */

Card::Card(const std::string &desc) : desc(desc) {
}

const std::string &Card::GetDesc() const {
    return this->desc;
}

/* ***************************************** RegularCard ******************************************************* */

RegularCard::RegularCard(int value, const std::string &desc) : Card(desc), value(value) {
}

void RegularCard::GetValue(std::vector<int> &result) const {
    std::for_each(result.begin(), result.end(), [&](int &i) { i += this->value; });
}

int RegularCard::GetRank() const {
    return this->value;
}

/* ***************************************** AceCard ******************************************************* */

AceCard::AceCard(const std::string &desc) : Card(desc) {
}

void AceCard::GetValue(std::vector<int> &result) const {
    std::vector<int> vect1(result);
    std::vector<int> vect2(result);
    std::for_each(vect1.begin(), vect1.end(), [&](int &i) { i += 1; });
    std::for_each(vect2.begin(), vect2.end(), [&](int &i) { i += 11; });
    result.clear();
    result.insert(result.begin(), vect1.begin(), vect1.end());
    result.insert(result.begin(), vect2.begin(), vect2.end());
}

int AceCard::GetRank() const {
    return 11;
}
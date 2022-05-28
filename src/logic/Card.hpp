#pragma  once


#include <vector>
#include <random>

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

    virtual int GetRank() const = 0;
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

    virtual int GetRank() const override;
};

class AceCard : public Card {
public:
    explicit AceCard(const std::string &desc);

    /**
     * An Ace will duplicate the elements in `result` and then add 1 to the first half and 11 to the second half.
     * @param result number of elements will be doubled
     */
    virtual void GetValue(std::vector<int> &result) const override;

    virtual int GetRank() const override;
};


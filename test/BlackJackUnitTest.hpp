
#ifndef BLACKJACK_BLACKJACKUNITTEST_HPP
#define BLACKJACK_BLACKJACKUNITTEST_HPP

#include "oatpp-test/UnitTest.hpp"

class BlackJackUnitTest : public oatpp::test::UnitTest {
public:

    BlackJackUnitTest() : UnitTest("TEST[BlackJackUnitTest]") {}

    void onRun() override;

private:
    void testSimple();
    void testDoubleAce();
    void testDoubleAce2();
    void testDoubleAce3();

};


#endif //BLACKJACK_BLACKJACKUNITTEST_HPP
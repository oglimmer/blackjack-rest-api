#pragma  once


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

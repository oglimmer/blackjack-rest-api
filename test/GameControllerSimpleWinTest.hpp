#ifndef GameControllerTest_hpp
#define GameControllerTest_hpp

#include "oatpp-test/UnitTest.hpp"

class GameControllerSimpleWinTest : public oatpp::test::UnitTest {
public:

    GameControllerSimpleWinTest() : UnitTest("TEST[GameControllerSimpleWinTest]") {}

    void onRun() override;

};

#endif // GameControllerTest_hpp

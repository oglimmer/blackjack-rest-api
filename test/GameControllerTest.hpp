#ifndef GameControllerTest_hpp
#define GameControllerTest_hpp

#include "oatpp-test/UnitTest.hpp"

class GameControllerTest : public oatpp::test::UnitTest {
public:

    GameControllerTest() : UnitTest("TEST[GameControllerTest]") {}

    void onRun() override;

};

#endif // GameControllerTest_hpp

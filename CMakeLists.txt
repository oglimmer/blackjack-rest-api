cmake_minimum_required(VERSION 3.1)

set(project_name blackjack) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 20)

find_package(oatpp 1.3.0 REQUIRED)
if(oatpp_FOUND)
    message(STATUS "Found oatpp version: ${oatpp_VERSION_STRING}")
else()
    message(FATAL_ERROR "Could not find oatpp")
endif()

find_package(oatpp-swagger  1.3.0 REQUIRED)
if(oatpp-swagger_FOUND)
    message(STATUS "Found oatpp-swagger version: ${oatpp-swagger_VERSION_STRING}")
else()
    message(FATAL_ERROR "Could not find oatpp-swagger")
endif()

include_directories(${oatpp_INCLUDE_DIRS})
include_directories(${oatpp-swagger_INCLUDE_DIRS})

set(SWAGGER_ROOT_PATH "/swagger" CACHE STRING "Default root path to the Swagger")
set(SWAGGER_UI_PATH "/ui" CACHE STRING "Default path suffix to the Swagger UI")

add_compile_definitions(
        SWAGGER_ROOT_PATH="${SWAGGER_ROOT_PATH}"
        SWAGGER_UI_PATH="${SWAGGER_UI_PATH}"
)

add_definitions(
        -DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res"
)


add_library(${project_name}-lib
        src/AppComponent.hpp
        src/controller/GameController.cpp
        src/controller/GameController.hpp
        src/dto/DTOs.hpp
        src/logic/GameLogic.cpp
        src/logic/GameLogic.hpp
        src/logic/Registries.cpp src/logic/Registries.hpp src/logic/Card.cpp src/logic/Card.hpp src/logic/Deck.cpp src/logic/Deck.hpp src/logic/HighscoreList.cpp src/logic/HighscoreList.hpp src/controller/RootController.cpp src/controller/RootController.hpp)

## link libs

find_package(oatpp 1.3.0 REQUIRED)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC oatpp::oatpp-swagger
        )

target_include_directories(${project_name}-lib PUBLIC src)

## add executables

add_executable(${project_name}-exe
        src/App.cpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
        test/GameControllerSimpleWinTest.cpp
        test/GameControllerSimpleWinTest.hpp
        test/BlackJackUnitTest.cpp test/BlackJackUnitTest.hpp test/GameControllerDealerBlackJackTest.cpp test/GameControllerDealerBlackJackTest.hpp)

target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name}-exe ${project_name}-test PROPERTIES
        CXX_STANDARD 20
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
        )

enable_testing()
add_test(project-tests ${project_name}-test)

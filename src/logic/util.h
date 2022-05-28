#pragma  once

#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "oatpp/web/server/api/ApiController.hpp"

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

class Rnd {
SINGLETON(Rnd)

private:
    std::default_random_engine engine;

    Rnd() {
        engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
    }

public:
    std::default_random_engine &GetEngine() {
        return engine;
    }
};

inline std::string toString(const std::chrono::time_point<std::chrono::system_clock>& input) {
    std::time_t t = std::chrono::system_clock::to_time_t(input);
    std::stringstream ss;
    ss << std::put_time( std::localtime( &t ), "%FT%T%z" );
    return ss.str();
}

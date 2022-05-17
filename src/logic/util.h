
#ifndef BLACKJACK_UTIL_H
#define BLACKJACK_UTIL_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <random>

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


#endif //BLACKJACK_UTIL_H



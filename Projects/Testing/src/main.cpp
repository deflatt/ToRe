#include <functional>
#include <iostream>
#include <Windows.h>
#include <map>
#include <tuple>
#include <set>
#include <limits>
#undef min
#undef max
#include <unordered_set>
#include <unordered_map>
#include <random>

struct Test {
    int i;
    struct Hash {
        size_t operator()(const Test& t) const {
            return 0;
        }
    };
};

int main() {
    
    std::default_random_engine randomEngine;
    std::uniform_real_distribution<float> dist;
    

    return 0;
}
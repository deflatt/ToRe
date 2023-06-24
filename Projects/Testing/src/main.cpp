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
    struct Hash {
        size_t operator()(const Test&) const {
            std::cout << "Hashed" << std::endl;
            return 0;
        }
    };
    bool operator==(const Test&) const {
        return true;
    }
};

int main() {

    std::unordered_map<Test, int, Test::Hash> m;
    m.insert({ Test(), 0 });
    if (m.count(Test())){}

    return 0;
}
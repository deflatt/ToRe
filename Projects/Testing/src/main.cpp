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

import TR.Essentials.Array;
using namespace TR;

struct Test {
    int i;
    struct Hash {
        size_t operator()(const Test& t) const {
            return 0;
        }
    };
};

int main() {
    
    std::unordered_set<Test, Test::Hash> set;
    

    return 0;
}
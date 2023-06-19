#include <functional>
#include <iostream>
#include <Windows.h>
#include <map>
#include <tuple>
#include <set>
#include <limits>
#undef min
#undef max
#include <unordered_map>

import TR.Essentials.Array;
using namespace TR;

struct Base {
    int i1, i2;
};

struct Test {
    Base b;
    
    Base* operator->() {
        return &b;
    }

};

int main() {
    
    Test t;
    
	
    return 0;
}
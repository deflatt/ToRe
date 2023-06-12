#include <functional>
#include <iostream>
#include <Windows.h>
#include <map>
#include <tuple>
#include <set>

import TR.Essentials.Array;
using namespace TR;

template <typename T_val, typename T_ind, size_t numDims, size_t degree>
struct BoxMap {

    using Position = Array<T_val, numDims>;

    struct Node {
        Position low = {}, high = {};
        T_ind sibling = -1;
        T_ind child = -1;
    };

};

int main() {
 
    

    return 0;
}
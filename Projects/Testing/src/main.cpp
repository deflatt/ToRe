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

template <typename T, size_t length>
struct std::hash<TR::Array<T, length>> {

	static constexpr size_t numBits = sizeof(size_t) * (size_t)8;

	size_t operator()(const TR::Array<T, length>& a) const {
		size_t result = 0;
		for (size_t i = 0; i < length; i++) {
			size_t curHash = std::hash<T>()(a[i]);
			result ^= (curHash << (i % numBits)) | (curHash >> (numBits - (i % numBits)));
		}
		return result;
	}

};

int main() {
    
	std::unordered_map<Float2, int> map;
	map.insert({ { 1.0f, 2.0f }, 3 });
	map.insert({ { 2.0f, 1.0f }, 4 });
	map.insert({ { 4.0f, 1.0f }, 5 });
	std::cout << map.at({ 2.0f, 1.0f }) << std::endl;

    return 0;
}
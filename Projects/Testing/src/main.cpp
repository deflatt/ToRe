#include <iostream>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <math.h>
#include <string>
#include <functional>
#include <chrono>

using uint = unsigned int;
using byte = unsigned char;

//import TR.Essentials.Clock;

struct Clock {

	using steady_clock = std::chrono::steady_clock;

	struct Duration : public steady_clock::duration {

		template <typename Rep>
		Rep Seconds() const {
			return std::chrono::duration_cast<std::chrono::duration<Rep>>(*this).count();
		}

	};

	Duration Elapsed() {
		return Duration(steady_clock::now() - start);
	}

	Duration Restart() {
		auto now = steady_clock::now();
		Duration duration(now - start);
		start = now;
		return duration;
	}

protected:

	steady_clock::time_point start = steady_clock::now();

};

int main() {

	//TR::Clock<float> clock;
	//while (true)
	//	std::cout << clock.Elapsed().Seconds() << std::endl;

	

	return 0;
}
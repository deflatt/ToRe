#include <iostream>

import TR.Essentials.Array;

int main() {
	using namespace TR;
	
	Float3 f(1.0f, 2.0f, 3.0f);
	std::cout << f.ToString("F: {", " ; ", "}") << std::endl;

	return 0;
}
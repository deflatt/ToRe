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

class Resource {

public:

	void Init() {
		std::cout << "Resource Init" << std::endl;
	}

};

class Upload {

public:

	Resource* resource = {};

	void Init() {
		std::cout << "Upload Init" << std::endl;
	}

};

class UploadResource : public Resource, public Upload {

public:

	UploadResource() {
		Upload::resource = this;
	}

	using Upload::Init;

};

int main() {

	UploadResource up;
	up.Init();

	return 0;
}
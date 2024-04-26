#include <iostream>

import App;

int main() {

	App::_App app = {};
	App::Init(app);

	while (App::IsRunning(app)) {
		App::Update(app);
	}

	return 0;
}
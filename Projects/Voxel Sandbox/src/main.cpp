#include <iostream>

import App;

int main() {

	App::App app = {};
	app.Init();

	while (app.IsRunning()) {
		app.Update();
	}

	return 0;
}
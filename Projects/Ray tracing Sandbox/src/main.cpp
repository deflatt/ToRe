import App;

int main() {
	
	App::_App app = {};
	App::Init(app);

	while (true) {
		App::Update(app);
	}

	return 0;
}

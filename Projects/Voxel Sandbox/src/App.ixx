export module App;

export import App.GUI;
export import App.Scene;

export namespace App {

	struct _App {

		GUI::_GUI gui = {};
		Scene::_Scene scene = {};

	};

	void Init(_App& app) {
		GUI::Init(app.gui, app.scene);
	}

	bool IsRunning(_App& app) {
		return GUI::IsOpen(app.gui);
	}

	void Update(_App& app) {
		GUI::Update(app.gui);
	}

}
export module App.GUI;

export import App.GUI.Window;
export import App.GUI.Scene;

export namespace App::GUI {

	struct _GUI {

		Window::_Window window = {};
		Scene::_Scene scene = {};

	};

	void Init(_GUI& gui, App::Scene::_Scene& scene) {
		Window::Init(gui.window);
		Scene::Init(gui.scene, gui.window, scene);
	}

	bool IsOpen(_GUI& gui) {
		return Window::IsOpen(gui.window);
	}

	void Update(_GUI& gui) {
		Window::HandleInput(gui.window);

		Scene::Update(gui.scene);

		Window::Clear(gui.window);

		Scene::Render(gui.scene);

		Window::Render(gui.window);
	}

}
export module App.GUI;

export import App.GUI.Window;
export import App.GUI.Scene;

export namespace App::GUI {

	class GUI {

	public:

		void Init() {
			window.Init();
			scene.graphics = &window.graphics;
			scene.Init();
		}

		bool IsOpen() {
			return true;
		}

		void Update() {
			window.HandleInput();
			scene.Update();
			window.Clear();
			scene.Render();
			window.Render();
		}

		Window::Window window = {};
		Scene::Scene scene = {};

	};

}
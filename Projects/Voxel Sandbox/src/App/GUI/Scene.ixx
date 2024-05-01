export module App.GUI.Scene;

export import App.GUI.Scene.Renderer;
export import App.GUI.Scene.Camera;
export import App.Scene;

export namespace App::GUI::Scene {

	class Scene {

	public:

		void Init() {
			renderer.graphics = graphics;
			renderer.Init();

			camera.data = &data->camera;
			camera.renderer = &renderer;
			camera.graphics = graphics;
			camera.Init();

			renderer.FinishInit();
		}

		void Update() {
			
		}

		void Render() {
			renderer.Render();
		}

		Renderer::Renderer renderer = {};
		Camera::Camera camera = {};

		Window::Graphics::Graphics* graphics = {};
		App::Scene::Scene* data = {};

	};

}
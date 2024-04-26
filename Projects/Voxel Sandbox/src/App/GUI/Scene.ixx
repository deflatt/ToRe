export module App.GUI.Scene;

export import App.GUI.Scene.Renderer;
export import App.GUI.Scene.Camera;
export import App.Scene;

export namespace App::GUI::Scene {

	struct _Scene {

		Renderer::_Renderer renderer = {};
		Camera::_Camera camera = {};

	};

	void Init(_Scene& scene, Window::_Window& window, App::Scene::_Scene& sceneData) {
		Renderer::Init(scene.renderer, window.graphics);
		Camera::Init(scene.camera, sceneData.camera, scene.renderer, window);
		Renderer::FinishInit(scene.renderer);
	}

	void Update(_Scene& scene) {
		Camera::Update(scene.camera);
	}

	void Render(_Scene& scene) {
		Renderer::Render(scene.renderer);
	}

}
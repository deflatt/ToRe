export module Graphics.Scene.Camera;

export import TR.Windows.Mouse;
export import TR.Windows.Keyboard;
export import Graphics.Window;
export import Scene.Camera;

export namespace Graphics::Scene::Camera {

	struct _Camera {

		TR::Listener keyboardListener = {};
		TR::Listener mouseListener = {};

		TR::Float2 mouseSensitivity = { 0.002f, 0.002f };

	};

	void Init(_Camera& camera, Window::_Window& window, ::Scene::Camera::_Camera& sceneCamera);

	void Update(_Camera& camera, ::Scene::Camera::_Camera& sceneCamera);

	void SetFocus(_Camera& scene);
	void RemoveFocus(_Camera& scene);

}
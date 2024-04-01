export module Graphics.Scene;

export import Graphics.Scene.RayTracer;
export import TR.Graphics.Graphics;
export import Graphics.Window;
export import TR.Windows.Mouse;
export import TR.Windows.Keyboard;
export import Graphics.Scene.Camera;
export import Scene;

export namespace Graphics::Scene {

	struct _Scene {

		RayTracer::_RayTracer rayTracer = {};
		Camera::_Camera camera = {};

		TR::Listener pauseListener = {};
		
		bool focused = false;

	};

	void Init(_Scene& scene, Window::_Window& window, ::Scene::_Scene& sceneData);

	void Update(_Scene& scene, ::Scene::_Scene& sceneData);

	void Render(_Scene& scene, Window::_Window& window);

	void SetFocus(_Scene& scene, Window::_Window& window);
	void RemoveFocus(_Scene& scene);

}
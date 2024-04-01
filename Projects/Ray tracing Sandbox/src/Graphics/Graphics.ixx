export module Graphics;

export import Graphics.Window;
export import Graphics.Scene;
export import Scene;

export namespace Graphics {

	struct _Graphics {

		Window::_Window window = {};
		Scene::_Scene scene = {};

	};

	void Init(_Graphics& graphics, ::Scene::_Scene& scene);

	void Update(_Graphics& graphics, ::Scene::_Scene& scene);

}
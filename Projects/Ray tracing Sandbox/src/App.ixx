export module App;

export import Scene;
export import Graphics;

export namespace App {

	struct _App {

		Scene::_Scene scene = {};
		Graphics::_Graphics graphics = {};

	};

	void Init(_App& app);
	void Update(_App& app);

}
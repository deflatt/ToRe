module Graphics;

import Graphics.Window;

namespace Graphics {

	void Init(_Graphics& graphics, ::Scene::_Scene& scene)
	{
		Window::Init(graphics.window);
		Scene::Init(graphics.scene, graphics.window, scene);
	}

	void Update(_Graphics& graphics, ::Scene::_Scene& scene)
	{
		Window::HandleInput(graphics.window);

		Scene::Update(graphics.scene, scene);

		Window::Clear(graphics.window);
		
		Scene::Render(graphics.scene, graphics.window);

		Window::Render(graphics.window);
	}

}
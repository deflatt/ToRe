module Graphics.Scene;

import <Windows.h>;

namespace Graphics::Scene {

	void Init(_Scene& scene, Window::_Window& window, ::Scene::_Scene& sceneData)
	{
		RayTracer::Init(scene.rayTracer, window.windowGraphics.graphics);

		scene.pauseListener = TR::KeydownListener(
			[&scene, &window](TR::_KeydownEvent event) {
				if (event.key == VK_ESCAPE) {
					if (scene.focused)
						RemoveFocus(scene);
					else
						SetFocus(scene, window);
				}
			}
		);
		TR::AddListener(&scene.pauseListener);

		Camera::Init(scene.camera, window, sceneData.camera);
	}

	void Update(_Scene& scene, ::Scene::_Scene& sceneData)
	{
		if (scene.focused)
			Camera::Update(scene.camera, sceneData.camera);
	}

	void Render(_Scene& scene, Window::_Window& window)
	{
		RayTracer::Render(scene.rayTracer, window);
	}

	void SetFocus(_Scene& scene, Window::_Window& window)
	{
		scene.focused = true;
		Camera::SetFocus(scene.camera);

		RECT rect;
		GetWindowRect(window.window.hwnd, &rect);
		ClipCursor(&rect);
		while (ShowCursor(FALSE) >= 0) {}
	}

	void RemoveFocus(_Scene& scene)
	{
		scene.focused = false;
		Camera::RemoveFocus(scene.camera);

		ClipCursor(nullptr);
		SetCursor(LoadCursor(nullptr, IDC_ARROW));
		while (ShowCursor(TRUE) < 0) {}
	}

}
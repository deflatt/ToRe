module Graphics.Scene.Camera;

import <Windows.h>;
import <iostream>;

namespace Graphics::Scene::Camera {

	void Init(_Camera& camera, Window::_Window& window, ::Scene::Camera::_Camera& sceneCamera)
	{
		TR::RegisterRawMouseInput(window.window.hwnd);

		camera.mouseListener = TR::RawMouseListener(
			[&camera, &sceneCamera](TR::_RawMouseMoveEvent event) {
				sceneCamera.data.rotation = sceneCamera.data.rotation + TR::Float2((float)event.movement[0] * camera.mouseSensitivity[0], (float)event.movement[1] * camera.mouseSensitivity[1]);
				std::cout << TR::ToString(sceneCamera.data.rotation) << std::endl;
			},
			[&camera, &sceneCamera](TR::_RawMouseButtonDownEvent event) {

			},
			[&camera, &sceneCamera](TR::_RawMouseButtonUpEvent event) {

			}
		);
	}

	bool Keydown(int key) {
		return GetKeyState(key) & 0x8000;
	}

	void Update(_Camera& camera, ::Scene::Camera::_Camera& sceneCamera)
	{
		TR::Float2 rot = sceneCamera.data.rotation;

		TR::Float3 dirForward = { cosf(rot[0]), sinf(rot[0]), 0.0f };
		TR::Float3 dirRight = { cosf(rot[0] + 3.14f / 2.0f), sinf(rot[0] + 3.14f / 2.0f), 0.0f };
		TR::Float3 dirLeft = -dirRight;
		TR::Float3 dirBack = -dirForward;
		TR::Float3 dirUp = { 0.0f, 0.0f, 1.0f };
		TR::Float3 dirDown = -dirUp;

		float speed = 0.01f;
		if (Keydown('W'))
			sceneCamera.data.position = sceneCamera.data.position + dirForward * speed;
		if (Keydown('D'))
			sceneCamera.data.position = sceneCamera.data.position + dirRight * speed;
		if (Keydown('A'))
			sceneCamera.data.position = sceneCamera.data.position + dirLeft * speed;
		if (Keydown('S'))
			sceneCamera.data.position = sceneCamera.data.position + dirBack * speed;
		if (Keydown(VK_SPACE))
			sceneCamera.data.position = sceneCamera.data.position + dirUp * speed;
		if (Keydown(VK_SHIFT))
			sceneCamera.data.position = sceneCamera.data.position + dirDown * speed;
	}

	void SetFocus(_Camera& scene)
	{
		TR::AddListener(&scene.mouseListener);
		TR::AddListener(&scene.keyboardListener);
	}

	void RemoveFocus(_Camera& scene)
	{
		TR::RemoveListener(&scene.mouseListener);
		TR::RemoveListener(&scene.keyboardListener);
	}

}
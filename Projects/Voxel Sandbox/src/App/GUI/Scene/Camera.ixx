export module App.GUI.Scene.Camera;

export import App.Scene.Camera;
export import App.GUI.Scene.Renderer;
export import TR.Graphics.ArrayResource;
export import TR.Graphics.BufferResource;
export import App.GUI.Window;
export import TR.Windows.Mouse;
export import TR.Windows.Keyboard;

import <Windows.h>;
import <iostream>;

export namespace App::GUI::Scene::Camera {

	struct _Camera {

		TR::_ArrayBuffer buffer = {};
		TR::Listener mouseListener = {};
		TR::Listener keyboardListener = {};

		App::Scene::Camera::_Camera* data = {};
		Renderer::_Renderer* renderer = {};
		Window::_Window* window = {};

		TR::Float2 mouseSensitivity = { 0.002f, 0.002f };
		bool focused = false;

	};

	void SetFocus(_Camera& camera) {
		camera.focused = true;
		TR::AddListener(&camera.mouseListener);
		//TR::AddListener(&camera.keyboardListener);

		RECT rect;
		GetClientRect(camera.window->window.hwnd, &rect);
		ClientToScreen(camera.window->window.hwnd, (POINT*)&rect.left);
		ClientToScreen(camera.window->window.hwnd, (POINT*)&rect.right);
		ClipCursor(&rect);
		while (ShowCursor(FALSE) >= 0) {}
	}

	void RemoveFocus(_Camera& camera) {
		camera.focused = false;
		TR::RemoveListener(&camera.mouseListener);
		//TR::AddListener(&camera.keyboardListener);

		ClipCursor(nullptr);
		SetCursor(LoadCursor(nullptr, IDC_ARROW));
		while (ShowCursor(TRUE) < 0) {}
	}

	void Init(_Camera& camera, App::Scene::Camera::_Camera& data, Renderer::_Renderer& renderer, Window::_Window& window) {
		camera.data = &data;
		camera.renderer = &renderer;
		camera.window = &window;

		data.aspectRatio = (float)window.size[1] / (float)window.size[0];

		TR::Init(&renderer.renderer.renderer.inputMap.arrays["camera"], 0);
		TR::Init(&camera.buffer, 1, sizeof(App::Scene::Camera::_Camera));
		renderer.renderer.renderer.inputMap.arrays["camera"].gpuAddress = TR::GetGPUAddress(&camera.buffer.resource);

		TR::RegisterRawMouseInput(window.window.hwnd);
		camera.mouseListener = TR::RawMouseListener(
			[&camera, &data](TR::_RawMouseMoveEvent event) {
				data.rotation = data.rotation + TR::Float2((float)event.movement[0] * camera.mouseSensitivity[0], (float)event.movement[1] * camera.mouseSensitivity[1]);
				std::cout << "Rotation " << TR::ToString(data.rotation) << std::endl;
			},
			[](TR::_RawMouseButtonDownEvent event) {

			},
			[](TR::_RawMouseButtonUpEvent event) {

			}
		);

		camera.keyboardListener = TR::KeydownListener(
			[&camera, &data](TR::_KeydownEvent event) {
				if (event.key == VK_ESCAPE) {
					if (camera.focused)
						RemoveFocus(camera);
					else
						SetFocus(camera);
				}
			}
		);
		TR::AddListener(&camera.keyboardListener);

		camera.focused = false;
	}

	bool Keydown(int key) {
		return GetKeyState(key) & 0x8000;
	}

	void Update(_Camera& camera) {
		if (camera.focused) {
			TR::Float2 rot = camera.data->rotation;

			TR::Float3 dirForward = { cosf(rot[0]), sinf(rot[0]), 0.0f };
			TR::Float3 dirRight = { cosf(rot[0] + 3.1415f / 2.0f), sinf(rot[0] + 3.1415f / 2.0f), 0.0f };
			TR::Float3 dirLeft = -dirRight;
			TR::Float3 dirBack = -dirForward;
			TR::Float3 dirUp = { 0.0f, 0.0f, 1.0f };
			TR::Float3 dirDown = -dirUp;

			float speed = 0.03f;
			if (Keydown('W'))
				camera.data->position = camera.data->position + dirForward * speed;
			if (Keydown('A'))
				camera.data->position = camera.data->position + dirLeft * speed;
			if (Keydown('S'))
				camera.data->position = camera.data->position + dirBack * speed;
			if (Keydown('D'))
				camera.data->position = camera.data->position + dirRight * speed;
			if (Keydown(VK_SPACE))
				camera.data->position = camera.data->position + dirUp * speed;
			if (Keydown(VK_SHIFT))
				camera.data->position = camera.data->position + dirDown * speed;
		}

		TR::UploadBuffer(&camera.buffer.upload, &camera.buffer.resource, camera.data, camera.renderer->graphics->graphics.graphics.cmdList.cmdList.Get());
	}

}
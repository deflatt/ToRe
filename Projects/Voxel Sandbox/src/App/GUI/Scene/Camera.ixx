export module App.GUI.Scene.Camera;

export import App.Scene.Camera;
export import App.GUI.Scene.Renderer;
export import TR.Graphics.ArrayResource;
export import TR.Graphics.BufferResource;
export import TR.Graphics.InputParameter;
export import App.GUI.Window;
export import TR.Windows.Mouse;
export import TR.Windows.Keyboard;

import <Windows.h>;

export namespace App::GUI::Scene::Camera {

	class Camera {

	public:

		void Init() {

			data->aspectRatio = (float)graphics->size[1] / (float)graphics->size[0];

			TR::Init(&renderer->renderer.renderer.inputMap.arrays["camera"], 0);			
			//TR::Init(&buffer, 1, sizeof(App::Scene::Camera::Camera));
			//renderer->renderer.renderer.inputMap.arrays["camera"].gpuAddress = TR::GetGPUAddress(&buffer.resource);

		}

		TR::_ArrayBuffer buffer = {};

		App::Scene::Camera::Camera* data = {};
		Renderer::Renderer* renderer = {};
		Window::Graphics::Graphics* graphics = {};

	};

}
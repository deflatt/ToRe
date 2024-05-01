export module App.GUI.Window.Graphics;

import TR.Graphics.WindowGraphics;
import TR.Graphics.Device;
import TR.Windows.Window;
export import <d3d12.h>;

export namespace App::GUI::Window::Graphics {

	class Graphics {

	public:

		void Init() {
			TR::InitDevice();

			RECT clientRect;
			GetClientRect(window->hwnd, &clientRect);
			size = { clientRect.right - clientRect.left, clientRect.bottom - clientRect.top };

			TR::Init(&graphics, window->hwnd, size, false, 3);
		}

		void Clear() {
			TR::Clear(&graphics, { 0.0f, 0.0f, 0.0f, 1.0f });
		}

		void Render() {
			TR::Render(&graphics);
		}

		ID3D12GraphicsCommandList* GetCommandList() {
			return graphics.graphics.cmdList.cmdList.Get();
		}

		TR::_Window* window = {};

		TR::_WindowGraphics graphics = {};
		TR::Int2 size = {};

	};

}
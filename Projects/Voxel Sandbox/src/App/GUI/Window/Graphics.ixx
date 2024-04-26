export module App.GUI.Window.Graphics;

export import TR.Windows.Window;
export import TR.Graphics.WindowGraphics;
import TR.Graphics.Device;

export namespace App::GUI::Window::Graphics {

	struct _Graphics {

		TR::_WindowGraphics graphics = {};
		TR::Int2 size = {};

		TR::_Window* window = {};

	};

	void Init(_Graphics& graphics, TR::_Window& window) {
		TR::InitDevice();

		graphics.window = &window;

		RECT clientRect;
		GetClientRect(window.hwnd, &clientRect);
		graphics.size = { clientRect.right - clientRect.left, clientRect.bottom - clientRect.top };

		TR::Init(&graphics.graphics, window.hwnd, graphics.size, false, 3);
	}

	void Clear(_Graphics& graphics) {
		TR::Clear(&graphics.graphics, { 0.0f, 1.0f, 1.0f, 1.0f });
	}

	void Render(_Graphics& graphics) {
		TR::Render(&graphics.graphics);
	}

}
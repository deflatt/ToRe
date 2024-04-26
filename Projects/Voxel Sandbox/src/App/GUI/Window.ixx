export module App.GUI.Window;

export import TR.Windows.Window;
export import App.GUI.Window.Graphics;
import <Windows.h>;

export namespace App::GUI::Window {

	struct _Window {

		Graphics::_Graphics graphics = {};

		TR::_Window window = {};
		TR::Int2 size = { 1280, 720 };

	};

	void Init(_Window& window) {
		TR::CreateClass("ToRe");
		TR::Create(&window.window, "ToRe", "ToRe Voxel Sandbox", window.size, { 100, 100 }, WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME) & ~(WS_MAXIMIZEBOX));
		TR::Show(&window.window);

		Graphics::Init(window.graphics, window.window);
	}

	bool IsOpen(_Window& window) {
		return IsWindow(window.window.hwnd);
	}

	void HandleInput(_Window& window) {
		TR::HandleMessages(&window.window);
	}

	void Clear(_Window& window) {
		Graphics::Clear(window.graphics);
	}

	void Render(_Window& window) {
		Graphics::Render(window.graphics);
	}

}
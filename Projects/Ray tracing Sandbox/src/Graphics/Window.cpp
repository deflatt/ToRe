module Graphics.Window;

import <Windows.h>;

namespace Graphics::Window {

	void Init(_Window& window)
	{
		using namespace TR;

		CreateClass("ToRe");
		Create(&window.window, "ToRe", "ToRe Ray tracing Sandbox", window.windowSize);
		Show(&window.window);

		Init(&window.windowGraphics, window.window.hwnd, window.windowSize, false, 3);
	}

	void HandleInput(_Window& window)
	{
		TR::HandleMessages(&window.window);
	}

	void Clear(_Window& window)
	{
		TR::Clear(&window.windowGraphics, { 0.0f, 0.0f, 0.0f, 1.0f });
	}

	void Render(_Window& window)
	{
		TR::Render(&window.windowGraphics);
	}

}
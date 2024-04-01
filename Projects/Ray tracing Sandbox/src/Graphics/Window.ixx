export module Graphics.Window;

export import TR.Graphics.WindowGraphics;
export import TR.Windows.Window;

export namespace Graphics::Window {

	struct _Window {

		TR::_Window window = {};
		TR::Int2 windowSize = { 1280, 720 };

		TR::_WindowGraphics windowGraphics = {};

	};

	void Init(_Window& window);

	void HandleInput(_Window& window);
	void Clear(_Window& window);
	void Render(_Window& window);

}
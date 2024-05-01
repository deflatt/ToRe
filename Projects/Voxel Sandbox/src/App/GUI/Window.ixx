export module App.GUI.Window;

import TR.Windows.Window;
import <Windows.h>;

export import App.GUI.Window.Graphics;

export namespace App::GUI::Window {

	class Window {

	public:

		void Init() {
			TR::CreateClass("ToRe");
			TR::Create(&window, "ToRe", "ToRe Voxel Sandbox", size, { 100, 100 }, WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME) & ~(WS_MAXIMIZEBOX));
			TR::Show(&window);

			graphics.window = &window;
			graphics.Init();
		}

		void HandleInput() {
			TR::HandleMessages(&window);
		}

		void Clear() {
			graphics.Clear();
		}

		void Render() {
			graphics.Render();
		}

		TR::_Window window = {};
		TR::Int2 size = { 1280, 720 };
		
		Graphics::Graphics graphics = {};

	};

}
#include <iostream>
#include <Windows.h>
#undef CreateWindow

import TR.Windows.Window;
import TR.Windows.KeyboardListener;
import TR.Essentials.Procedure;
import TR.Graphics.WinGraphics;
import TR.Graphics.ConstantResource;
import TR.Graphics.ArrayResource;

using namespace TR;

int main() {

	try {
		Graphics::Device::Init();

		Windows::Context::CreateClass("ToRe Window Class", 0);
		Windows::_Context window = {};
	
		Windows::Input::listeners.Insert(&Windows::Input::keyboardListener);
	
		window.SetClass("ToRe Window Class");
		window.CreateWindow("ToRe Sandbox", WS_POPUP | WS_VISIBLE, 0, { 100, 100 }, { 1280, 720 });
	
		Procedure<Windows::Input::_KeydownEvent*> proc = [](Windows::Input::_KeydownEvent* e) {
			std::cout << "Pressed " << e->key << std::endl;
		};
		window.GetContext()->listeners.Insert(&proc);
	
		Graphics::_WinGraphics graphics = {};
		graphics.Init(window.GetContext()->hwnd, { 1280, 720 });

		Graphics::_ArrayBuffer buffer = {};
		buffer.Init(1, 8);
		
		std::string text = "helloooo";
		buffer.Upload(&text[0], graphics.GetContext()->cmdList.GetContext()->cmdList.Get());

		float angle = 0.0f;

		while (true) {
			window.HandleMessages();

			angle += 0.002f;

			graphics.Clear({ (sinf(angle) + 1.0f) / 2.0f, 0.0f, (-sinf(angle) + 1.0f) / 2.0f, 1.0f});

			graphics.Render();
		}
	
	}
	catch (_Exception e) {
		std::cout << e.msg << std::endl;
	}

	return 0;
}
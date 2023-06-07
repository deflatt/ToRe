#include <iostream>
#include <Windows.h>
#undef CreateWindow

import TR.Windows.Window;
import TR.Windows.KeyboardListener;
import TR.Essentials.Procedure;
import TR.Graphics.WinGraphics;
import TR.Graphics.ConstantResource;
import TR.Graphics.ArrayResource;
import TR.Graphics.Shader;
import TR.Graphics.Renderer;

using namespace TR;

int main() {

	try {

		Graphics::Device::Init();

		Windows::Window::CreateClass("ToRe Window Class", 0);
		Windows::_Window window = {};

		window.SetClass("ToRe Window Class");
		window.CreateWindow("ToRe Sandbox", WS_POPUP | WS_VISIBLE, 0, { 100, 100 }, { 1280, 720 });
	
		Graphics::_WinGraphics graphics = {};
		graphics.Init(window.context.hwnd, { 1280, 720 });
		
		


		float angle = 0.0f;
		while (true) {
			window.HandleMessages();
			
			angle += 0.002f;

			graphics.Clear({ (sinf(angle) + 1.0f) / 2.0f, 0.0f, (-sinf(angle) + 1.0f) / 2.0f, 1.0f});

			graphics.Render();
		}
	
	}
	catch (Graphics::Shader::E_FailedCompilation e) {
		std::cout << e.msg << " ret: " << (void*)e.returnValue <<  " errors:\n" << e.errors << std::endl;
	}
	catch (Graphics::_D3D12Exception e) {
		std::cout << e.msg << " ret: " << (void*)e.returnValue << " code: " << (void*)e.d3d12ErrorCode << std::endl;
	}
	catch (_Exception e) {
		std::cout << e.msg << std::endl;
	}

	return 0;
}
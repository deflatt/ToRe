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
import TR.Graphics.VertexBuffer;

using namespace TR;

int main() {

	try {

		Graphics::Device::Init();

		Int2 windowSize = { 1280, 720 };

		Windows::Window::CreateClass("ToRe Window Class", 0);
		Windows::_Window window = {};

		window.SetClass("ToRe Window Class");
		window.CreateWindow("ToRe Sandbox", WS_POPUP | WS_VISIBLE, 0, { 100, 100 }, windowSize);
	
		Graphics::_WinGraphics graphics = {};
		graphics.Init(window.window.hwnd, (Long2)windowSize);
		auto cmdList = graphics.graphics.cmdList.cmdList.cmdList.Get();
		
		Graphics::_Renderer renderer = {};
		
		Graphics::_Shader vertexShader = {};
		vertexShader.compiler.profile = "vs_6_0";
		vertexShader.Compile("src/TestVS.hlsl");
		
		Graphics::_Shader pixelShader = {};
		pixelShader.compiler.profile = "ps_6_0";
		pixelShader.Compile("src/TestPS.hlsl");

		renderer.renderer.shaderSet.vertex = vertexShader;
		renderer.renderer.shaderSet.pixel = pixelShader;

		struct InputVertex {
			Float2 position = {};
			Float3 color = {};
		};

		renderer.renderer.vertexLayout.elements = {
			{ "POSITION", DXGI_FORMAT_R32G32_FLOAT },
			{ "COLOR", DXGI_FORMAT_R32G32B32_FLOAT }
		};
		
		renderer.Init();

		Graphics::_VertexBuffer vertexBuffer = {};
		vertexBuffer.Init(3, sizeof(InputVertex));
		std::vector<InputVertex> vertices = {
			{ { 0.0f, 0.5f }, { 0.0f, 0.0f, 1.0f } }, { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } }, { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } }
		};
		vertexBuffer.Upload(&vertices[0], cmdList);

		while (true) {
			window.HandleMessages();

			graphics.Clear({});

			renderer.Render(cmdList, vertexBuffer.vertexBuffer.vbView, { 0.0f, 0.0f, (float)windowSize[0], (float)windowSize[1], 0.0f, 1.0f }, { 0, 0, (LONG)windowSize[0], (LONG)windowSize[1] });

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
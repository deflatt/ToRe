#include <Windows.h>
#undef CreateWindow

#include <iostream>
#include <functional>
#include <d3d12sdklayers.h>
#include <random>

import TR.Essentials.Definitions;
import TR.Essentials.Vec;
import TR.Essentials.IndexTree;

using namespace TR;

import TR.Essentials.Proc;
import TR.Essentials.Vec;
import TR.Essentials.Mat;
import TR.Windows.Window;
import TR.Windows.Keyboard;
import TR.Windows.Mouse;
import TR.Graphics.Device;
import TR.Graphics.WindowGraphics;
import TR.Graphics.ArrayResource;
import TR.Graphics.Shader;
import TR.Graphics.Renderer;
import TR.Graphics.Definitions;
import TR.Graphics.Debugging;
import TR.Graphics.BufferResource;
import TR.Graphics.FullRenderer;

int main() {
	using namespace TR;

	try {

		EnableDebugger();
		InitDevice();

		CreateClass("ToRe Sandbox Class");

		_Window window = {};
		Int2 windowSize = { 1280, 720 };
		Create(&window, "ToRe Sandbox Class", "ToRe Sandbox", windowSize);
		Show(&window);

		Listener keydownListener = CharListener([](_CharEvent event) {
			std::cout << event.key;
			});
		AddListener(&keydownListener);

		RegisterRawMouseInput(window.hwnd);
		Listener rawMouseListener = RawMouseListener([](_RawMouseMoveEvent event) {
			std::cout << ToString(event.movement) << std::endl;
		}, {}, {});
		AddListener(&rawMouseListener);

		_WindowGraphics winGraphics = {};
		Init(&winGraphics, window.hwnd, windowSize, false, 3);
		ID3D12GraphicsCommandList* cmdList = winGraphics.graphics.cmdList.cmdList.Get();

		_FullRenderer renderer = {};

		{
			_Compiler compiler = {};
			compiler.profile = "vs_6_6";
			Compile(&renderer.renderer.shaderSet.vertex, "src/Shaders/TestVS.hlsl", compiler);
		}
		{
			_Compiler compiler = {};
			compiler.profile = "ps_6_6";
			Compile(&renderer.renderer.shaderSet.pixel, "src/Shaders/TestPS.hlsl", compiler);
		}
		Init(&renderer.renderer.vertexLayout, { { "POSITION", DXGI_FORMAT_R32G32_FLOAT }});

		Init(&renderer.renderer.inputMap.arrays["vals"], 0);
		std::vector<float> vals = { 1.0f };
		_ArrayBuffer valsBuffer = {};
		Init(&valsBuffer, vals.size(), sizeof(float));
		UploadBuffer(&valsBuffer.upload, &valsBuffer.resource, &vals[0], cmdList);
		renderer.renderer.inputMap.arrays["vals"].gpuAddress = GetGPUAddress(&valsBuffer.resource);

		Init(&renderer.renderer.inputMap);
		Init(&renderer, cmdList);

		//_VertexBuffer vBuffer = {};
		//Init(&vBuffer, 3, sizeof(Float2));
		//
		//std::vector<Float2> vertices = {
		//	{ 0.0f, 0.5f }, { 0.5f, -0.5f }, { -0.5f, -0.5f }
		//};
		//Upload(&vBuffer, &vertices[0], cmdList);

		RECT scissorRect = { 0, 0, windowSize[0], windowSize[1] };
		D3D12_VIEWPORT viewPort = { 0.0f, 0.0f, (float)windowSize[0], (float)windowSize[1], 0.0f, 1.0f };

		float x = 0.0f;

		while (true) {
			HandleMessages(&window);
			
			UploadBuffer(&valsBuffer.upload, &valsBuffer.resource, &vals[0], cmdList);

			Clear(&winGraphics, { 0.0f, 0.0f, 0.0f, 1.0f});
			
			Render(&renderer, cmdList, viewPort, scissorRect);

			Render(&winGraphics);
		}
	}
	catch (_FailedCompilationException e) {
		std::cout << "Failed compilation: " << e.errors << std::endl;
	}
	catch (_D3D12Exception e) {
		std::cout << "D3D12Exception: " << e.msg << " ret: " << e.returnValue << std::endl;
	}
	catch (_Exception e) {
		std::cout << "Exception: " << e.msg << std::endl;
	}

	return 0;
}

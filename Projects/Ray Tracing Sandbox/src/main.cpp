
#include <random>
#include <iostream>
#include <Windows.h>
#undef CreateWindow

import TR.Windows.Window;
import TR.Windows.Input;
import TR.Graphics.WinGraphics;
import TR.Graphics.Renderer;
import TR.Graphics.ArrayResource;
import TR.Graphics.RWArrayResource;
import FullscreenRenderer;
import Camera;
import State;
import BoxMap;

using namespace TR;

int main() {

	try {

		Graphics::Device::Init();

		Int2 windowSize = { 1280, 720 };

		Windows::Window::CreateClass("ToRe Window Class", 0);
		
		Windows::_Window window = {};

		window.SetClass("ToRe Window Class");
		window.CreateWindow("ToRe Sandbox", WS_POPUP | WS_VISIBLE, 0, { 100, 100 }, windowSize);
		RECT windowRect;
		GetWindowRect(window.window.hwnd, &windowRect);
		ClipCursor(&windowRect);

		Windows::Input::RawMouseListener::Register(window.window.hwnd);
		Windows::Input::listeners.Insert(&Windows::Input::rawMouseListener);
		Windows::Input::listeners.Insert(&Windows::Input::keyboardListener);

		Graphics::_WinGraphics graphics = {};
		graphics.Init(window.window.hwnd, (Long2)windowSize);
		auto cmdList = graphics.graphics.cmdList.cmdList.cmdList.Get();

		_FullscreenRenderer renderer = {};

		Graphics::_Shader vertexShader = {};
		vertexShader.compiler.profile = "vs_6_6";
		vertexShader.Compile("src/Shaders/RayVS.hlsl");

		Graphics::_Shader pixelShader = {};
		pixelShader.compiler.profile = "ps_6_6";
		pixelShader.Compile("src/Shaders/RayPS.hlsl");

		renderer.renderer.shaderSet.vertex = vertexShader;
		renderer.renderer.shaderSet.pixel = pixelShader;

		Camera camera = {};
		camera.Init(&renderer.inputMap);
		camera.info.aspectRatio = (float)windowSize[1] / (float)windowSize[0];


		using _BoxMap = BoxMap<float, uint, 3, 4, 2.0f>;
		_BoxMap boxMap = {};
		boxMap.Init((1 << 17));

		std::default_random_engine randomEngine = {};
		std::uniform_real_distribution<float> randomLow(-160.0f, 160.0f);
		std::uniform_real_distribution<float> randomSize(0.2f, 5.0f);

		//for (UINT i = 0; i < 10000; i++) {
		//	Float3 low = { randomLow(randomEngine), randomLow(randomEngine), randomLow(randomEngine) };
		//	Float3 high = low + Float3{ randomSize(randomEngine), randomSize(randomEngine), randomSize(randomEngine) };
		//	boxMap.Insert({ { low, high }, 0 });
		//}
		//for (float x = 0; x < 100; x++) {
		//	for (float y = 0; y < 100; y++) {
		//		for (float z = 0; z < 100; z++) {
		//			if (x == 0 || x == 99 || z == 0 || z == 99) {
		//				Float3 low = { x, y, z };
		//				Float3 high = { x + 1, y + 1, z + 1 };
		//				boxMap.Insert({ { low, high }, 0 });
		//			}
		//		}
		//	}
		//}
		//for (UINT i = 0; i < 100000; i++) {
		//	_BoxMap::Location l = boxMap.Insert({ { { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }}, 0 });
		//	boxMap.Remove(l);
		//	std::cout << i << std::endl;
		//}
		boxMap.Insert({ { { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }}, 0 });
		_BoxMap::Location loc = boxMap.Insert({ { { 2.0f, 2.0f, 2.0f }, { 3.0f, 3.0f, 3.0f }}, 0 });

		Graphics::_ArrayBuffer containerBuffer = {};
		containerBuffer.Init(boxMap.containers.elements.size(), sizeof(_BoxMap::Container));
		containerBuffer.Upload(&boxMap.containers.elements[0], cmdList);

		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["containers"], 1);
		renderer.inputMap.arrayResources["containers"].gpuAddress = containerBuffer.resource.resource->GetGPUVirtualAddress();

		Graphics::_ArrayBuffer nodeBuffer = {};
		nodeBuffer.Init(boxMap.nodes.elements.size(), sizeof(_BoxMap::Node));
		nodeBuffer.Upload(&boxMap.nodes.elements[0], cmdList);

		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["nodes"], 2);
		renderer.inputMap.arrayResources["nodes"].gpuAddress = nodeBuffer.resource.resource->GetGPUVirtualAddress();

		renderer.Init((Long2)windowSize, cmdList);

		State::Init(window.window.hwnd);

		while (true) {
			window.HandleMessages();

			if (GetKeyState(VK_LEFT) & 0x8000) {
				_BoxMap::Box b = boxMap.nodes.elements[boxMap.containers.elements[loc.back()].node].box;
				b.low[0] -= 0.01f;
				b.high[0] -= 0.01f;
				loc = boxMap.Move(loc, b);
			}
			if (GetKeyState(VK_RIGHT) & 0x8000) {
				_BoxMap::Box b = boxMap.nodes.elements[boxMap.containers.elements[loc.back()].node].box;
				b.low[0] += 0.01f;
				b.high[0] += 0.01f;
				loc = boxMap.Move(loc, b);
			}
			std::cout << boxMap.nodes.nextElement << std::endl;
			containerBuffer.Upload(&boxMap.containers.elements[0], cmdList);
			nodeBuffer.Upload(&boxMap.nodes.elements[0], cmdList);

			camera.Update(cmdList);
			//std::cout << camera.targetVelocity.ToString() << std::endl;

			graphics.Clear({});

			renderer.Render(cmdList);

			graphics.Render();
		}

	}
	catch (Graphics::Shader::E_FailedCompilation e) {
		std::cout << e.msg << " ret: " << (void*)e.returnValue << " errors:\n" << e.errors << std::endl;
	}
	catch (Graphics::_D3D12Exception e) {
		std::cout << e.msg << " ret: " << (void*)e.returnValue << " code: " << (void*)e.d3d12ErrorCode << std::endl;
	}
	catch (_Exception e) {
		std::cout << e.msg << std::endl;
	}

	return 0;
}
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

using _BoxMap = BoxMap<float, uint, 3, 8.0f, 2.0f>;

void Print(_BoxMap& boxMap, uint container, int depth = 0) {
	std::string white(depth * 4, ' ');

	std::cout << white << "{" << std::endl;
	_BoxMap::Container& c = boxMap.containers[container];
	std::cout << white << "offset = " << c.offset.ToString() << std::endl;

	_BoxMap::Node& node = boxMap.nodes[c.node];
	std::cout << white << "size = " << node.size.ToString() << std::endl;
	std::cout << white << "refCount = " << node.refCount << std::endl;

	if (node.type == _BoxMap::Node::Type::object) {
		std::cout << white << "child = " << node.child << std::endl;
	}
	else {
		std::cout << white << "children = [" << std::endl;
		for (uint childInd = node.child; childInd != _BoxMap::noInd; childInd = boxMap.containers[childInd].sibling) {
			Print(boxMap, childInd, depth + 1);
		}
		std::cout << white << "]" << std::endl;
	}
	std::cout << white << "}" << std::endl;
}

void Print(_BoxMap& boxMap) {
	Print(boxMap, 0);
}


int main() {

	try {

		
		_BoxMap boxMap = {};

		boxMap.Init(1 << 8);

		boxMap.InsertObject({ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, 0);

		Print(boxMap);
		return 0;
		boxMap.InsertObject({ 5.0f, 5.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, 1);
		boxMap.InsertObject({ 4.5f, 6.5f, -2.5f }, { 1.0f, 1.0f, 1.0f }, 2);
		Float3 offset = { 3.0f, 6.0f, -2.0f };
		_BoxMap::Location loc = boxMap.InsertObject(offset, { 1.0f, 1.0f, 1.0f }, 3);
		boxMap.InsertObject({ -1.0f, 2.0f, 9.0f }, { 1.0f, 1.0f, 1.0f }, 4);
		boxMap.InsertObject({ -5.0f, -3.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, 5);
		boxMap.InsertObject({ -4.0f, -2.0f, -1.5f }, { 1.0f, 1.0f, 1.0f }, 6);

		//for (float x = -64.0f; x <= 64.0f; x += 2.0f) {
		//	for (float y = -64.0f; y <= 64.0f; y += 2.0f) {
		//		for (float z = -64.0f; z <= 64.0f; z += 2.0f) {
		//			boxMap.InsertObject({ x, y, z }, { 0.5f, 0.5f, 0.5f }, 0);
		//		}
		//	}
		//}

		//uint objInd = boxMap.CreateObject();
		//boxMap.nodes[objInd].size = { 1.0f, 1.0f, 1.0f };
		//boxMap.nodes[objInd].child = 0;
		//
		//boxMap.Insert(objInd, { 1.0f, 1.0f, 1.0f });
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

		Graphics::_ArrayBuffer containerBuffer = {};
		containerBuffer.Init((UINT)boxMap.containers.elements.size(), sizeof(_BoxMap::Container));
		containerBuffer.Upload(&boxMap.containers.elements[0], cmdList);

		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["containers"], 1);
		renderer.inputMap.arrayResources["containers"].gpuAddress = containerBuffer.resource.resource->GetGPUVirtualAddress();

		Graphics::_ArrayBuffer nodeBuffer = {};
		nodeBuffer.Init((UINT)boxMap.nodes.elements.size(), sizeof(_BoxMap::Node));
		nodeBuffer.Upload(&boxMap.nodes.elements[0], cmdList);

		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["nodes"], 2);
		renderer.inputMap.arrayResources["nodes"].gpuAddress = nodeBuffer.resource.resource->GetGPUVirtualAddress();

		renderer.Init((Long2)windowSize, cmdList);

		State::Init(window.window.hwnd);

		while (true) {
			window.HandleMessages();

			camera.Update(cmdList);
			
			if (GetKeyState(VK_NUMPAD4) & 0x8000) {
				offset[2] -= 0.05f;
				boxMap.Remove(loc);
				loc = boxMap.InsertObject(offset, { 1.0f, 1.0f, 1.0f }, 3);
			}
			if (GetKeyState(VK_NUMPAD6) & 0x8000) {
				offset[2] += 0.05f;
				boxMap.Remove(loc);
				loc = boxMap.InsertObject(offset, { 1.0f, 1.0f, 1.0f }, 3);
			}
			if (GetKeyState(VK_NUMPAD8) & 0x8000) {
				offset[0] += 0.05f;
				boxMap.Remove(loc);
				loc = boxMap.InsertObject(offset, { 1.0f, 1.0f, 1.0f }, 3);
			}
			if (GetKeyState(VK_NUMPAD2) & 0x8000) {
				offset[0] -= 0.05f;
				boxMap.Remove(loc);
				loc = boxMap.InsertObject(offset, { 1.0f, 1.0f, 1.0f }, 3);
			}
			if (GetKeyState(VK_NUMPAD5) & 0x8000) {
				offset[1] += 0.05f;
				boxMap.Remove(loc);
				loc = boxMap.InsertObject(offset, { 1.0f, 1.0f, 1.0f }, 3);
			}
			if (GetKeyState(VK_NUMPAD0) & 0x8000) {
				offset[1] -= 0.05f;
				boxMap.Remove(loc);
				loc = boxMap.InsertObject(offset, { 1.0f, 1.0f, 1.0f }, 3);
			}

			nodeBuffer.Upload(&boxMap.nodes.elements[0], cmdList);
			containerBuffer.Upload(&boxMap.containers.elements[0], cmdList);

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
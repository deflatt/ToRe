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

using _BoxMap = BoxMap<float, uint, 3, 128.0f, 2.0f>;

void Print(_BoxMap& boxMap, uint container, int depth = 0) {
	std::string white(depth * 4, ' ');

	std::cout << white << "{ (" << container << ")" << std::endl;
	_BoxMap::Container& c = boxMap.containers[container];
	std::cout << white << "offset = " << c.offset.ToString() << std::endl;
	std::cout << white << "node index = " << c.node << std::endl;

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

		boxMap.Init(1 << 18);

		//boxMap.InsertObject({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 100);
		//boxMap.InsertObject({ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, 101);
		//boxMap.InsertObject({ 2.0f, 2.0f, 2.0f }, { 1.0f, 1.0f, 1.0f }, 102);
		//boxMap.InsertObject({ 3.0f, 3.0f, 3.0f }, { 1.0f, 1.0f, 1.0f }, 103);
		
		//
		

		//return 0;
		//boxMap.InsertObject({ 5.0f, 5.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, 1);
		//boxMap.InsertObject({ 4.5f, 6.5f, -2.5f }, { 1.0f, 1.0f, 1.0f }, 2);
		//Float3 offset = { 3.0f, 6.0f, -2.0f };
		//_BoxMap::Location loc = boxMap.InsertObject(offset, { 1.0f, 1.0f, 1.0f }, 3);
		//boxMap.InsertObject({ -1.0f, 2.0f, 9.0f }, { 1.0f, 1.0f, 1.0f }, 4);
		//boxMap.InsertObject({ -5.0f, -3.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, 5);
		//boxMap.InsertObject({ -4.0f, -2.0f, -1.5f }, { 1.0f, 1.0f, 1.0f }, 6);

		struct Cube {
			_BoxMap::Location loc = {};
			Float3 position = {};
			Float3 dir = {};
		};
		std::vector<Cube> cubes((1 << 8));

		{
			std::default_random_engine randomEngine;
			std::uniform_real_distribution<float> posDist(-64.0f, 64.0f);
			std::uniform_real_distribution<float> dirDist(-1.0f, 1.0f);
			for (int i = 0; i < cubes.size(); i++) {
				cubes[i].position = { posDist(randomEngine), posDist(randomEngine), posDist(randomEngine) };
				cubes[i].dir = { dirDist(randomEngine), dirDist(randomEngine), dirDist(randomEngine) };
				cubes[i].loc = boxMap.InsertObject(cubes[i].position, { 1.0f, 1.0f, 1.0f }, 0);
				//boxMap.Remove(cubes[i].loc);
				//cubes[i].loc = boxMap.InsertObject(cubes[i].position, { 1.0f, 1.0f, 1.0f }, 0);
			}
		}
		{
			//boxMap.Remove(cubes[2].loc);
			//for (int i = 0; i < cubes.size(); i++) {
			//	boxMap.Remove(cubes[i].loc);
			//}
		}
		//Print(boxMap);
		//return 0;

		//for (float x = -16; x <= 16; x += 2.0f) {
		//	for (float y = -16; y <= 16; y += 2.0f) {
		//		for (float z = -16; z <= 16; z += 2.0f) {
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

		Int2 windowSize = { 1920, 1080 };

		Windows::Window::CreateClass("ToRe Window Class", 0);
		
		Windows::_Window window = {};

		window.SetClass("ToRe Window Class");
		window.CreateWindow("ToRe Sandbox", WS_POPUP | WS_VISIBLE, 0, { 0, 0 }, windowSize);
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

		Clock<float> deltaClock;
		while (true) {
			window.HandleMessages();

			camera.Update(cmdList);
			
			float delta = deltaClock.Restart().Seconds();

			for (int i = 0; i < cubes.size(); i++) {
				cubes[i].position += cubes[i].dir * delta;
				boxMap.Remove(cubes[i].loc);
				cubes[i].loc = boxMap.InsertObject(cubes[i].position, { 1.0f, 1.0f, 1.0f }, 0);
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
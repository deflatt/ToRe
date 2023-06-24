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
import BoxSet;

using namespace TR;

using _BoxSet = BoxSet<float, uint, 3, 128.0f, 2.0f>;

void Print(_BoxSet& BoxSet, uint container, int depth = 0) {
	std::string white(depth * 4, ' ');

	std::cout << white << "{ (" << container << ")" << std::endl;
	_BoxSet::Container& c = BoxSet.containers[container];
	std::cout << white << "offset = " << c.offset.ToString() << std::endl;
	std::cout << white << "node index = " << c.node << std::endl;

	_BoxSet::Node& node = BoxSet.nodes[c.node];
	std::cout << white << "size = " << node.size.ToString() << std::endl;
	std::cout << white << "refCount = " << node.refCount << std::endl;

	if (node.type == _BoxSet::Node::Type::object) {
		std::cout << white << "child = " << node.child << std::endl;
	}
	else {
		std::cout << white << "children = [" << std::endl;
		for (uint childInd = node.child; childInd != _BoxSet::noInd; childInd = BoxSet.containers[childInd].sibling) {
			Print(BoxSet, childInd, depth + 1);
		}
		std::cout << white << "]" << std::endl;
	}
	std::cout << white << "}" << std::endl;
}

void Print(_BoxSet& BoxSet) {
	Print(BoxSet, 0);
}


int main() {

	try {

		
		_BoxSet BoxSet = {};

		BoxSet.Init(1 << 18);

		BoxSet.InsertObject({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 0);
		BoxSet.InsertObject({ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, 0);
		BoxSet.InsertObject({ 2.0f, 2.0f, 2.0f }, { 1.0f, 1.0f, 1.0f }, 0);
		BoxSet.InsertObject({ 3.0f, 3.0f, 3.0f }, { 1.0f, 1.0f, 1.0f }, 0);

		struct Material {
			Float3 emission = {};
		};
		std::vector<Material> materials = {
			{ { 1.0f, 0.0f, 1.0f } }
		};
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
		containerBuffer.Init((UINT)BoxSet.containers.elements.size(), sizeof(_BoxSet::Container));
		containerBuffer.Upload(&BoxSet.containers.elements[0], cmdList);

		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["containers"], 1);
		renderer.inputMap.arrayResources["containers"].gpuAddress = containerBuffer.resource.resource->GetGPUVirtualAddress();

		Graphics::_ArrayBuffer nodeBuffer = {};
		nodeBuffer.Init((UINT)BoxSet.nodes.elements.size(), sizeof(_BoxSet::Node));
		nodeBuffer.Upload(&BoxSet.nodes.elements[0], cmdList);

		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["nodes"], 2);
		renderer.inputMap.arrayResources["nodes"].gpuAddress = nodeBuffer.resource.resource->GetGPUVirtualAddress();

		Graphics::_ArrayBuffer materialBuffer = {};
		materialBuffer.Init((UINT)materials.size(), sizeof(Material));
		materialBuffer.Upload(&materials[0], cmdList);

		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["materials"], 3);
		renderer.inputMap.arrayResources["materials"].gpuAddress = materialBuffer.resource.resource->GetGPUVirtualAddress();

		renderer.Init((Long2)windowSize, cmdList);

		State::Init(window.window.hwnd);

		Clock<float> deltaClock;
		while (true) {
			window.HandleMessages();

			camera.Update(cmdList);
			
			float delta = deltaClock.Restart().Seconds();
			
			//nodeBuffer.Upload(&BoxSet.nodes.elements[0], cmdList);
			//containerBuffer.Upload(&BoxSet.containers.elements[0], cmdList);

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
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
import TR.Media.FileDecoder;

using namespace TR;

struct MaterialBoxSet {

	using _BoxSet = BoxSet<float, uint, 3>;

	struct Material {
		Float3 emission = {};
		struct Hash {
			size_t operator()(const Material& material) const {
				return ArrayHash<float, 3>()(material.emission);
			}
		};
		bool operator==(const Material& that) const {
			return emission == that.emission;
		}
	};
	std::vector<Material> materials = {};
	std::unordered_map<Material, uint, Material::Hash> materialMap = {};
	Graphics::_ArrayBuffer materialBuffer = {};

	_BoxSet boxSet = {};
	Graphics::_ArrayBuffer containerBuffer = {};
	Graphics::_ArrayBuffer nodeBuffer = {};

	void Init(uint setSize, uint numMaterials) {
		boxSet.Init(setSize);
		materials.resize(numMaterials);

		materialBuffer.Init(numMaterials, sizeof(Material));
		containerBuffer.Init(setSize, sizeof(_BoxSet::Container));
		nodeBuffer.Init(setSize, sizeof(_BoxSet::Node));
	}

	uint GetMaterial(Material mat) {
		if (materialMap.count(mat))
			return materialMap.at(mat);
		else {
			uint ind = materialMap.size();
			materials[ind] = mat;
			materialMap[mat] = ind;
			return ind;
		}
	}

	void Update(ID3D12GraphicsCommandList* cmdList) {
		materialBuffer.Upload(&materials[0], cmdList);
		containerBuffer.Upload(&boxSet.containers.elements[0], cmdList);
		nodeBuffer.Upload(&boxSet.nodes.elements[0], cmdList);
	}

};

struct BlockBoxSet {

	MaterialBoxSet materialBoxSet = {};
	
	std::unordered_map<std::string, uint> blockMap = {};

	void Init(uint setSize, uint numMaterials) {
		materialBoxSet.Init(setSize, numMaterials);
	}

	void LoadBlock(std::string name, std::string path) {
		Media::FileDecoder::_Context fileDecoder = {};
		Media::FileDecoder::Init(&fileDecoder, path);
		Media::Frame::_Context frame = {};
		frame.format = AV_PIX_FMT_RGBA;
		Media::FileDecoder::DecodeFrame(&fileDecoder, &frame, true);
		Media::FileDecoder::Release(&fileDecoder);

		uint rootInd = materialBoxSet.boxSet.CreateRoot();

		for (int x = 0; x < frame.size[0]; x++){
			for (int y = 0; y < frame.size[1]; y++) {
				for (int z = 0; z < frame.size[0]; z++) {
					Float3 pos = { (float)x / (float)frame.size[0], (float)y / (float)frame.size[1], (float)z / (float)frame.size[0] };
					Float3 size = { 1.0f / (float)frame.size[0], 1.0f / (float)frame.size[1], 1.0f / (float)frame.size[0] };
					int px = x, py = y;
					if (x == 0 || x + 1 == frame.size[0]) {
						px = y;
						py = z;
					}
					else if (y == 0 || y + 1 == frame.size[1]) {
						px = x;
						py = z;
					}
					Byte3 byteCol = *(Byte3*)&frame.data[(py * frame.size[0] + px) * 4];
					MaterialBoxSet::Material material = {};
					material.emission = (Float3)byteCol / 255.0f;

					materialBoxSet.boxSet.InsertObject({ {}, size }, pos, materialBoxSet.GetMaterial(material), 1.0f, rootInd);
				}
			}
		}
		blockMap.insert({ name, rootInd });
		materialBoxSet.boxSet.nodeInfo[rootInd].refCount++;
	}

	void InsertBlock(std::string name, Float3 position) {
		materialBoxSet.boxSet.InsertRoot(blockMap.at(name), position, 1024.0f);
	}

	void RemoveBlock(std::string name, Float3 position) {
		materialBoxSet.boxSet.RemoveRoot(blockMap.at(name), position);
	}

};

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

		BlockBoxSet blocks = {};
		blocks.Init(1 << 16, 1 << 14);

		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["containers"], 1);
		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["nodes"], 2);
		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["materials"], 3);
		renderer.inputMap.arrayResources["containers"].gpuAddress = blocks.materialBoxSet.containerBuffer.resource.resource->GetGPUVirtualAddress();
		renderer.inputMap.arrayResources["nodes"].gpuAddress = blocks.materialBoxSet.nodeBuffer.resource.resource->GetGPUVirtualAddress();
		renderer.inputMap.arrayResources["materials"].gpuAddress = blocks.materialBoxSet.materialBuffer.resource.resource->GetGPUVirtualAddress();

		renderer.Init((Long2)windowSize, cmdList);

		blocks.LoadBlock("dirt", "dirt.png");
		blocks.LoadBlock("sand", "sand.png");
		blocks.LoadBlock("stone", "stone.png");

		for (int x = 0; x < 16; x++) {
			for (int z = 0; z < 16; z++) {
				blocks.InsertBlock("stone", { (float)x, 0.0f, (float)z });
				blocks.InsertBlock("dirt", { (float)x, 1.0f, (float)z });
			}
		}
		blocks.InsertBlock("sand", { 8.0f, 2.0f, 7.0f });
		blocks.InsertBlock("sand", { 8.0f, 2.0f, 8.0f });
		blocks.InsertBlock("sand", { 8.0f, 2.0f, 9.0f });
		blocks.InsertBlock("sand", { 8.0f, 3.0f, 8.0f });
		blocks.InsertBlock("sand", { 8.0f, 4.0f, 8.0f });
		blocks.InsertBlock("sand", { 8.0f, 5.0f, 8.0f });

		State::Init(window.window.hwnd);
	
		Clock<float> deltaClock;
	
		Float3 offset = {};
		float speed = 100.0f;
	
		Clock<double> fpsClock;
		while (true) {
			window.HandleMessages();
	
			camera.Update(cmdList);
			
			blocks.materialBoxSet.Update(cmdList);
	
			graphics.Clear({});
	
			renderer.Render(cmdList);
	
			graphics.Render();
	
			while (fpsClock.Elapsed().Seconds() < 1.0 / 60.0);
			fpsClock.Restart();
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
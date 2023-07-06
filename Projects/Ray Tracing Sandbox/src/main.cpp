#include <random>
#include <iostream>
#include <fstream>
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
		Float3 reflection = {};
		struct Hash {
			size_t operator()(const Material& material) const {
				return ArrayHash<float, 3>()(material.emission) ^ ArrayHash<float, 3>()(material.reflection);
			}
		};
		bool operator==(const Material& that) const {
			return emission == that.emission && reflection == that.reflection;
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

float Dist(Float3 a, Float3 b) {
	float sum = 0.0f;
	for (size_t i = 0; i < 3; i++)
		sum += (a[i] - b[i]) * (a[i] - b[i]);
	return sqrtf(sum);
}

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

		std::default_random_engine randomEngine = {};
		std::uniform_int_distribution<int> distX(0, frame.size[0] - 1), distY(0, frame.size[1] - 1);

		for (int x = 0; x < frame.size[0]; x++){
			for (int y = 0; y < frame.size[1]; y++) {
				for (int z = 0; z < frame.size[0]; z++) {
					Float3 pos = { (float)x / (float)frame.size[0], (float)y / (float)frame.size[1], (float)z / (float)frame.size[0] };
					Float3 size = { 1.0f / (float)frame.size[0], 1.0f / (float)frame.size[1], 1.0f / (float)frame.size[0] };
					int px, py;
					if (x == 0 || x + 1 == frame.size[0]) {
						px = y;
						py = z;
					}
					else if (y == 0 || y + 1 == frame.size[1]) {
						px = x;
						py = z;
					}
					else if (z == 0 || z + 1 == frame.size[0]) {
						px = x;
						py = y;
					}
					else {
						px = distX(randomEngine);
						py = distY(randomEngine);
					}
					Byte3 byteCol = *(Byte3*)&frame.data[(py * frame.size[0] + px) * 4];
					byte alpha = frame.data[(py * frame.size[0] + px) * 4 + 3];
					if (alpha == 0)
						continue;
					MaterialBoxSet::Material material = {};
					material.reflection = (Float3)byteCol / 255.0f;
					material.emission = {};

					materialBoxSet.boxSet.InsertObject({ {}, size }, pos, materialBoxSet.GetMaterial(material), 1.0f, rootInd);
				}
			}
		}
		blockMap.insert({ name, rootInd });
		materialBoxSet.boxSet.nodeInfo[rootInd].refCount++;
	}

	void InsertBlock(std::string name, Float3 position) {
		materialBoxSet.boxSet.InsertRoot(blockMap.at(name), position, 128.0f);
	}

	void RemoveBlock(std::string name, Float3 position) {
		materialBoxSet.boxSet.RemoveRoot(blockMap.at(name), position);
	}

};

int main() {

	try {
	
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
		camera.info.windowSize = windowSize;

		BlockBoxSet blocks = {};
		blocks.Init(1 << 20, 1 << 14);

		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["camera"], 0);
		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["containers"], 1);
		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["nodes"], 2);
		Graphics::InputParameter::Init(&renderer.inputMap.arrayResources["materials"], 3);
		renderer.inputMap.arrayResources["camera"].gpuAddress = camera.buffer.resource.resource->GetGPUVirtualAddress();
		renderer.inputMap.arrayResources["containers"].gpuAddress = blocks.materialBoxSet.containerBuffer.resource.resource->GetGPUVirtualAddress();
		renderer.inputMap.arrayResources["nodes"].gpuAddress = blocks.materialBoxSet.nodeBuffer.resource.resource->GetGPUVirtualAddress();
		renderer.inputMap.arrayResources["materials"].gpuAddress = blocks.materialBoxSet.materialBuffer.resource.resource->GetGPUVirtualAddress();

		struct PixelInfo {
			uint ind;
			Float3 pos;
			Float3 normal;
			Float3 incomingLight;
		};
		Graphics::_RWArrayBuffer pixelInfoBuffer = {};
		pixelInfoBuffer.Init(windowSize[0] * windowSize[1], sizeof(PixelInfo));

		Graphics::InputParameter::Init(&renderer.inputMap.rwArrayResources["pixelInfo"], 1);
		renderer.inputMap.rwArrayResources["pixelInfo"].gpuAddress = pixelInfoBuffer.resource.resource->GetGPUVirtualAddress();

		renderer.Init((Long2)windowSize, cmdList);

		std::cout << "Initialized renderer" << std::endl;

		_FullscreenRenderer denoiser = {};

		Graphics::_Shader denoiseVS = {};
		denoiseVS.compiler.profile = "vs_6_6";
		denoiseVS.Compile("src/Shaders/DenoiseVS.hlsl");

		Graphics::_Shader denoisePS = {};
		denoisePS.compiler.profile = "ps_6_6";
		denoisePS.Compile("src/Shaders/DenoisePS.hlsl");

		denoiser.renderer.shaderSet.vertex = denoiseVS;
		denoiser.renderer.shaderSet.pixel = denoisePS;

		Graphics::InputParameter::Init(&denoiser.inputMap.arrayResources["camera"], 0);
		Graphics::InputParameter::Init(&denoiser.inputMap.arrayResources["materials"], 1);
		denoiser.inputMap.arrayResources["camera"].gpuAddress = camera.buffer.resource.resource->GetGPUVirtualAddress();
		denoiser.inputMap.arrayResources["materials"].gpuAddress = blocks.materialBoxSet.materialBuffer.resource.resource->GetGPUVirtualAddress();

		Graphics::InputParameter::Init(&denoiser.inputMap.rwArrayResources["pixelInfo"], 1);
		denoiser.inputMap.rwArrayResources["pixelInfo"].gpuAddress = pixelInfoBuffer.resource.resource->GetGPUVirtualAddress();

		denoiser.Init((Long2)windowSize, cmdList);
		std::cout << "Initialized denoiser" << std::endl;

		blocks.LoadBlock("dirt", "dirt.png");
		blocks.LoadBlock("sand", "sand.png");
		blocks.LoadBlock("stone", "stone.png");
		blocks.LoadBlock("grass_block", "grass_block_top.png");
		blocks.LoadBlock("acacia_log", "acacia_log.png");
		blocks.LoadBlock("acacia_leaves", "acacia_leaves.png");
		blocks.LoadBlock("oak_leaves", "oak_leaves.png");
		blocks.LoadBlock("oak_log", "oak_log.png");
		blocks.LoadBlock("gravel", "gravel.png");

		{
			std::ifstream ifile("savanna_100x40x100.mcw");
			Int3 origin, size;
			for (size_t i = 0; i < 3; i++) {
				ifile >> origin[i] >> size[i];
			}
			std::unordered_set<std::string> blockTypes;
			for (int x = 0; x < size[0]; x++) {
				for (int y = 0; y < size[1]; y++) {
					for (int z = 0; z < size[2]; z++) {
						std::string type;
						ifile >> type;
						if (!blockTypes.count(type)) {
							std::cout << "New block type: " << type << std::endl;
							blockTypes.insert(type);
						}
						if (blocks.blockMap.count(type)) {
							blocks.InsertBlock(type, { (float)x, (float)y, (float)z });
						}
					}
				}
			}
			ifile.close();
		}
		blocks.InsertBlock("dirt", {});

		State::Init(window.window.hwnd);
	
		Clock<float> deltaClock;
	
		Float3 offset = { 5.0f, 5.0f, 5.0f };
		blocks.InsertBlock("sand", offset);

		float speed = 5.0f;
	
		Clock<double> fpsClock;
		while (true) {
			window.HandleMessages();
			
			float deltaTime = deltaClock.Restart().Seconds();

			blocks.RemoveBlock("sand", offset);
			if (GetKeyState(VK_LEFT) & 0x8000) {
				offset[2] -= speed * deltaTime;
			}
			if (GetKeyState(VK_RIGHT) & 0x8000) {
				offset[2] += speed * deltaTime;
			}
			if (GetKeyState(VK_UP) & 0x8000) {
				offset[1] += speed * deltaTime;
			}
			if (GetKeyState(VK_DOWN) & 0x8000) {
				offset[1] -= speed * deltaTime;
			}
			if (GetKeyState(VK_NUMPAD8) & 0x8000) {
				offset[0] += speed * deltaTime;
			}
			if (GetKeyState(VK_NUMPAD2) & 0x8000) {
				offset[0] -= speed * deltaTime;
			}
			blocks.InsertBlock("sand", offset);

			camera.Update(cmdList);
			
			blocks.materialBoxSet.Update(cmdList);
	
			graphics.Clear({});
	
			renderer.Render(cmdList);
			denoiser.Render(cmdList);
	
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
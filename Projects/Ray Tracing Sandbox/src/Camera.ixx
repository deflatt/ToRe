module;

#include <Windows.h>
#undef min
#undef max

export module Camera;

export import Math;
export import Clock;
export import TR.Graphics.InputParameter;
export import TR.Graphics.ArrayResource;
export import TR.Windows.Input;
export import TR.Windows.RawKeyboardListener;
export import TR.Windows.RawMouseListener;
import State;

import <algorithm>;

using namespace TR;

export struct Camera {

	struct Info {
		Float3 position = {};
		Float2 rotation = {};
		float fov = 3.14f / 2.0f;
		float aspectRatio = 1.0f;
	} info = {};

	float speed = 10.0f;
	float acceleration = 10.0f;
	float sensitivity = 0.001f;

	Float3 velocity = {};
	Float3 targetVelocity = {};
	Clock<float> clock = {};

	std::vector<std::pair<int, int>> movementKeys = {
		{ 'W', 'S' }, { VK_SPACE, VK_SHIFT }, { 'D', 'A' }
	};

	Graphics::_ArrayBuffer buffer = {};

	void Init(Graphics::InputParameter::Map::_Context* inputMap) {
		buffer.Init(1, sizeof(Info));
		
		Graphics::InputParameter::Init(&inputMap->arrayResources["camera"], 0);
		inputMap->arrayResources["camera"].gpuAddress = buffer.resource.resource->GetGPUVirtualAddress();
		
		Procedure<Windows::Input::_RawMouseMoveEvent*> mouseMoveListener = [this](Windows::Input::_RawMouseMoveEvent* e) {
			if (!State::active)
				return;
			if (!e->foreground)
				return;
			this->info.rotation[1] += (float)e->movement[0] * sensitivity;
			this->info.rotation[0] -= (float)e->movement[1] * sensitivity;
		};
		Windows::Input::listeners.Add(mouseMoveListener);

		clock.Restart();
	}

	void Update(ID3D12GraphicsCommandList* cmdList) {
		targetVelocity *= 0.0f;
		if (State::active) {
			for (size_t i = 0; i < movementKeys.size(); i++) {
				if (GetKeyState(movementKeys[i].first) & 0x8000)
					targetVelocity[i] += speed;
				if (GetKeyState(movementKeys[i].second) & 0x8000)
					targetVelocity[i] -= speed;
			}
			Rotate(targetVelocity, info.rotation);
		}

		float delta = clock.Restart().Seconds();
		Float3 diff = targetVelocity - velocity;
		velocity = velocity + diff * std::min(delta * acceleration, 1.0f);
		info.position += velocity * delta;

		buffer.Upload(&info, cmdList);
	}


};
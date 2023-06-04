module;

#include "GraphicsHeader.h"

export module TR.Graphics.Device;

export import TR.Graphics.Exception;

export namespace TR::Graphics {

	ComPtr<ID3D12Device> device = {};

	namespace Device {

		struct E_FailedDeviceCreation : public _D3D12Exception {
			E_FailedDeviceCreation(HRESULT returnValue) : _D3D12Exception("Failed to create Direct3D 12 device.", returnValue) {}
		};

		void Init();

	}

}
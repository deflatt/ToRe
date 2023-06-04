module;

#include "GraphicsHeader.h"

module TR.Graphics.Device;

namespace TR::Graphics {

	namespace Device {

		void Init() 
		{
			HRESULT ret = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), &device);
			if (ret != 0) {
				throw E_FailedDeviceCreation(ret);
			}
		}

	}

}
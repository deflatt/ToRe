module TR.Graphics.Device;

import <d3d12.h>;

namespace TR {

	namespace Graphics {

		void InitDevice()
		{
			HRESULT ret = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device), &device);
			if (ret != 0) {
				throw _FailedDeviceCreationException(ret);
			}
		}

	}
	using namespace Graphics;

}
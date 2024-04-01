export module TR.Graphics.Device;

import TR.Graphics.Definitions;
import TR.Graphics.Exception;

export namespace TR {

	namespace Graphics {

		ComPtr<ID3D12Device> device = {};

		struct _FailedDeviceCreationException : public _D3D12Exception {
			_FailedDeviceCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create Direct3D 12 device.", returnValue) {}
		};

		void InitDevice();

	}
	using namespace Graphics;

}
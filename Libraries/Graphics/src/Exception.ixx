export module TR.Graphics.Exception;

export import TR.Essentials.Exception;
import TR.Essentials.Definitions;
import <d3d12.h>;

export namespace TR {

	namespace Graphics {

		struct _D3D12Exception : public _Exception {
			HRESULT returnValue;
			int d3d12ErrorCode;
			_D3D12Exception(std::string msg, HRESULT returnValue) : _Exception(msg), returnValue(returnValue), d3d12ErrorCode(GetLastError()) {}
		};

	}

}
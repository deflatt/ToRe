module;

#include "GraphicsHeader.h"

export module TR.Graphics.Exception;

export import TR.Essentials.Exception;

namespace TR::Graphics {

	export struct _D3D12Exception : public _Exception {
		HRESULT returnValue;
		int d3d12ErrorCode;
		_D3D12Exception(std::string msg = {}, HRESULT returnValue = 0) : _Exception(msg)
			, returnValue(returnValue), d3d12ErrorCode(GetLastError()) {}
	};

}
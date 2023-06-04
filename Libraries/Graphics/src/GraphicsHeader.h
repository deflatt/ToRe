#pragma once

#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi.h>
#include <wrl.h>
#include <dxcapi.h>

#undef min
#undef max

namespace TR::Graphics {

	template <typename T_ptr>
	using ComPtr = Microsoft::WRL::ComPtr<T_ptr>;

}
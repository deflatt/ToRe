module;

#include "GraphicsHeader.h"

export module TR.Graphics.SwapChain;

export import TR.Graphics.Device;
export import TR.Essentials.Array;
export import TR.Graphics.RenderTarget;

export namespace TR::Graphics {

	namespace SwapChain {

		struct E_FailedDXGIFactoryCreation : public _D3D12Exception {
			E_FailedDXGIFactoryCreation(HRESULT returnValue) : _D3D12Exception("Failed to create DXGI factory.", returnValue) {}
		};
		struct E_FailedSwapChainCreation : public _D3D12Exception {
			E_FailedSwapChainCreation(HRESULT returnValue) : _D3D12Exception("Failed to create swap chain.", returnValue) {}
		};
		struct E_FailedSwapChainConversion : public _D3D12Exception {
			E_FailedSwapChainConversion(HRESULT returnValue) : _D3D12Exception("Failed to convert swap chain.", returnValue) {}
		};
		struct E_FailedBufferCreation : public _D3D12Exception {
			E_FailedBufferCreation(HRESULT returnValue) : _D3D12Exception("Failed to create swap chain buffer.", returnValue) {}
		};
		struct E_FailedBufferSwap : public _D3D12Exception {
			E_FailedBufferSwap(HRESULT returnValue) : _D3D12Exception("Failed to swap swap chain buffers.", returnValue) {}
		};

		struct _Context {
			ComPtr<IDXGIFactory2> factory = {};
			ComPtr<IDXGISwapChain3> swapChain = {};
		};

		void Init(_Context* swapChain, ID3D12CommandQueue* cmdQueue, HWND hwnd, Long2 size
			, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT numBuffers = 2, bool fullscreen = false);

		void InitBuffer(_Context* swapChain, Resource::_Context* resource, UINT index);
		void SwapBuffers(_Context* swapChain);

		void Release(_Context* swapChain);

	}

	struct _SwapChain {

		SwapChain::_Context swapChain = {};

		void Init(ID3D12CommandQueue* cmdQueue, HWND hwnd, Long2 size
			, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT numBuffers = 2, bool fullscreen = false);

		void InitBuffer(Resource::_Context* resource, UINT index);
		void SwapBuffers();

		~_SwapChain();

	};

}
export module TR.Graphics.SwapChain;

import TR.Graphics.Definitions;
import TR.Graphics.Exception;
import TR.Graphics.Resource;
import <dxgi1_6.h>;
export import TR.Essentials.Vec;

export namespace TR {

	namespace Graphics {

		namespace SwapChain {

			struct _SwapChain {
				ComPtr<IDXGIFactory2> factory = {};
				ComPtr<IDXGISwapChain3> swapChain = {};
			};

			struct _FailedDXGIFactoryCreationException : public _D3D12Exception {
				_FailedDXGIFactoryCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create DXGI factory.", returnValue) {}
			};
			struct _FailedSwapChainCreationException : public _D3D12Exception {
				_FailedSwapChainCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create swap chain.", returnValue) {}
			};
			struct _FailedSwapChainConversionException : public _D3D12Exception {
				_FailedSwapChainConversionException(HRESULT returnValue) : _D3D12Exception("Failed to convert swap chain.", returnValue) {}
			};
			struct _FailedBufferCreationException : public _D3D12Exception {
				_FailedBufferCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create swap chain buffer.", returnValue) {}
			};
			struct _FailedBufferSwapException : public _D3D12Exception {
				_FailedBufferSwapException(HRESULT returnValue) : _D3D12Exception("Failed to swap swap chain buffers.", returnValue) {}
			};

			void Init(_SwapChain* swapChain, ID3D12CommandQueue* cmdQueue, HWND hwnd, Int2 size
				, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT numBuffers = 2, bool fullscreen = false);

			void InitBuffer(_SwapChain* swapChain, _Resource* resource, UINT index);
			void SwapBuffers(_SwapChain* swapChain);

			void Release(_SwapChain* swapChain);

		}
		using namespace SwapChain;

	}
	using namespace Graphics;

}
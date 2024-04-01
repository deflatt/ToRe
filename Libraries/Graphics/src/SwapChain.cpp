module TR.Graphics.SwapChain;

import <dxgi1_6.h>;

namespace TR {

	namespace Graphics {

		namespace SwapChain {

			void Init(_SwapChain* swapChain, ID3D12CommandQueue* cmdQueue, HWND hwnd, Int2 size, DXGI_FORMAT format, UINT numBuffers, bool fullscreen)
			{
				HRESULT ret = CreateDXGIFactory1(__uuidof(IDXGIFactory2), &swapChain->factory);
				if (ret != 0) {
					throw _FailedDXGIFactoryCreationException(ret);
				}

				DXGI_SWAP_CHAIN_DESC1 desc = {};
				desc.Width = size[0];
				desc.Height = size[1];
				desc.Format = format;
				desc.Stereo = FALSE;
				desc.SampleDesc = { 1, 0 };
				desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				desc.BufferCount = numBuffers;
				desc.Scaling = DXGI_SCALING_NONE;
				desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
				desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
				desc.Flags = 0;

				DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc = {};
				fsDesc.Windowed = (BOOL)!fullscreen;
				fsDesc.RefreshRate = { 0, 0 }; // Look into

				ComPtr<IDXGISwapChain1> swapChainTmp = {};
				ret = swapChain->factory->CreateSwapChainForHwnd(cmdQueue, hwnd, &desc, &fsDesc, nullptr, &swapChainTmp);
				if (ret != 0) {
					throw _FailedSwapChainCreationException(ret);
				}

				ret = swapChainTmp->QueryInterface(__uuidof(IDXGISwapChain3), &swapChain->swapChain);
				if (ret != 0) {
					throw _FailedSwapChainConversionException(ret);
				}

				if (fullscreen)
					swapChain->swapChain->SetFullscreenState(TRUE, nullptr);
			}

			void InitBuffer(_SwapChain* swapChain, _Resource* resource, UINT index)
			{
				HRESULT ret = swapChain->swapChain->GetBuffer(index, __uuidof(ID3D12Resource), &resource->resource);
				if (ret != 0) {
					throw _FailedBufferCreationException(ret);
				}
			}

			void SwapBuffers(_SwapChain* swapChain)
			{
				HRESULT ret = swapChain->swapChain->Present(0, 0);
				if (ret != 0) {
					throw _FailedBufferSwapException(ret);
				}
			}

			void Release(_SwapChain* swapChain)
			{
				swapChain->swapChain->SetFullscreenState(FALSE, nullptr);
			}

		}

	}

}
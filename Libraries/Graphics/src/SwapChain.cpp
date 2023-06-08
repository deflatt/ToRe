module;

#include "GraphicsHeader.h"

module TR.Graphics.SwapChain;

namespace TR::Graphics {

	namespace SwapChain {

		void Init(_Context* swapChain, ID3D12CommandQueue* cmdQueue, HWND hwnd, Long2 size, DXGI_FORMAT format, UINT numBuffers, bool fullscreen)
		{
			HRESULT ret = CreateDXGIFactory1(__uuidof(IDXGIFactory2), &swapChain->factory);
			if (ret != 0) {
				throw E_FailedDXGIFactoryCreation(ret);
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
				throw E_FailedSwapChainCreation(ret);
			}

			ret = swapChainTmp->QueryInterface(__uuidof(IDXGISwapChain3), &swapChain->swapChain);
			if (ret != 0) {
				throw E_FailedSwapChainConversion(ret);
			}

			if (fullscreen)
				swapChain->swapChain->SetFullscreenState(TRUE, nullptr);
		}

		void InitBuffer(_Context* swapChain, Resource::_Context* resource, UINT index)
		{
			HRESULT ret = swapChain->swapChain->GetBuffer(index, __uuidof(ID3D12Resource), &resource->resource);
			if (ret != 0) {
				throw E_FailedBufferCreation(ret);
			}
		}

		void SwapBuffers(_Context* swapChain)
		{
			HRESULT ret = swapChain->swapChain->Present(0, 0);
			if (ret != 0) {
				throw E_FailedBufferSwap(ret);
			}
		}

		void Release(_Context* swapChain)
		{
			swapChain->swapChain->SetFullscreenState(FALSE, nullptr);
		}

	}

	void _SwapChain::Init(ID3D12CommandQueue* cmdQueue, HWND hwnd, Long2 size, DXGI_FORMAT format, UINT numBuffers, bool fullscreen)
	{
		SwapChain::Init(&swapChain, cmdQueue, hwnd, size, format, numBuffers, fullscreen);
	}

	void _SwapChain::InitBuffer(Resource::_Context* resource, UINT index)
	{
		SwapChain::InitBuffer(&swapChain, resource, index);
	}

	void _SwapChain::SwapBuffers()
	{
		SwapChain::SwapBuffers(&swapChain);
	}

	_SwapChain::~_SwapChain()
	{
		SwapChain::Release(&swapChain);
	}

}
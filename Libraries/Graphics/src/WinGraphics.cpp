module;

#include "GraphicsHeader.h"

module TR.Graphics.WinGraphics;

namespace TR::Graphics {

	namespace Graphics {

		namespace Win {

			void Init(_Context* win, Graphics::_Context* graphics, HWND hwnd, Long2 size, bool fullscreen, UINT numBuffers, DXGI_FORMAT format)
			{
				graphics->cmdQueue.Init();
				graphics->cmdList.Init(graphics->cmdQueue.cmdQueue.cmdQueue.Get(), numBuffers, 2);

				win->swapChain.Init(graphics->cmdQueue.cmdQueue.cmdQueue.Get(), hwnd, size, format, numBuffers, fullscreen);
				graphics->renderTargets.Init(numBuffers);
				for (UINT i = 0; i < numBuffers; i++) {
					win->swapChain.InitBuffer(&graphics->renderTargets.rtHeap.renderTargets[i].resource, i);
				}
				graphics->renderTargets.PlaceTargets();
			}

			void Clear(Graphics::_Context* graphics, Float4 color)
			{
				UINT frameIndex = graphics->renderTargets.rtHeap.frameIndex;

				ID3D12GraphicsCommandList* cmdList = graphics->cmdList.cmdList.cmdList.Get();

				graphics->cmdList.Close();

				graphics->cmdQueue.Execute(cmdList);
				graphics->cmdList.cmdList.allocators[0][frameIndex].Signal(graphics->cmdQueue.cmdQueue.cmdQueue.Get());

				graphics->cmdList.cmdList.allocators[1][frameIndex].Wait();
				graphics->cmdList.Reset(1, frameIndex);

				// Maybe replace with OO later
				Resource::Transition(&graphics->renderTargets.rtHeap.renderTargets[frameIndex].resource
					, D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList);
				
				graphics->renderTargets.SetCurrent(cmdList);
				graphics->renderTargets.ClearCurrent(cmdList, color);
			}

			void Render(_Context* win, Graphics::_Context* graphics)
			{
				UINT frameIndex = graphics->renderTargets.rtHeap.frameIndex;

				ID3D12GraphicsCommandList* cmdList = graphics->cmdList.cmdList.cmdList.Get();

				Resource::Transition(&graphics->renderTargets.rtHeap.renderTargets[frameIndex].resource
					, D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList);

				graphics->cmdList.Close();
				graphics->cmdList.cmdList.allocators[0][frameIndex].Wait();

				graphics->cmdQueue.Execute(cmdList);
				graphics->cmdList.cmdList.allocators[1][frameIndex].Signal(graphics->cmdQueue.cmdQueue.cmdQueue.Get());

				win->swapChain.SwapBuffers();
				frameIndex = win->swapChain.swapChain.swapChain->GetCurrentBackBufferIndex();
				graphics->renderTargets.SetFrameIndex(frameIndex);

				graphics->cmdList.Reset(0, frameIndex);
			}

		}

	}

	void _WinGraphics::Init(HWND hwnd, Long2 size, bool fullscreen, UINT numBuffers, DXGI_FORMAT format)
	{
		Graphics::Win::Init(&win, &graphics, hwnd, size, fullscreen, numBuffers, format);
	}

	void _WinGraphics::Clear(Float4 color)
	{
		Graphics::Win::Clear(&graphics, color);
	}

	void _WinGraphics::Render()
	{
		Graphics::Win::Render(&win, &graphics);
	}

}
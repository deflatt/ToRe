module;

#include "GraphicsHeader.h"

module TR.Graphics.WinGraphics;

namespace TR::Graphics {

	namespace Graphics {

		namespace Win {

			void Init(_Context* context, Graphics::_Context* graphics, HWND hwnd, Long2 size, bool fullscreen, UINT numBuffers, DXGI_FORMAT format)
			{
				graphics->cmdQueue.Init();
				graphics->cmdList.Init(graphics->cmdQueue.GetContext()->cmdQueue.Get(), numBuffers, 2);

				context->swapChain.Init(graphics->cmdQueue.GetContext()->cmdQueue.Get(), hwnd, size, format, numBuffers, fullscreen);
				graphics->renderTargets.Init(numBuffers);
				for (UINT i = 0; i < numBuffers; i++) {
					context->swapChain.InitBuffer(graphics->renderTargets.GetContext()->renderTargets[i].GetResource(), i);
				}
				graphics->renderTargets.PlaceTargets();
			}

			void Clear(Graphics::_Context* graphics, Float4 color)
			{
				UINT frameIndex = graphics->renderTargets.GetContext()->frameIndex;

				ID3D12GraphicsCommandList* cmdList = graphics->cmdList.GetContext()->cmdList.Get();

				graphics->cmdList.Close();

				graphics->cmdQueue.Execute(cmdList);
				graphics->cmdList.GetContext()->allocators[0][frameIndex].Signal(graphics->cmdQueue.GetContext()->cmdQueue.Get());

				graphics->cmdList.GetContext()->allocators[1][frameIndex].Wait();
				graphics->cmdList.Reset(1, frameIndex);

				// Maybe replace with OO later
				Resource::Transition(graphics->renderTargets.GetContext()->renderTargets[frameIndex].GetResource()
					, D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList);
				
				graphics->renderTargets.SetCurrent(cmdList);
				graphics->renderTargets.ClearCurrent(cmdList, color);
			}

			void Render(_Context* context, Graphics::_Context* graphics)
			{
				UINT frameIndex = graphics->renderTargets.GetContext()->frameIndex;

				ID3D12GraphicsCommandList* cmdList = graphics->cmdList.GetContext()->cmdList.Get();

				Resource::Transition(graphics->renderTargets.GetContext()->renderTargets[frameIndex].GetResource()
					, D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList);

				graphics->cmdList.Close();
				graphics->cmdList.GetContext()->allocators[0][frameIndex].Wait();

				graphics->cmdQueue.Execute(cmdList);
				graphics->cmdList.GetContext()->allocators[1][frameIndex].Signal(graphics->cmdQueue.GetContext()->cmdQueue.Get());

				context->swapChain.SwapBuffers();
				frameIndex = context->swapChain.GetContext()->swapChain->GetCurrentBackBufferIndex();
				graphics->renderTargets.SetFrameIndex(frameIndex);

				graphics->cmdList.Reset(0, frameIndex);
			}

		}

	}

	void _WinGraphics::Init(HWND hwnd, Long2 size, bool fullscreen, UINT numBuffers, DXGI_FORMAT format)
	{
		Graphics::Win::Init(&winContext, &context, hwnd, size, fullscreen, numBuffers, format);
	}

	void _WinGraphics::Clear(Float4 color)
	{
		Graphics::Win::Clear(&context, color);
	}

	void _WinGraphics::Render()
	{
		Graphics::Win::Render(&winContext, &context);
	}

}
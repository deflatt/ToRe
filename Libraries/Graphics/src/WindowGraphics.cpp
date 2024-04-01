module TR.Graphics.WindowGraphics;

namespace TR {

	namespace Graphics {

		namespace Window {

			void Init(_WindowPart* window, _Graphics* graphics, HWND hwnd, Int2 size, bool fullscreen, UINT numBuffers, DXGI_FORMAT format)
			{
				Init(&graphics->cmdQueue);
				Init(&graphics->cmdList, graphics->cmdQueue.cmdQueue.Get(), numBuffers, 2);

				Init(&window->swapChain, graphics->cmdQueue.cmdQueue.Get(), hwnd, size, format, numBuffers, fullscreen);
				Init(&graphics->renderTargets, numBuffers);
				for (UINT i = 0; i < numBuffers; i++) {
					InitBuffer(&window->swapChain, &graphics->renderTargets.rtList.renderTargets[i], i);
				}
				PlaceTargets(&graphics->renderTargets);
			}

			void Clear(_Graphics* graphics, Float4 color)
			{
				UINT frameIndex = graphics->renderTargets.rtList.frameIndex;

				ID3D12GraphicsCommandList* cmdList = graphics->cmdList.cmdList.Get();

				Close(&graphics->cmdList);

				Execute(&graphics->cmdQueue, cmdList);
				Signal(&graphics->cmdList.allocators[0][frameIndex], graphics->cmdQueue.cmdQueue.Get());

				Wait(&graphics->cmdList.allocators[1][frameIndex]);
				Reset(&graphics->cmdList, 1, frameIndex);

				Transition(&graphics->renderTargets.rtList.renderTargets[frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList);
				SetCurrent(&graphics->renderTargets, cmdList);
				ClearCurrent(&graphics->renderTargets, cmdList, color);
			}

			void Render(_WindowPart* window, _Graphics* graphics)
			{
				UINT frameIndex = graphics->renderTargets.rtList.frameIndex;

				ID3D12GraphicsCommandList* cmdList = graphics->cmdList.cmdList.Get();

				Transition(&graphics->renderTargets.rtList.renderTargets[frameIndex], D3D12_RESOURCE_STATE_PRESENT, cmdList);

				Close(&graphics->cmdList);
				Wait(&graphics->cmdList.allocators[0][frameIndex]);

				Execute(&graphics->cmdQueue, cmdList);
				Signal(&graphics->cmdList.allocators[1][frameIndex], graphics->cmdQueue.cmdQueue.Get());

				SwapBuffers(&window->swapChain);
				frameIndex = window->swapChain.swapChain->GetCurrentBackBufferIndex();
				SetFrameIndex(&graphics->renderTargets, frameIndex);

				Reset(&graphics->cmdList, 0, frameIndex);
			}

			void Init(_WindowGraphics* windowGraphics, HWND hwnd, Int2 size, bool fullscreen, UINT numBuffers, DXGI_FORMAT format)
			{
				Init(&windowGraphics->window, &windowGraphics->graphics, hwnd, size, fullscreen, numBuffers, format);
			}

			void Clear(_WindowGraphics* windowGraphics, Float4 color)
			{
				Clear(&windowGraphics->graphics, color);
			}

			void Render(_WindowGraphics* windowGraphics)
			{
				Render(&windowGraphics->window, &windowGraphics->graphics);
			}

			ID3D12GraphicsCommandList* GetCommandList(_WindowGraphics* windowGraphics)
			{
				return windowGraphics->graphics.cmdList.cmdList.Get();
			}

		}

	}

}
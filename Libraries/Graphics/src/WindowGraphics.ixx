export module TR.Graphics.WindowGraphics;

import TR.Graphics.SwapChain;
import TR.Graphics.Definitions;
import TR.Graphics.Exception;
import TR.Graphics.Graphics;

export namespace TR {

	namespace Graphics {

		namespace Window {

			struct _WindowPart {
				_SwapChain swapChain = {};
			};

			void Init(_WindowPart* window, _Graphics* graphics, HWND hwnd, Int2 size, bool fullscreen = false, UINT numBuffers = 2, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

			void Clear(_Graphics* graphics, Float4 color);
			void Render(_WindowPart* window, _Graphics* graphics);

			struct _WindowGraphics {
				_WindowPart window = {};
				_Graphics graphics = {};
			};

			void Init(_WindowGraphics* windowGraphics, HWND hwnd, Int2 size, bool fullscreen = false, UINT numBuffers = 2, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

			void Clear(_WindowGraphics* windowGraphics, Float4 color);
			void Render(_WindowGraphics* windowGraphics);

			ID3D12GraphicsCommandList* GetCommandList(_WindowGraphics* windowGraphics);

		}
		using namespace Window;

	}
	using namespace Graphics;

}
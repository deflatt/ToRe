module;

#include "GraphicsHeader.h"

export module TR.Graphics.WinGraphics;

export import TR.Graphics.Graphics;
export import TR.Graphics.SwapChain;

export namespace TR::Graphics {

	namespace Graphics {

		namespace Win {

			struct _Context {
				_SwapChain swapChain = {};
			};

			void Init(_Context* win, Graphics::_Context* graphics, HWND hwnd, Long2 size, bool fullscreen = false, UINT numBuffers = 2, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

			void Clear(Graphics::_Context* graphics, Float4 color);
			void Render(_Context* win, Graphics::_Context* graphics);

		}

	}

	struct _WinGraphics {

		Graphics::_Context graphics = {};
		Graphics::Win::_Context win = {};

		void Init(HWND hwnd, Long2 size, bool fullscreen = false, UINT numBuffers = 2, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

		void Clear(Float4 color);
		void Render();

	};

}
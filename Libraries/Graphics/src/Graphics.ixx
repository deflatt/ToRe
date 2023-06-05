module;

#include "GraphicsHeader.h"

export module TR.Graphics.Graphics;

export import TR.Graphics.Device;
export import TR.Graphics.CommandQueue;
export import TR.Graphics.CommandList;
export import TR.Graphics.RenderTarget;

export namespace TR::Graphics {

	namespace Graphics {

		struct _Context {
			_CommandQueue cmdQueue = {};
			_CommandList cmdList = {};
			_RenderTargetHeap renderTargets = {};
		};

	}

}
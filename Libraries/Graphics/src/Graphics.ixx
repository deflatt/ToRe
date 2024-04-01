export module TR.Graphics.Graphics;

export import TR.Graphics.CommandQueue;
export import TR.Graphics.CommandList;
export import TR.Graphics.RenderTarget;

export namespace TR {

	namespace Graphics {

		struct _Graphics {
			_CommandQueue cmdQueue = {};
			_CommandList cmdList = {};
			_RenderTargetHeap renderTargets = {};
		};

	}
	using namespace Graphics;

}
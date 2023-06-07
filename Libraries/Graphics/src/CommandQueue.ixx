module;

#include "GraphicsHeader.h"

export module TR.Graphics.CommandQueue;

export import TR.Graphics.Device;

export namespace TR::Graphics {

	namespace CommandQueue {

		struct E_FailedCommandQueueCreation : public _D3D12Exception {
			E_FailedCommandQueueCreation(HRESULT returnValue) : _D3D12Exception("Failed to create command queue.", returnValue) {}
		};

		struct _Context {
			ComPtr<ID3D12CommandQueue> cmdQueue = {};
		};

		void Init(_Context* context);
		void Execute(_Context* context, ID3D12CommandList* const* lists, UINT numLists);
		void Execute(_Context* context, ID3D12CommandList* list);

	}

	struct _CommandQueue  {

		CommandQueue::_Context context = {};

		void Init();
		void Execute(ID3D12CommandList* const* lists, UINT numLists);
		void Execute(ID3D12CommandList* list);

	};

}
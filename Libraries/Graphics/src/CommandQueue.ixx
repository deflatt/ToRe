export module TR.Graphics.CommandQueue;

import TR.Graphics.Definitions;
import TR.Graphics.Exception;

export namespace TR {

	namespace Graphics {

		namespace CommandQueue {

			struct _CommandQueue {
				ComPtr<ID3D12CommandQueue> cmdQueue = {};
			};

			struct _FailedCommandQueueCreationException : public _D3D12Exception {
				_FailedCommandQueueCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create command queue.", returnValue) {}
			};

			void Init(_CommandQueue* cmdQueue);
			void Execute(_CommandQueue* cmdQueue, ID3D12CommandList* const* lists, UINT numLists);
			void Execute(_CommandQueue* cmdQueue, ID3D12CommandList* list);

		}
		using namespace CommandQueue;

	}
	using namespace Graphics;

}
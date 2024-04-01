export module TR.Graphics.CommandAllocator;

import TR.Graphics.Definitions;
import TR.Graphics.Exception;

export namespace TR {

	namespace Graphics {

		namespace CommandAllocator {

			struct _CommandAllocator {
				ComPtr<ID3D12CommandAllocator> cmdAllocator = {};
				ComPtr<ID3D12Fence> fence = {};
				UINT64 fenceTarget = 0;
				HANDLE fenceEvent = nullptr;
			};

			struct _FailedCommandAllocatorCreationException : public _D3D12Exception {
				_FailedCommandAllocatorCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create command allocator.", returnValue) {}
			};
			struct _FailedFenceCreationException : public _D3D12Exception {
				_FailedFenceCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create fence.", returnValue) {}
			};
			struct _FailedFenceEventCreationException : public _Exception {
				_FailedFenceEventCreationException() : _Exception("Failed to create fence event.") {}
			};
			struct _FailedFenceSignalingException : public _D3D12Exception {
				_FailedFenceSignalingException(HRESULT returnValue) : _D3D12Exception("Failed to signal fence.", returnValue) {}
			};
			struct _FailedFenceWaitingException : public _D3D12Exception {
				_FailedFenceWaitingException(HRESULT returnValue) : _D3D12Exception("Failed to wait for event.", returnValue) {}
			};
			struct _FailedCommandAllocatorResetException : public _D3D12Exception {
				_FailedCommandAllocatorResetException(HRESULT returnValue) : _D3D12Exception("Failed to reset command allocator.", returnValue) {}
			};

			void Init(_CommandAllocator* cmdAllocator);

			void Signal(_CommandAllocator* cmdAllocator, ID3D12CommandQueue* cmdQueue);
			void Wait(_CommandAllocator* cmdAllocator);
			void Reset(_CommandAllocator* cmdAllocator);

			void Release(_CommandAllocator* cmdAllocator);

		}
		using namespace CommandAllocator;

	}
	using namespace Graphics;

}
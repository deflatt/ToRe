module;

#include "GraphicsHeader.h"

export module TR.Graphics.CommandAllocator;

export import TR.Graphics.Device;

export namespace TR::Graphics {

	namespace CommandAllocator {

		struct E_FailedCommandAllocatorCreation : public _D3D12Exception {
			E_FailedCommandAllocatorCreation(HRESULT returnValue) : _D3D12Exception("Failed to create command allocator.", returnValue) {}
		};
		struct E_FailedFenceCreation : public _D3D12Exception {
			E_FailedFenceCreation(HRESULT returnValue) : _D3D12Exception("Failed to create fence.", returnValue) {}
		};
		struct E_FailedFenceEventCreation : public _Exception {
			E_FailedFenceEventCreation() : _Exception("Failed to create fence event.") {}
		};
		struct E_FailedFenceSignaling : public _D3D12Exception {
			E_FailedFenceSignaling(HRESULT returnValue) : _D3D12Exception("Failed to signal fence.", returnValue) {}
		};
		struct E_FailedFenceWaiting : public _D3D12Exception {
			E_FailedFenceWaiting(HRESULT returnValue) : _D3D12Exception("Failed to wait for event.", returnValue) {}
		};
		struct E_FailedCommandAllocatorReset : public _D3D12Exception {
			E_FailedCommandAllocatorReset(HRESULT returnValue) : _D3D12Exception("Failed to reset command allocator.", returnValue) {}
		};

		struct _Context {
			ComPtr<ID3D12CommandAllocator> cmdAllocator = {};
			ComPtr<ID3D12Fence> fence = {};
			UINT64 fenceTarget = 0;
			HANDLE fenceEvent = nullptr;
		};

		void Init(_Context* context);

		void Signal(_Context* context, ID3D12CommandQueue* cmdQueue);
		void Wait(_Context* context);
		void Reset(_Context* context);

		void Release(_Context* context);

	}

	struct _CommandAllocator {

		CommandAllocator::_Context context = {};

		void Init();

		void Signal(ID3D12CommandQueue* cmdQueue);
		void Wait();
		void Reset();

		~_CommandAllocator();

	};

}
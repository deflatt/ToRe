module;

#include "GraphicsHeader.h"

export module TR.Graphics.CommandList;

export import TR.Graphics.Device;
export import TR.Graphics.CommandAllocator;
export import <vector>;

export namespace TR::Graphics {

	namespace CommandList {

		struct E_FailedCommandListCreation : public _D3D12Exception {
			E_FailedCommandListCreation(HRESULT returnValue) : _D3D12Exception("Failed to create command list.", returnValue) {}
		};
		struct E_FailedCommandListReset : public _D3D12Exception {
			E_FailedCommandListReset(HRESULT returnValue) : _D3D12Exception("Failed to reset command list.", returnValue) {}
		};
		struct E_FailedCommandListClosure : public _D3D12Exception {
			E_FailedCommandListClosure(HRESULT returnValue) : _D3D12Exception("Failed to close command list.", returnValue) {}
		};

		struct _Context {
			std::vector<std::vector<_CommandAllocator>> allocators = {};
			UINT numParallel = 0;
			UINT numAllocators = 0;
			ComPtr<ID3D12GraphicsCommandList> cmdList = {};
		};

		void Init(_Context* cmdList, ID3D12CommandQueue* cmdQueue, UINT numParallel, UINT numAllocators);

		void Reset(_Context* cmdList, UINT allocator, UINT index);
		void Close(_Context* cmdList);

	}

	struct _CommandList {

		CommandList::_Context cmdList = {};

		void Init(ID3D12CommandQueue* cmdQueue, UINT numParallel, UINT numAllocators);

		void Reset(UINT allocator, UINT index);
		void Close();

	};

}
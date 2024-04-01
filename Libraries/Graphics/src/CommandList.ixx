export module TR.Graphics.CommandList;

import TR.Graphics.Definitions;
import TR.Graphics.Exception;
import TR.Graphics.CommandAllocator;
import <vector>;

export namespace TR {

	namespace Graphics {

		namespace CommandList {

			struct _CommandList {
				std::vector<std::vector<_CommandAllocator>> allocators = {};
				UINT numParallel = 0;
				UINT numAllocators = 0;
				ComPtr<ID3D12GraphicsCommandList> cmdList = {};
			};

			struct _FailedCommandListCreationException : public _D3D12Exception {
				_FailedCommandListCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create command list.", returnValue) {}
			};
			struct _FailedCommandListResetException : public _D3D12Exception {
				_FailedCommandListResetException(HRESULT returnValue) : _D3D12Exception("Failed to reset command list.", returnValue) {}
			};
			struct _FailedCommandListClosureException : public _D3D12Exception {
				_FailedCommandListClosureException(HRESULT returnValue) : _D3D12Exception("Failed to close command list.", returnValue) {}
			};

			void Init(_CommandList* cmdList, ID3D12CommandQueue* cmdQueue, UINT numParallel, UINT numAllocators);

			void Reset(_CommandList* cmdList, UINT allocator, UINT index);
			void Close(_CommandList* cmdList);

		}
		using namespace CommandList;

	}
	using namespace Graphics;

}
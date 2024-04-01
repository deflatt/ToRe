export module TR.Graphics.Resource;

import TR.Graphics.Definitions;
import TR.Graphics.Exception;
import <d3d12.h>;

export namespace TR {

	namespace Graphics {

		namespace Resource {

			struct _Resource {
				ComPtr<ID3D12Resource> resource = {};
				D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;
			};

			struct _FailedResourceCreationException : public _D3D12Exception {
				_FailedResourceCreationException(HRESULT returnValue) : _D3D12Exception("Failed to create resource.", returnValue) {}
			};

			void Init(_Resource* resource, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_DESC desc,
				D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

			void Transition(_Resource* resource, D3D12_RESOURCE_STATES targetState, ID3D12GraphicsCommandList* cmdList);

			D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress(_Resource* resource);

		}
		using namespace Resource;

	}
	using namespace Graphics;

}
export module TR.Graphics.BufferResource;

import TR.Graphics.UploadResource;
import TR.Graphics.Definitions;
import TR.Graphics.Exception;
import <d3d12.h>;

export namespace TR {

	namespace Graphics {

		namespace BufferResource {

			void InitBuffer(_Resource* resource, UINT64 numBytes
				, D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT
				, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON
				, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

			void InitBuffer(_Resource* resource, _UploadPart* upload, UINT64 numBytes
				, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON
				, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

			void UploadBuffer(_UploadPart* upload, _Resource* dstResource, const void* data, ID3D12GraphicsCommandList* cmdList);

		}
		using namespace BufferResource;

	}
	using namespace Graphics;

}
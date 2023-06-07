module;

#include "GraphicsHeader.h"

export module TR.Graphics.BufferResource;

export import TR.Graphics.UploadResource;

export namespace TR::Graphics {

	namespace Resource {

		void InitBuffer(_Context* resource, UINT64 numBytes
			, D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT
			, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON
			, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		void InitBuffer(Resource::_Context* resource, Upload::_Context* upload, UINT64 numBytes
			, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON
			, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		namespace Upload {

			void UploadBuffer(Upload::_Context* upload, Resource::_Context* dstResource, const void* data, ID3D12GraphicsCommandList* cmdList);
			
		}

	}

	struct _BufferResource {

		Resource::_Context resource = {};
		Resource::Upload::_Context upload = {};

		void Init(UINT64 numBytes
			, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON
			, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		void Upload(const void* data, ID3D12GraphicsCommandList* cmdList);

	};

}
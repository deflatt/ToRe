module;

#include "GraphicsHeader.h"

export module TR.Graphics.UploadResource;

export import TR.Graphics.Resource;

export namespace TR::Graphics {

	namespace Resource {

		namespace Upload {

			struct _Context {
				_Resource uploadResource = {};
			};

			void Init(_Context* upload, D3D12_RESOURCE_DESC desc, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);
			void Init(_Context* upload, Resource::_Context* dstResource, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

			void UploadData(_Context* upload, Resource::_Context* dstResource, const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList);

		}

		void Init(Resource::_Context* resource, Upload::_Context* upload, D3D12_RESOURCE_DESC desc
			, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON
			, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

	}

	struct _UploadResource {

		Resource::_Context resource = {};
		Resource::Upload::_Context upload = {};

		void Init(D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		void UploadData(const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList);

	};

}
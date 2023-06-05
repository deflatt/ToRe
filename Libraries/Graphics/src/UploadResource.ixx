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

			void Init(_Context* context, D3D12_RESOURCE_DESC desc, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

			void Upload(_Context* context, Resource::_Context* dstResource, const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList);

		}

	}

	// Inherit?
	struct _UploadResource {

		void Init(D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);

		void Upload(const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList);

	protected:

		Resource::_Context context = {};
		Resource::Upload::_Context uploadContext = {};

	};

}
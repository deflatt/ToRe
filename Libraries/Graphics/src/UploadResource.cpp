module;

#include "GraphicsHeader.h"

module TR.Graphics.UploadResource;

namespace TR::Graphics {

	namespace Resource {

		namespace Upload {

			void Init(_Context* context, D3D12_RESOURCE_DESC desc, D3D12_HEAP_FLAGS heapFlags)
			{
				context->uploadResource.Init(D3D12_HEAP_TYPE_UPLOAD, desc, D3D12_RESOURCE_STATE_COMMON, heapFlags);
			}

			void TR::Graphics::Resource::Upload::Upload(_Context* context, Resource::_Context* dstResource, const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList)
			{
				D3D12_RESOURCE_STATES oldState = dstResource->state;
				Resource::Transition(dstResource, D3D12_RESOURCE_STATE_COPY_DEST, cmdList);

				UpdateSubresources(cmdList, dstResource->resource.Get()
					, context->uploadResource.GetContext()->resource.Get(), 0, 0, 1, &data);

				Resource::Transition(dstResource, oldState, cmdList);
			}

		}

	}

	void _UploadResource::Init(D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
	{
		Resource::Upload::Init(&uploadContext, desc);
		Resource::Init(&context, D3D12_HEAP_TYPE_DEFAULT, desc, state, heapFlags);
	}

	void _UploadResource::Upload(const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList)
	{
		Resource::Upload::Upload(&uploadContext, &context, data, cmdList);
	}

}
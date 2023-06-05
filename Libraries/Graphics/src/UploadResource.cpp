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

			void Init(_Context* context, Resource::_Context* resource, D3D12_HEAP_FLAGS heapFlags)
			{
				Init(context, resource->resource->GetDesc(), heapFlags);
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
		Resource::Init(&context, D3D12_HEAP_TYPE_DEFAULT, desc, state, heapFlags);
		Resource::Upload::Init(&uploadContext, &context);
	}

	void _UploadResource::Upload(const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList)
	{
		Resource::Upload::Upload(&uploadContext, &context, data, cmdList);
	}

	namespace Resource {

		void InitBuffer(_Context* context, UINT64 numBytes, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
		{
			Init(context, heapType, CD3DX12_RESOURCE_DESC::Buffer(numBytes), state, heapFlags);
		}

		namespace Upload {

			void UploadBuffer(_Context* context, Resource::_Context* dstResource, const void* data, ID3D12GraphicsCommandList* cmdList)
			{
				D3D12_SUBRESOURCE_DATA subData = {};
				subData.pData = data;
				subData.RowPitch = dstResource->resource->GetDesc().Width;
				Upload::Upload(context, dstResource, subData, cmdList);
			}

		}

	}

	void _BufferResource::Init(UINT64 numBytes, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
	{
		Resource::InitBuffer(&context, numBytes, D3D12_HEAP_TYPE_DEFAULT, state, heapFlags);
		Resource::Upload::Init(&uploadContext, &context);
	}

	void _BufferResource::Upload(const void* data, ID3D12GraphicsCommandList* cmdList)
	{
		Resource::Upload::UploadBuffer(&uploadContext, &context, data, cmdList);
	}

	Resource::_Context* _BufferResource::GetContext() noexcept
	{
		return &context;
	}

	Resource::Upload::_Context* _BufferResource::GetUploadContext() noexcept
	{
		return &uploadContext;
	}

}
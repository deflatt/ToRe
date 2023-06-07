module;

#include "GraphicsHeader.h"

module TR.Graphics.BufferResource;

namespace TR::Graphics {

	namespace Resource {

		void InitBuffer(_Context* resource, UINT64 numBytes, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
		{
			Init(resource, heapType, CD3DX12_RESOURCE_DESC::Buffer(numBytes), state, heapFlags);
		}

		namespace Upload {

			void UploadBuffer(Upload::_Context* upload, Resource::_Context* dstResource, const void* data, ID3D12GraphicsCommandList* cmdList)
			{
				D3D12_SUBRESOURCE_DATA subData = {};
				subData.pData = data;
				subData.RowPitch = dstResource->resource->GetDesc().Width;
				Upload::UploadData(upload, dstResource, subData, cmdList);
			}

		}

		void InitBuffer(Resource::_Context* resource, Upload::_Context* upload, UINT64 numBytes, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
		{
			Resource::Init(resource, upload, CD3DX12_RESOURCE_DESC::Buffer(numBytes), state, heapFlags);
		}

	}

	void _BufferResource::Init(UINT64 numBytes, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
	{
		Resource::InitBuffer(&resource, &upload, numBytes, state, heapFlags);
	}

	void _BufferResource::Upload(const void* data, ID3D12GraphicsCommandList* cmdList)
	{
		Resource::Upload::UploadBuffer(&upload, &resource, data, cmdList);
	}

}
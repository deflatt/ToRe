module;

#include "GraphicsHeader.h"

module TR.Graphics.RWArrayResource;

namespace TR::Graphics {

	namespace RWArrayResource {

		void Init(_Context* rwArrayResource, D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, UINT numElements, UINT elementSize)
		{
			rwArrayResource->uavDesc = uavDesc;
			rwArrayResource->numElements = numElements;
			rwArrayResource->elementSize = elementSize;
		}

		void Place(_Context* rwArrayResource, Resource::_Context* resource, DescriptorHeap::_Context* descriptorHeap, UINT index)
		{
			device->CreateUnorderedAccessView(resource->resource.Get(), nullptr, &rwArrayResource->uavDesc, DescriptorHeap::GetHandle(descriptorHeap, index));
		}

	}

	namespace RWArrayResource {

		void InitBuffer(Resource::_Context* resource, Resource::Upload::_Context* upload, RWArrayResource::_Context* rwArrayResource, UINT numElements, UINT elementSize)
		{
			// Skipping D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS seems to be fine
			Resource::InitBuffer(resource, upload, (UINT64)numElements * (UINT64)elementSize);

			D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.Buffer.FirstElement = 0;
			uavDesc.Buffer.NumElements = numElements;
			uavDesc.Buffer.StructureByteStride = elementSize;
			uavDesc.Buffer.CounterOffsetInBytes = 0;
			uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

			RWArrayResource::Init(rwArrayResource, uavDesc, numElements, elementSize);
		}

	}

	void _RWArrayBuffer::Init(UINT numElements, UINT elementSize)
	{
		RWArrayResource::InitBuffer(&resource, &upload, &rwArrayResource, numElements, elementSize);
	}

	void _RWArrayBuffer::Upload(const void* data, ID3D12GraphicsCommandList* cmdList)
	{
		Resource::Upload::UploadBuffer(&upload, &resource, data, cmdList);
	}

	void _RWArrayBuffer::Place(DescriptorHeap::_Context* descriptorHeap, UINT index)
	{
		RWArrayResource::Place(&rwArrayResource, &resource, descriptorHeap, index);
	}

}
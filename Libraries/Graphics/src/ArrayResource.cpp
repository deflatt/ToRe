module;

#include "GraphicsHeader.h"

module TR.Graphics.ArrayResource;

namespace TR::Graphics {

	namespace ArrayResource {

		void Init(_Context* arrayResource, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, UINT numElements, UINT elementSize)
		{
			arrayResource->srvDesc = srvDesc;
			arrayResource->numElements = numElements;
			arrayResource->elementSize = elementSize;
		}

		void Place(_Context* arrayResource, Resource::_Context* resource, DescriptorHeap::_Context* descriptorHeap, UINT index)
		{
			device->CreateShaderResourceView(resource->resource.Get(), &arrayResource->srvDesc, DescriptorHeap::GetHandle(descriptorHeap, index));
		}

	}

	namespace ArrayResource {

		void InitBuffer(Resource::_Context* resource, Resource::Upload::_Context* upload, ArrayResource::_Context* arrayResource, UINT numElements, UINT elementSize)
		{
			Resource::InitBuffer(resource, upload, (UINT64)numElements * (UINT64)elementSize);
			
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Buffer.FirstElement = 0;
			srvDesc.Buffer.NumElements = numElements;
			srvDesc.Buffer.StructureByteStride = elementSize;
			srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			
			ArrayResource::Init(arrayResource, srvDesc, numElements, elementSize);
		}

	}

	void _ArrayBuffer::Init(UINT numElements, UINT elementSize)
	{
		ArrayResource::InitBuffer(&resource, &upload, &arrayResource, numElements, elementSize);
	}

	void _ArrayBuffer::Upload(const void* data, ID3D12GraphicsCommandList* cmdList)
	{
		Resource::Upload::UploadBuffer(&upload, &resource, data, cmdList);
	}

	void _ArrayBuffer::Place(DescriptorHeap::_Context* descriptorHeap, UINT index)
	{
		ArrayResource::Place(&arrayResource, &resource, descriptorHeap, index);
	}

}
module;

#include "GraphicsHeader.h"

module TR.Graphics.ConstantResource;

namespace TR::Graphics {

	namespace ConstantResource {

		void Init(_Context* constantResource, Resource::_Context* resource)
		{
			constantResource->cbvDesc.BufferLocation = resource->resource->GetGPUVirtualAddress();
			constantResource->cbvDesc.SizeInBytes = (UINT)resource->resource->GetDesc().Width;
		}

		void Place(_Context* constantResource, DescriptorHeap::_Context* descriptorHeap, UINT index)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = ((CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap->heap->GetCPUDescriptorHandleForHeapStart()).Offset(index, descriptorHeap->handleIncrementSize);
			device->CreateConstantBufferView(&constantResource->cbvDesc, cpuHandle);
		}

	}

	namespace ConstantResource {

		void InitBuffer(Resource::_Context* resource, Resource::Upload::_Context* upload, ConstantResource::_Context* constantResource, UINT64 numBytes)
		{
			Resource::InitBuffer(resource, upload, numBytes);
			ConstantResource::Init(constantResource, resource);
		}

	}

	void _ConstantBuffer::Init(UINT64 numBytes)
	{
		ConstantResource::InitBuffer(&resource, &upload, &constantResource, numBytes);
	}

	void _ConstantBuffer::Upload(const void* data, ID3D12GraphicsCommandList* cmdList)
	{
		Resource::Upload::UploadBuffer(&upload, &resource, data, cmdList);
	}

	void _ConstantBuffer::Place(DescriptorHeap::_Context* descriptorHeap, UINT index)
	{
		ConstantResource::Place(&constantResource, descriptorHeap, index);
	}

}
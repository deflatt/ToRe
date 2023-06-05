module;

#include "GraphicsHeader.h"

module TR.Graphics.ConstantResource;

namespace TR::Graphics {

	namespace ConstantResource {

		void Init(_Context* context, Resource::_Context* resource)
		{
			context->cbvDesc.BufferLocation = resource->resource->GetGPUVirtualAddress();
			context->cbvDesc.SizeInBytes = (UINT)resource->resource->GetDesc().Width;
		}

		void Place(_Context* context, DescriptorHeap::_Context* descriptorHeap, UINT index)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = ((CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap->heap->GetCPUDescriptorHandleForHeapStart()).Offset(index, descriptorHeap->handleIncrementSize);
			device->CreateConstantBufferView(&context->cbvDesc, cpuHandle);
		}

	}

	void _ConstantBuffer::Init(UINT64 size)
	{
		resource.Init(size);
		ConstantResource::Init(&context, resource.GetContext());
	}

	void _ConstantBuffer::Upload(const void* data, ID3D12GraphicsCommandList* cmdList)
	{
		resource.Upload(data, cmdList);
	}

	void _ConstantBuffer::Place(DescriptorHeap::_Context* descriptorHeap, UINT index)
	{
		ConstantResource::Place(&context, descriptorHeap, index);
	}

	//void _ConstantResource::Init(D3D12_RESOURCE_DESC desc)
	//{
	//	resource.Init(D3D12_HEAP_TYPE_DEFAULT, desc); // State?
	//	ConstantResource::Init(&context, resource.GetContext());
	//}

	//void _ConstantResource::Place(DescriptorHeap::_Context* descriptorHeap, UINT index)
	//{
	//	ConstantResource::Place(&context, descriptorHeap, index);
	//}

}
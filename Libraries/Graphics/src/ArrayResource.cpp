module;

#include "GraphicsHeader.h"

module TR.Graphics.ArrayResource;

namespace TR::Graphics {

	namespace ArrayResource {

		void Init(_Context* context, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, UINT numElements, UINT elementSize)
		{
			context->srvDesc = srvDesc;
			context->numElements = numElements;
			context->elementSize = elementSize;
		}

		void Place(_Context* context, Resource::_Context* resource, DescriptorHeap::_Context* descriptorHeap, UINT index)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = ((CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap->heap->GetCPUDescriptorHandleForHeapStart()).Offset(index, descriptorHeap->handleIncrementSize);
			device->CreateShaderResourceView(resource->resource.Get(), &context->srvDesc, cpuHandle);
		}

	}

	void _ArrayBuffer::Init(UINT numElements, UINT elementSize)
	{
		resource.Init((UINT64)numElements * (UINT64)elementSize);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = numElements;
		srvDesc.Buffer.StructureByteStride = elementSize;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

		ArrayResource::Init(&context, srvDesc, numElements, elementSize);
	}

	void _ArrayBuffer::Upload(const void* data, ID3D12GraphicsCommandList* cmdList)
	{
		resource.Upload(data, cmdList);
	}

	void _ArrayBuffer::Place(DescriptorHeap::_Context* descriptorHeap, UINT index)
	{
		ArrayResource::Place(&context, resource.GetContext(), descriptorHeap, index);
	}

	namespace RWArrayResource {

		void Init(_Context* context, D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, UINT numElements, UINT elementSize)
		{
			context->uavDesc = uavDesc;
			context->numElements = numElements;
			context->elementSize = elementSize;
		}

		void Place(_Context* context, Resource::_Context* resource, DescriptorHeap::_Context* descriptorHeap, UINT index)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = ((CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap->heap->GetCPUDescriptorHandleForHeapStart()).Offset(index, descriptorHeap->handleIncrementSize);
			device->CreateUnorderedAccessView(resource->resource.Get(), nullptr, &context->uavDesc, cpuHandle);
		}

	}

	void _RWArrayBuffer::Init(UINT numElements, UINT elementSize)
	{
		// Skipping D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS seems to be fine
		resource.Init((UINT64)numElements * (UINT64)elementSize);
		
		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = numElements;
		uavDesc.Buffer.StructureByteStride = elementSize;
		uavDesc.Buffer.CounterOffsetInBytes = 0;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

		RWArrayResource::Init(&context, uavDesc, numElements, elementSize);
	}

	void _RWArrayBuffer::Upload(const void* data, ID3D12GraphicsCommandList* cmdList)
	{
		resource.Upload(data, cmdList);
	}

	void _RWArrayBuffer::Place(DescriptorHeap::_Context* descriptorHeap, UINT index)
	{
		RWArrayResource::Place(&context, resource.GetContext(), descriptorHeap, index);
	}

}
module;

#include "GraphicsHeader.h"

module TR.Graphics.DescriptorHeap;

namespace TR::Graphics {

	namespace DescriptorHeap {

		void Init(_Context* descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Type = type;
			desc.NumDescriptors = numDescriptors;
			desc.Flags = flags;
			desc.NodeMask = 0;

			HRESULT ret = device->CreateDescriptorHeap(&desc, __uuidof(ID3D12DescriptorHeap), &descriptorHeap->heap);
			if (ret != 0) {
				throw E_FailedDescriptorHeapCreation(ret);
			}
			descriptorHeap->handleIncrementSize = device->GetDescriptorHandleIncrementSize(type);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE TR::Graphics::DescriptorHeap::GetHandle(_Context* descriptorHeap, UINT index)
		{
			return ((CD3DX12_CPU_DESCRIPTOR_HANDLE)descriptorHeap->heap->GetCPUDescriptorHandleForHeapStart())
				.Offset(index, descriptorHeap->handleIncrementSize);
		}

	}

	void _DescriptorHeap::Init(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		DescriptorHeap::Init(&descriptorHeap, type, numDescriptors, flags);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE _DescriptorHeap::GetHandle(UINT index)
	{
		return DescriptorHeap::GetHandle(&descriptorHeap, index);
	}

}
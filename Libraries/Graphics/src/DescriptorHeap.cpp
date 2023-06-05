module;

#include "GraphicsHeader.h"

module TR.Graphics.DescriptorHeap;

namespace TR::Graphics {

	namespace DescriptorHeap {

		void Init(_Context* context, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Type = type;
			desc.NumDescriptors = numDescriptors;
			desc.Flags = flags;
			desc.NodeMask = 0;

			HRESULT ret = device->CreateDescriptorHeap(&desc, __uuidof(ID3D12DescriptorHeap), &context->heap);
			if (ret != 0) {
				throw E_FailedDescriptorHeapCreation(ret);
			}
			context->handleIncrementSize = device->GetDescriptorHandleIncrementSize(type);
		}

	}

	void _DescriptorHeap::Init(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		DescriptorHeap::Init(&context, type, numDescriptors, flags);
	}

	DescriptorHeap::_Context* _DescriptorHeap::GetContext() noexcept
	{
		return &context;
	}

}
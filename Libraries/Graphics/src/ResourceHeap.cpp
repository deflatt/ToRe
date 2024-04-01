module TR.Graphics.ResourceHeap;

import TR.Graphics.Device;

namespace TR {

	namespace Graphics {

		namespace ResourceHeap {

			void Init(_ResourceHeap* resourceHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
			{
				D3D12_DESCRIPTOR_HEAP_DESC desc = {};
				desc.Type = type;
				desc.NumDescriptors = numDescriptors;
				desc.Flags = flags;
				desc.NodeMask = 0;

				HRESULT ret = device->CreateDescriptorHeap(&desc, __uuidof(ID3D12DescriptorHeap), &resourceHeap->heap);
				if (ret != 0) {
					throw _FailedResourceHeapCreationException(ret);
				}
				resourceHeap->handleIncrementSize = device->GetDescriptorHandleIncrementSize(type);
			}

			D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(_ResourceHeap* resourceHeap, UINT index)
			{
				return ((CD3DX12_CPU_DESCRIPTOR_HANDLE)resourceHeap->heap->GetCPUDescriptorHandleForHeapStart())
					.Offset(index, resourceHeap->handleIncrementSize);
			}

		}

	}

}
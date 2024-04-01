module TR.Graphics.Resource;

import TR.Graphics.Device;

namespace TR {

	namespace Graphics {

		namespace Resource {

			void Init(_Resource* resource, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_DESC desc,
				D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
			{
				HRESULT ret;

				if (heapType == D3D12_HEAP_TYPE_UPLOAD)
					state = D3D12_RESOURCE_STATE_GENERIC_READ;
				else if (heapType == D3D12_HEAP_TYPE_READBACK)
					state = D3D12_RESOURCE_STATE_COPY_DEST;

				CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(heapType);

				if (heapType == D3D12_HEAP_TYPE_UPLOAD) {
					UINT64 bufferSize = 0;
					device->GetCopyableFootprints(&desc, 0, 1, 0, nullptr, nullptr, nullptr, &bufferSize);
					desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
					desc.Alignment = 0;
					desc.Format = DXGI_FORMAT_UNKNOWN;
					desc.Width = bufferSize;
					desc.Height = 1;
				}

				ret = device->CreateCommittedResource(&heapProps, heapFlags, &desc, state, nullptr, __uuidof(ID3D12Resource), &resource->resource);
				if (ret != 0) {
					throw _FailedResourceCreationException(ret);
				}
				resource->state = state;
			}

			void Transition(_Resource* resource, D3D12_RESOURCE_STATES targetState, ID3D12GraphicsCommandList* cmdList)
			{
				if (resource->state == targetState)
					return;
				CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(resource->resource.Get(), resource->state, targetState);
				cmdList->ResourceBarrier(1, &resourceBarrier);
				resource->state = targetState;
			}

			D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress(_Resource* resource)
			{
				return resource->resource->GetGPUVirtualAddress();
			}

		}

	}

}
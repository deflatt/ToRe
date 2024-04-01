module TR.Graphics.BufferResource;

import <d3dx12.h>;

namespace TR {

	namespace Graphics {

		namespace BufferResource {

			void InitBuffer(_Resource* resource, UINT64 numBytes, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
			{
				Init(resource, heapType, CD3DX12_RESOURCE_DESC::Buffer(numBytes), state, heapFlags);
			}


			void InitBuffer(_Resource* resource, _UploadPart* upload, UINT64 numBytes, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
			{
				Init(resource, upload, CD3DX12_RESOURCE_DESC::Buffer(numBytes), state, heapFlags);
			}

			void UploadBuffer(_UploadPart* upload, _Resource* dstResource, const void* data, ID3D12GraphicsCommandList* cmdList)
			{
				D3D12_SUBRESOURCE_DATA subData = {};
				subData.pData = data;
				subData.RowPitch = dstResource->resource->GetDesc().Width;
				UploadData(upload, dstResource, subData, cmdList);
			}

		}

	}

}
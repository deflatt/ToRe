module TR.Graphics.ArrayResource;

import TR.Graphics.Device;
import <d3d12.h>;

namespace TR {

	namespace Graphics {

		namespace ArrayResource {

			void Init(_ArrayPart* array, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, UINT numElements, UINT elementSize)
			{
				array->srvDesc = srvDesc;
				array->numElements = numElements;
				array->elementSize = elementSize;
			}

			void Place(_ArrayPart* array, _Resource* resource, _ResourceHeap* resourceHeap, UINT index)
			{
				device->CreateShaderResourceView(resource->resource.Get(), &array->srvDesc, GetHandle(resourceHeap, index));
			}

			void InitBuffer(_Resource* resource, _UploadPart* upload, _ArrayPart* array
				, UINT numElements, UINT elementSize)
			{
				BufferResource::InitBuffer(resource, upload, (UINT64)numElements * (UINT64)elementSize);

				D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
				srvDesc.Format = DXGI_FORMAT_UNKNOWN;
				srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				srvDesc.Buffer.FirstElement = 0;
				srvDesc.Buffer.NumElements = numElements;
				srvDesc.Buffer.StructureByteStride = elementSize;
				srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

				Init(array, srvDesc, numElements, elementSize);
			}

			void Init(_ArrayBuffer* arrayBuffer, UINT numElements, UINT elementSize)
			{
				InitBuffer(&arrayBuffer->resource, &arrayBuffer->upload, &arrayBuffer->array, numElements, elementSize);
			}

			void Init(_RWArrayPart* rwArray, D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, UINT numElements, UINT elementSize)
			{
				rwArray->uavDesc = uavDesc;
				rwArray->numElements = numElements;
				rwArray->elementSize = elementSize;
			}

			void Place(_RWArrayPart* rwArray, _Resource* resource, _ResourceHeap* resourceHeap, UINT index)
			{
				device->CreateUnorderedAccessView(resource->resource.Get(), nullptr, &rwArray->uavDesc, GetHandle(resourceHeap, index));
			}

		}

	}

}
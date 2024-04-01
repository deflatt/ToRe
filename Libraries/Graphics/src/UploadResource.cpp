module TR.Graphics.UploadResource;

import TR.Graphics.Device;

namespace TR {

	namespace Graphics {

		namespace UploadResource {

			void Init(_UploadPart* upload, D3D12_RESOURCE_DESC desc, D3D12_HEAP_FLAGS heapFlags)
			{
				Init(&upload->uploadResource, D3D12_HEAP_TYPE_UPLOAD, desc, D3D12_RESOURCE_STATE_COMMON, heapFlags);
			}

			void Init(_UploadPart* upload, _Resource* dstResource, D3D12_HEAP_FLAGS heapFlags)
			{
				Init(upload, dstResource->resource->GetDesc(), heapFlags);
			}

			void UploadData(_UploadPart* upload, _Resource* dstResource, const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList)
			{
				D3D12_RESOURCE_STATES oldState = dstResource->state;
				Transition(dstResource, D3D12_RESOURCE_STATE_COPY_DEST, cmdList);

				UpdateSubresources(cmdList, dstResource->resource.Get()
					, upload->uploadResource.resource.Get(), 0, 0, 1, &data);

				Resource::Transition(dstResource, oldState, cmdList);
			}

			void Init(_Resource* resource, _UploadPart* upload, D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
			{
				Init(resource, D3D12_HEAP_TYPE_DEFAULT, desc, state, heapFlags);
				Init(upload, resource, heapFlags);
			}

			void Init(_UploadResource* uploadResource, D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
			{
				Init(&uploadResource->resource, &uploadResource->upload, desc, state, heapFlags);
			}

		}

	}

}
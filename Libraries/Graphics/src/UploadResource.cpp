module;

#include "GraphicsHeader.h"

module TR.Graphics.UploadResource;

namespace TR::Graphics {

	namespace Resource {

		namespace Upload {

			void Init(_Context* upload, D3D12_RESOURCE_DESC desc, D3D12_HEAP_FLAGS heapFlags)
			{
				upload->uploadResource.Init(D3D12_HEAP_TYPE_UPLOAD, desc, D3D12_RESOURCE_STATE_COMMON, heapFlags);
			}

			void Init(_Context* upload, Resource::_Context* dstResource, D3D12_HEAP_FLAGS heapFlags)
			{
				Init(upload, dstResource->resource->GetDesc(), heapFlags);
			}

			void UploadData(_Context* upload, Resource::_Context* dstResource, const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList)
			{
				D3D12_RESOURCE_STATES oldState = dstResource->state;
				Resource::Transition(dstResource, D3D12_RESOURCE_STATE_COPY_DEST, cmdList);

				UpdateSubresources(cmdList, dstResource->resource.Get()
					, upload->uploadResource.context.resource.Get(), 0, 0, 1, &data);

				Resource::Transition(dstResource, oldState, cmdList);
			}

		}

		void Init(Resource::_Context* resource, Upload::_Context* upload, D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
		{
			Init(resource, D3D12_HEAP_TYPE_DEFAULT, desc, state, heapFlags);
			Upload::Init(upload, resource, heapFlags);
		}

	}

	void _UploadResource::Init(D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES state, D3D12_HEAP_FLAGS heapFlags)
	{
		Resource::Init(&resource, &upload, desc, state, heapFlags);
	}

	void _UploadResource::UploadData(const D3D12_SUBRESOURCE_DATA& data, ID3D12GraphicsCommandList* cmdList)
	{
		Resource::Upload::UploadData(&upload, &resource, data, cmdList);
	}

}